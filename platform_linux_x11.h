#ifndef PLATFORM_LINUX_X11_H
#define PLATFORM_LINUX_X11_H

/* C standard lib */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

/* X11 window shit */
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/Xrandr.h>

/*x11 keyboards shit */
#include <X11/XKBlib.h>

/* Opengl */
#include <glad/glad.h>
#include <GL/gl.h>
#include <GL/glx.h>

/* dlopen/dlsym/dlclose */
#include <dlfcn.h>

/* linux-specific headers */
#include <linux/input.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <libudev.h>
#include <unistd.h>
#include <fcntl.h>

struct pal_window {
    Window window;
    GLXContext gl_context;
    GC graphics_context;
    float width;
    float height;
    float x,y;
};

struct pal_sound {
    /* Core audio data */
    unsigned char* data; /* Raw PCM audio data (initial buffer) */
    size_t data_size;    /* Size in bytes of initial buffer */
    int sample_rate;     /* Samples per second (e.g., 44100) */
    int channels;        /* Number of audio channels (e.g., 2 for stereo) */
    int bits_per_sample; /* Usually 16 or 32 */
    int is_float;        /* 0 = PCM, 1 = IEEE float */

    /* pulseaudio */

    /* Streaming - OGG */
    void* decoder;  /* stb_vorbis* (using void* to avoid header dependency) */
    char* filename; /* Filename for reopening OGG decoder */

    /* Streaming - WAV */
    FILE* source_file;
    size_t total_data_size; /* Total size of audio data in file */
    size_t bytes_streamed;  /* How many bytes we've read so far */
    size_t data_offset;     /* Offset in file where audio data starts */

    /* Streaming control */
    float preload_seconds; /* How many seconds were preloaded */
    int is_streaming;      /* 1 if this is a streaming sound */
    int stream_finished;   /* 1 when streaming is complete */
};

typedef struct pal_monitor {
    Display *display;
    RROutput output;
} pal_monitor;

Display *g_display = NULL;
Atom g_wm_delete = 0;

/* XKB keyboard translation support (uses existing g_display) */
static XkbDescPtr g_xkb = NULL;
static XIM g_xim = NULL;
static XIC g_xic = NULL;

void linux_x11_init_raw_input();

PALAPI void pal_init(void) {
    pal__init_eventq();
    linux_x11_init_raw_input();
    g_display = XOpenDisplay(NULL);
    if (!g_display) {
        fprintf(stderr, "Failed to open display\n");
        return;
    }
    g_wm_delete = XInternAtom(g_display, "WM_DELETE_WINDOW", False);
}



void linux_x11_cleanup_raw_input();
PALAPI void pal_shutdown() {
    if(g_display) {
        XCloseDisplay(g_display);
        linux_x11_cleanup_raw_input();
    }
}

pal_bool g_message_pump_drained = pal_false;
pal_event_queue g_event_queue;

void linux_x11_translate_event(XEvent *xevent) {
    pal_event event = {0};
    
    switch(xevent->type) {
        case ClientMessage: {
            if ((Atom)xevent->xclient.data.l[0] == g_wm_delete) {
                event.type = PAL_EVENT_WINDOW_CLOSE_REQUESTED;
                event.window.windowid = (uint32_t)xevent->xclient.window;
                pal__eventq_push(&g_event_queue, event);
            }
            break;
        }
        
        case ConfigureNotify: {
            XConfigureEvent *ce = &xevent->xconfigure;
            
            /* Window moved */
            event.type = PAL_EVENT_WINDOW_MOVED;
            event.window.windowid = (uint32_t)ce->window;
            event.window.x = ce->x;
            event.window.y = ce->y;
            pal__eventq_push(&g_event_queue, event);
            
            /* Window resized */
            event.type = PAL_EVENT_WINDOW_RESIZED;
            event.window.windowid = (uint32_t)ce->window;
            event.window.width = ce->width;
            event.window.height = ce->height;
            pal__eventq_push(&g_event_queue, event);
            break;
        }
        
        case MapNotify: {
            event.type = PAL_EVENT_WINDOW_SHOWN;
            event.window.windowid = (uint32_t)xevent->xmap.window;
            event.window.visible = 1;
            pal__eventq_push(&g_event_queue, event);
            break;
        }
        
        case UnmapNotify: {
            event.type = PAL_EVENT_WINDOW_HIDDEN;
            event.window.windowid = (uint32_t)xevent->xunmap.window;
            event.window.visible = 0;
            pal__eventq_push(&g_event_queue, event);
            break;
        }
        
        case Expose: {
            if (xevent->xexpose.count == 0) { /* Only on last expose event */
                event.type = PAL_EVENT_WINDOW_EXPOSED;
                event.window.windowid = (uint32_t)xevent->xexpose.window;
                pal__eventq_push(&g_event_queue, event);
            }
            break;
        }
        
        case FocusIn: {
            event.type = PAL_EVENT_WINDOW_GAINED_FOCUS;
            event.window.windowid = (uint32_t)xevent->xfocus.window;
            event.window.focused = 1;
            pal__eventq_push(&g_event_queue, event);
            break;
        }
        
        case FocusOut: {
            event.type = PAL_EVENT_WINDOW_LOST_FOCUS;
            event.window.windowid = (uint32_t)xevent->xfocus.window;
            event.window.focused = 0;
            pal__eventq_push(&g_event_queue, event);
            break;
        }
        
        case DestroyNotify: {
            event.type = PAL_EVENT_WINDOW_CLOSED;
            event.window.windowid = (uint32_t)xevent->xdestroywindow.window;
            pal__eventq_push(&g_event_queue, event);
            break;
        }
        
        default:
            break;
    }
}
void linux_x11_poll_raw_input();

PALAPI pal_bool pal_poll_events(pal_event* event) {
    pal_event_queue* queue = &g_event_queue;
    if (!g_message_pump_drained) {
        linux_x11_poll_raw_input();

        while (XPending(g_display) > 0) {
            XEvent xevent;
            XNextEvent(g_display, &xevent);
            linux_x11_translate_event(&xevent);
        }
        g_message_pump_drained = pal_true;
    }

    if (queue->size) {
        /* peek */
        *event = queue->events[queue->front];
        /* dequeue */
        queue->front = (queue->front + 1) % queue->capacity;
        queue->size--;
        return 1;
    } else {
        g_message_pump_drained = pal_false;
        return 0;
    }
}

#define MAX_KEYBOARDS 16
#define MAX_MICE 16
#define MAX_KEYS 256
#define MAX_SCANCODES 512
#define MAX_MOUSE_BUTTONS 8

typedef struct {
    int fds[MAX_KEYBOARDS];               /* fds of keyboards */
    char names[MAX_KEYBOARDS][256];       /* names of keyboards */
    int count;                            /* number of keyboards */
    unsigned char keys[MAX_KEYBOARDS][MAX_SCANCODES];         /* per-keyboard key states */
    unsigned char keys_toggled[MAX_KEYBOARDS][MAX_SCANCODES]; /* per-keyboard toggle tracking */
    int cached_modifiers[MAX_KEYBOARDS];
} pal_keyboard_state;
pal_keyboard_state g_keyboards = {0};

typedef struct {
    int fds[MAX_MICE];                     /* fds of mice */
    char names[MAX_MICE][256];             /* names of mice */
    int count;
    int buttons[MAX_MICE][MAX_MOUSE_BUTTONS];        /* per-mouse button states */
    int buttons_toggled[MAX_MICE][MAX_MOUSE_BUTTONS]; /* per-mouse toggle tracking */
    int dx[MAX_MICE];                      /* per-mouse accumulated movement */
    int dy[MAX_MICE];
    int wheel[MAX_MICE];                             
} pal_mouse_state;

pal_mouse_state g_mice = {0};

/* udev context and monitor */
struct udev *g_udev = NULL;
struct udev_monitor *g_monitor = NULL;

/* -------- Helpers -------- */
int is_keyboard(int fd) {
    unsigned long evbits[(EV_MAX+7)/8] = {0};
    ioctl(fd, EVIOCGBIT(0, EV_MAX), evbits);
    
    int has_key = (evbits[EV_KEY/8] & (1 << (EV_KEY % 8))) != 0;
    int has_rel = (evbits[EV_REL/8] & (1 << (EV_REL % 8))) != 0;
    int has_abs = (evbits[EV_ABS/8] & (1 << (EV_ABS % 8))) != 0;
    
    /* It's a keyboard if it has keys but NOT mouse/touchpad properties */
    return has_key && !has_rel && !has_abs;
}

int is_mouse(int fd) {
    unsigned long evbits[(EV_MAX+7)/8] = {0};
    ioctl(fd, EVIOCGBIT(0, EV_MAX), evbits);
    
    int has_rel = (evbits[EV_REL/8] & (1 << (EV_REL % 8))) != 0;
    int has_abs = (evbits[EV_ABS/8] & (1 << (EV_ABS % 8))) != 0;
    
    /* It's a mouse/touchpad if it has relative motion OR absolute positioning */
    return has_rel || has_abs;
}

/* -------- Initialization -------- */

Window g_dummy_window;

void linux_x11_create_dummy_window() {
    int screen = DefaultScreen(g_display);

    g_dummy_window = XCreateSimpleWindow(
        g_display,
        RootWindow(g_display, screen),
        0, 0, 1, 1, 0,
        BlackPixel(g_display, screen),
        BlackPixel(g_display, screen)
    );

    XMapWindow(g_display, g_dummy_window);
    XFlush(g_display);
}

void linux_x11_init_raw_input() {
    /* Initialize X11 keyboard translation support */
    if (!g_display) {
        fprintf(stderr, "Warning: Failed to open X display for keyboard translation\n");
        fprintf(stderr, "Text input will fall back to US layout\n");
    } else {
        g_xkb = XkbGetMap(g_display, XkbAllComponentsMask, XkbUseCoreKbd);
        if (!g_xkb) {
            fprintf(stderr, "Warning: Failed to get XKB map\n");
        }

        /* Set up input method for IME support (Chinese, Japanese, Korean, etc.) */
        g_xim = XOpenIM(g_display, NULL, NULL, NULL);
        if (g_xim) {
            linux_x11_create_dummy_window();
            g_xic = XCreateIC(g_xim,
                XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
                XNClientWindow, g_dummy_window,
                XNFocusWindow, g_dummy_window,
                NULL);
            if (!g_xic) {
                fprintf(stderr, "Warning: Failed to create input context\n");
            }
        }
    }

    /* Initialize udev for device hotplug detection */
    g_udev = udev_new();
    if (!g_udev) {
        fprintf(stderr, "Failed to create udev context\n");
        exit(1);
    }

    g_monitor = udev_monitor_new_from_netlink(g_udev, "udev");
    udev_monitor_filter_add_match_subsystem_devtype(g_monitor, "input", NULL);
    udev_monitor_enable_receiving(g_monitor);

    /* Enumerate existing devices */
    struct udev_enumerate *enumerate = udev_enumerate_new(g_udev);
    udev_enumerate_add_match_subsystem(enumerate, "input");
    udev_enumerate_scan_devices(enumerate);
    struct udev_list_entry *devices = udev_enumerate_get_list_entry(enumerate);
    struct udev_list_entry *entry;

    udev_list_entry_foreach(entry, devices) {
        const char *path = udev_list_entry_get_name(entry);
        struct udev_device *dev = udev_device_new_from_syspath(g_udev, path);
        const char *devnode = udev_device_get_devnode(dev);
        if (!devnode) { udev_device_unref(dev); continue; }
        
        int dev_fd = open(devnode, O_RDONLY | O_NONBLOCK);
        if (dev_fd < 0) { udev_device_unref(dev); continue; }
        
        char name[256] = {0};
        ioctl(dev_fd, EVIOCGNAME(sizeof(name)), name);
        
        if (is_keyboard(dev_fd) && g_keyboards.count < MAX_KEYBOARDS) {
            g_keyboards.fds[g_keyboards.count] = dev_fd;
            strncpy(g_keyboards.names[g_keyboards.count], name, 255);
            memset(g_keyboards.keys[g_keyboards.count], 0, MAX_KEYS);
            memset(g_keyboards.keys_toggled[g_keyboards.count], 0, MAX_KEYS);
            g_keyboards.count++;
        } else if (is_mouse(dev_fd) && g_mice.count < MAX_MICE) {
            g_mice.fds[g_mice.count] = dev_fd;
            strncpy(g_mice.names[g_mice.count], name, 255);
            memset(g_mice.buttons[g_mice.count], 0, MAX_MOUSE_BUTTONS * sizeof(int));
            memset(g_mice.buttons_toggled[g_mice.count], 0, MAX_MOUSE_BUTTONS * sizeof(int));
            g_mice.dx[g_mice.count] = 0;
            g_mice.dy[g_mice.count] = 0;
            g_mice.wheel[g_mice.count] = 0;
            g_mice.count++;
        } else {
            close(dev_fd);
        }
        
        udev_device_unref(dev);
    }
    
    udev_enumerate_unref(enumerate);
}

void linux_x11_cleanup_raw_input() {
    /* Clean up X11 resources */
    if (g_xic) XDestroyIC(g_xic);
    if (g_xim) XCloseIM(g_xim);
    if (g_xkb) XkbFreeKeyboard(g_xkb, XkbAllComponentsMask, True);
    if (g_display) XCloseDisplay(g_display);
    
    /* Clean up keyboards */
    for (int i = 0; i < g_keyboards.count; i++) {
        close(g_keyboards.fds[i]);
    }
    
    /* Clean up mice */
    for (int i = 0; i < g_mice.count; i++) {
        close(g_mice.fds[i]);
    }
    
    /* Clean up udev */
    if (g_monitor) udev_monitor_unref(g_monitor);
    if (g_udev) udev_unref(g_udev);
}

PALAPI pal_vec2 pal_get_mouse_position(pal_window *window) {
    pal_vec2 mouse_pos;

    if (!g_display || !window || !window->window) {
        mouse_pos.x = 0;
        mouse_pos.y = 0;
        return mouse_pos;
    }

    Window root_return, child_return;
    int root_x, root_y, win_x, win_y;
    unsigned int mask_return;

    /* Query pointer position relative to the window */
    XQueryPointer(g_display,
                  window->window,
                  &root_return, &child_return,
                  &root_x, &root_y,
                  &win_x, &win_y,
                  &mask_return);

    mouse_pos.x = win_x;
    mouse_pos.y = win_y;
    return mouse_pos;
}

static int linux_button_to_pal_button(int linux_code) {
    switch(linux_code) {
        case BTN_LEFT:    return PAL_MOUSE_LEFT;    /* 0x0 */
        case BTN_RIGHT:   return PAL_MOUSE_RIGHT;   /* 0x1 */
        case BTN_MIDDLE:  return PAL_MOUSE_MIDDLE;  /* 0x2 */
        case BTN_SIDE:    return PAL_MOUSE_4;       /* 0x3 (also known as "back") */
        case BTN_EXTRA:   return PAL_MOUSE_5;       /* 0x4 (also known as "forward") */
        case BTN_FORWARD: return PAL_MOUSE_5;       /* 0x4 (alternative code for forward) */
        case BTN_BACK:    return PAL_MOUSE_4;       /* 0x3 (alternative code for back) */
        default:          return -1;
    }
}

/* Mapping table from Linux input key codes to PAL virtual key codes */
static const int linux_keycode_to_pal_vk[KEY_MAX] = {
    [KEY_ESC]           = PAL_KEY_ESCAPE,
    [KEY_1]             = PAL_KEY_1,
    [KEY_2]             = PAL_KEY_2,
    [KEY_3]             = PAL_KEY_3,
    [KEY_4]             = PAL_KEY_4,
    [KEY_5]             = PAL_KEY_5,
    [KEY_6]             = PAL_KEY_6,
    [KEY_7]             = PAL_KEY_7,
    [KEY_8]             = PAL_KEY_8,
    [KEY_9]             = PAL_KEY_9,
    [KEY_0]             = PAL_KEY_0,
    [KEY_MINUS]         = PAL_KEY_MINUS,
    [KEY_EQUAL]         = PAL_KEY_EQUAL,
    [KEY_BACKSPACE]     = PAL_KEY_BACKSPACE,
    [KEY_TAB]           = PAL_KEY_TAB,
    [KEY_Q]             = PAL_KEY_Q,
    [KEY_W]             = PAL_KEY_W,
    [KEY_E]             = PAL_KEY_E,
    [KEY_R]             = PAL_KEY_R,
    [KEY_T]             = PAL_KEY_T,
    [KEY_Y]             = PAL_KEY_Y,
    [KEY_U]             = PAL_KEY_U,
    [KEY_I]             = PAL_KEY_I,
    [KEY_O]             = PAL_KEY_O,
    [KEY_P]             = PAL_KEY_P,
    [KEY_LEFTBRACE]     = PAL_KEY_LEFTBRACE,  /* VK_OEM_4 '[' */
    [KEY_RIGHTBRACE]    = PAL_KEY_RIGHTBRACE,  /* VK_OEM_6 ']' */
    [KEY_ENTER]         = PAL_KEY_ENTER,
    [KEY_LEFTCTRL]      = PAL_KEY_LCTRL,
    [KEY_A]             = PAL_KEY_A,
    [KEY_S]             = PAL_KEY_S,
    [KEY_D]             = PAL_KEY_D,
    [KEY_F]             = PAL_KEY_F,
    [KEY_G]             = PAL_KEY_G,
    [KEY_H]             = PAL_KEY_H,
    [KEY_J]             = PAL_KEY_J,
    [KEY_K]             = PAL_KEY_K,
    [KEY_L]             = PAL_KEY_L,
    [KEY_SEMICOLON]     = PAL_KEY_SEMICOLON,  /* VK_OEM_1 ';' */
    [KEY_APOSTROPHE]    = PAL_KEY_APOSTROPHE,  /* VK_OEM_7 ''' */
    [KEY_GRAVE]         = PAL_KEY_BACKTICK,  /* VK_OEM_3 '`' */
    [KEY_LEFTSHIFT]     = PAL_KEY_LSHIFT,
    [KEY_BACKSLASH]     = PAL_KEY_BACKSLASH,  /* VK_OEM_5 '\' */
    [KEY_Z]             = PAL_KEY_Z,
    [KEY_X]             = PAL_KEY_X,
    [KEY_C]             = PAL_KEY_C,
    [KEY_V]             = PAL_KEY_V,
    [KEY_B]             = PAL_KEY_B,
    [KEY_N]             = PAL_KEY_N,
    [KEY_M]             = PAL_KEY_M,
    [KEY_COMMA]         = PAL_KEY_COMMA,  /* VK_OEM_COMMA */
    [KEY_DOT]           = PAL_KEY_DOT,  /* VK_OEM_PERIOD */
    [KEY_SLASH]         = PAL_KEY_FORWARD_SLASH,  /* VK_OEM_2 '/' */
    [KEY_RIGHTSHIFT]    = PAL_KEY_RSHIFT,
    [KEY_KPASTERISK]    = PAL_KEY_MULTIPLY,
    [KEY_LEFTALT]       = PAL_KEY_LALT,
    [KEY_SPACE]         = PAL_KEY_SPACE,
    [KEY_CAPSLOCK]      = PAL_KEY_CAPSLOCK,
    [KEY_F1]            = PAL_KEY_F1,
    [KEY_F2]            = PAL_KEY_F2,
    [KEY_F3]            = PAL_KEY_F3,
    [KEY_F4]            = PAL_KEY_F4,
    [KEY_F5]            = PAL_KEY_F5,
    [KEY_F6]            = PAL_KEY_F6,
    [KEY_F7]            = PAL_KEY_F7,
    [KEY_F8]            = PAL_KEY_F8,
    [KEY_F9]            = PAL_KEY_F9,
    [KEY_F10]           = PAL_KEY_F10,
    [KEY_NUMLOCK]       = PAL_KEY_NUMLOCKCLEAR,
    [KEY_SCROLLLOCK]    = PAL_KEY_SCROLLLOCK,
    [KEY_KP7]           = PAL_KEY_NUMPAD_7,
    [KEY_KP8]           = PAL_KEY_NUMPAD_8,
    [KEY_KP9]           = PAL_KEY_NUMPAD_9,
    [KEY_KPMINUS]       = PAL_KEY_SUBTRACT,
    [KEY_KP4]           = PAL_KEY_NUMPAD_4,
    [KEY_KP5]           = PAL_KEY_NUMPAD_5,
    [KEY_KP6]           = PAL_KEY_NUMPAD_6,
    [KEY_KPPLUS]        = PAL_KEY_ADD,
    [KEY_KP1]           = PAL_KEY_NUMPAD_1,
    [KEY_KP2]           = PAL_KEY_NUMPAD_2,
    [KEY_KP3]           = PAL_KEY_NUMPAD_3,
    [KEY_KP0]           = PAL_KEY_NUMPAD_0,
    [KEY_KPDOT]         = PAL_KEY_NUMPAD_PERIOD,
    [KEY_F11]           = PAL_KEY_F11,
    [KEY_F12]           = PAL_KEY_F12,
    [KEY_KPENTER]       = PAL_KEY_ENTER,
    [KEY_RIGHTCTRL]     = PAL_KEY_RCTRL,
    [KEY_KPSLASH]       = PAL_KEY_DIVIDE,
    [KEY_SYSRQ]         = PAL_KEY_PRINTSCREEN,
    [KEY_RIGHTALT]      = PAL_KEY_RALT,
    [KEY_HOME]          = PAL_KEY_HOME,
    [KEY_UP]            = PAL_KEY_UP,
    [KEY_PAGEUP]        = PAL_KEY_PAGEUP,
    [KEY_LEFT]          = PAL_KEY_LEFT,
    [KEY_RIGHT]         = PAL_KEY_RIGHT,
    [KEY_END]           = PAL_KEY_END,
    [KEY_DOWN]          = PAL_KEY_DOWN,
    [KEY_PAGEDOWN]      = PAL_KEY_PAGEDOWN,
    [KEY_INSERT]        = PAL_KEY_INSERT,
    [KEY_DELETE]        = PAL_KEY_DELETE,
    [KEY_PAUSE]         = PAL_KEY_PAUSE,
    [KEY_LEFTMETA]      = PAL_KEY_LWIN,
    [KEY_RIGHTMETA]     = PAL_KEY_RWIN,
    [KEY_MENU]          = PAL_KEY_MENU,
};

static const int linux_scancode_to_pal_scancode[256] = {
    /*   0 */ PAL_SCAN_NONE,
    /*   1 */ PAL_SCAN_ESCAPE,
    /*   2 */ PAL_SCAN_1,
    /*   3 */ PAL_SCAN_2,
    /*   4 */ PAL_SCAN_3,
    /*   5 */ PAL_SCAN_4,
    /*   6 */ PAL_SCAN_5,
    /*   7 */ PAL_SCAN_6,
    /*   8 */ PAL_SCAN_7,
    /*   9 */ PAL_SCAN_8,
    /*  10 */ PAL_SCAN_9,
    /*  11 */ PAL_SCAN_0,
    /*  12 */ PAL_SCAN_MINUS,
    /*  13 */ PAL_SCAN_EQUALS,
    /*  14 */ PAL_SCAN_BACKSPACE,
    /*  15 */ PAL_SCAN_TAB,
    /*  16 */ PAL_SCAN_Q,
    /*  17 */ PAL_SCAN_W,
    /*  18 */ PAL_SCAN_E,
    /*  19 */ PAL_SCAN_R,
    /*  20 */ PAL_SCAN_T,
    /*  21 */ PAL_SCAN_Y,
    /*  22 */ PAL_SCAN_U,
    /*  23 */ PAL_SCAN_I,
    /*  24 */ PAL_SCAN_O,
    /*  25 */ PAL_SCAN_P,
    /*  26 */ PAL_SCAN_LEFTBRACKET,
    /*  27 */ PAL_SCAN_RIGHTBRACKET,
    /*  28 */ PAL_SCAN_RETURN,
    /*  29 */ PAL_SCAN_LCTRL,
    /*  30 */ PAL_SCAN_A,
    /*  31 */ PAL_SCAN_S,
    /*  32 */ PAL_SCAN_D,
    /*  33 */ PAL_SCAN_F,
    /*  34 */ PAL_SCAN_G,
    /*  35 */ PAL_SCAN_H,
    /*  36 */ PAL_SCAN_J,
    /*  37 */ PAL_SCAN_K,
    /*  38 */ PAL_SCAN_L,
    /*  39 */ PAL_SCAN_SEMICOLON,
    /*  40 */ PAL_SCAN_APOSTROPHE,
    /*  41 */ PAL_SCAN_GRAVE,
    /*  42 */ PAL_SCAN_LSHIFT,
    /*  43 */ PAL_SCAN_BACKSLASH,
    /*  44 */ PAL_SCAN_Z,
    /*  45 */ PAL_SCAN_X,
    /*  46 */ PAL_SCAN_C,
    /*  47 */ PAL_SCAN_V,
    /*  48 */ PAL_SCAN_B,
    /*  49 */ PAL_SCAN_N,
    /*  50 */ PAL_SCAN_M,
    /*  51 */ PAL_SCAN_COMMA,
    /*  52 */ PAL_SCAN_PERIOD,
    /*  53 */ PAL_SCAN_SLASH,
    /*  54 */ PAL_SCAN_RSHIFT,
    /*  55 */ PAL_SCAN_KP_MULTIPLY,
    /*  56 */ PAL_SCAN_LALT,
    /*  57 */ PAL_SCAN_SPACE,
    /*  58 */ PAL_SCAN_CAPSLOCK,
    /*  59 */ PAL_SCAN_F1,
    /*  60 */ PAL_SCAN_F2,
    /*  61 */ PAL_SCAN_F3,
    /*  62 */ PAL_SCAN_F4,
    /*  63 */ PAL_SCAN_F5,
    /*  64 */ PAL_SCAN_F6,
    /*  65 */ PAL_SCAN_F7,
    /*  66 */ PAL_SCAN_F8,
    /*  67 */ PAL_SCAN_F9,
    /*  68 */ PAL_SCAN_F10,
    /*  69 */ PAL_SCAN_NUMCLEAR,      /* NumLock */
    /*  70 */ PAL_SCAN_SCROLLLOCK,
    /*  71 */ PAL_SCAN_KP_7,
    /*  72 */ PAL_SCAN_KP_8,
    /*  73 */ PAL_SCAN_KP_9,
    /*  74 */ PAL_SCAN_KP_MINUS,
    /*  75 */ PAL_SCAN_KP_4,
    /*  76 */ PAL_SCAN_KP_5,
    /*  77 */ PAL_SCAN_KP_6,
    /*  78 */ PAL_SCAN_KP_PLUS,
    /*  79 */ PAL_SCAN_KP_1,
    /*  80 */ PAL_SCAN_KP_2,
    /*  81 */ PAL_SCAN_KP_3,
    /*  82 */ PAL_SCAN_KP_0,
    /*  83 */ PAL_SCAN_KP_PERIOD,
    /*  84 */ PAL_SCAN_NONUSBACKSLASH,  /* 102nd key (ISO) */
    /*  85 */ PAL_SCAN_F11,
    /*  86 */ PAL_SCAN_F12,
    /*  87 */ PAL_SCAN_KP_ENTER,
    /*  88 */ PAL_SCAN_RCTRL,
    /*  89 */ PAL_SCAN_KP_DIVIDE,
    /*  90 */ PAL_SCAN_SYSREQ,
    /*  91 */ PAL_SCAN_RALT,       /* AltGr */
    /*  92 */ PAL_SCAN_HOME,
    /*  93 */ PAL_SCAN_UP,
    /*  94 */ PAL_SCAN_PAGEUP,
    /*  95 */ PAL_SCAN_LEFT,
    /*  96 */ PAL_SCAN_RIGHT,
    /*  97 */ PAL_SCAN_END,
    /*  98 */ PAL_SCAN_DOWN,
    /*  99 */ PAL_SCAN_PAGEDOWN,
    /* 100 */ PAL_SCAN_INSERT,
    /* 101 */ PAL_SCAN_DELETE,
    /* 102 */ PAL_SCAN_MUTE,
    /* 103 */ PAL_SCAN_VOLUMEDOWN,
    /* 104 */ PAL_SCAN_VOLUMEUP,
    /* 105 */ PAL_SCAN_POWER,
    /* 106 */ PAL_SCAN_KP_EQUALS,
    /* 107 */ PAL_SCAN_F13,
    /* 108 */ PAL_SCAN_F14,
    /* 109 */ PAL_SCAN_F15,
    /* 110 */ PAL_SCAN_HELP,
    /* 111 */ PAL_SCAN_MENU,
    /* 112 */ PAL_SCAN_STOP,
    /* 113 */ PAL_SCAN_AGAIN,
    /* 114 */ PAL_SCAN_AC_BACK,
    /* 115 */ PAL_SCAN_AC_FORWARD,
    /* 116 */ PAL_SCAN_AC_REFRESH,
    /* 117 */ PAL_SCAN_AC_STOP,
    /* 118 */ PAL_SCAN_AC_SEARCH,
    /* 119 */ PAL_SCAN_AC_BOOKMARKS,
    /* 120 */ PAL_SCAN_MUTE,
    /* 121 */ PAL_SCAN_VOLUMEDOWN,
    /* 122 */ PAL_SCAN_VOLUMEUP,
    /* 123 */ PAL_SCAN_POWER,   /* some keyboards */
    /* 124 */ PAL_SCAN_NONE,
    /* 125 */ PAL_SCAN_LGUI,
    /* 126 */ PAL_SCAN_RGUI,
    /* 127 */ PAL_SCAN_APPLICATION,

    /* 128–255 = mostly OEM / media keys */
    [128] = PAL_SCAN_MEDIA_PLAY_PAUSE,
    [129] = PAL_SCAN_MEDIA_STOP,
    [130] = PAL_SCAN_MEDIA_PREVIOUS_TRACK,
    [131] = PAL_SCAN_MEDIA_NEXT_TRACK,
    [132] = PAL_SCAN_MEDIA_EJECT,
    [133] = PAL_SCAN_SLEEP,
    [134] = PAL_SCAN_WAKE,
    [135] = PAL_SCAN_AC_HOME,
    [136] = PAL_SCAN_AC_SEARCH,
    [137] = PAL_SCAN_AC_BACK,
    [138] = PAL_SCAN_AC_FORWARD,
    [139] = PAL_SCAN_AC_REFRESH,
    [140] = PAL_SCAN_AC_BOOKMARKS,

};

int linux_keycode_to_pal_virtual_key(int linux_code) {
    if (linux_code < 0 || linux_code >= KEY_MAX) {
        return PAL_KEY_NONE;
    }
    return linux_keycode_to_pal_vk[linux_code];
}


int linux_keycode_to_utf8(int keycode, unsigned char *key_state, char *out, size_t out_size);
/* Add this helper function to convert key states to modifier flags */
static int compute_modifiers_from_key_state(unsigned char *keys) {
    int modifiers = PAL_MOD_NONE;
    
    if (keys[PAL_SCAN_LSHIFT]) modifiers |= PAL_MOD_LSHIFT;
    if (keys[PAL_SCAN_RSHIFT]) modifiers |= PAL_MOD_RSHIFT;
    if (keys[PAL_SCAN_LCTRL]) modifiers |= PAL_MOD_LCTRL;
    if (keys[PAL_SCAN_RCTRL]) modifiers |= PAL_MOD_RCTRL;
    if (keys[PAL_SCAN_LALT]) modifiers |= PAL_MOD_LALT;
    if (keys[PAL_SCAN_RALT]) modifiers |= PAL_MOD_RALT;
    if (keys[PAL_SCAN_LGUI]) modifiers |= PAL_MOD_LSUPER;
    if (keys[PAL_SCAN_RGUI]) modifiers |= PAL_MOD_RSUPER;
    if (keys[PAL_SCAN_CAPSLOCK]) modifiers |= PAL_MOD_CAPS;
    if (keys[PAL_SCAN_NUMCLEAR]) modifiers |= PAL_MOD_NUM;
    if (keys[PAL_SCAN_SCROLLLOCK]) modifiers |= PAL_MOD_SCROLL;
    if (keys[PAL_SCAN_ALTGR]) modifiers |= PAL_MOD_ALTGR;
    
    return modifiers;
}

/* Update this in linux_x11_poll_raw_input() after processing keyboard events */
void linux_x11_poll_raw_input() {
    struct pollfd fds[MAX_KEYBOARDS + MAX_MICE + 1];
    int nfds = 0;

    /* Clear toggle flags */
    for (int i = 0; i < g_keyboards.count; i++)
        memset(g_keyboards.keys_toggled[i], 0, sizeof(g_keyboards.keys_toggled[i]));
    for (int i = 0; i < g_mice.count; i++) {
        memset(g_mice.buttons_toggled[i], 0, sizeof(g_mice.buttons_toggled[i]));
        g_mice.dx[i] = 0;
        g_mice.dy[i] = 0;
        g_mice.wheel[i] = 0;
    }

    /* Add keyboard fds */
    for (int i = 0; i < g_keyboards.count; i++) {
        fds[nfds].fd = g_keyboards.fds[i];
        fds[nfds].events = POLLIN;
        nfds++;
    }

    /* Add mouse fds */
    for (int i = 0; i < g_mice.count; i++) {
        fds[nfds].fd = g_mice.fds[i];
        fds[nfds].events = POLLIN;
        nfds++;
    }

    /* Add udev monitor */
    int monitor_fd = udev_monitor_get_fd(g_monitor);
    fds[nfds].fd = monitor_fd;
    fds[nfds].events = POLLIN;
    nfds++;

    int ret = poll(fds, nfds, 0);
    if (ret <= 0) return;

    struct input_event ev;
    pal_event event = {0};

    /* ---- Process keyboard events ---- */
    for (int i = 0; i < g_keyboards.count; i++) {
        uint32_t tmp_scancode = 0;

        while (read(fds[i].fd, &ev, sizeof(ev)) > 0) {
            if (ev.type == EV_MSC && ev.code == MSC_SCAN) {
                tmp_scancode = linux_scancode_to_pal_scancode[ev.value];
            }
            else if (ev.type == EV_KEY) {
                int pal_vk = linux_keycode_to_pal_virtual_key(ev.code);
                if (pal_vk == PAL_KEY_NONE) continue;

                uint32_t sc = tmp_scancode;
                unsigned char old_state = g_keyboards.keys[i][sc];
                unsigned char new_state = (ev.value > 0) ? 1 : 0; /* 0=released, 1=pressed, 2=repeat */
                int is_repeat = (ev.value == 2);

                g_keyboards.keys[i][sc] = new_state;

                /* Send key down/up event */
                if (old_state != new_state || is_repeat) {
                    if (!is_repeat) {
                        g_keyboards.keys_toggled[i][sc] = 1;
                    }

                    event.type = new_state ? PAL_EVENT_KEY_DOWN : PAL_EVENT_KEY_UP;
                    event.key.pressed = new_state;
                    event.key.scancode = sc;
                    event.key.virtual_key = pal_vk;
                    event.key.repeat = is_repeat;
                    event.key.keyboard_id = i;
                    g_keyboards.cached_modifiers[i] = compute_modifiers_from_key_state(g_keyboards.keys[i]);
                    event.key.modifiers = g_keyboards.cached_modifiers[i];
                    pal__eventq_push(&g_event_queue, event);

                    /* Generate text input event for printable characters */
                    if (new_state || is_repeat) {
                        char text[8] = {0};
                        if (linux_keycode_to_utf8(ev.code, g_keyboards.keys[i], text, sizeof(text))) {
                            event.type = PAL_EVENT_TEXT_INPUT;
                            strncpy(event.text.text, text, sizeof(event.text.text) - 1);
                            event.text.text[sizeof(event.text.text) - 1] = '\0';
                            event.text.keyboard_id = i;
                            pal__eventq_push(&g_event_queue, event);
                        }
                    }
                }
            }
            else if (ev.type == EV_SYN && ev.code == SYN_REPORT) {
                tmp_scancode = 0; /* reset per frame */
            }
        }
    }

    /* ---- Process mouse events ---- */
    for (int i = 0; i < g_mice.count; i++) {
        int motion_accumulated = 0;
        int wheel_accumulated = 0;

        while (read(fds[g_keyboards.count + i].fd, &ev, sizeof(ev)) > 0) {
            if (ev.type == EV_KEY) {
                int pal_button = linux_button_to_pal_button(ev.code);
                if (pal_button < 0 || pal_button >= MAX_MOUSE_BUTTONS) continue;

                int old_state = g_mice.buttons[i][pal_button];
                int new_state = ev.value;
                g_mice.buttons[i][pal_button] = new_state;

                if (old_state != new_state) {
                    g_mice.buttons_toggled[i][pal_button] = 1;

                    /* Combine modifiers from all keyboards */
                    int combined_modifiers = PAL_MOD_NONE;
                    for(int k = 0; k < g_keyboards.count; k++) {
                        combined_modifiers |= g_keyboards.cached_modifiers[k];
                    }

                    event.type = new_state ? PAL_EVENT_MOUSE_BUTTON_DOWN : PAL_EVENT_MOUSE_BUTTON_UP;
                    event.button.pressed = new_state;
                    event.button.button = pal_button;
                    event.button.clicks = 1;
                    event.button.mouse_id = i;
                    event.button.x = 0;
                    event.button.y = 0;
                    event.button.modifiers = combined_modifiers;
                    pal__eventq_push(&g_event_queue, event);
                }
            }
            else if (ev.type == EV_REL) {
                if (ev.code == REL_X) { g_mice.dx[i] += ev.value; motion_accumulated = 1; }
                if (ev.code == REL_Y) { g_mice.dy[i] += ev.value; motion_accumulated = 1; }
                if (ev.code == REL_WHEEL) { g_mice.wheel[i] += ev.value; wheel_accumulated = 1; }
            }
        }

        if (motion_accumulated) {
            event.type = PAL_EVENT_MOUSE_MOTION;
            event.motion.delta_x = g_mice.dx[i];
            event.motion.delta_y = g_mice.dy[i];
            event.motion.mouse_id = i;
            event.motion.x = 0;
            event.motion.y = 0;
            event.motion.buttons = 0;
            pal__eventq_push(&g_event_queue, event);
        }

        if (wheel_accumulated) {
            /* Combine modifiers from all keyboards */
            int combined_modifiers = PAL_MOD_NONE;
            for(int k = 0; k < g_keyboards.count; k++) {
                combined_modifiers |= g_keyboards.cached_modifiers[k];
            }

            event.type = PAL_EVENT_MOUSE_WHEEL;
            event.wheel.y = (float)g_mice.wheel[i];
            event.wheel.x = 0.0f;
            event.wheel.mouse_id = i;
            event.wheel.mouse_x = 0;
            event.wheel.mouse_y = 0;
            event.wheel.wheel_direction = PAL_MOUSEWHEEL_VERTICAL;
            event.wheel.modifiers = combined_modifiers;
            pal__eventq_push(&g_event_queue, event);
        }
    }

    /* ---- Check udev hotplug ---- */
    if (fds[nfds-1].revents & POLLIN) {
        struct udev_device *dev = udev_monitor_receive_device(g_monitor);
        if (!dev) return;

        const char *action = udev_device_get_action(dev);
        const char *devnode = udev_device_get_devnode(dev);
        if (devnode && action && strcmp(action, "add") == 0) {
            int dev_fd = open(devnode, O_RDONLY | O_NONBLOCK);
            if (dev_fd >= 0) {
                char name[256] = {0};
                ioctl(dev_fd, EVIOCGNAME(sizeof(name)), name);

                if (is_keyboard(dev_fd) && g_keyboards.count < MAX_KEYBOARDS) {
                    g_keyboards.fds[g_keyboards.count] = dev_fd;
                    strncpy(g_keyboards.names[g_keyboards.count], name, 255);
                    memset(g_keyboards.keys[g_keyboards.count], 0, MAX_KEYS);
                    memset(g_keyboards.keys_toggled[g_keyboards.count], 0, MAX_KEYS);
                    g_keyboards.cached_modifiers[g_keyboards.count] = PAL_MOD_NONE;
                    g_keyboards.count++;
                    printf("[Keyboard Added] %s\n", name);
                }
                else if (is_mouse(dev_fd) && g_mice.count < MAX_MICE) {
                    g_mice.fds[g_mice.count] = dev_fd;
                    strncpy(g_mice.names[g_mice.count], name, 255);
                    memset(g_mice.buttons[g_mice.count], 0, MAX_MOUSE_BUTTONS * sizeof(int));
                    memset(g_mice.buttons_toggled[g_mice.count], 0, MAX_MOUSE_BUTTONS * sizeof(int));
                    g_mice.dx[g_mice.count] = 0;
                    g_mice.dy[g_mice.count] = 0;
                    g_mice.wheel[g_mice.count] = 0;
                    g_mice.count++;
                    printf("[Mouse Added] %s\n", name);
                }
                else close(dev_fd);
            }
        }
        udev_device_unref(dev);
    }
}

/* Fallback function for when XKB is not available (US layout only) */
int linux_keycode_to_utf8_fallback(int keycode, unsigned char *key_state, char *out, size_t out_size) {
    if (out_size < 8) return 0;
    
    int pal_vk = linux_keycode_to_pal_virtual_key(keycode);
    if (pal_vk == PAL_KEY_NONE) return 0;
    
    /* Check modifier states */
    int shift = key_state[PAL_SCAN_LSHIFT] || key_state[PAL_SCAN_RSHIFT];
    int caps = key_state[PAL_SCAN_CAPSLOCK];
    int numlock = key_state[PAL_SCAN_NUMCLEAR];
    
    /* Handle letters (a-z) */
    if (pal_vk >= 'a' && pal_vk <= 'z') {
        char c = pal_vk;
        if (shift ^ caps) c -= 0x20; /* Convert to uppercase */
        out[0] = c;
        out[1] = '\0';
        return 1;
    }
    
    /* Handle digits with shift modifiers */
    if (pal_vk >= '0' && pal_vk <= '9') {
        if (shift) {
            const char symbols[] = ")!@#$%^&*(";
            out[0] = symbols[pal_vk - '0'];
        } else {
            out[0] = pal_vk;
        }
        out[1] = '\0';
        return 1;
    }
    
    /* Handle punctuation and symbols that match ASCII directly */
    switch (pal_vk) {
        /* Direct ASCII matches (no shift needed) */
        case PAL_KEY_SPACE:
        case PAL_KEY_TAB:
        case PAL_KEY_ENTER:
            out[0] = pal_vk;
            out[1] = '\0';
            return 1;
        
        /* Keys that change with shift */
        case PAL_KEY_MINUS:
            out[0] = shift ? '_' : '-';
            out[1] = '\0';
            return 1;
        case PAL_KEY_EQUAL:
            out[0] = shift ? '+' : '=';
            out[1] = '\0';
            return 1;
        case PAL_KEY_LEFTBRACE:
            out[0] = shift ? '{' : '[';
            out[1] = '\0';
            return 1;
        case PAL_KEY_RIGHTBRACE:
            out[0] = shift ? '}' : ']';
            out[1] = '\0';
            return 1;
        case PAL_KEY_BACKSLASH:
            out[0] = shift ? '|' : '\\';
            out[1] = '\0';
            return 1;
        case PAL_KEY_SEMICOLON:
            out[0] = shift ? ':' : ';';
            out[1] = '\0';
            return 1;
        case PAL_KEY_APOSTROPHE:
            out[0] = shift ? '"' : '\'';
            out[1] = '\0';
            return 1;
        case PAL_KEY_BACKTICK:
            out[0] = shift ? '~' : '`';
            out[1] = '\0';
            return 1;
        case PAL_KEY_COMMA:
            out[0] = shift ? '<' : ',';
            out[1] = '\0';
            return 1;
        case PAL_KEY_DOT:
            out[0] = shift ? '>' : '.';
            out[1] = '\0';
            return 1;
        case PAL_KEY_FORWARD_SLASH:
            out[0] = shift ? '?' : '/';
            out[1] = '\0';
            return 1;
        
        /* Numpad keys (always produce characters regardless of numlock) */
        case PAL_KEY_NUMPAD_DIVIDE:
            out[0] = '/';
            out[1] = '\0';
            return 1;
        case PAL_KEY_NUMPAD_MULTIPLY:
            out[0] = '*';
            out[1] = '\0';
            return 1;
        case PAL_KEY_NUMPAD_MINUS:
            out[0] = '-';
            out[1] = '\0';
            return 1;
        case PAL_KEY_NUMPAD_PLUS:
            out[0] = '+';
            out[1] = '\0';
            return 1;
        
        /* Numpad digits and period (only produce characters when numlock is on) */
        case PAL_KEY_NUMPAD_0:
            if (!numlock) return 0;
            out[0] = '0';
            out[1] = '\0';
            return 1;
        case PAL_KEY_NUMPAD_1:
            if (!numlock) return 0;
            out[0] = '1';
            out[1] = '\0';
            return 1;
        case PAL_KEY_NUMPAD_2:
            if (!numlock) return 0;
            out[0] = '2';
            out[1] = '\0';
            return 1;
        case PAL_KEY_NUMPAD_3:
            if (!numlock) return 0;
            out[0] = '3';
            out[1] = '\0';
            return 1;
        case PAL_KEY_NUMPAD_4:
            if (!numlock) return 0;
            out[0] = '4';
            out[1] = '\0';
            return 1;
        case PAL_KEY_NUMPAD_5:
            if (!numlock) return 0;
            out[0] = '5';
            out[1] = '\0';
            return 1;
        case PAL_KEY_NUMPAD_6:
            if (!numlock) return 0;
            out[0] = '6';
            out[1] = '\0';
            return 1;
        case PAL_KEY_NUMPAD_7:
            if (!numlock) return 0;
            out[0] = '7';
            out[1] = '\0';
            return 1;
        case PAL_KEY_NUMPAD_8:
            if (!numlock) return 0;
            out[0] = '8';
            out[1] = '\0';
            return 1;
        case PAL_KEY_NUMPAD_9:
            if (!numlock) return 0;
            out[0] = '9';
            out[1] = '\0';
            return 1;
        case PAL_KEY_NUMPAD_PERIOD:
            if (!numlock) return 0;
            out[0] = '.';
            out[1] = '\0';
            return 1;
    }
    
    /* Non-printable keys return 0 */
    return 0;
}
/* Helper function to convert Linux keycode to UTF-8 text */
/* Returns 1 if a printable character was generated, 0 otherwise */
int linux_keycode_to_utf8(int linux_keycode, unsigned char *key_state,
                          char *out, size_t out_size) {
    /* Fallback to your original function if X11 not available */
    if (!g_display || !g_xkb) {
        return linux_keycode_to_utf8_fallback(linux_keycode, key_state, out, out_size);
    }
    
    if (out_size < 8) return 0;
    
    /* Linux keycode to X11 keycode (offset by 8) */
    KeyCode x_keycode = linux_keycode + 8;
    
    /* Build modifier state */
    unsigned int modifiers = 0;
    if (key_state[PAL_SCAN_LSHIFT] || key_state[PAL_SCAN_RSHIFT])
        modifiers |= ShiftMask;
    if (key_state[PAL_SCAN_LCTRL] || key_state[PAL_SCAN_RCTRL])
        modifiers |= ControlMask;
    if (key_state[PAL_SCAN_LALT] || key_state[PAL_SCAN_RALT])
        modifiers |= Mod1Mask;
    if (key_state[PAL_SCAN_CAPSLOCK])
        modifiers |= LockMask;
    
    /* Translate using XKB */
    KeySym keysym;
    int shift_level = (modifiers & ShiftMask) ? 1 : 0;
    keysym = XkbKeycodeToKeysym(g_display, x_keycode, 0, shift_level);
    
    if (keysym == NoSymbol) return 0;
    
    /* Try IME-aware lookup first */
    if (g_xic) {
        XKeyEvent event = {
            .type = KeyPress,
            .display = g_display,
            .keycode = x_keycode,
            .state = modifiers
        };
        
        Status status;
        int len = Xutf8LookupString(g_xic, &event, out, out_size - 1, 
                                     &keysym, &status);
        
        if (status == XLookupChars || status == XLookupBoth) {
            out[len] = '\0';
            return len > 0 ? 1 : 0;
        }
    }
    
    /* Fallback: direct keysym to UTF-8 */
    int len = XkbTranslateKeySym(g_display, &keysym, modifiers, 
                                  out, out_size - 1, NULL);
    
    if (len > 0) {
        out[len] = '\0';
        return 1;
    }
    
    return 0;
}

PALAPI pal_window *pal_create_window(int width, int height, const char *window_title, uint64_t flags) {
    pal_window *window = (pal_window*)malloc(sizeof(pal_window));
    if (!window) return NULL;

    int screen = DefaultScreen(g_display);

    /* --- Default graphics API to OpenGL if none specified --- */
    if (!(flags & (PAL_WINDOW_OPENGL | PAL_WINDOW_VULKAN | PAL_WINDOW_METAL))) {
        flags |= PAL_WINDOW_OPENGL;
    }

    GLXFBConfig fb = 0;
    XVisualInfo visual_info;
    Colormap colormap = 0;

    /* --- OpenGL path --- */
    if (flags & PAL_WINDOW_OPENGL) {
        int fb_attribs[] = {
            GLX_X_RENDERABLE, True,
            GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
            GLX_RENDER_TYPE,   GLX_RGBA_BIT,
            GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
            GLX_RED_SIZE,      8,
            GLX_GREEN_SIZE,    8,
            GLX_BLUE_SIZE,     8,
            GLX_ALPHA_SIZE,    8,
            GLX_DEPTH_SIZE,    24,
            GLX_STENCIL_SIZE,  8,
            GLX_DOUBLEBUFFER,  True,
            None
        };

        int fb_count = 0;
        GLXFBConfig *fb_configs = glXChooseFBConfig(g_display, screen, fb_attribs, &fb_count);
        if (!fb_configs || fb_count == 0) {
            fprintf(stderr, "GLX ERROR: No framebuffer configs found!\n");
            if (fb_configs) XFree(fb_configs);
            free(window);
            return NULL;
        }

        fb = fb_configs[0];
        XVisualInfo *vi = glXGetVisualFromFBConfig(g_display, fb);
        if (!vi) {
            fprintf(stderr, "GLX ERROR: Failed to get XVisualInfo from FBConfig!\n");
            XFree(fb_configs);
            free(window);
            return NULL;
        }

        visual_info = *vi;
        XFree(vi);
        XFree(fb_configs);

        colormap = XCreateColormap(g_display, RootWindow(g_display, screen),
                                   visual_info.visual, AllocNone);
    } else {
        /* Non-OpenGL graphics APIs (unimplemented) */
        visual_info.visual = DefaultVisual(g_display, screen);
        visual_info.depth  = DefaultDepth(g_display, screen);
        colormap = XCreateColormap(g_display, RootWindow(g_display, screen),
                                   visual_info.visual, AllocNone);
    }

    /* --- Create the X11 window --- */
    XSetWindowAttributes swa;
    swa.colormap = colormap;
    swa.override_redirect = False;
    swa.background_pixel = WhitePixel(g_display, screen);
    swa.border_pixel = BlackPixel(g_display, screen);
    swa.event_mask = ExposureMask | KeyPressMask | StructureNotifyMask;

    unsigned long valuemask = CWColormap | CWBackPixel | CWBorderPixel | CWEventMask | CWOverrideRedirect;

    window->window = XCreateWindow(g_display, RootWindow(g_display, screen),
                                   0, 0, width, height, 0,
                                   visual_info.depth,
                                   InputOutput,
                                   visual_info.visual,
                                   valuemask, &swa);
    if (!window->window) {
        fprintf(stderr, "XCreateWindow failed!\n");
        XFreeColormap(g_display, colormap);
        free(window);
        return NULL;
    }

    XStoreName(g_display, window->window, window_title);

    /* --- Optional: set class hints, WM protocols, etc. --- */
    XClassHint *class_hint = XAllocClassHint();
    if (class_hint) {
        class_hint->res_name = "my_app";
        class_hint->res_class = "MyApp";
        XSetClassHint(g_display, window->window, class_hint);
        XFree(class_hint);
    }
    XSetWMProtocols(g_display, window->window, &g_wm_delete, 1);

    /* --- Create OpenGL context if requested --- */
    window->gl_context = NULL;
    if (flags & PAL_WINDOW_OPENGL) {
        PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB =
            (void*)glXGetProcAddressARB((const GLubyte*)"glXCreateContextAttribsARB");

        int context_attribs[] = {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
            GLX_CONTEXT_MINOR_VERSION_ARB, 3,
            GLX_CONTEXT_PROFILE_MASK_ARB,  GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
            None
        };

        if (glXCreateContextAttribsARB) {
            window->gl_context = glXCreateContextAttribsARB(g_display, fb, NULL, True, context_attribs);
        }

        if (!window->gl_context) {
            window->gl_context = glXCreateNewContext(g_display, fb, GLX_RGBA_TYPE, NULL, True);
        }

        if (!window->gl_context) {
            fprintf(stderr, "Failed to create OpenGL context!\n");
            XDestroyWindow(g_display, window->window);
            XFreeColormap(g_display, colormap);
            free(window);
            return NULL;
        }
    }

    /* --- Map window --- */
    XMapWindow(g_display, window->window);

    /* --- Optional GC --- */
    window->graphics_context = XCreateGC(g_display, window->window, 0, NULL);

    return window;
}

PALAPI void pal_close_window(pal_window *window) {
    if(window->graphics_context) {
        XFreeGC(g_display, window->graphics_context);
    }
    if(window->window) {
        XDestroyWindow(g_display, window->window);
    }

    pal_event event = {0};
    event.type = PAL_EVENT_WINDOW_CLOSED;
    pal__eventq_push(&g_event_queue, event);
}

static void linux_x11_send_wm_state_message(Window win, long action, Atom property) {
    XEvent e;
    memset(&e, 0, sizeof(e));
    e.xclient.type = ClientMessage;
    e.xclient.serial = 0;
    e.xclient.send_event = True;
    e.xclient.message_type = XInternAtom(g_display, "_NET_WM_STATE", False);
    e.xclient.window = win;
    e.xclient.format = 32;
    e.xclient.data.l[0] = action;      /* 1 = add, 0 = remove, 2 = toggle */
    e.xclient.data.l[1] = property;    /* _NET_WM_STATE_MAXIMIZED_VERT or HORZ */
    e.xclient.data.l[2] = 0;           /* second property (unused) */
    e.xclient.data.l[3] = 1;           /* source: application */
    e.xclient.data.l[4] = 0;

    XSendEvent(
        g_display,
        DefaultRootWindow(g_display),
        False,
        SubstructureNotifyMask | SubstructureRedirectMask,
        &e
    );
}

PALAPI pal_bool pal_set_window_title(pal_window *window, const char *string) {
    if (!window || !string) return pal_false;

    XStoreName(g_display, window->window, string);
    XSetIconName(g_display, window->window, string);
    XFlush(g_display);

    return pal_true;
}

PALAPI pal_bool pal_make_window_fullscreen(pal_window *window) {
    if (!window) return pal_false;

    Atom fullscreen = XInternAtom(g_display, "_NET_WM_STATE_FULLSCREEN", False);
    linux_x11_send_wm_state_message(window->window, 1, fullscreen);

    XFlush(g_display);
    return pal_true;
}

PALAPI pal_bool pal_make_window_fullscreen_ex(pal_window *window,
                                              int width,
                                              int height,
                                              int refresh_rate) {
    if (!window) return pal_false;

    Window root = DefaultRootWindow(g_display);

    XRRScreenResources *res = XRRGetScreenResources(g_display, root);
    if (!res) return pal_false;

    RROutput primary = XRRGetOutputPrimary(g_display, root);
    if (!primary)
        primary = res->outputs[0];

    XRROutputInfo *out = XRRGetOutputInfo(g_display, res, primary);
    if (!out) {
        XRRFreeScreenResources(res);
        return pal_false;
    }

    XRRCrtcInfo *crtc = XRRGetCrtcInfo(g_display, res, out->crtc);
    if (!crtc) {
        XRRFreeOutputInfo(out);
        XRRFreeScreenResources(res);
        return pal_false;
    }

    /* Find matching mode */
    RRMode selected_mode = 0;
    for (int i = 0; i < res->nmode; i++) {
        XRRModeInfo *m = &res->modes[i];

        int mode_refresh =
            (int)(m->dotClock / (m->hTotal * m->vTotal));

        if (m->width == (unsigned int)width &&
            m->height == (unsigned int)height &&
            mode_refresh == refresh_rate)
        {
            selected_mode = m->id;
            break;
        }
    }

    if (!selected_mode) {
        XRRFreeCrtcInfo(crtc);
        XRRFreeOutputInfo(out);
        XRRFreeScreenResources(res);
        return pal_false;
    }

    /* Apply the mode */
    XRRSetCrtcConfig(
        g_display,
        res,
        out->crtc,
        CurrentTime,
        crtc->x, crtc->y,
        selected_mode,
        crtc->rotation,
        &primary,
        1
    );

    XRRFreeCrtcInfo(crtc);
    XRRFreeOutputInfo(out);
    XRRFreeScreenResources(res);

    /* Then set fullscreen */
    pal_make_window_fullscreen(window);

    return pal_true;
}

PALAPI pal_bool pal_make_window_fullscreen_windowed(pal_window *window)
{
    if (!window) return pal_false;

    Atom wmHints = XInternAtom(g_display, "_MOTIF_WM_HINTS", False);

    /* Remove decorations using Motif WM hints */
    struct {
        unsigned long flags;
        unsigned long functions;
        unsigned long decorations;
        long inputMode;
        unsigned long status;
    } hints;

    hints.flags = (1L << 1); /* MWM_HINTS_DECORATIONS */
    hints.decorations = 0;   /* No decorations */

    XChangeProperty(
        g_display,
        window->window,
        wmHints,
        wmHints,
        32,
        PropModeReplace,
        (unsigned char*)&hints,
        5
    );

    /* Resize window to match screen size */
    int sw = DisplayWidth(g_display, DefaultScreen(g_display));
    int sh = DisplayHeight(g_display, DefaultScreen(g_display));

    XMoveResizeWindow(g_display, window->window, 0, 0, sw, sh);

    XFlush(g_display);
    return pal_true;
}

PALAPI pal_bool pal_make_window_windowed(pal_window *window) {
    if (!window) return pal_false;

    Atom wmHints = XInternAtom(g_display, "_MOTIF_WM_HINTS", False);

    struct {
        unsigned long flags;
        unsigned long functions;
        unsigned long decorations;
        long inputMode;
        unsigned long status;
    } hints;

    /* Restore decorations */
    hints.flags = (1L << 1);
    hints.decorations = 1;  /* enable */

    XChangeProperty(
        g_display,
        window->window,
        wmHints,
        wmHints,
        32,
        PropModeReplace,
        (unsigned char *)&hints,
        5
    );

    /* Remove fullscreen state (if any) */
    Atom fullscreen = XInternAtom(g_display, "_NET_WM_STATE_FULLSCREEN", False);
    linux_x11_send_wm_state_message(window->window, 0, fullscreen);

    XFlush(g_display);
    return pal_true;
}
PALAPI pal_bool pal_maximize_window(pal_window *window)
{
    if (!window || !g_display)
        return pal_false;

    Atom max_vert = XInternAtom(g_display, "_NET_WM_STATE_MAXIMIZED_VERT", False);
    Atom max_horz = XInternAtom(g_display, "_NET_WM_STATE_MAXIMIZED_HORZ", False);

    /* action = 1 → add */
    linux_x11_send_wm_state_message(window->window, 1, max_vert);
    linux_x11_send_wm_state_message(window->window, 1, max_horz);

    XFlush(g_display);
    return pal_true;
}

PALAPI pal_bool pal_minimize_window(pal_window *window)
{
    if (!window || !g_display)
        return pal_false;

    /* Minimize is via _NET_ACTIVE_WINDOW with a special request */
    XIconifyWindow(
        g_display,
        window->window,
        DefaultScreen(g_display)
    );

    XFlush(g_display);
    return pal_true;
}

PALAPI void pal_draw_rect(pal_window *window, int x, int y, int width, int height, pal_vec4 color) {
    if (!window) return;

    /* Convert float color (0.0 - 1.0) to 24-bit RGB */
    unsigned long pixel = ((unsigned long)(color.r * 255) << 16) |
                          ((unsigned long)(color.g * 255) << 8)  |
                          ((unsigned long)(color.b * 255) << 0);

    XSetForeground(g_display, window->graphics_context, pixel);
    XFillRectangle(g_display, window->window, window->graphics_context, x, y, width, height);
}

/* Helper to intern an atom */
PALAPI int pal_make_context_current(pal_window *window) {
    if (window && window->gl_context) {
        if (!glXMakeCurrent(g_display, window->window, window->gl_context)) {
            fprintf(stderr, "GLX ERROR: glXMakeCurrent failed!\n");
            return -1;
        }
    } else {
        glXMakeCurrent(g_display, None, NULL); /* unbind context */
    }
    return 0;
}

PALAPI void pal_swap_buffers(pal_window *window) {
    glXSwapBuffers(g_display, window->window);
}

PALAPI int pal_show_cursor(pal_window *window) {
    int event_base, error_base;
    if (!XFixesQueryExtension(g_display, &event_base, &error_base)) {
        fprintf(stderr, "XFixes extension not available.\n");
        return -1;
    }

    XFixesShowCursor(g_display, window->window);
    XFlush(g_display);
    return 0;
}

PALAPI int pal_hide_cursor(pal_window *window) {
    int event_base, error_base;
    if (!XFixesQueryExtension(g_display, &event_base, &error_base)) {
        fprintf(stderr, "XFixes extension not available.\n");
        return -1;
    }

    XFixesHideCursor(g_display, window->window);
    XFlush(g_display);
    return 0;
}

PALAPI pal_monitor *pal_get_primary_monitor(void) {
    return NULL;
}

PALAPI pal_bool pal_set_video_mode(pal_video_mode *mode) {
    if (!mode) return pal_false;

    if (!g_display) {
        fprintf(stderr, "Cannot open X display\n");
        return pal_false;
    }

    Window root = DefaultRootWindow(g_display);
    XRRScreenResources *res = XRRGetScreenResourcesCurrent(g_display, root);
    if (!res) {
        XCloseDisplay(g_display);
        return pal_false;
    }

    /* Pick the first connected output */
    RROutput output = 0;
    for (int i = 0; i < res->noutput; i++) {
        XRROutputInfo *outInfo = XRRGetOutputInfo(g_display, res, res->outputs[i]);
        if (outInfo->connection == RR_Connected) {
            output = res->outputs[i];
            XRRFreeOutputInfo(outInfo);
            break;
        }
        XRRFreeOutputInfo(outInfo);
    }

    if (output == 0) {
        XRRFreeScreenResources(res);
        XCloseDisplay(g_display);
        return pal_false;
    }

    XRROutputInfo *outInfo = XRRGetOutputInfo(g_display, res, output);
    if (!outInfo || outInfo->crtc == 0) {
        if (outInfo) XRRFreeOutputInfo(outInfo);
        XRRFreeScreenResources(res);
        XCloseDisplay(g_display);
        return pal_false;
    }

    XRRCrtcInfo *crtcInfo = XRRGetCrtcInfo(g_display, res, outInfo->crtc);
    if (!crtcInfo) {
        XRRFreeOutputInfo(outInfo);
        XRRFreeScreenResources(res);
        XCloseDisplay(g_display);
        return pal_false;
    }

    /* Find a mode that matches width, height, and refresh rate */
    RRMode newMode = 0;
    for (int i = 0; i < res->nmode; i++) {
        XRRModeInfo *m = &res->modes[i];
        int hRate = (m->hTotal && m->vTotal) ? (int)((double)m->dotClock / (m->hTotal * m->vTotal) + 0.5) : 0;
        if (m->width == (unsigned int)mode->width && m->height == (unsigned int)mode->height &&
            (mode->refresh_rate == 0 || hRate == mode->refresh_rate)) {
            newMode = m->id;
            break;
        }
    }

    if (newMode == 0) {
        fprintf(stderr, "No matching mode found\n");
        XRRFreeCrtcInfo(crtcInfo);
        XRRFreeOutputInfo(outInfo);
        XRRFreeScreenResources(res);
        XCloseDisplay(g_display);
        return pal_false;
    }

    /* Apply the new mode */
    Status status = XRRSetCrtcConfig(g_display, res, outInfo->crtc, CurrentTime,
                                     crtcInfo->x, crtcInfo->y,
                                     newMode, crtcInfo->rotation,
                                     &output, 1);

    XRRFreeCrtcInfo(crtcInfo);
    XRRFreeOutputInfo(outInfo);
    XRRFreeScreenResources(res);
    XCloseDisplay(g_display);

    return (status == RRSetConfigSuccess) ? pal_true : pal_false;
}

PALAPI pal_video_mode *pal_get_video_mode(pal_monitor *monitor) {
    if (!monitor || !monitor->display) return NULL;

    Window root = DefaultRootWindow(g_display);
    XRRScreenResources *screenRes = XRRGetScreenResourcesCurrent(g_display, root);
    if (!screenRes) return NULL;

    XRROutputInfo *outputInfo = XRRGetOutputInfo(g_display, screenRes, monitor->output);
    if (!outputInfo || outputInfo->connection != RR_Connected || outputInfo->crtc == 0) {
        if (outputInfo) XRRFreeOutputInfo(outputInfo);
        XRRFreeScreenResources(screenRes);
        return NULL;
    }

    XRRCrtcInfo *crtcInfo = XRRGetCrtcInfo(g_display, screenRes, outputInfo->crtc);
    XRRModeInfo *modeInfo = NULL;
    for (int i = 0; i < screenRes->nmode; i++) {
        if (screenRes->modes[i].id == crtcInfo->mode) {
            modeInfo = &screenRes->modes[i];
            break;
        }
    }

    pal_video_mode *mode = malloc(sizeof(pal_video_mode));
    if (!mode) {
        XRRFreeCrtcInfo(crtcInfo);
        XRRFreeOutputInfo(outputInfo);
        XRRFreeScreenResources(screenRes);
        return NULL;
    }

    mode->width  = crtcInfo->width;
    mode->height = crtcInfo->height;
    mode->refresh_rate = (modeInfo && modeInfo->hTotal && modeInfo->vTotal) ?
        (int)((double)modeInfo->dotClock / (modeInfo->hTotal * modeInfo->vTotal) + 0.5) : 0;

    XWindowAttributes attrs;
    if (XGetWindowAttributes(g_display, root, &attrs)) {
        mode->bits_per_pixel = attrs.depth;
    } else {
        mode->bits_per_pixel = 0;
    }

    XRRFreeCrtcInfo(crtcInfo);
    XRRFreeOutputInfo(outputInfo);
    XRRFreeScreenResources(screenRes);

    return mode;
}

PALAPI void *pal_gl_get_proc_address(const char *procname) {
    return (void *)glXGetProcAddress((const GLubyte *)procname);
}

PALAPI void *pal_load_dynamic_library(const char *dll) {
    void *lib = dlopen(dll, RTLD_NOW | RTLD_LOCAL);
    if (!lib) {
        fprintf(stderr, "dlopen failed: %s\n", dlerror());
        assert(0 && "Failed to load shared library");
    }
    return lib;
}

PALAPI void *pal_load_dynamic_function(void *dll, char *func_name) {
    dlerror(); /* Clear existing errors */
    void *symbol = dlsym(dll, func_name);
    const char *error = dlerror();
    if (error) {
        fprintf(stderr, "dlsym failed: %s\n", error);
        assert(0 && "Failed to load function from shared library");
    }
    return symbol;
}

PALAPI pal_bool pal_free_dynamic_library(void *dll) {
    int result = dlclose(dll);
    if (result != 0) {
        fprintf(stderr, "dlclose failed: %s\n", dlerror());
        assert(0 && "Failed to unload shared library");
    }
    return (uint8_t)(result == 0);
}
#endif /* PLATFORM_LINUX_X11_H */

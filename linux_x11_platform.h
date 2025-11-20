#ifndef LINUX_X11_PLATFORM_H
#define LINUX_X11_PLATFORM_H

// C standard lib
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

// X11
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/Xrandr.h>

// Opengl
#include <glad/glad.h>
#include <GL/gl.h>
#include <GL/glx.h>

// dlopen/dlsym/dlclose
#include <dlfcn.h>

// linux-specific headers
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
    // Core audio data
    unsigned char* data; // Raw PCM audio data (initial buffer)
    size_t data_size;    // Size in bytes of initial buffer
    int sample_rate;     // Samples per second (e.g., 44100)
    int channels;        // Number of audio channels (e.g., 2 for stereo)
    int bits_per_sample; // Usually 16 or 32
    int is_float;        // 0 = PCM, 1 = IEEE float

    // pulseaudio

    // Streaming - OGG
    void* decoder;  // stb_vorbis* (using void* to avoid header dependency)
    char* filename; // Filename for reopening OGG decoder

    // Streaming - WAV
    FILE* source_file;
    size_t total_data_size; // Total size of audio data in file
    size_t bytes_streamed;  // How many bytes we've read so far
    size_t data_offset;     // Offset in file where audio data starts

    // Streaming control
    float preload_seconds; // How many seconds were preloaded
    int is_streaming;      // 1 if this is a streaming sound
    int stream_finished;   // 1 when streaming is complete
};

typedef struct pal_monitor {
    Display *display;
    RROutput output;
} pal_monitor;

Display *g_display = NULL;
Atom g_wm_delete = 0;


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


PALAPI void pal_shutdown() {
    if(g_display) {
        XCloseDisplay(g_display);
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
            
            // Window moved
            event.type = PAL_EVENT_WINDOW_MOVED;
            event.window.windowid = (uint32_t)ce->window;
            event.window.x = ce->x;
            event.window.y = ce->y;
            pal__eventq_push(&g_event_queue, event);
            
            // Window resized
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
            if (xevent->xexpose.count == 0) { // Only on last expose event
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
    if (!g_message_pump_drained) {
        linux_x11_poll_raw_input();

        while (XPending(g_display) > 0) {
            XEvent xevent;
            XNextEvent(g_display, &xevent);
            linux_x11_translate_event(&xevent);
        }
        g_message_pump_drained = pal_true;
    }

    pal_event_queue* queue = &g_event_queue;

    if (queue->size) {
        *event = queue->events[queue->front];
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
#define MAX_MOUSE_BUTTONS 8

typedef struct {
    int fds[MAX_KEYBOARDS];               // fds of keyboards
    char names[MAX_KEYBOARDS][256];       // names of keyboards
    int count;                             // number of keyboards
    unsigned char keys[MAX_KEYBOARDS][MAX_KEYS];         // per-keyboard key states
    unsigned char keys_toggled[MAX_KEYBOARDS][MAX_KEYS]; // per-keyboard toggle tracking
} pal_keyboard_state;

typedef struct {
    int fds[MAX_MICE];                     // fds of mice
    char names[MAX_MICE][256];             // names of mice
    int count;
    int buttons[MAX_MICE][MAX_MOUSE_BUTTONS];        // per-mouse button states
    int buttons_toggled[MAX_MICE][MAX_MOUSE_BUTTONS]; // per-mouse toggle tracking
    int dx[MAX_MICE];                      // per-mouse accumulated movement
    int dy[MAX_MICE];
    int wheel[MAX_MICE];                             
} pal_mouse_state;

pal_keyboard_state g_keyboards = {0};
pal_mouse_state g_mice = {0};

// udev context and monitor
struct udev *g_udev = NULL;
struct udev_monitor *g_monitor = NULL;

// -------- Helpers --------
int is_keyboard(int fd) {
    unsigned long evbits[(EV_MAX+7)/8] = {0};
    ioctl(fd, EVIOCGBIT(0, EV_MAX), evbits);
    
    int has_key = (evbits[EV_KEY/8] & (1 << (EV_KEY % 8))) != 0;
    int has_rel = (evbits[EV_REL/8] & (1 << (EV_REL % 8))) != 0;
    int has_abs = (evbits[EV_ABS/8] & (1 << (EV_ABS % 8))) != 0;
    
    // It's a keyboard if it has keys but NOT mouse/touchpad properties
    return has_key && !has_rel && !has_abs;
}

int is_mouse(int fd) {
    unsigned long evbits[(EV_MAX+7)/8] = {0};
    ioctl(fd, EVIOCGBIT(0, EV_MAX), evbits);
    
    int has_rel = (evbits[EV_REL/8] & (1 << (EV_REL % 8))) != 0;
    int has_abs = (evbits[EV_ABS/8] & (1 << (EV_ABS % 8))) != 0;
    
    // It's a mouse/touchpad if it has relative motion OR absolute positioning
    return has_rel || has_abs;
}
// -------- Initialization --------
void linux_x11_init_raw_input() {
    g_udev = udev_new();
    if (!g_udev) {
        fprintf(stderr, "Failed to create udev context\n");
        exit(1);
    }

    g_monitor = udev_monitor_new_from_netlink(g_udev, "udev");
    udev_monitor_filter_add_match_subsystem_devtype(g_monitor, "input", NULL);
    udev_monitor_enable_receiving(g_monitor);

    // Enumerate existing devices
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
            g_keyboards.count++;
        } else if (is_mouse(dev_fd) && g_mice.count < MAX_MICE) {
            g_mice.fds[g_mice.count] = dev_fd;
            strncpy(g_mice.names[g_mice.count], name, 255);
            g_mice.count++;
        } else {
            close(dev_fd);
        }

        udev_device_unref(dev);
    }
    udev_enumerate_unref(enumerate);
}

static int linux_button_to_pal_button(int linux_code) {
    switch(linux_code) {
        case BTN_LEFT:    return PAL_MOUSE_LEFT;    // 0x0
        case BTN_RIGHT:   return PAL_MOUSE_RIGHT;   // 0x1
        case BTN_MIDDLE:  return PAL_MOUSE_MIDDLE;  // 0x2
        case BTN_SIDE:    return PAL_MOUSE_4;       // 0x3 (also known as "back")
        case BTN_EXTRA:   return PAL_MOUSE_5;       // 0x4 (also known as "forward")
        case BTN_FORWARD: return PAL_MOUSE_5;       // 0x4 (alternative code for forward)
        case BTN_BACK:    return PAL_MOUSE_4;       // 0x3 (alternative code for back)
        default:          return -1;
    }
}

void linux_x11_poll_raw_input() {
    struct pollfd fds[MAX_KEYBOARDS + MAX_MICE + 1];
    int nfds = 0;

    // Clear toggle flags at the start of each poll
    for (int i = 0; i < g_keyboards.count; i++) {
        memset(g_keyboards.keys_toggled[i], 0, sizeof(g_keyboards.keys_toggled[i]));
    }
    for (int i = 0; i < g_mice.count; i++) {
        memset(g_mice.buttons_toggled[i], 0, sizeof(g_mice.buttons_toggled[i]));
        g_mice.dx[i] = 0;
        g_mice.dy[i] = 0;
        g_mice.wheel[i] = 0;
    }

    // Add keyboard fds
    for (int i = 0; i < g_keyboards.count; i++) {
        fds[nfds].fd = g_keyboards.fds[i];
        fds[nfds].events = POLLIN;
        nfds++;
    }

    // Add mouse fds
    for (int i = 0; i < g_mice.count; i++) {
        fds[nfds].fd = g_mice.fds[i];
        fds[nfds].events = POLLIN;
        nfds++;
    }

    // Add udev monitor
    int monitor_fd = udev_monitor_get_fd(g_monitor);
    fds[nfds].fd = monitor_fd;
    fds[nfds].events = POLLIN;
    nfds++;

    int ret = poll(fds, nfds, 0);
    if (ret <= 0) return;

    struct input_event ev;
    int index = 0;
    pal_event event = {0};

// Mapping table from Linux input key codes to PAL virtual key codes
static const int linux_keycode_to_pal_vk[KEY_MAX] = {
    [KEY_ESC]           = PAL_ESCAPE,
    [KEY_1]             = PAL_1,
    [KEY_2]             = PAL_2,
    [KEY_3]             = PAL_3,
    [KEY_4]             = PAL_4,
    [KEY_5]             = PAL_5,
    [KEY_6]             = PAL_6,
    [KEY_7]             = PAL_7,
    [KEY_8]             = PAL_8,
    [KEY_9]             = PAL_9,
    [KEY_0]             = PAL_0,
    [KEY_MINUS]         = PAL_MINUS,
    [KEY_EQUAL]         = PAL_EQUAL,
    [KEY_BACKSPACE]     = PAL_BACKSPACE,
    [KEY_TAB]           = PAL_TAB,
    [KEY_Q]             = PAL_Q,
    [KEY_W]             = PAL_W,
    [KEY_E]             = PAL_E,
    [KEY_R]             = PAL_R,
    [KEY_T]             = PAL_T,
    [KEY_Y]             = PAL_Y,
    [KEY_U]             = PAL_U,
    [KEY_I]             = PAL_I,
    [KEY_O]             = PAL_O,
    [KEY_P]             = PAL_P,
    [KEY_LEFTBRACE]     = PAL_LEFTBRACE,  // VK_OEM_4 '['
    [KEY_RIGHTBRACE]    = PAL_RIGHTBRACE,  // VK_OEM_6 ']'
    [KEY_ENTER]         = PAL_ENTER,
    [KEY_LEFTCTRL]      = PAL_CONTROL,
    [KEY_A]             = PAL_A,
    [KEY_S]             = PAL_S,
    [KEY_D]             = PAL_D,
    [KEY_F]             = PAL_F,
    [KEY_G]             = PAL_G,
    [KEY_H]             = PAL_H,
    [KEY_J]             = PAL_J,
    [KEY_K]             = PAL_K,
    [KEY_L]             = PAL_L,
    [KEY_SEMICOLON]     = PAL_SEMICOLON,  // VK_OEM_1 ';'
    [KEY_APOSTROPHE]    = PAL_APOSTROPHE,  // VK_OEM_7 '''
    [KEY_GRAVE]         = PAL_BACKTICK,  // VK_OEM_3 '`'
    [KEY_LEFTSHIFT]     = PAL_SHIFT,
    [KEY_BACKSLASH]     = PAL_BACKSLASH,  // VK_OEM_5 '\'
    [KEY_Z]             = PAL_Z,
    [KEY_X]             = PAL_X,
    [KEY_C]             = PAL_C,
    [KEY_V]             = PAL_V,
    [KEY_B]             = PAL_B,
    [KEY_N]             = PAL_N,
    [KEY_M]             = PAL_M,
    [KEY_COMMA]         = PAL_COMMA,  // VK_OEM_COMMA
    [KEY_DOT]           = PAL_DOT,  // VK_OEM_PERIOD
    [KEY_SLASH]         = PAL_FORWARD_SLASH,  // VK_OEM_2 '/'
    [KEY_RIGHTSHIFT]    = PAL_SHIFT,
    [KEY_KPASTERISK]    = PAL_MULTIPLY,
    [KEY_LEFTALT]       = PAL_ALT,
    [KEY_SPACE]         = PAL_SPACE,
    [KEY_CAPSLOCK]      = PAL_CAPSLOCK,
    [KEY_F1]            = PAL_F1,
    [KEY_F2]            = PAL_F2,
    [KEY_F3]            = PAL_F3,
    [KEY_F4]            = PAL_F4,
    [KEY_F5]            = PAL_F5,
    [KEY_F6]            = PAL_F6,
    [KEY_F7]            = PAL_F7,
    [KEY_F8]            = PAL_F8,
    [KEY_F9]            = PAL_F9,
    [KEY_F10]           = PAL_F10,
    [KEY_NUMLOCK]       = PAL_NUMLOCK,
    [KEY_SCROLLLOCK]    = PAL_SCROLLLOCK,
    [KEY_KP7]           = PAL_NUMPAD_7,
    [KEY_KP8]           = PAL_NUMPAD_8,
    [KEY_KP9]           = PAL_NUMPAD_9,
    [KEY_KPMINUS]       = PAL_SUBTRACT,
    [KEY_KP4]           = PAL_NUMPAD_4,
    [KEY_KP5]           = PAL_NUMPAD_5,
    [KEY_KP6]           = PAL_NUMPAD_6,
    [KEY_KPPLUS]        = PAL_ADD,
    [KEY_KP1]           = PAL_NUMPAD_1,
    [KEY_KP2]           = PAL_NUMPAD_2,
    [KEY_KP3]           = PAL_NUMPAD_3,
    [KEY_KP0]           = PAL_NUMPAD_0,
    [KEY_KPDOT]         = PAL_DECIMAL,
    [KEY_F11]           = PAL_F11,
    [KEY_F12]           = PAL_F12,
    [KEY_KPENTER]       = PAL_ENTER,
    [KEY_RIGHTCTRL]     = PAL_CONTROL,
    [KEY_KPSLASH]       = PAL_DIVIDE,
    [KEY_SYSRQ]         = PAL_PRINTSCREEN,
    [KEY_RIGHTALT]      = PAL_ALT,
    [KEY_HOME]          = PAL_HOME,
    [KEY_UP]            = PAL_UP,
    [KEY_PAGEUP]        = PAL_PAGEUP,
    [KEY_LEFT]          = PAL_LEFT,
    [KEY_RIGHT]         = PAL_RIGHT,
    [KEY_END]           = PAL_END,
    [KEY_DOWN]          = PAL_DOWN,
    [KEY_PAGEDOWN]      = PAL_PAGEDOWN,
    [KEY_INSERT]        = PAL_INSERT,
    [KEY_DELETE]        = PAL_DELETE,
    [KEY_PAUSE]         = PAL_PAUSE,
    [KEY_LEFTMETA]      = PAL_LWIN,
    [KEY_RIGHTMETA]     = PAL_RWIN,
    [KEY_MENU]          = PAL_APPS,
};

// Helper function to convert Linux key code to PAL virtual key
int linux_keycode_to_pal_virtual_key(int linux_code) {
    if (linux_code < 0 || linux_code >= KEY_MAX) {
        return PAL_KEY_NONE;
    }
    return linux_keycode_to_pal_vk[linux_code];
}
    // ---- Process keyboard events ----
for (int i = 0; i < g_keyboards.count; i++, index++) {
    while (read(fds[index].fd, &ev, sizeof(ev)) > 0) {
        if (ev.type == EV_KEY && ev.code < MAX_KEYS) {

            pal_key key = linux_keycode_to_pal_virtual_key(ev.code);
            if (key == PAL_KEY_NONE)
                continue;

            unsigned char old_state = g_keyboards.keys[i][key];
            unsigned char new_state = ev.value;

            g_keyboards.keys[i][key] = new_state;

            if (old_state != new_state) {
                g_keyboards.keys_toggled[i][key] = 1;

                event.type = new_state ? PAL_EVENT_KEY_DOWN : PAL_EVENT_KEY_UP;
                event.key.pressed     = new_state;
                event.key.scancode    = key;  // store pal_key, not ev.code
                event.key.virtual_key = key;  // if virtual_key == pal_key
                event.key.repeat      = 0;
                event.key.keyboard_id = i;
                event.key.modifiers   = PAL_MOD_NONE;

                pal__eventq_push(&g_event_queue, event);
            }
        }
    }
}
    // ---- Process mouse events ----
for (int i = 0; i < g_mice.count; i++, index++) {
    int motion_accumulated = 0;
    int wheel_accumulated = 0;
    
    while (read(fds[index].fd, &ev, sizeof(ev)) > 0) {
        if (ev.type == EV_KEY) {
            // Convert Linux button code to PAL button code
            int pal_button = linux_button_to_pal_button(ev.code);
            if (pal_button >= 0 && pal_button < MAX_MOUSE_BUTTONS) {
                int old_state = g_mice.buttons[i][pal_button];
                int new_state = ev.value;
                
                g_mice.buttons[i][pal_button] = new_state;
                
                if (old_state != new_state) {
                    g_mice.buttons_toggled[i][pal_button] = 1;
                    
                    // Push button event immediately
                    if (new_state) {
                        event.type = PAL_EVENT_MOUSE_BUTTON_DOWN;
                        event.button.pressed = 1;
                    } else {
                        event.type = PAL_EVENT_MOUSE_BUTTON_UP;
                        event.button.pressed = 0;
                    }
                    event.button.button = pal_button;
                    event.button.clicks = 1;
                    event.button.mouse_id = i;
                    event.button.x = 0;
                    event.button.y = 0;
                    event.button.modifiers = PAL_MOD_NONE;
                    pal__eventq_push(&g_event_queue, event);
                }
            } else {
                printf("  -> Button code out of range or invalid\n");
            }
        } else if (ev.type == EV_REL) {
            if (ev.code == REL_X) {
                g_mice.dx[i] += ev.value;
                motion_accumulated = 1;
            } else if (ev.code == REL_Y) {
                g_mice.dy[i] += ev.value;
                motion_accumulated = 1;
            } else if (ev.code == REL_WHEEL) {
                g_mice.wheel[i] += ev.value;
                wheel_accumulated = 1;
            }
        }
    }
        // Push accumulated motion event
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
        
        // Push wheel event
        if (wheel_accumulated) {
            event.type = PAL_EVENT_MOUSE_WHEEL;
            event.wheel.y = (float)g_mice.wheel[i];
            event.wheel.x = 0.0f;
            event.wheel.mouse_id = i;
            event.wheel.mouse_x = 0;
            event.wheel.mouse_y = 0;
            event.wheel.wheel_direction = PAL_MOUSEWHEEL_VERTICAL;
            event.wheel.modifiers = PAL_MOD_NONE;
            pal__eventq_push(&g_event_queue, event);
        }
    }

    // ---- Check udev hotplug ----
    if (fds[nfds-1].revents & POLLIN) {
        struct udev_device *dev = udev_monitor_receive_device(g_monitor);
        if (dev) {
            const char *action = udev_device_get_action(dev);
            const char *devnode = udev_device_get_devnode(dev);
            if (devnode && action && strcmp(action,"add")==0) {
                int dev_fd = open(devnode, O_RDONLY | O_NONBLOCK);
                if (dev_fd >= 0) {
                    char name[256] = {0};
                    ioctl(dev_fd, EVIOCGNAME(sizeof(name)), name);
                    if (is_keyboard(dev_fd) && g_keyboards.count < MAX_KEYBOARDS) {
                        g_keyboards.fds[g_keyboards.count] = dev_fd;
                        strncpy(g_keyboards.names[g_keyboards.count], name, 255);
                        memset(g_keyboards.keys[g_keyboards.count], 0, MAX_KEYS);
                        memset(g_keyboards.keys_toggled[g_keyboards.count], 0, MAX_KEYS);
                        
                        g_keyboards.count++;
                        printf("[Keyboard Added] %s\n", name);
                    } else if (is_mouse(dev_fd) && g_mice.count < MAX_MICE) {
                        g_mice.fds[g_mice.count] = dev_fd;
                        strncpy(g_mice.names[g_mice.count], name, 255);
                        memset(g_mice.buttons[g_mice.count], 0, MAX_MOUSE_BUTTONS * sizeof(int));
                        memset(g_mice.buttons_toggled[g_mice.count], 0, MAX_MOUSE_BUTTONS * sizeof(int));
                        g_mice.dx[g_mice.count] = 0;
                        g_mice.dy[g_mice.count] = 0;
                        g_mice.wheel[g_mice.count] = 0;
                        
                        g_mice.count++;
                        printf("[Mouse Added] %s\n", name);
                    } else {
                        close(dev_fd);
                    }
                }
            }
            udev_device_unref(dev);
        }
    }
}

PALAPI pal_window *pal_create_window(int width, int height, const char *title, uint64_t flags) {
    pal_window *window = (pal_window*)malloc(sizeof(pal_window));
    if (!window) return NULL;

    int screen = DefaultScreen(g_display);

    // --- Default graphics API to OpenGL if none specified ---
    if (!(flags & (PAL_WINDOW_OPENGL | PAL_WINDOW_VULKAN | PAL_WINDOW_METAL))) {
        flags |= PAL_WINDOW_OPENGL;
    }

    GLXFBConfig fb = 0;
    XVisualInfo visual_info;
    Colormap colormap = 0;

    // --- OpenGL path ---
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
        // Non-OpenGL graphics APIs (unimplemented)
        visual_info.visual = DefaultVisual(g_display, screen);
        visual_info.depth  = DefaultDepth(g_display, screen);
        colormap = XCreateColormap(g_display, RootWindow(g_display, screen),
                                   visual_info.visual, AllocNone);
    }

    // --- Create the X11 window ---
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

    XStoreName(g_display, window->window, title);

    // --- Optional: set class hints, WM protocols, etc. ---
    XClassHint *class_hint = XAllocClassHint();
    if (class_hint) {
        class_hint->res_name = "my_app";
        class_hint->res_class = "MyApp";
        XSetClassHint(g_display, window->window, class_hint);
        XFree(class_hint);
    }
    XSetWMProtocols(g_display, window->window, &g_wm_delete, 1);

    // --- Create OpenGL context if requested ---
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

    // --- Map window ---
    XMapWindow(g_display, window->window);

    // --- Optional GC ---
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
    e.xclient.data.l[0] = action;      // 1 = add, 0 = remove, 2 = toggle
    e.xclient.data.l[1] = property;    // _NET_WM_STATE_MAXIMIZED_VERT or HORZ
    e.xclient.data.l[2] = 0;           // second property (unused)
    e.xclient.data.l[3] = 1;           // source: application
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

    // Find matching mode
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

    // Apply the mode
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

    // Then set fullscreen
    pal_make_window_fullscreen(window);

    return pal_true;
}

PALAPI pal_bool pal_make_window_fullscreen_windowed(pal_window *window)
{
    if (!window) return pal_false;

    Atom wmHints = XInternAtom(g_display, "_MOTIF_WM_HINTS", False);

    // Remove decorations using Motif WM hints
    struct {
        unsigned long flags;
        unsigned long functions;
        unsigned long decorations;
        long inputMode;
        unsigned long status;
    } hints;

    hints.flags = (1L << 1); // MWM_HINTS_DECORATIONS
    hints.decorations = 0;   // No decorations

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

    // Resize window to match screen size
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

    // Restore decorations
    hints.flags = (1L << 1);
    hints.decorations = 1;  // enable

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

    // Remove fullscreen state (if any)
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

    // action = 1 → add
    linux_x11_send_wm_state_message(window->window, 1, max_vert);
    linux_x11_send_wm_state_message(window->window, 1, max_horz);

    XFlush(g_display);
    return pal_true;
}

PALAPI pal_bool pal_minimize_window(pal_window *window)
{
    if (!window || !g_display)
        return pal_false;

    // Minimize is via _NET_ACTIVE_WINDOW with a special request
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

    // Convert float color (0.0 - 1.0) to 24-bit RGB
    unsigned long pixel = ((unsigned long)(color.r * 255) << 16) |
                          ((unsigned long)(color.g * 255) << 8)  |
                          ((unsigned long)(color.b * 255) << 0);

    XSetForeground(g_display, window->graphics_context, pixel);
    XFillRectangle(g_display, window->window, window->graphics_context, x, y, width, height);
}

// Helper to intern an atom
PALAPI int pal_make_context_current(pal_window *window) {
    if (window && window->gl_context) {
        if (!glXMakeCurrent(g_display, window->window, window->gl_context)) {
            fprintf(stderr, "GLX ERROR: glXMakeCurrent failed!\n");
            return -1;
        }
    } else {
        glXMakeCurrent(g_display, None, NULL); // unbind context
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

    // Pick the first connected output
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

    // Find a mode that matches width, height, and refresh rate
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

    // Apply the new mode
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
    dlerror(); // Clear existing errors
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
#endif // LINUX_X11_PLATFORM_H

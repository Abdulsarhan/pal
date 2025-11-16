#ifndef LINUX_X11_PLATFORM_H
#define LINUX_X11_PLATFORM_H

// C standard lib
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

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

// KEYBOARD SHIT:
static int g_keyboard_count = 0;
#define MAX_KEYBOARDS 8

typedef struct {
    void *device_handle; // on linux, we prolly don't need this. It's probably as simple as just reading a file.
    uint8_t keys[256];
    uint8_t keys_processed[256];
    uint8_t key_is_down[256];
    char device_name[256];
} pal_keyboard_state;
static pal_keyboard_state g_keyboards[MAX_KEYBOARDS] = {0};

// MOUSE SHIT:
static int g_mouse_count = 0;
#define MAX_MICE 8

typedef struct {
    void *device_handle;
    uint8_t buttons[8];
    uint8_t buttons_processed[8];
    int32_t delta_x;
    int32_t delta_y;
    char device_name[256];
}pal_mouse_state;
static pal_mouse_state g_mice[MAX_MICE] = {0};

PALAPI void pal_init(void) {
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
PALAPI void pal_destroy_window(pal_window *window) {
    if(window->graphics_context) {
        XFreeGC(g_display, window->graphics_context);
    }
    if(window->window) {
        XDestroyWindow(g_display, window->window);
    }
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

PALAPI pal_bool pal_poll_events(pal_event *event) {

    return pal_false;
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

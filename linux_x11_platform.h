#ifndef LINUX_X11_PLATFORM_H
#define LINUX_X11_PLATFORM_H

// C standard lib
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

// X11
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xlib-xcb.h>
#include <xcb/xcb.h>

// Opengl
#include <glad/glad.h>
#include <GL/gl.h>
#include <GL/glx.h>

// dlopen/dlsym/dlclose
#include <dlfcn.h>

pal_window pal_create_window() {
    pal_window window = {0};
    // 1. Connect to X11 and get the XCB connection
    window.display = XOpenDisplay(NULL);
    if (!window.display)
        printf("ERROR: Failed to open X display");

    window.xcb_conn = XGetXCBConnection(window.display);
    int default_screen = DefaultScreen(window.display);

    // 2. Get XCB screen
    const xcb_setup_t *setup = xcb_get_setup(window.xcb_conn);
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
    for (int i = 0; i < default_screen; ++i)
        xcb_screen_next(&iter);
    xcb_screen_t *screen = iter.data;
    // 3. Create XCB window
    window.window = xcb_generate_id(window.xcb_conn);
    uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    uint32_t values[] = {
        screen->black_pixel,
        XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS};

    xcb_create_window(
        window.xcb_conn,
        XCB_COPY_FROM_PARENT,
        window.window,
        screen->root,
        0,
        0,
        800,
        600,
        0,
        XCB_WINDOW_CLASS_INPUT_OUTPUT,
        screen->root_visual,
        mask,
        values);

    xcb_map_window(window.xcb_conn, window.window);
    xcb_flush(window.xcb_conn);
    XVisualInfo vinfo_template;
    int nitems;
    vinfo_template.screen = default_screen;
    XVisualInfo *vinfo = XGetVisualInfo(window.display, mask, &vinfo_template, &nitems);
    // 4. Create OpenGL context using GLX
    window.ctx = glXCreateContext(
        window.display,
        vinfo,
        NULL,
        GL_TRUE);
    if (!window.ctx)
        printf("ERROR: Failed to create GLX context");

    // 5. Create X11 window from XCB window
    return window;
}

// Helper to intern an atom
static xcb_atom_t linux_x11_get_atom(xcb_connection_t *connection, const char *name) {
    xcb_intern_atom_cookie_t cookie = xcb_intern_atom(connection, 0, strlen(name), name);
    xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(connection, cookie, NULL);
    if (!reply)
        return XCB_ATOM_NONE;
    xcb_atom_t atom = reply->atom;
    free(reply);
    return atom;
}

// Request fullscreen at a specific resolution (doesn't change refresh rate)
pal_bool pal_make_window_fullscreen_ex(pal_window *window, int width, int height, int refresh_rate) {
    (void)refresh_rate; // Ignored unless using XRandR
    xcb_atom_t wm_state = linux_x11_get_atom(window->connection, "_NET_WM_STATE");
    xcb_atom_t fullscreen = linux_x11_get_atom(window->connection, "_NET_WM_STATE_FULLSCREEN");
    if (wm_state == XCB_ATOM_NONE || fullscreen == XCB_ATOM_NONE)
        return pal_false;

    xcb_client_message_event_t ev = {
        .response_type = XCB_CLIENT_MESSAGE,
        .format = 32,
        .window = window->window,
        .type = wm_state,
        .data.data32 = {
            1, // _NET_WM_STATE_ADD
            fullscreen,
            0,
            0,
            0}};

    xcb_screen_t *screen = xcb_setup_roots_iterator(xcb_get_setup(window->connection)).data;
    xcb_send_event(window->connection, 0, screen->root, XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY, (const char *)&ev);

    xcb_configure_window(window->connection, window->window, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y | XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, (uint32_t[]){0, 0, (uint32_t)width, (uint32_t)height});

    xcb_flush(window->connection);
    return pal_true;
}

// Request fullscreen covering the monitor without changing mode
pal_bool pal_make_window_fullscreen_windowed(pal_window *window) {
    xcb_atom_t wm_state = linux_x11_get_atom(window->connection, "_NET_WM_STATE");
    xcb_atom_t fullscreen = linux_x11_get_atom(window->connection, "_NET_WM_STATE_FULLSCREEN");
    if (wm_state == XCB_ATOM_NONE || fullscreen == XCB_ATOM_NONE)
        return pal_false;

    xcb_screen_t *screen = xcb_setup_roots_iterator(xcb_get_setup(window->connection)).data;

    xcb_client_message_event_t ev = {
        .response_type = XCB_CLIENT_MESSAGE,
        .format = 32,
        .window = window->window,
        .type = wm_state,
        .data.data32 = {
            1, // _NET_WM_STATE_ADD
            fullscreen,
            0,
            0,
            0}};

    xcb_send_event(window->connection, 0, screen->root, XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY, (const char *)&ev);

    xcb_configure_window(window->connection, window->window, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y | XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, (uint32_t[]){0, 0, screen->width_in_pixels, screen->height_in_pixels});

    xcb_flush(window->connection);
    return pal_true;
}

// Remove fullscreen request (return to windowed mode)
pal_bool pal_make_window_windowed(pal_window *window) {
    xcb_atom_t wm_state = linux_x11_get_atom(window->connection, "_NET_WM_STATE");
    xcb_atom_t fullscreen = linux_x11_get_atom(window->connection, "_NET_WM_STATE_FULLSCREEN");
    if (wm_state == XCB_ATOM_NONE || fullscreen == XCB_ATOM_NONE)
        return pal_false;

    xcb_client_message_event_t ev = {
        .response_type = XCB_CLIENT_MESSAGE,
        .format = 32,
        .window = window->window,
        .type = wm_state,
        .data.data32 = {
            0, // _NET_WM_STATE_REMOVE
            fullscreen,
            0,
            0,
            0}};

    xcb_screen_t *screen = xcb_setup_roots_iterator(xcb_get_setup(window->connection)).data;
    xcb_send_event(window->connection, 0, screen->root, XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY, (const char *)&ev);

    // Optionally restore size here (currently no saved state)
    xcb_flush(window->connection);
    return pal_true;
}

// Fullscreen using current monitor resolution (like EnumDisplaySettings)
pal_bool pal_make_window_fullscreen(pal_window *window) {
    xcb_screen_t *screen = xcb_setup_roots_iterator(xcb_get_setup(window->connection)).data;
    if (!screen)
        return pal_false;

    int width = screen->width_in_pixels;
    int height = screen->height_in_pixels;

    return pal_make_window_fullscreen_ex(window, width, height, 0);
}
void pal_make_context_current(pal_window window) {
    glXMakeCurrent(window.display, window.window, window.ctx);
}

void pal_swap_buffers(pal_window *window) {
    glXSwapBuffers(window->display, window->window);
}

static int pal_make_context_current(pal_window *window) {
    if (!glXMakeCurrent(window->display, window->window, window->context)) {
        fprintf(stderr, "glXMakeCurrent() failed.\n");
        return 1;
    }
    return 0;
}

static int pal_show_cursor(pal_window *window) {
    int event_base, error_base;
    if (!XFixesQueryExtension(window->display, &event_base, &error_base)) {
        fprintf(stderr, "XFixes extension not available.\n");
        return -1;
    }

    XFixesShowCursor(window->display, window->window);
    XFlush(window->display);
    return 0;
}

static int pal_hide_cursor(pal_window *window) {
    int event_base, error_base;
    if (!XFixesQueryExtension(window->display, &event_base, &error_base)) {
        fprintf(stderr, "XFixes extension not available.\n");
        return -1;
    }

    XFixesHideCursor(window->display, window->window);
    XFlush(window->display);
    return 0;
}

uint8_t pal_poll_events(pal_event *event, pal_window *window) {
    xcb_generic_event_t *event;
    while ((event = xcb_poll_for_event(window->xcb_conn))) {
        switch (event->response_type & ~0x80) {
            case XCB_KEY_PRESS:
                free(event);
                //          window->window_should_close = true;
        }
    }
}

uint8_t pal_poll_events(pal_window *window) {
    pal_poll_events(window);
}

void *pal_gl_get_proc_address(const char *procname) {
    return glXGetProcAddress(procname);
}

void *pal_load_dynamic_library(const char *so_name) {
    void *lib = dlopen(so_name, RTLD_NOW | RTLD_LOCAL);
    if (!lib) {
        fprintf(stderr, "dlopen failed: %s\n", dlerror());
        assert(0 && "Failed to load shared library");
    }
    return lib;
}

void *pal_load_dynamic_function(void *so_handle, const char *func_name) {
    dlerror(); // Clear existing errors
    void *symbol = dlsym(so_handle, func_name);
    const char *error = dlerror();
    if (error) {
        fprintf(stderr, "dlsym failed: %s\n", error);
        assert(0 && "Failed to load function from shared library");
    }
    return symbol;
}

uint8_t pal_free_dynamic_library(void *so_handle) {
    int result = dlclose(so_handle);
    if (result != 0) {
        fprintf(stderr, "dlclose failed: %s\n", dlerror());
        assert(0 && "Failed to unload shared library");
    }
    return (uint8_t)(result == 0);
}
#endif // LINUX_X11_PLATFORM_H

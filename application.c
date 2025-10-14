#include <stdio.h>
#include <assert.h>

#pragma warning(disable : 4996)
#include <glad/glad.h>
#include "application.h"
#include "pal.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

typedef struct OpenglInfo {
    const char* vendor;
    const char* renderer;
    const char* version;
    const char* shadingLanguageVersion;
    char extensions[8124];
} OpenglInfo;

static OpenglInfo get_opengl_info(void) {
    OpenglInfo info = {0};
    info.vendor = (char*)glGetString(GL_VENDOR);
    info.renderer = (char*)glGetString(GL_RENDERER);
    info.version = (char*)glGetString(GL_VERSION);
    info.shadingLanguageVersion = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
    if (glGetStringi) {
        int numExtensions;
        glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
        for (int i = 0; i < numExtensions; i++) {
            const char* ext = (const char*)glGetStringi(GL_EXTENSIONS, i);
            strcat(info.extensions, ext);
            strcat(info.extensions, " ");
        }
    } else {
        info.extensions[0] = (char)glGetString(GL_EXTENSIONS);
    }

    return info;
}

int main() {
    // int wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE  hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {

    pal_init();

    pal_monitor* monitor = pal_get_primary_monitor();
    pal_video_mode* mode = pal_get_video_mode(monitor);
    pal_window* window = pal_create_window(1280, 720, "Window Title", PAL_WINDOW_RESIZABLE);
    pal_make_context_current(window);
    if (!gladLoadGLLoader((GLADloadproc)pal_gl_get_proc_address)) {
        fprintf(stderr, "ERROR: Failed to initialize glad!\n");
    }

    //OpenglInfo openglInfo = get_opengl_info();
    pal_sound* music = pal_load_music("sine_wave.wav");
    pal_play_music(music, 0.1f);
    //pal_set_window_icon_legacy(window, "icon.ico");
    //pal_set_taskbar_icon(window, "png.png");
    uint8_t running = pal_true;
    pal_event event;
    pal_gamepad_state state;
    pal_bool is_fullscreen = pal_false;
    while (running) {
        while (pal_poll_events(&event)) {

            switch (event.type) {

                case PAL_EVENT_MOUSE_BUTTON_DOWN:
                    if (event.button.button == PAL_MOUSE_LEFT) {
                        printf("mouse button left!\n");
                    }
                    break;
                case PAL_EVENT_MOUSE_BUTTON_UP:
                    break;
                case PAL_EVENT_KEY_DOWN:
                    printf("Key down!\n");
                    if (event.key.scancode == PAL_SCAN_ESCAPE) {
                        printf("Exited!\n");
                        printf("scancode: %d", event.key.scancode);
                        running = pal_false;
                    }
                    break;
                case PAL_EVENT_KEY_UP:
                    printf("Keyboard UP!\n");
                    break;
                case PAL_EVENT_QUIT:
                    printf("Window closed");
                    running = pal_false;
                    break;
                case PAL_EVENT_MOUSE_MOTION:
                    printf("X: %d, Y: %d\n", event.motion.delta_x, event.motion.delta_y);
                    //printf("mouse moved!\n");
                    break;
                case PAL_EVENT_WINDOW_LOST_FOCUS:
                    if (is_fullscreen) {
						pal_set_video_mode(NULL);
						pal_minimize_window(window);
                    }
                    break;
                case PAL_EVENT_WINDOW_GAINED_FOCUS:
                    if (is_fullscreen) {
						pal_make_window_fullscreen(window);
                    }
                    break;
                default:
                    // printf("%d\n", event.type);
                    break;
            }
        }
        // The is_* functions only work after all the events have been polled.
        // do not call this in the message loop.
        if (pal_is_key_pressed(PAL_W)) {
            printf("PRESSED W!\n");
        }
        if (pal_is_mouse_pressed(PAL_MOUSE_LEFT)) {
            printf("Pressed LMB!\n");
        }
        if (pal_is_mouse_pressed(PAL_MOUSE_RIGHT)) {
            printf("Pressed LMB!\n");
        }
        if (pal_is_mouse_pressed(PAL_MOUSE_MIDDLE)) {
            printf("Pressed LMB!\n");
        }

        if (pal_is_mouse_pressed(PAL_MOUSE_4)) {
            printf("Pressed mouse4!\n");
        }
        if (pal_is_mouse_pressed(PAL_MOUSE_5)) {
            printf("Pressed mouse5!\n");
        }

        for (int i = 0; i < pal_get_gamepad_count(); i++) {
            if (pal_get_gamepad_state(i, &state)) {
                /*
                                printf("\nController %d: %s\n", i, state.name);
                                printf("  Left Stick: %.2f, %.2f\n", state.axes.left_x, state.axes.left_y);
                                printf("  Right Stick: %.2f, %.2f\n", state.axes.right_x, state.axes.right_y);
                                printf("  Triggers: L=%.2f R=%.2f\n", state.axes.left_trigger, state.axes.right_trigger);
                                printf("  Buttons: A=%d B=%d X=%d Y=%d\n",
                                      state.buttons.a, state.buttons.b,
                                      state.buttons.x, state.buttons.y);
                */
                // 6. Example vibration (Xbox controllers only)
                if (state.buttons.a && state.is_xinput) {
                    pal_set_gamepad_vibration(i, 0.5f, 0.5f, 0, 0);
                } else {
                    pal_stop_gamepad_vibration(i);
                }
            }
        }
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        pal_swap_buffers(window);
    }
    pal_shutdown();
    return 0;
}

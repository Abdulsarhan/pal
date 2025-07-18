#include <stdio.h>
#include <assert.h>

#include <Windows.h>

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
}OpenglInfo;

static OpenglInfo get_opengl_info(void) {
    OpenglInfo info = { 0 };
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
    }
    else {
        info.extensions[0] = (const char*)glGetString(GL_EXTENSIONS);
    }

    return info;
}

int main() {
//int wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE  hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {

    pal_init();
    // This should be done using the
    // window flags that we will be adding
    // to the window creation function.

    //TODO: @fix monitor and video mode functions have problems.
    // They don't return the correct sizes.
    pal_monitor* monitor = pal_get_primary_monitor();
    VideoMode* mode = pal_get_video_mode(monitor);
    pal_window* window = pal_create_window(1280, 720, "Window Title", PAL_WINDOW_RESIZABLE);
    make_context_current(window);

    if (!gladLoadGLLoader((GLADloadproc)gl_get_proc_address)) {
        fprintf(stderr, "ERROR: Failed to initialize glad!\n");
    }

    OpenglInfo openglInfo = get_opengl_info();

    if (register_input_devices(window) != 0)
        return;

    pal_set_window_icon_legacy(window, "icon.ico");
    pal_set_taskbar_icon(window, "png.png");
    pal_set_cursor(window, "png.png", 16);
    uint8_t running = 1;
    pal_event event;
    pal_gamepad_state state;
    while (running) {
		while (pal_poll_events(&event, window))
		{

            switch (event.type) {

            case PAL_MOUSE_BUTTON_DOWN:
                if(event.button.button == PAL_MOUSE_LEFT)
				printf("Mouse button DOWN!\n");
                break;
            case PAL_MOUSE_BUTTON_UP:
				printf("Mouse Button UP!\n");
                break;
            case PAL_KEY_DOWN:
                if (event.key.virtual_key == PAL_ESCAPE)
                    exit(0);
                break;
            case PAL_KEY_UP:
				printf("Keyboard UP!\n");
                break;
            case PAL_QUIT:
                printf("SHOULD HAVE CLOSED THE WINDOW!\n");
                running = FALSE;
                break;
            case PAL_MOUSE_MOTION:
                break;
            default:
                //printf("%d\n", event.type);
                break;
            }
		}
        // The is_* functions only work after all the events have been polled.
        // do not call this in the message loop.
        if (is_key_pressed(PAL_W)) {
            printf("PRESSED W!\n");
        }
        if (is_mouse_pressed(PAL_MOUSE_LEFT)) {
            printf("Pressed LMB!\n");
        }
        if (is_mouse_pressed(PAL_MOUSE_RIGHT)) {
            printf("Pressed LMB!\n");
        }
        if (is_mouse_pressed(PAL_MOUSE_MIDDLE)) {
            printf("Pressed LMB!\n");
        }

        if (is_mouse_pressed(PAL_MOUSE_4)) {
            printf("Pressed mouse4!\n");
        }
        if (is_mouse_pressed(PAL_MOUSE_5)) {
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
		begin_drawing();
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		end_drawing(window);
    }

    return 0;
}

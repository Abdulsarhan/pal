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
    //TODO: @fix Make the API of this better (if possible).
    pal_window_hint(GL_VERSION_MAJOR, 3);
    pal_window_hint(GL_VERSION_MINOR, 3);
    pal_window_hint(FLOATING, 0);
    pal_window_hint(DOUBLE_BUFFER, 1);
    pal_window_hint(RESIZABLE, 1);

    //TODO: @fix monitor and video mode functions have problems.
    pal_monitor* monitor = pal_get_primary_monitor();
    VideoMode* mode = pal_get_video_mode(monitor);
    pal_window* window = pal_create_window(1280, 720, "Fucking Windows!");
    make_context_current(window);

    if (!gladLoadGLLoader((GLADloadproc)gl_get_proc_address)) {
        fprintf(stderr, "ERROR: Failed to initialize glad!\n");
    }

    OpenglInfo openglInfo = get_opengl_info();

    if (register_input_devices(window) != 0)
        return;

	Sound sound = { 0 };
	load_sound("C:\\Users\\abdul.DESKTOP-S9KEIDK\\Desktop\\sal-rewrite\\Project1\\Project1\\sine_wave.wav", &sound);

	play_sound(&sound, 0.1);

    pal_set_window_icon_legacy(window, "C:\\Users\\abdul.DESKTOP-S9KEIDK\\Desktop\\sal-rewrite\\Project1\\Project1\\icon.ico");
    pal_set_taskbar_icon(window, "C:\\Users\\abdul.DESKTOP-S9KEIDK\\Desktop\\sal-rewrite\\Project1\\Project1\\png.png");
    pal_set_cursor(window, "C:\\Users\\abdul.DESKTOP-S9KEIDK\\Desktop\\sal-rewrite\\Project1\\Project1\\png.png", 16);
    uint8_t running = 1;
    pal_event event;
    while (running) {
		while (pal_poll_events(&event, window))
		{

            switch (event.type) {

            case PAL_MOUSE_BUTTON_DOWN:
                if(event.button.button == LEFT_MOUSE_BUTTON)
				printf("Mouse button DOWN!\n");
                break;
            case PAL_MOUSE_BUTTON_UP:
				printf("Mouse Button UP!\n");
                break;
            case PAL_KEY_DOWN:
                if (event.key.virtual_key == KEY_ENTER )
                    printf("%d\n", event.key.modifiers);
                    printf("pressed alt!\n");
                break;
            case PAL_KEY_UP:
				printf("Keyboard UP!\n");
                break;
            default:
                //printf("%d\n", event.type);
                break;
            }
		}
        if (is_key_down(KEY_W)) {
            printf("PRESSED W!\n");
        }

/*
		if (is_key_down(KEY_SPACE)) {
			printf("Pressed the A key!");
		}

		if (is_mouse_down(SIDE_MOUSE_BUTTON1)) {
			printf("MOUSE PRESSED!\n");
		}

		if (is_button_down(1, 0x1000)) {
			printf("INFO GAMEPAD A PRESSED!\n");
		}

*/
		begin_drawing();
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		end_drawing(window);
    }

    return 0;
}

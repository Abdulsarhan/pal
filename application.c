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

#ifdef _WIN32

int main() {
//int wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE  hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {

    init_pal();
    //TODO: @fix Make the API of this better (if possible).
    set_window_hint(GL_VERSION_MAJOR, 3);
    set_window_hint(GL_VERSION_MINOR, 3);
    set_window_hint(FLOATING, 0);
    set_window_hint(DOUBLE_BUFFER, 1);
    set_window_hint(RESIZABLE, 1);

    //TODO: @fix monitor and video mode functions have problems.
    pal_monitor* monitor = get_primary_monitor();
    VideoMode* mode = get_video_mode(monitor);
    pal_window* window = init_window(1280, 720, "Fucking Windows!");
    make_context_current(window);

    if (!gladLoadGLLoader((GLADloadproc)gl_get_proc_address)) {
        fprintf(stderr, "ERROR: Failed to initialize glad!\n");
    }

    OpenglInfo openglInfo = get_opengl_info();

    if (register_input_devices(window) != 0)
        return;

	Sound sound = { 0 };
	load_sound("C:\\Users\\abdul.DESKTOP-S9KEIDK\\Desktop\\sal-rewrite\\Project1\\Project1\\piano.wav", &sound);

	play_sound(&sound, 0.1);

    uint8_t running = 1;
    while (running) {
		while (poll_events())
		{
			if (is_key_down(KEY_SPACE)) {
				printf("Pressed the A key!");
			}

			if (is_mouse_down(SIDE_MOUSE_BUTTON1)) {
				printf("MOUSE PRESSED!\n");
			}

			if (is_button_down(1, 0x1000)) {
				printf("INFO GAMEPAD A PRESSED!\n");
			}
			begin_drawing();
			v2 mouse_pos = get_mouse_position(window);
			printf("Mouse Position, %f, %f\n", mouse_pos.x, mouse_pos.y);
			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			end_drawing(window);
		}

    }

    return 0;
}
#endif

#ifdef __linux__

int main() {
	init_window();
}

#endif

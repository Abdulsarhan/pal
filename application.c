#include <Windows.h>
#include <glad/glad.h>
#include "application.h"
#include "sal.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

typedef struct OpenglInfo {
    const char* vendor;
    const char* renderer;
    const char* version;
    const char* shadingLanguageVersion;
    char extensions[8124];
}OpenglInfo;

static OpenglInfo GetOpenGlInfo(void) {
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
        info.extensions[0] = (char*)glGetString(GL_EXTENSIONS);
    }

    return info;
}

#ifdef _WIN32

int wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE  hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {

    InitSal();
    SetWindowHint(GL_VERSION_MAJOR, 3);
    SetWindowHint(GL_VERSION_MINOR, 3);
    SetWindowHint(FLOATING, 0);
    SetWindowHint(DOUBLE_BUFFER, 1);
    SetWindowHint(RESIZABLE, 0);

    Monitor* monitor = GetPrimaryMonitor();

    VideoMode* mode = GetVideoMode(monitor);

    Window* window = InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Fucking Windows!");

    MakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)GlGetProcAddress)) {
        OutputDebugString("ERROR: FAILED TO INITIALIZE GLAD!");
    }
    OpenglInfo openglInfo = GetOpenGlInfo();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        PollEvents();

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        EndDrawing();
    }

    return 0;
}
#endif

#ifdef __linux__

int main() {
	InitWindow();
}

#endif

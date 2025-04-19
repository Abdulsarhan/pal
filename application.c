#include <Windows.h>
#include "application.h"
#include "sal.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#ifdef _WIN32

int wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE  hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {

    SalInit();
    SetWindowHint(GL_VERSION_MAJOR, 3);
    SetWindowHint(GL_VERSION_MINOR, 3);
    SetWindowHint(FLOATING, false);
    SetWindowHint(DOUBLE_BUFFER, true);
    SetWindowHint(RESIZABLE, false);

    VideoMode mode = GetVideoMode(GetPrimaryMonitor());

    Window window = InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Fucking Windows.");

    MakeContextCurrent(window);

    if (!gladLoadGL()) {
        MessageBoxA(window, "gladLoadGL() failed.", "Try again later", MB_ICONERROR);
        return 1;
    }

    SetWindowTextA(window, (LPCSTR)glGetString(GL_RENDERER));
    OpenglInfo openglInfo = GetOpenglInfo();
    while (WindowShouldNotClose())
    {
        BeginDrawing();
        PollEvents();

        if(IsKeyPressedRepeat(0x41))
            OutputDebugStringW(L"A");


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

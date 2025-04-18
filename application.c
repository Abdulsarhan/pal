#include <Windows.h>
#include "application.h"
#include "sal.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#ifdef _WIN32

int wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE  hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {

    VideoMode mode = GetVideoMode(GetPrimaryMonitor());

    Window window = InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Fucking Windows.");

    MakeContextCurrent(window);

    if (!gladLoadGL()) {
        MessageBoxA(window, "gladLoadGL() failed.", "Try again later", MB_ICONERROR);
        return 1;
    }

    SetWindowTextA(window, (LPCSTR)glGetString(GL_VERSION));

    while (WindowShouldNotClose())
    {
        BeginDrawing();
        PollEvents(window);

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

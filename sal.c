#include "sal.h"

#ifdef _WIN32
#include "win32_platform.h"
#elif __LINUX__
#include "linux_platform.h"
#endif


void InitSal() {

}

#ifdef _WIN32
SALAPI Window* InitWindow(int width, int height, const char* windowTitle) {
	Win32InitWindow(width, height, windowTitle);
}

SALAPI void SetWindowTitle(Window* window, const char* string) {
	SetWindowTextA(window, string);
}

SALAPI void SetWindowHint(int type, int value) {
	Win32SetWindowHint(type, value);
}
SALAPI VideoMode* GetVideoMode(Monitor* monitor) {
	Win32GetVideoMode(monitor);
}
SALAPI Monitor* GetPrimaryMonitor(void) {
	Win32GetPrimaryMonitor();
}
SALAPI ProcAddress GlGetProcAddress(const char* proc) {
	wglGetProcAddress(proc);
}
SALAPI uint8_t WindowShouldClose(void) {
	Win32WindowShouldClose();
}
SALAPI void PollEvents(void) {
	Win32PollEvents();
}
SALAPI int MakeContextCurrent(Window* window) {
	Win32MakeContextCurrent(window);
}
SALAPI void BeginDrawing(void) {
	Win32BeginDrawing();
}
SALAPI void EndDrawing(void) {
	Win32EndDrawing();
}
SALAPI uint8_t IsWhiteSpace(char* ch) {
	Win32IsWhiteSpace(ch);
}
SALAPI uint8_t AreStringsEqual(int count, char* str1, char* str2) {
	Win32AreStringsEqual(count, str1, str2);
}
SALAPI uint8_t IsEndOfLine(char* ch) {
	Win32IsEndOfLine(ch);
}
#endif

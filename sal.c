#include "sal.h"

#ifdef _WIN32
#include "win32_platform.h"
#elif __LINUX__
#include "linux_platform.h"
#endif


SALAPI void InitSal() {
}

#ifdef _WIN32

/*
------------------------------------
|       Window & Monitor Stuff     |
------------------------------------
*/

SALAPI Window* InitWindow(int width, int height, const char* windowTitle) {
	 return Win32InitWindow(width, height, windowTitle);
}

SALAPI uint8_t WindowShouldClose(void) {
	return Win32WindowShouldClose();
}

SALAPI void SetWindowTitle(Window* window, const char* string) {
	(void)SetWindowTextA(window, string);
}

SALAPI void SetWindowHint(int type, int value) {
	(void)Win32SetWindowHint(type, value);
}

SALAPI VideoMode* GetVideoMode(Monitor* monitor) {
	return Win32GetVideoMode(monitor);
}
SALAPI Monitor* GetPrimaryMonitor(void) {
	return Win32GetPrimaryMonitor();
}

SALAPI ProcAddress* GlGetProcAddress(const char* proc) {
	return wglGetProcAddress(proc);
}

SALAPI int RegisterInputDevices(Window* window) {
	return Win32RegisterRawInputDevices(window->handle);
}

SALAPI void PollEvents(void) {
	(void)Win32PollEvents();
}

SALAPI int MakeContextCurrent(Window* window) {
	return Win32MakeContextCurrent(window->handle);
}

/*
-----------------------------
|    Rendering functions.   |
-----------------------------
*/
SALAPI void BeginDrawing(void) {
	(void)Win32BeginDrawing();
}

SALAPI void DrawTriangle() {

}

SALAPI void EndDrawing(void) {
	(void)Win32EndDrawing();
}


/*
	Library Loading Functions.
*/

void* LoadDynamicLibrary(char* dll) {
	return Win32LoadDynamicLibrary(dll);

}

void* LoadDynamicFunction(void* dll, char* func_name) {
	return Win32LoadDynamicFunction(dll, func_name);

}

uint8_t FreeDynamicLibrary(void* dll) {
	return Win32FreeDynamicLibrary(dll);

}
#endif

/*
-----------------------------
|   String Parsing Helpers  |
-----------------------------
*/

SALAPI uint8_t IsCapitalLetter(char ch) {
	return ((ch >= 'A') && (ch <= 'Z'));
}

SALAPI uint8_t IsLowerCaseLetter(char ch) {
	return ((ch >= 'a') && (ch <= 'z'));
}

SALAPI uint8_t IsLetter(char ch) {
	return (IsCapitalLetter(ch) || IsLowerCaseLetter(ch));
}

SALAPI uint8_t IsEndOfLine(char ch) {
	return ((ch == '\r') || (ch == '\n'));
}

SALAPI uint8_t IsWhiteSpace(char ch) {
	return ((ch == ' ') || (ch == '\t') || (ch == '\v') || (ch == '\f'));
}

SALAPI uint8_t IsNumber(char ch) {
	return ((ch >= '0') && (ch <= '9'));
}

SALAPI uint8_t IsUnderscore(char ch) {
	return (ch == '_');
}

SALAPI uint8_t IsHyphen(char ch) {
	return (ch == '-');
}

SALAPI uint8_t IsDot(char ch) {
	return (ch == '.');
}

SALAPI uint8_t AreCharsEqual(char ch1, char ch2) {
	return (ch1 == ch2);
}

SALAPI uint8_t AreStringsEqual(int count, char* str1, char* str2) {
	for (int i = 0; i < count; i++) {
		if (str1 == NULL || str2 == NULL)
			return 0;
		if (*str1 != *str2) {
			return 0;
		}
		str1++;
		str2++;
	}
	return 1;
}

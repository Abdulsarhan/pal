#ifndef SAL_H
#define SAL_H

#ifdef _WIN32
#include <glad/glad.h>
#include <gl/gl.h>
#include <GL/glext.h> 
#include <GL/wglext.h>
#endif

#include "types.h"

#if defined(_WIN32)
#if defined(__TINYC__)
#define __declspec(x) __attribute__((x))
#endif
#if defined(BUILD_LIBTYPE_SHARED)
#define SALAPI __declspec(dllexport)     // We are building the library as a Win32 shared library (.dll)
#elif defined(USE_LIBTYPE_SHARED)
#define SALAPI __declspec(dllimport)     // We are using the library as a Win32 shared library (.dll)
#endif
#else
#if defined(BUILD_LIBTYPE_SHARED)
#define SALAPI __attribute__((visibility("default"))) // We are building as a Unix shared library (.so/.dylib)
#endif
#endif

#ifndef SALAPI
#define SALAPI       // Functions defined as 'extern' by default (implicit specifiers)
#endif

//----------------------------------------------------------------------------------
// Some basic Defines
//----------------------------------------------------------------------------------
#ifndef PI
#define PI 3.14159265358979323846f
#endif
#ifndef DEG2RAD
#define DEG2RAD (PI/180.0f)
#endif
#ifndef RAD2DEG
#define RAD2DEG (180.0f/PI)
#endif

#if defined(__cplusplus)
#define CLITERAL(type)      type
#else
#define CLITERAL(type)      (type)
#endif

#ifdef _WIN32
#define KEY_DOWN_BIT 0x80
#define KEY_REPEAT_BIT 0b1
typedef HWND Window;
typedef HMONITOR Monitor;
static MSG msg = { 0 };
static HDC fakeDC = { 0 };
static HDC hdc = { 0 };
static HGLRC rc = {0};
static HDC currenthdc;
typedef struct Input {
	BYTE KeyBuffer[256];
	BYTE KeysPressed[32]; // We pack bits here.
	BYTE KeysReleased[32]; // We Also pack bits here.
	u32 MouseButtons; // 4
	u32 MouseButtonsProcessed; // 4
}Input;
Input input = { 0 };
#endif

typedef struct VideoMode {
	int width;
	int height;
}VideoMode;

SALAPI void SalInit();
SALAPI Window InitWindow(int width, int height, const char* windowTitle);
SALAPI VideoMode GetVideoMode(Monitor monitor);
SALAPI Monitor GetPrimaryMonitor();
SALAPI bool WindowShouldNotClose();
SALAPI void PollEvents(Window window);
SALAPI i32 MakeContextCurrent(Window window);

// Mouse input
SALAPI bool IsMousePressed(i32 button);
SALAPI bool IsMouseHeld(i32 button);
SALAPI bool IsMouseProccessed(i32 button);
SALAPI void SetMouseProccessed(i32 button);
SALAPI void SetMouseState(i32 button, bool state);

// Keyboard Input
SALAPI bool IsKeyPressed(i32 key);
SALAPI bool IsKeyReleased(i32 key);
SALAPI bool IsKeyDown(i32 key);
SALAPI bool IsKeypressDone(i32 key);
SALAPI void SetKeyPressDone(i32 key);
SALAPI void UnsetKeyPressed(i32 key);
SALAPI void SetKeyReleased(i32 key);
SALAPI void UnsetKeyReleased(i32 key);

SALAPI void SetKeyState(i32 key, bool state);
SALAPI void BeginDrawing();
SALAPI void EndDrawing();

#ifdef _WIN32
LRESULT CALLBACK Win32WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Win32WindowResizeCallback(HWND hwnd, UINT flag, int width, int height);
void Win32KeyDownCallback(HWND hwnd, WPARAM key);
void Win32KeyUpCallback(HWND hwnd, WPARAM key);
void Win32InitOpenGL();
#endif // _WIN32

void SalInit() {

}

// Mouse input
bool IsMousePressed(i32 button) {

	if (IsMouseHeld(button) && !IsMouseProccessed(button)) {
		SetMouseProccessed(button);
		return true;
	}
	else {
		return false;
	}
}

bool IsMouseHeld(i32 button) {
	return (input.MouseButtons & (1U << button));
}

bool IsMouseProccessed(i32 button) {
	return (input.MouseButtonsProcessed & (1U << button));
}

void SetMouseProccessed(i32 button) {
	input.MouseButtonsProcessed |= (1U << button); // Mark as processed
}

void SetMouseState(i32 button, bool state) {
	if (state) {
		input.MouseButtons |= (1U << (button));  // Set the bit
	}
	else {
		input.MouseButtons &= ~(1U << (button)); // Clear the bit
	}
}

// Keyboard input
//Returns true the first time a key is pressed during a frame, will return false for subsequent frames until the key is released.
bool IsKeyPressed(i32 key) {

	if (IsKeyDown(key) && !IsKeypressDone(key)) {
		SetKeyPressDone(key);
		return true;
	}
	else {
		return false;
	}

}

// Returns true on the first frame a key is released after being pressed, returns false for subsequent frames until the key is pressed & released again.
bool IsKeyReleased(i32 key) {

	if (input.KeysReleased[key / 8] & (1U << (key % 8))) {
		UnsetKeyReleased(key);
		return true;
	}
	else {
		return false;
	}

}

bool IsKeypressDone(i32 key) {
	return (input.KeysPressed[key / 8] & (1U << (key % 8)));
}

void SetKeyPressDone(i32 key) {
	input.KeysPressed[key / 8] |= (1U << (key % 8));  // Mark as processed
}

void UnsetKeyPressed(i32 key) {
	input.KeysPressed[key / 8] &= ~(1U << (key % 8));  // Mark as processed
}

void SetKeyReleased(i32 key) {
	input.KeysReleased[key / 8] |= (1U << (key % 8));  // Mark as processed
}

void UnsetKeyReleased(i32 key) {
	input.KeysReleased[key / 8] &= ~(1U << (key % 8));  // Mark as processed
}

void SetKeyState(i32 key, bool state) {
	if (state) {
		input.KeyBuffer[key] |= KEY_DOWN_BIT; // Set the bit
	}
	else {
		input.KeyBuffer[key] &= ~KEY_DOWN_BIT; // Clear the bit
	}
}

#ifdef _WIN32
// Returns true so long as the key is pressed down.
bool IsKeyDown(i32 key) {
	(void)GetKeyboardState(input.KeyBuffer);
	return (input.KeyBuffer[key] & KEY_DOWN_BIT);

}

bool IsKeyPressedRepeat(i32 key) {
	if (GetAsyncKeyState(key) & 0b1)
		return true;
}
#endif

#ifdef _WIN32
#include <Windows.h>
#include "windowsx.h"

// Called whenever the window manager recieves a message about a key being pressed or released.
void Win32KeyDownCallback(HWND hwnd, WPARAM key) {
	// Ensure key is within a valid range (0 to 256)
	UnsetKeyReleased(key);
}

// This function fails to get called whenever a message box appears, because when a message box appears, the main window stops recieving messages.
void Win32KeyUpCallback(HWND hwnd, WPARAM key) {
	UnsetKeyPressed(key);
	SetKeyReleased(key);

}

void Win32WindowResizeCallback(HWND hwnd, UINT flag, int width, int height)
{
	// Handle resizing
}

LRESULT CALLBACK Win32WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Screen Painting Shit. Might get rid of it.
	PAINTSTRUCT ps = {0};

	// Keyboard Shit.
	SetFocus(hwnd);

	// Mouse Shit
	UINT button = GET_XBUTTON_WPARAM(wParam);
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);

	// Screen Size Shit.
	int width = LOWORD(lParam);  // Macro to get the low-order word.
	int height = HIWORD(lParam); // Macro to get the high-order word.

	switch (uMsg) {

	case WM_SIZE:

		// Respond to the message:
		Win32WindowResizeCallback(hwnd, (UINT)wParam, width, height);

		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		DestroyWindow(hwnd);
		break;

	case WM_LBUTTONDOWN:
		MessageBoxA(hwnd, "BUTTON CLICKED!", "You Clicked the Left Mouse Button!", MB_OK);
		break;

	case WM_RBUTTONDOWN:
		MessageBoxA(hwnd, "BUTTON CLICKED!", "You Clicked the Right Mouse Button!", MB_OK);
		break;

	case WM_MBUTTONDOWN:
		MessageBoxA(hwnd, "BUTTON CLICKED!", "You Clicked the Middle Mouse Button!", MB_OK);
		break;

	case WM_XBUTTONDOWN:

		if (button == XBUTTON1)
		{
			MessageBoxA(hwnd, "BUTTON CLICKED!", "You Clicked a side mouse button!", MB_OK);

		}
		else if (button == XBUTTON2)
		{
			MessageBoxA(hwnd, "BUTTON CLICKED!", "You Clicked a side mouse button!", MB_OK);

		}
		break;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		Win32KeyDownCallback(hwnd, wParam);
		break;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		Win32KeyUpCallback(hwnd, wParam);
		break;

	case WM_PAINT:
		break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

WNDCLASSEXA RegisterWindowClass() {
	WNDCLASSEXA wc = { 0 };

	wc.cbSize = sizeof(WNDCLASSEXA);
	wc.lpfnWndProc = Win32WindowProc;
	wc.hInstance = GetModuleHandleA(0);
	wc.lpszClassName = "Win32 Window Classs";
	wc.hCursor = LoadCursorA(NULL, IDC_ARROW);

	RegisterClassExA(&wc);
	return wc;
}

Window InitWindow(int width, int height, const char* windowTitle) {

	WNDCLASSEXA fakewc = RegisterWindowClass();

	Window fakewindow = CreateWindowExA(
		0,                              // Optional window styles.
		fakewc.lpszClassName,                     // Window class
		"Fake Ass Window.",          // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // Parent window    
		NULL,       // Menu
		fakewc.hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (fakewindow == NULL)
	{
		return 1;
	}

	fakeDC = GetDC(fakewindow);

	PIXELFORMATDESCRIPTOR fakePFD;
	ZeroMemory(&fakePFD, sizeof(fakePFD));
	fakePFD.nSize = sizeof(fakePFD);
	fakePFD.nVersion = 1;
	fakePFD.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	fakePFD.iPixelType = PFD_TYPE_RGBA;
	fakePFD.cColorBits = 32;
	fakePFD.cAlphaBits = 8;
	fakePFD.cDepthBits = 24;

	int fakePFDID = ChoosePixelFormat(fakeDC, &fakePFD);

	if (fakePFDID == 0) {
		MessageBoxA(fakewindow, "ChoosePixelFormat() failed.", "Try again later", MB_ICONERROR);
		return 1;
	}
	if (SetPixelFormat(fakeDC, fakePFDID, &fakePFD) == false) {
		MessageBoxA(fakewindow, "SetPixelFormat() failed.", "Try again later", MB_ICONERROR);
		return 1;
	}

	HGLRC fakeRC = wglCreateContext(fakeDC);
	if (fakeRC == 0) {
		MessageBoxA(fakewindow, "wglCreateContext() failed.", "Try again later", MB_ICONERROR);
		return 1;
	}
	if (wglMakeCurrent(fakeDC, fakeRC) == false) {
		MessageBoxA(fakewindow, "wglMakeCurrent() failed.", "Try again later", MB_ICONERROR);
		return 1;
	}
	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;
	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)(wglGetProcAddress("wglChoosePixelFormatARB"));
	if (wglChoosePixelFormatARB == NULL) {
		MessageBoxA(fakewindow, "wglGetProcAddress() failed.", "Try again later", MB_ICONERROR);
		return 1;
	}
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)(wglGetProcAddress("wglCreateContextAttribsARB"));
	if (wglCreateContextAttribsARB == NULL) {
		MessageBoxA(fakewindow, "wglGetProcAddress() failed.", "Try again later", MB_ICONERROR);
		return 1;
	}

	WNDCLASSEXA wc = RegisterWindowClass();

	Window window = CreateWindowExA(
		0,                              // Optional window styles.
		wc.lpszClassName,                     // Window class
		windowTitle,          // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,

		NULL,       // Parent window    
		NULL,       // Menu
		wc.hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (window == NULL) {
		return -1;
	}

	hdc = GetDC(window);

	const int pixelAttribs[] = {
	WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
	WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
	WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
	WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
	WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
	WGL_COLOR_BITS_ARB, 32,
	WGL_ALPHA_BITS_ARB, 8,
	WGL_DEPTH_BITS_ARB, 24,
	WGL_STENCIL_BITS_ARB, 8,
	WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
	WGL_SAMPLES_ARB, 4, // NOTE: Maybe this is used for multisampling?
	0
	};

	int pixelFormatID; UINT numFormats;
	bool status = wglChoosePixelFormatARB(hdc, pixelAttribs, NULL, 1, &pixelFormatID, &numFormats);
	if (status == false || numFormats == 0) {
		MessageBoxA(window, "wglChoosePixelFormatARB() failed.", "Try again later", MB_ICONERROR);
		return 1;
	}

	PIXELFORMATDESCRIPTOR PFD;
	DescribePixelFormat(hdc, pixelFormatID, sizeof(PFD), &PFD);
	SetPixelFormat(hdc, pixelFormatID, &PFD);
	const int major_min = 4, minor_min = 5;

	int contextAttribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, major_min,
		WGL_CONTEXT_MINOR_VERSION_ARB, minor_min,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	rc = wglCreateContextAttribsARB(hdc, 0, contextAttribs);
	if (rc == NULL) {
		MessageBoxA(window, "wglCreateContextAttribsARB() failed.", "Try again later", MB_ICONERROR);
		return 1;
	}

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(fakeRC);
	ReleaseDC(fakewindow, fakeDC);
	DestroyWindow(fakewindow);

	ShowWindow(window, SW_SHOW);
	SetForegroundWindow(window);
	SetFocus(window);

	return window;
}

i32 MakeContextCurrent(Window window) {
	hdc = GetDC(window);
	if (!wglMakeCurrent(hdc, rc)) {
		MessageBoxA(window, "wglMakeCurrent() failed.", "Try again later", MB_ICONERROR);
		return 1;
	}
}

void PollEvents(Window window) {
	TranslateMessage(&msg);
	DispatchMessageA(&msg);

}

bool WindowShouldNotClose() {
	return GetMessageA(&msg, NULL, 0, 0);
}

VideoMode GetVideoMode(Monitor monitor) {

	if (monitor) {
		MONITORINFO mi = { 0 };

		// You now have a handle to the primary monitor
		mi.cbSize = sizeof(MONITORINFO);
		if (GetMonitorInfoA(monitor, &mi)) {

			return (VideoMode) {
				mi.rcWork.left,
				mi.rcWork.right
			};
		}
		return (VideoMode) {
			0, 0
		};
	}
	else {
		return (VideoMode) {
			0, 0
		};
	}

}

Monitor GetPrimaryMonitor() {
	// Define a point at the origin (0, 0)
	POINT ptZero = { 0, 0 };

	// Get the handle to the primary monitor
	HMONITOR hPrimaryMonitor = MonitorFromPoint(ptZero, MONITOR_DEFAULTTOPRIMARY);

	return hPrimaryMonitor;

}

void BeginDrawing() {

}

void EndDrawing() {
	SwapBuffers(hdc);
}
#endif //_WIN32

#ifdef __linux__
void InitWindow() {

}
#endif // __linux__

#endif //SAL_H

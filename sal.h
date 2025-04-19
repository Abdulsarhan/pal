#ifndef SAL_H
#define SAL_H
#include "types.h"

#ifdef _WIN32
#include <glad/glad.h>
#include <gl/gl.h>
#include <GL/glext.h> 
#include <GL/wglext.h>
#endif

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
// Math Defines
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

//----------------------------------------------------------------------------------
// Window Hint Types
//----------------------------------------------------------------------------------
#define GL_PROFILE 0x1
#define GL_VERSION_MAJOR 0x2
#define GL_VERSION_MINOR 0x3
#define RESIZABLE 0x4
#define DOUBLE_BUFFER 0x5
#define FLOATING 0x6

//----------------------------------------------------------------------------------
// Window Hint Values
//----------------------------------------------------------------------------------
#define GL_PROFILE_CORE 0x7
#define GL_PROFILE_COMPAT 0x8

//----------------------------------------------------------------------------------
// Input Modes
//----------------------------------------------------------------------------------
#define RAW_MOUSE_INPUT 0x1
#define CURSOR 0x2

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
static MSG s_msg = { 0 };
static HDC s_fakeDC = { 0 };
static HDC s_hdc = { 0 };
static HGLRC s_rc = {0};
static HDC s_currenthdc;
static int s_glVersionMajor = 3;
static int s_glVersionMinor = 3;
static int s_glProfile = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
static int s_resizable = WS_OVERLAPPEDWINDOW;
static int s_floating = 0;
static int s_doubleBuffer = PFD_DOUBLEBUFFER;

// Keyboard & Mouse Input
typedef struct Input {
	BYTE KeyBuffer[256];
	BYTE KeysPressed[32]; // We pack bits here.
	BYTE KeysReleased[32]; // We Also pack bits here.
	u32 MouseButtons; // 4
	u32 MouseButtonsProcessed; // 4
}Input;
Input input = { 0 };
#endif

typedef struct OpenglInfo {
	const char* vendor;
	const char* renderer;
	const char* version;
	const char* shadingLanguageVersion;
	const char* extensions;
}OpenglInfo;

typedef struct VideoMode {
	int width;
	int height;
}VideoMode;

SALAPI void SalInit();
SALAPI Window (* InitWindow)(int width, int height, const char* windowTitle);
SALAPI void (*SetWindowHint)(int type, int value);
SALAPI VideoMode (* GetVideoMode)(Monitor monitor);
SALAPI Monitor (* GetPrimaryMonitor)(void);
SALAPI bool (* WindowShouldNotClose)(void);
SALAPI void (* PollEvents)(void);
SALAPI i32 (* MakeContextCurrent)(Window window);
SALAPI bool (* IsMousePressed)(i32 button);
SALAPI bool (* IsMouseHeld)(i32 button);
SALAPI bool (* IsMouseProccessed)(i32 button);
SALAPI void (* SetMouseProccessed)(i32 button);
SALAPI void (* SetMouseState)(i32 button, bool state);
SALAPI bool (* IsKeyPressed)(i32 key);
SALAPI bool (* IsKeyPressedRepeat)(i32 key);
SALAPI bool (* IsKeyReleased)(i32 key);
SALAPI bool (* IsKeyDown)(i32 key);
SALAPI bool (* IsKeypressDone)(i32 key);
SALAPI void (* SetKeyPressDone)(i32 key);
SALAPI void (* UnsetKeyPressed)(i32 key);
SALAPI void (* SetKeyReleased)(i32 key);
SALAPI void (* UnsetKeyReleased)(i32 key);
SALAPI void (* SetKeyState)(i32 key, bool state);
SALAPI void (* BeginDrawing)(void);
SALAPI void (* EndDrawing)(void);
SALAPI OpenglInfo (* GetOpenglInfo)(void);
SALAPI bool (* IsWhiteSpace)(char* ch);
SALAPI bool (* AreStringsEqual)(i32 count, char* str1, char* str2);
SALAPI bool (*IsEndOfLine)(char* ch);

static void Win32WindowHint(i32 type, i32 value);
static bool SalIsMousePressed(i32 button);
static bool SalIsMouseHeld(i32 button);
static bool SalIsMouseProccessed(i32 button);
static void SalSetMouseProccessed(i32 button);
static void SalSetMouseState(i32 button, bool state);
static bool SalIsKeyPressed(i32 key);
static bool SalIsKeyReleased(i32 key);
static bool SalIsKeypressDone(i32 key);
static void SalSetKeyPressDone(i32 key);
static void SalUnsetKeyPressed(i32 key);
static void SalSetKeyReleased(i32 key);
static void SalUnsetKeyReleased(i32 key);
static void SalSetKeyState(i32 key, bool state);
void SalBeginDrawing();
OpenglInfo SalGetOpenglInfo();
static bool SalIsWhiteSpace(char* ch);
static bool SalAreStringsEqual(i32 count, char* str1, char* str2);
static bool SalIsEndOfLine(char* ch);

#ifdef _WIN32
static HWND Win32InitWindow(i32 width, i32 height, const char* windowTitle);
static VideoMode Win32GetVideoMode(HMONITOR monitor);
static HMONITOR Win32GetPrimaryMonitor(void);
static bool Win32WindowShouldNotClose(void);
static void Win32PollEvents(void);
static i32 Win32MakeContextCurrent(HWND hwnd);
static bool Win32IsKeyPressedRepeat(i32 key);
static bool Win32IsKeyDown(i32 key);
void Win32EndDrawing();
LRESULT CALLBACK Win32WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Win32WindowResizeCallback(HWND hwnd, UINT flag, i32 width, i32 height);
void Win32KeyDownCallback(HWND hwnd, WPARAM key);
void Win32KeyUpCallback(HWND hwnd, WPARAM key);
void Win32InitOpenGL();
#endif // _WIN32

void SalInit() {
#ifdef _WIN32
	SetWindowHint = Win32WindowHint;
	InitWindow = Win32InitWindow;
	GetVideoMode = Win32GetVideoMode;
	GetPrimaryMonitor = Win32GetPrimaryMonitor;
	WindowShouldNotClose = Win32WindowShouldNotClose;
	PollEvents = Win32PollEvents;
	MakeContextCurrent = Win32MakeContextCurrent;
	IsKeyPressedRepeat = Win32IsKeyPressedRepeat;
	IsKeyDown = Win32IsKeyDown;
	EndDrawing = Win32EndDrawing;

#endif
	IsMousePressed = SalIsMousePressed;
	IsMouseHeld = SalIsMouseHeld;
	IsMouseProccessed = SalIsMouseProccessed;
	SetMouseProccessed = SalSetMouseProccessed;
	SetMouseState = SalSetMouseState;
	IsKeyPressed = SalIsKeyPressed;
	IsKeyReleased = SalIsKeyReleased;
	IsKeypressDone = SalIsKeypressDone;
	SetKeyPressDone = SalSetKeyPressDone;
	UnsetKeyPressed = SalUnsetKeyPressed;
	SetKeyReleased = SalSetKeyReleased;
	UnsetKeyReleased = SalUnsetKeyReleased;
	SetKeyState = SalSetKeyState;
	BeginDrawing = SalBeginDrawing;
	GetOpenglInfo = SalGetOpenglInfo;
	IsWhiteSpace = SalIsWhiteSpace;
	AreStringsEqual = SalAreStringsEqual;
	IsEndOfLine = SalIsEndOfLine;
}

// Mouse input
static bool SalIsMousePressed(i32 button) {

	if (IsMouseHeld(button) && !IsMouseProccessed(button)) {
		SetMouseProccessed(button);
		return true;
	}
	else {
		return false;
	}
}

static bool SalIsMouseHeld(i32 button) {
	return (input.MouseButtons & (1U << button));
}

static bool SalIsMouseProccessed(i32 button) {
	return (input.MouseButtonsProcessed & (1U << button));
}

static void SalSetMouseProccessed(i32 button) {
	input.MouseButtonsProcessed |= (1U << button); // Mark as processed
}

static void SalSetMouseState(i32 button, bool state) {
	if (state) {
		input.MouseButtons |= (1U << (button));  // Set the bit
	}
	else {
		input.MouseButtons &= ~(1U << (button)); // Clear the bit
	}
}

// Keyboard input
//Returns true the first time a key is pressed during a frame, will return false for subsequent frames until the key is released.
static bool SalIsKeyPressed(i32 key) {

	if (IsKeyDown(key) && !IsKeypressDone(key)) {
		SetKeyPressDone(key);
		return true;
	}
	else {
		return false;
	}

}

// Returns true on the first frame a key is released after being pressed, returns false for subsequent frames until the key is pressed & released again.
static bool SalIsKeyReleased(i32 key) {

	if (input.KeysReleased[key / 8] & (1U << (key % 8))) {
		UnsetKeyReleased(key);
		return true;
	}
	else {
		return false;
	}

}

static bool SalIsKeypressDone(i32 key) {
	return (input.KeysPressed[key / 8] & (1U << (key % 8)));
}

static void SalSetKeyPressDone(i32 key) {
	input.KeysPressed[key / 8] |= (1U << (key % 8));  // Mark as processed
}

static void SalUnsetKeyPressed(i32 key) {
	input.KeysPressed[key / 8] &= ~(1U << (key % 8));  // Mark as processed
}

static void SalSetKeyReleased(i32 key) {
	input.KeysReleased[key / 8] |= (1U << (key % 8));  // Mark as processed
}

static void SalUnsetKeyReleased(i32 key) {
	input.KeysReleased[key / 8] &= ~(1U << (key % 8));  // Mark as processed
}

static void SalSetKeyState(i32 key, bool state) {
	if (state) {
		input.KeyBuffer[key] |= KEY_DOWN_BIT; // Set the bit
	}
	else {
		input.KeyBuffer[key] &= ~KEY_DOWN_BIT; // Clear the bit
	}
}

static OpenglInfo SalGetOpenglInfo(void) {
	OpenglInfo info = {0};
	info.vendor = (char*) glGetString(GL_VENDOR);
	info.renderer = (char*)glGetString(GL_RENDERER);
	info.version = (char*)glGetString(GL_VERSION);
	info.shadingLanguageVersion = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	info.extensions;
	i32 numExtensions;
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

	for (i32 i = 0; i < numExtensions; i++) {
		const char* ext = (const char*)glGetStringi(GL_EXTENSIONS, i);
		OutputDebugString(ext);
	}
	/*
	char* at = info.extensions;
	while (*at) {
		while (IsWhiteSpace(*at)) { ++at; }
		char* end = at;
		while (*end && !IsWhiteSpace(*end)) { ++end; }

		uintptr_t count = end - at;

		if (0) {}
		else if (AreStringsEqual(count, at, "EXT_TEXTURE_SRGB_DECODE")) {
			OutputDebugString("INFO: They Are fucking Equal!");
		}
		at = end;

	}
	*/
	return info;
}

static bool SalIsWhiteSpace(char* ch) {
	return ((ch == ' ') ||
		   (ch == '\t') ||
		   (ch == '\v') ||
		   (ch = '\f')  ||
		   IsEndOfLine(ch));
}

static bool SalAreStringsEqual(i32 count, char* str1, char* str2) {
	for (i32 i = 0; i < count; i++) {
		if (str1 == NULL || str2 == NULL)
			return false;
		if (*str1 != *str2) {
			return false;
		}
		str1++;
		str2++;
	}
	return true;
}

static bool SalIsEndOfLine(char* ch) {
	return ((ch == '\n') || (ch == '\r'));
}

#ifdef _WIN32
// Returns true so long as the key is pressed down.
static bool Win32IsKeyDown(i32 key) {
	(void)GetKeyboardState(input.KeyBuffer);
	return (input.KeyBuffer[key] & KEY_DOWN_BIT);

}

static bool Win32IsKeyPressedRepeat(i32 key) {
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

void Win32WindowResizeCallback(HWND hwnd, UINT flag, i32 width, i32 height)
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

// Window Hints
static void Win32WindowHint(i32 type, i32 value) {
	switch (type) {
	case GL_VERSION_MAJOR: s_glVersionMajor = value;
		break;
	case GL_VERSION_MINOR: s_glVersionMinor = value;
		break;
	case RESIZABLE:
		if (value)
			s_resizable = WS_OVERLAPPEDWINDOW;
		else
			s_resizable = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
		break;
	case FLOATING:
		if (value)
			s_floating = WS_EX_TOPMOST;
		else
			s_floating = 0;
		break;
	case DOUBLE_BUFFER:
		if (value)
			s_doubleBuffer = PFD_DOUBLEBUFFER;
		else
			s_doubleBuffer = 0x00000000;
		break;
	case GL_PROFILE:
		if (value == GL_PROFILE_CORE)
			s_glProfile = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
		if (value == GL_PROFILE_COMPAT)
			s_glProfile = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
		break;
	}
}
static HWND Win32InitWindow(i32 width, i32 height, const char* windowTitle) {

	WNDCLASSEXA fakewc = RegisterWindowClass();

	HWND fakehwnd = CreateWindowExA(
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

	if (fakehwnd == NULL)
	{
		return 1;
	}

	s_fakeDC = GetDC(fakehwnd);

	PIXELFORMATDESCRIPTOR fakePFD;
	ZeroMemory(&fakePFD, sizeof(fakePFD));
	fakePFD.nSize = sizeof(fakePFD);
	fakePFD.nVersion = 1;
	fakePFD.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	fakePFD.iPixelType = PFD_TYPE_RGBA;
	fakePFD.cColorBits = 32;
	fakePFD.cAlphaBits = 8;
	fakePFD.cDepthBits = 24;

	int fakePFDID = ChoosePixelFormat(s_fakeDC, &fakePFD);

	if (fakePFDID == 0) {
		MessageBoxA(fakehwnd, "ChoosePixelFormat() failed.", "Try again later", MB_ICONERROR);
		return 1;
	}
	if (SetPixelFormat(s_fakeDC, fakePFDID, &fakePFD) == false) {
		MessageBoxA(fakehwnd, "SetPixelFormat() failed.", "Try again later", MB_ICONERROR);
		return 1;
	}

	HGLRC fakeRC = wglCreateContext(s_fakeDC);
	if (fakeRC == 0) {
		MessageBoxA(fakehwnd, "wglCreateContext() failed.", "Try again later", MB_ICONERROR);
		return 1;
	}
	if (wglMakeCurrent(s_fakeDC, fakeRC) == false) {
		MessageBoxA(fakehwnd, "wglMakeCurrent() failed.", "Try again later", MB_ICONERROR);
		return 1;
	}
	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;
	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)(wglGetProcAddress("wglChoosePixelFormatARB"));
	if (wglChoosePixelFormatARB == NULL) {
		MessageBoxA(fakehwnd, "wglGetProcAddress() failed.", "Try again later", MB_ICONERROR);
		return 1;
	}
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)(wglGetProcAddress("wglCreateContextAttribsARB"));
	if (wglCreateContextAttribsARB == NULL) {
		MessageBoxA(fakehwnd, "wglGetProcAddress() failed.", "Try again later", MB_ICONERROR);
		return 1;
	}

	WNDCLASSEXA wc = RegisterWindowClass();

	HWND hwnd = CreateWindowExA(
		s_floating,           // Optional window styles.
		wc.lpszClassName,     // Window class
		windowTitle,          // Window text
		s_resizable,          // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,

		NULL,       // Parent window    
		NULL,       // Menu
		wc.hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL) {
		return -1;
	}

	s_hdc = GetDC(hwnd);

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
	bool status = wglChoosePixelFormatARB(s_hdc, pixelAttribs, NULL, 1, &pixelFormatID, &numFormats);
	if (status == false || numFormats == 0) {
		MessageBoxA(hwnd, "wglChoosePixelFormatARB() failed.", "Try again later", MB_ICONERROR);
		return 1;
	}

	PIXELFORMATDESCRIPTOR PFD;
	PFD.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | s_doubleBuffer;
	DescribePixelFormat(s_hdc, pixelFormatID, sizeof(PFD), &PFD);
	SetPixelFormat(s_hdc, pixelFormatID, &PFD);

	int contextAttribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, s_glVersionMajor,
		WGL_CONTEXT_MINOR_VERSION_ARB, s_glVersionMinor,
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
		WGL_CONTEXT_PROFILE_MASK_ARB, s_glProfile,
		0
	};

	s_rc = wglCreateContextAttribsARB(s_hdc, 0, contextAttribs);
	if (s_rc) {

		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(fakeRC);
		ReleaseDC(fakehwnd, s_fakeDC);
		DestroyWindow(fakehwnd);

		ShowWindow(hwnd, SW_SHOW);
		SetForegroundWindow(hwnd);
		SetFocus(hwnd);

		return hwnd;
		OutputDebugStringA("INFO: Using modern OpenGL Context.");
	}
	else {
		OutputDebugStringA("INFO: Using old OpenGL Context.");

		ShowWindow(fakehwnd, SW_SHOW);
		SetForegroundWindow(fakehwnd);
		SetFocus(fakehwnd);
		return fakehwnd;
	}

}

static i32 Win32MakeContextCurrent(HWND hwnd) {
	s_hdc = GetDC(hwnd);
	if (!wglMakeCurrent(s_hdc, s_rc)) {
		MessageBoxA(hwnd, "wglMakeCurrent() failed.", "Try again later", MB_ICONERROR);
		return 1;
	}
}

static void Win32PollEvents(void) {
	TranslateMessage(&s_msg);
	DispatchMessageA(&s_msg);

}

static bool Win32WindowShouldNotClose() {
	return GetMessageA(&s_msg, NULL, 0, 0);
}

static VideoMode Win32GetVideoMode(HMONITOR monitor) {

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

static Monitor Win32GetPrimaryMonitor() {
	// Define a point at the origin (0, 0)
	POINT ptZero = { 0, 0 };

	// Get the handle to the primary monitor
	HMONITOR hPrimaryMonitor = MonitorFromPoint(ptZero, MONITOR_DEFAULTTOPRIMARY);

	return hPrimaryMonitor;

}

void SalBeginDrawing() {

}

void Win32EndDrawing() {
	SwapBuffers(s_hdc);
}
#endif //_WIN32

#ifdef __linux__
void InitWindow() {

}
#endif // __linux__

#endif //SAL_H

#ifndef WIN32_PLATFORM_H
#define WIN32_PLATFORM_H

#include <Windows.h>
#include <windowsx.h>
#include <WinUser.h>
#include <gl/gl.h>
#include <GL/glext.h> 
#include <GL/wglext.h>
#include <assert.h>
#include <winDNS.h>
#include <stdio.h>

#define KEY_DOWN_BIT 0x80
#define KEY_REPEAT_BIT 0b1

#define Megabytes(x)(x * 1024 * 1024)
static MSG s_msg = { 0 };
static HDC s_fakeDC = { 0 };
static HDC s_hdc = { 0 };
static HGLRC s_rc = { 0 };
static HDC s_currenthdc;
static int s_glVersionMajor = 3;
static int s_glVersionMinor = 3;
static int s_glProfile = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
static int s_resizable = WS_OVERLAPPEDWINDOW;
static int s_floating = 0;
static int s_doubleBuffer = PFD_DOUBLEBUFFER;
static int s_WindowShouldNotClose = 1;

struct Window {
	HWND handle;
};
struct Monitor {
	HMONITOR handle;
};
struct ProcAddress {
	PROC handle;
};

// Keyboard & Mouse Input
typedef struct Input {
	BYTE KeyBuffer[256];
	BYTE KeysPressed[32]; // We pack bits here.
	BYTE KeysReleased[32]; // We Also pack bits here.
	unsigned int MouseButtons; // 4
	unsigned int MouseButtonsPressed; // 4
	unsigned int MouseButtonsReleased; // 4
}Input;
Input input = { 0 };

typedef struct {
	RAWINPUT* buffer;
	UINT bufferSize;
}RawInputStruct;
RawInputStruct rawInputStruct = {0};

static void Win32SetWindowHint(int type, int value);
static Window* Win32InitWindow(int width, int height, const char* windowTitle);
static VideoMode* Win32GetVideoMode(Monitor* monitor);
static Monitor* Win32GetPrimaryMonitor(void);
static uint8_t Win32WindowShouldClose(void);
static void Win32PollEvents(void);
static int Win32MakeContextCurrent(HWND window);

void Win32BeginDrawing();
void Win32EndDrawing();
LRESULT CALLBACK Win32WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Win32WindowResizeCallback(HWND hwnd, UINT flag, int width, int height);
static uint8_t Win32IsWhiteSpace(char* ch);
static uint8_t Win32AreStringsEqual(int count, char* str1, char* str2);
static uint8_t Win32IsEndOfLine(char* ch);
void* Win32LoadDynamicLibrary(char* dll);
void* Win32LoadDynamicFunction(HMODULE dll, char* func_name);
uint8_t Win32FreeDynamicLibrary(HMODULE dll);

void Win32WindowResizeCallback(HWND hwnd, UINT flag, int width, int height)
{
	// Handle resizing
}

LRESULT CALLBACK Win32WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
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
		s_WindowShouldNotClose = 0;
		PostQuitMessage(0);
		break;

	case WM_DESTROY:
		DestroyWindow(hwnd);
		s_WindowShouldNotClose = 0;
		break;

	case WM_PAINT:
		break;
	case WM_SYSKEYUP:
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
	case WM_KEYUP:
		break;
	}


	return DefWindowProcA(hwnd, uMsg, wParam, lParam);
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
static void Win32SetWindowHint(int type, int value) {
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

static Window* Win32InitWindow(int width, int height, const char* windowTitle) {
	Window* fakewindow = (Window*)malloc(sizeof(Window));
	WNDCLASSEXA fakewc = RegisterWindowClass();

	fakewindow->handle = CreateWindowExA(
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

	if (fakewindow->handle == NULL)
	{
		return fakewindow;
	}

	s_fakeDC = GetDC(fakewindow->handle);

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
		MessageBoxA(fakewindow->handle, "ChoosePixelFormat() failed.", "Try again later", MB_ICONERROR);
		return fakewindow;
	}
	if (SetPixelFormat(s_fakeDC, fakePFDID, &fakePFD) == 0) {
		MessageBoxA(fakewindow->handle, "SetPixelFormat() failed.", "Try again later", MB_ICONERROR);
		return fakewindow;
	}

	HGLRC fakeRC = wglCreateContext(s_fakeDC);
	if (fakeRC == 0) {
		MessageBoxA(fakewindow->handle, "wglCreateContext() failed.", "Try again later", MB_ICONERROR);
		return fakewindow;
	}
	if (wglMakeCurrent(s_fakeDC, fakeRC) == 0) {
		MessageBoxA(fakewindow->handle, "wglMakeCurrent() failed.", "Try again later", MB_ICONERROR);
		return fakewindow;
	}
	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;
	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)(wglGetProcAddress("wglChoosePixelFormatARB"));
	if (wglChoosePixelFormatARB == NULL) {
		MessageBoxA(fakewindow->handle, "wglGetProcAddress() failed.", "Try again later", MB_ICONERROR);
		return fakewindow;
	}
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)(wglGetProcAddress("wglCreateContextAttribsARB"));
	if (wglCreateContextAttribsARB == NULL) {
		MessageBoxA(fakewindow->handle, "wglGetProcAddress() failed.", "Try again later", MB_ICONERROR);
		return fakewindow;
	}

	WNDCLASSEXA wc = RegisterWindowClass();
	Window* window = (Window*)malloc(sizeof(Window));
	window->handle = CreateWindowExA(
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

	if (window->handle == NULL) {
		return window;
	}

	s_hdc = GetDC(window->handle);

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
	uint8_t status = wglChoosePixelFormatARB(s_hdc, pixelAttribs, NULL, 1, &pixelFormatID, &numFormats);
	if (status == 0 || numFormats == 0) {
		MessageBoxA(window->handle, "wglChoosePixelFormatARB() failed.", "Try again later", MB_ICONERROR);
		return window;
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
		ReleaseDC(fakewindow->handle, s_fakeDC);
		DestroyWindow(fakewindow->handle);

		ShowWindow(window->handle, SW_SHOWNORMAL);
		SetForegroundWindow(window->handle);
		SetFocus(window->handle);
		OutputDebugStringA("INFO: Using modern OpenGL Context.");
		return window;
	}
	else {

		ShowWindow(fakewindow->handle, SW_SHOW);
		SetForegroundWindow(fakewindow->handle);
		SetFocus(fakewindow->handle);
		OutputDebugStringA("INFO: Using old OpenGL Context.");
		return fakewindow;
	}

}

static int Win32MakeContextCurrent(HWND hwnd) {
	s_hdc = GetDC(hwnd);
	if (!wglMakeCurrent(s_hdc, s_rc)) {
		MessageBoxA(hwnd, "wglMakeCurrent() failed.", "Try again later", MB_ICONERROR);
		return 1;
	}
}

static void Win32PollEvents(void) {
	Win32GetRawInputBuffer();

	while (PeekMessageA(&s_msg, NULL , 0, 0, PM_REMOVE)) {
		if (s_msg.message == WM_QUIT) {
			s_WindowShouldNotClose = 0;
			exit(0);
		}
		TranslateMessage(&s_msg);
		DispatchMessageA(&s_msg);
	}

}

static uint8_t Win32WindowShouldClose() {
	return s_WindowShouldNotClose;
}

static VideoMode* Win32GetVideoMode(Monitor* monitor) {

	MONITORINFO mi = { 0 };
	VideoMode* videoMode = calloc(1, sizeof(VideoMode));
	
	if (monitor) {
		mi.cbSize = sizeof(MONITORINFO);
		if (GetMonitorInfoA(monitor->handle, &mi)) {
			videoMode->width = mi.rcWork.left;
			videoMode->height = mi.rcWork.right;
		}
		else {
			printf("ERROR: Couldn't get monitor info!\n");
		}
	}
	else {
		printf("ERROR: invalid pointer to monitor!\n");

	}
	return videoMode;
}

static Monitor* Win32GetPrimaryMonitor() {
	Monitor* monitor = malloc(sizeof(Monitor));
	// Define a point at the origin (0, 0)
	POINT ptZero = { 0, 0 };

	// Get the handle to the primary monitor
	monitor->handle = MonitorFromPoint(ptZero, MONITOR_DEFAULTTOPRIMARY);
	
	return monitor;

}

void Win32BeginDrawing() {

}

void Win32EndDrawing() {
	SwapBuffers(s_hdc);
}


static uint8_t Win32IsWhiteSpace(char* ch) {
	return ((ch == ' ') ||
		(ch == '\t') ||
		(ch == '\v') ||
		(ch = '\f') ||
		IsEndOfLine(ch));
}

static uint8_t Win32AreStringsEqual(int count, char* str1, char* str2) {
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

static uint8_t Win32IsEndOfLine(char* ch) {
	return ((ch == '\n') || (ch == '\r'));
}

#define MAX_RAW_INPUTS 16

// Handler function signatures
typedef void (*RawInputHandler)(const RAWINPUT*);

// Mouse input handler
void Win32HandleMouse(const RAWINPUT* raw) {
	LONG dx = raw->data.mouse.lLastX;
	LONG dy = raw->data.mouse.lLastY;
	USHORT buttons = raw->data.mouse.usButtonFlags;
	printf("Mouse: dx=%ld dy=%ld buttons=0x%04x\n", dx, dy, buttons);
}

// Keyboard input handler
void Win32HandleKeyboard(const RAWINPUT* raw) {
	USHORT key = raw->data.keyboard.VKey;
	BOOL keyUp = (raw->data.keyboard.Flags & RI_KEY_BREAK) != 0;
	printf("Keyboard: key=0x%02x %s\n", key, keyUp ? "up" : "down");
}

// Handles Gamepads, Joysticks, Steering wheels, etc...
void Win32HandleHID(const RAWINPUT* raw) {
	// Ignore unhandled device types (e.g. HID)
}

// Handler function table indexed by dwType (0 = mouse, 1 = keyboard, 2 = HID)
RawInputHandler Win32InputHandlers[3] = {
	Win32HandleMouse,      // RIM_TYPEMOUSE (0)
	Win32HandleKeyboard,   // RIM_TYPEKEYBOARD (1)
	Win32HandleHID        // RIM_TYPEHID (2) This is for joysticks, gamepads, and steering wheels.
};

int Win32RegisterRawInputDevices(HWND window) {
	UINT numDevices = 0;

	if (GetRawInputDeviceList(NULL, &numDevices, sizeof(RAWINPUTDEVICELIST)) != 0) {
		MessageBoxA(window, "ERROR:", "Failed to get the number of devices!", MB_ICONERROR);
		exit(-1);
	}

	RAWINPUTDEVICELIST* deviceList = (RAWINPUTDEVICELIST*)malloc(sizeof(RAWINPUTDEVICELIST) * numDevices);
	if (!deviceList) {
		MessageBoxA(window, "ERROR:", "Couldn't allocate memory for deviceList!", MB_ICONERROR);
		exit(-1);
	}

	if (GetRawInputDeviceList(deviceList, &numDevices, sizeof(RAWINPUTDEVICELIST)) == (UINT)-1) {
		free(deviceList);
		MessageBoxA(window, "ERROR:", "Couldn't acquire device list!", MB_ICONERROR);
		exit(-1);
	}

	RAWINPUTDEVICE inputDevices[3];
	UINT count = 0;
	BOOL registeredMouse = FALSE, registeredKeyboard = FALSE, registeredHID = FALSE;

	for (UINT i = 0; i < numDevices && count < 3; ++i) {
		RAWINPUTDEVICE* rid = &inputDevices[count];
		rid->usUsagePage = 0x01;
		rid->dwFlags = RIDEV_INPUTSINK;
		rid->hwndTarget = window;

		switch (deviceList[i].dwType) {
		case RIM_TYPEMOUSE:
			if (!registeredMouse) {
				rid->usUsage = 0x02;
				registeredMouse = TRUE;
				++count;
			}
			break;
		case RIM_TYPEKEYBOARD:
			if (!registeredKeyboard) {
				rid->usUsage = 0x06;
				registeredKeyboard = TRUE;
				++count;
			}
			break;
		case RIM_TYPEHID:
			if (!registeredHID) {
				rid->usUsage = 0x05;
				registeredHID = TRUE;
				++count;
			}
			break;
		}
	}

	if (count > 0) {
		if (RegisterRawInputDevices(inputDevices, count, sizeof(RAWINPUTDEVICE))) {
			printf("INFO: Successfully Registered raw input devices!\n");
		}
		else {
			MessageBoxA(window, "ERROR:", "Couldn't register raw input devices!", MB_ICONERROR);
		}
	}
	else {
		MessageBoxA(window, "ERROR:", "No input devices to register!", MB_ICONERROR);
	}

	free(deviceList);
	return 0;
}

#define RAW_INPUT_BUFFER_CAPACITY (64 * 1024) // 64 KB

static BYTE g_rawInputBuffer[RAW_INPUT_BUFFER_CAPACITY];

int Win32GetRawInputBuffer() {
	UINT bufferSize = RAW_INPUT_BUFFER_CAPACITY;
	UINT inputCount = GetRawInputBuffer((PRAWINPUT)g_rawInputBuffer, &bufferSize, sizeof(RAWINPUTHEADER));

	if (inputCount == (UINT)-1 || inputCount == 0) {
		return -1;
	}

	PRAWINPUT raw = (PRAWINPUT)g_rawInputBuffer;
	for (UINT i = 0; i < inputCount; ++i) {
		UINT type = raw->header.dwType;
		Win32InputHandlers[type](raw);
		raw = NEXTRAWINPUTBLOCK(raw);
	}

	return 0;
}

void* Win32LoadDynamicLibrary(char* dll) {
	HMODULE result = LoadLibraryA(dll);
	assert(result);
	return result;
}

void* Win32LoadDynamicFunction(HMODULE dll, char* func_name) {
	FARPROC proc = GetProcAddress(dll, func_name);
	assert(proc);
	return (void*)proc;
}

uint8_t Win32FreeDynamicLibrary(HMODULE dll) {
	uint8_t free_result = FreeLibrary(dll);
	assert(free_result);
	return (uint8_t)free_result;
}

#endif // WIN32_PLATFORM_H
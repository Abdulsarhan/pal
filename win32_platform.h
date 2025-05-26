#ifndef WIN32_PLATFORM_H
#define WIN32_PLATFORM_H

// Windows system headers
#include <Windows.h>
#include <windowsx.h>         // Useful macros (e.g., GET_X_LPARAM)
#include <xaudio2.h>          // XAudio2
#include <mmreg.h>            // WAVEFORMATEX

// C Standard Library
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

// OpenGL
#include <gl/gl.h>
#include <GL/glext.h>
#include <GL/wglext.h>

// Project headers (always last)
#include "pal_platform.h"

#define KEY_DOWN_BIT 0x80
#define KEY_REPEAT_BIT 0b1

#define MB(x)((size_t)(x) * 1024 * 1024)

typedef unsigned __int64 QWORD;

// TODO: it might be a good idea to move all of this shit into the window structure
// because the lifetime of all this shit is related to the lifetime of the window.
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

IXAudio2* g_xaudio2 = NULL;
IXAudio2MasteringVoice* g_mastering_voice = NULL;

struct Window {
	HWND handle;
};
struct Monitor {
	HMONITOR handle;
};

// Keyboard & Mouse Input
#define MAX_KEYS 256
#define MAX_MOUSEBUTTONS 32
typedef struct Input {
	uint8_t keys[MAX_KEYS];
	uint8_t keys_processed[MAX_KEYS];
	uint8_t mouse_buttons[MAX_MOUSEBUTTONS];
	uint8_t mouse_buttons_processed[MAX_MOUSEBUTTONS];
	Vector2 mouse;
}Input;
Input input = { 0 };

void Win32WindowResizeCallback(HWND hwnd, UINT flag, int width, int height)
{
	// Handle resizing
}

LRESULT CALLBACK win32_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	wc.lpfnWndProc = win32_window_proc;
	wc.hInstance = GetModuleHandleA(0);
	wc.lpszClassName = "Win32 Window Classs";
	wc.hCursor = LoadCursorA(NULL, IDC_ARROW);

	RegisterClassExA(&wc);
	return wc;
}

// Window Hints
static void platform_set_window_hint(int type, int value) {
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

static Window* platform_init_window(int width, int height, const char* windowTitle) {
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
	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if (wglSwapIntervalEXT == NULL) {
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

static int platform_make_context_current(Window* window) {
	s_hdc = GetDC(window->handle);
	s_currenthdc = s_hdc;
	if (!wglMakeCurrent(s_currenthdc, s_rc)) {
		MessageBoxA(window->handle, "wglMakeCurrent() failed.", "Try again later", MB_ICONERROR);
		return 1;
	}
	return 0;
}

static int Win32GetRawInputBuffer();

static void platform_poll_events(void) {
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

static uint8_t platform_window_should_close() {
	return s_WindowShouldNotClose;
}

static uint8_t platform_set_window_title(Window* window, const char* string) {
	return SetWindowTextA(window->handle, string);
}

static VideoMode* platform_get_video_mode(Monitor* monitor) {

	MONITORINFO mi = { 0 };
	VideoMode* videoMode = calloc(1, sizeof(VideoMode));
	
	if (monitor->handle) {
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

static Monitor* platform_get_primary_monitor() {
	Monitor* monitor = malloc(sizeof(Monitor));

	// Define a point at the origin (0, 0)
	POINT ptZero = { 0, 0 };

	// Get the handle to the primary monitor
	monitor->handle = MonitorFromPoint(ptZero, MONITOR_DEFAULTTOPRIMARY);
	
	return monitor;

}

static void* platform_gl_get_proc_address(const char* proc) {
	 
	return wglGetProcAddress(proc);
}

void platform_begin_drawing() {

}

void platform_end_drawing() {
	SwapBuffers(s_currenthdc);
}


#define MAX_RAW_INPUTS 16

// Handler function signatures
typedef void (*RawInputHandler)(const RAWINPUT*);

// Mouse input handler
void Win32HandleMouse(const RAWINPUT* raw) {
	LONG dx = raw->data.mouse.lLastX;
	LONG dy = raw->data.mouse.lLastY;
	USHORT buttons = raw->data.mouse.usButtonFlags;

	input.mouse.x = (float)dx;
	input.mouse.y = (float)dy;

	for (int i = 0; i < 16; ++i) {
		uint16_t down = (buttons >> (i * 2)) & 1;
		uint16_t up = (buttons >> (i * 2 + 1)) & 1;

		// If down is 1, set to 1; if up is 1, set to 0; otherwise leave unchanged
		input.mouse_buttons[i] = (input.mouse_buttons[i] & ~up) | down;
	}

	printf("Mouse: dx=%ld dy=%ld buttons=0x%04x\n", dx, dy, buttons);
}

void Win32HandleKeyboard(const RAWINPUT* raw) {
	USHORT key = raw->data.keyboard.VKey;
	input.keys[key] = ~(raw->data.keyboard.Flags) & RI_KEY_BREAK; 
	//printf("Key %u %s\n", key, input.keys[key] ? "up" : "down"); // FOR DEBUGGING NOCHECKIN!

}

// Handles Gamepads, Joysticks, Steering wheels, etc...
void Win32HandleHID(const RAWINPUT* raw) {
	printf("%d", raw->data.hid.dwCount);
	//TODO: TEST THIS.
}

// Handler function table indexed by dwType (0 = mouse, 1 = keyboard, 2 = HID)
RawInputHandler Win32InputHandlers[3] = {
	Win32HandleMouse,      // RIM_TYPEMOUSE (0)
	Win32HandleKeyboard,   // RIM_TYPEKEYBOARD (1)
	Win32HandleHID        // RIM_TYPEHID (2) This is for joysticks, gamepads, and steering wheels.
};

int platform_register_raw_input_devices(Window* window) {
	RAWINPUTDEVICE rid[3];

	// 1. Keyboard
	rid[0].usUsagePage = 0x01; // Generic desktop controls
	rid[0].usUsage = 0x06;     // Keyboard
	rid[0].dwFlags = RIDEV_INPUTSINK | RIDEV_DEVNOTIFY; // Receive input even when not focused
	rid[0].hwndTarget = window->handle;

	// 2. Mouse
	rid[1].usUsagePage = 0x01; // Generic desktop controls
	rid[1].usUsage = 0x02;     // Mouse
	rid[1].dwFlags = RIDEV_INPUTSINK | RIDEV_DEVNOTIFY;
	rid[1].hwndTarget = window->handle;

	// 3. Joystick/Gamepad (Note: Not all controllers appear as HIDs)
	rid[2].usUsagePage = 0x01; // Generic desktop controls
	rid[2].usUsage = 0x04;     // Joystick
	rid[2].dwFlags = RIDEV_INPUTSINK | RIDEV_DEVNOTIFY;
	rid[2].hwndTarget = window->handle;

	if (!RegisterRawInputDevices(rid, 3, sizeof(RAWINPUTDEVICE))) {
		DWORD error = GetLastError();
		printf("RegisterRawInputDevices failed. Error code: %lu\n", error);
		return -1;
	}

	printf("Raw input devices registered successfully.\n");
	return 0;
}

#define RAW_INPUT_BUFFER_CAPACITY (64 * 1024) // 8 KB

static BYTE g_rawInputBuffer[RAW_INPUT_BUFFER_CAPACITY];

static int Win32GetRawInputBuffer() {
	UINT bufferSize = RAW_INPUT_BUFFER_CAPACITY;
	UINT inputEventCount = GetRawInputBuffer((PRAWINPUT)g_rawInputBuffer, &bufferSize, sizeof(RAWINPUTHEADER));

	PRAWINPUT raw = (PRAWINPUT)g_rawInputBuffer;
	for (UINT i = 0; i < inputEventCount; ++i) {
		UINT type = raw->header.dwType;
		Win32InputHandlers[type](raw);
		raw = NEXTRAWINPUTBLOCK(raw);
	}
	return 0;
}

int platform_init_sound() {
	int hr;

	// Initialize COM (needed for XAudio2)
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr)) {
		return hr;
	}

	// Initialize XAudio2 engine
	hr = XAudio2Create(&g_xaudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if (FAILED(hr)) {
		return hr;
	}

	// Create mastering voice
	hr = g_xaudio2->lpVtbl->CreateMasteringVoice(g_xaudio2, &g_mastering_voice,
		XAUDIO2_DEFAULT_CHANNELS,
		XAUDIO2_DEFAULT_SAMPLERATE,
		0, NULL, NULL, AudioCategory_GameEffects);

	return hr;
}

// Helper function to compare GUIDs
static int is_equal_guid(const GUID* a, const GUID* b) {
	return memcmp(a, b, sizeof(GUID)) == 0;
}

static int platform_play_sound(const Sound* sound) {
	if (!g_xaudio2 || !g_mastering_voice) {
		return E_FAIL;
	}

	// PCM: 00000001-0000-0010-8000-00aa00389b71
	static const GUID KSDATAFORMAT_SUBTYPE_PCM = {
		0x00000001, 0x0000, 0x0010,
		{ 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 }
	};

	// IEEE_FLOAT: 00000003-0000-0010-8000-00aa00389b71
	static const GUID KSDATAFORMAT_SUBTYPE_IEEE_FLOAT = {
		0x00000003, 0x0000, 0x0010,
		{ 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 }
	};

	IXAudio2SourceVoice* source_voice = NULL;

	WAVEFORMATEXTENSIBLE wfex = { 0 };

	wfex.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
	wfex.Format.nChannels = sound->channels;
	wfex.Format.nSamplesPerSec = sound->sampleRate;
	wfex.Format.wBitsPerSample = sound->bitsPerSample;
	wfex.Format.nBlockAlign = (wfex.Format.nChannels * wfex.Format.wBitsPerSample) / 8;
	wfex.Format.nAvgBytesPerSec = wfex.Format.nSamplesPerSec * wfex.Format.nBlockAlign;
	wfex.Format.cbSize = 22;

	wfex.Samples.wValidBitsPerSample = sound->bitsPerSample;
	wfex.dwChannelMask = (sound->channels == 2) ? SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT : 0;
	wfex.SubFormat = (sound->bitsPerSample == 32)
		? KSDATAFORMAT_SUBTYPE_IEEE_FLOAT
		: KSDATAFORMAT_SUBTYPE_PCM;

	// Create the source voice
	HRESULT hr = g_xaudio2->lpVtbl->CreateSourceVoice(g_xaudio2, &source_voice, (const WAVEFORMATEX*)&wfex,
		0, XAUDIO2_DEFAULT_FREQ_RATIO,
		NULL, NULL, NULL);
	if (FAILED(hr)) {
		return hr;
	}

	// Prepare the buffer
	XAUDIO2_BUFFER buffer = {
		.AudioBytes = (UINT32)sound->dataSize,
		.pAudioData = sound->data,
		.Flags = XAUDIO2_END_OF_STREAM

	};

	// Submit the buffer
	hr = source_voice->lpVtbl->SubmitSourceBuffer(source_voice, &buffer, NULL);
	if (FAILED(hr)) {
		source_voice->lpVtbl->DestroyVoice(source_voice);
		return hr;
	}

	// Start playing
	hr = source_voice->lpVtbl->Start(source_voice, 0, 0);
	if (FAILED(hr)) {
		source_voice->lpVtbl->DestroyVoice(source_voice);
		return hr;
	}

	// Voice will destroy itself after playback if managed elsewhere
	return S_OK;
}

void* platform_load_dynamic_library(char* dll) {
	HMODULE result = LoadLibraryA(dll);
	assert(result);
	return result;
}

void* platform_load_dynamic_function(void* dll, char* func_name) {
	FARPROC proc = GetProcAddress(dll, func_name);
	assert(proc);
	return (void*)proc;
}

uint8_t platform_free_dynamic_library(void* dll) {
	uint8_t free_result = FreeLibrary(dll);
	assert(free_result);
	return (uint8_t)free_result;
}

#endif // WIN32_PLATFORM_H
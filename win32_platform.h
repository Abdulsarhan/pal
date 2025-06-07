#ifndef WIN32_PLATFORM_H
#define WIN32_PLATFORM_H

// Windows system headers
#include <Windows.h>
#include <windowsx.h>         // Useful macros (e.g., GET_X_LPARAM)
#include <xaudio2.h>          // XAudio2
#include <mmreg.h>            // WAVEFORMATEX
#include <Xinput.h>

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
static int s_glVersionMajor = 3;
static int s_glVersionMinor = 3;
static int s_glProfile = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
static int s_resizable = WS_OVERLAPPEDWINDOW;
static int s_floating = 0;
static int s_doubleBuffer = PFD_DOUBLEBUFFER;

IXAudio2* g_xaudio2 = NULL;
IXAudio2MasteringVoice* g_mastering_voice = NULL;

struct pal_window {
	HWND hwnd;
	HDC hdc;
	HGLRC hglrc;
};

struct pal_monitor {
	HMONITOR handle;
};

struct pal_common_event {
    int member;
};

struct pal_display_event {
    int member;
};

struct pal_window_event {
    int member;
};

struct pal_keyboard_device_event {
    int member;
};

struct pal_keyboard_event {
    int member;
};

struct pal_text_editing_event {
    int member;
};

struct pal_text_editing_candidates_event {
    int member;
};

struct pal_text_input_event {
    int member;
};

struct pal_mouse_device_event {
    int member;
};

struct pal_mouse_motion_event {
    int member;
};

struct pal_mouse_button_event {
    int member;
};

struct pal_mouse_wheel_event {
    int member;
};

struct pal_joy_device_event {
    int member;
};

struct pal_joy_axis_event {
    int member;
};

struct pal_joy_ball_event {
    int member;
};

struct pal_joy_hat_event {
    int member;
};

struct pal_joy_button_event {
    int member;
};

struct pal_joy_battery_event {
    int member;
};

struct pal_gamepad_device_event {
    int member;
};

struct pal_gamepad_axis_event {
    int member;
};

struct pal_gamepad_button_event {
    int member;
};

struct pal_gamepad_touchpad_event {
    int member;
};

struct pal_gamepad_sensor_event {
    int member;
};

struct pal_audio_device_event {
    int member;
};

struct pal_camera_device_event {
    int member;
};

struct pal_sensor_event {
    int member;
};

struct pal_quit_event {
    int member;
};

struct pal_user_event {
    int member;
};

struct pal_touch_finger_event {
    int member;
};

struct pal_pen_proximity_event {
    int member;
};

struct pal_pen_touch_event {
    int member;
};

struct pal_pen_motion_event {
    int member;
};

struct pal_pen_button_event {
    int member;
};

struct pal_pen_axis_event {
    int member;
};

struct pal_render_event {
    int member;
};

struct pal_drop_event {
    int member;
};

struct pal_clipboard_event {
    int member;
};

typedef union pal_event
{
    uint8_t type;                              /**< Event type, shared with all events, Uint32 to cover user events which are not in the SDL_EventType enumeration */
    pal_common_event common;                  /**< Common event data */
    pal_display_event display;                /**< Display event data */
    pal_window_event window;                  /**< Window event data */
    pal_keyboard_device_event kdevice;       /**< Keyboard device change event data */
    pal_keyboard_event key;                   /**< Keyboard event data */
    pal_text_editing_event edit;              /**< Text editing event data */
    pal_text_editing_candidates_event edit_candidates; /**< Text editing candidates event data */
    pal_text_input_event text;                /**< Text input event data */
    pal_mouse_device_event mdevice;           /**< Mouse device change event data */
    pal_mouse_motion_event motion;            /**< Mouse motion event data */
    pal_mouse_button_event button;            /**< Mouse button event data */
    pal_mouse_wheel_event wheel;              /**< Mouse wheel event data */
    pal_joy_device_event jdevice;             /**< Joystick device change event data */
    pal_joy_axis_event jaxis;                  /**< Joystick axis event data */
    pal_joy_ball_event jball;                  /**< Joystick ball event data */
    pal_joy_hat_event jhat;                    /**< Joystick hat event data */
    pal_joy_button_event jbutton;              /**< Joystick button event data */
    pal_joy_battery_event jbattery;            /**< Joystick battery event data */
    pal_gamepad_device_event gdevice;          /**< Gamepad device event data */
    pal_gamepad_axis_event gaxis;               /**< Gamepad axis event data */
    pal_gamepad_button_event gbutton;           /**< Gamepad button event data */
    pal_gamepad_touchpad_event gtouchpad;       /**< Gamepad touchpad event data */
    pal_gamepad_sensor_event gsensor;           /**< Gamepad sensor event data */
    pal_audio_device_event adevice;              /**< Audio device event data */
    pal_camera_device_event cdevice;             /**< Camera device event data */
    pal_sensor_event sensor;                     /**< Sensor event data */
    pal_quit_event quit;                         /**< Quit request event data */
    pal_user_event user;                         /**< Custom event data */
    pal_touch_finger_event tfinger;             /**< Touch finger event data */
    pal_pen_proximity_event pproximity;         /**< Pen proximity event data */
    pal_pen_touch_event ptouch;                   /**< Pen tip touching event data */
    pal_pen_motion_event pmotion;                 /**< Pen motion event data */
    pal_pen_button_event pbutton;                 /**< Pen button event data */
    pal_pen_axis_event paxis;                     /**< Pen axis event data */
    pal_render_event render;                       /**< Render event data */
    pal_drop_event drop;                           /**< Drag and drop event data */
    pal_clipboard_event clipboard;                 /**< Clipboard event data */

    uint8_t padding[128];
} pal_event;

// Keyboard & Mouse Input
//TODO: @fix We should not have this here and just have the user call
//the functions that fill up the input struct.

#define MAX_KEYS 256
#define MAX_MOUSEBUTTONS 32
#define MAX_CONTROLLERS 4

typedef struct Input {
	uint8_t keys[MAX_KEYS];
	uint8_t keys_processed[MAX_KEYS];
	uint8_t mouse_buttons[MAX_MOUSEBUTTONS];
	uint8_t mouse_buttons_processed[MAX_MOUSEBUTTONS];
	v2 mouse_position;
	v2 mouse;
	uint8_t controller_connected[MAX_CONTROLLERS];
	XINPUT_STATE controller_state[MAX_CONTROLLERS];
	XINPUT_STATE controller_prev_state[MAX_CONTROLLERS];
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

	case WM_CREATE:
		break;
	case WM_DESTROY:
		DestroyWindow(hwnd);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		PostQuitMessage(0);
		break;
	case WM_QUIT:
		break;
	case WM_MOVE:
		break;
	case WM_SIZE:
		Win32WindowResizeCallback(hwnd, (UINT)wParam, width, height);
		break;
	case WM_SHOWWINDOW:
		break;
	case WM_PAINT:
		break;
	case WM_SETFOCUS:
		break;
	case WM_KILLFOCUS:
		break;
	case WM_ACTIVATE:
		break;
	case WM_GETMINMAXINFO:
		break;
	case WM_NCACTIVATE:
		break;
	case WM_ERASEBKGND:
		break;

	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_XBUTTONDBLCLK:
	case WM_MOUSEWHEEL:
	case WM_MOUSEHWHEEL:
	case WM_MOUSELEAVE:
	case WM_MOUSEACTIVATE:
	case WM_SETCURSOR:
		break;

	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_SYSCHAR:
	case WM_UNICHAR:
	case WM_DEADCHAR:
	case WM_SYSDEADCHAR:
		break;

	case WM_ENTERSIZEMOVE:
		break;

	case WM_EXITSIZEMOVE:
		break;

	case WM_DISPLAYCHANGE:
		break;

	case WM_DPICHANGED:
		break;

	case WM_WINDOWPOSCHANGED:
		break;

	case WM_WINDOWPOSCHANGING:
		break;

	case WM_STYLECHANGED:
		break;

	case WM_STYLECHANGING:
		break;

	case WM_SYSCOMMAND:
		break;

	case WM_COMMAND:
		break;

	case WM_TIMER:
		break;

	case WM_HOTKEY:
		break;

	case WM_INPUT:
		break;

	case WM_DROPFILES:
		break;

	case WM_CAPTURECHANGED:
		break;

	case WM_SETICON:
	case WM_GETICON:
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

static pal_window* platform_init_window(int width, int height, const char* windowTitle) {
	pal_window* fakewindow = (pal_window*)malloc(sizeof(pal_window));
	WNDCLASSEXA fakewc = RegisterWindowClass();

	fakewindow->hwnd = CreateWindowExA(
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

	if (fakewindow->hwnd == NULL)
	{
		return fakewindow;
	}

	s_fakeDC = GetDC(fakewindow->hwnd);

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
		MessageBoxA(fakewindow->hwnd, "ChoosePixelFormat() failed.", "Try again later", MB_ICONERROR);
		return fakewindow;
	}
	if (SetPixelFormat(s_fakeDC, fakePFDID, &fakePFD) == 0) {
		MessageBoxA(fakewindow->hwnd, "SetPixelFormat() failed.", "Try again later", MB_ICONERROR);
		return fakewindow;
	}

	HGLRC fakeRC = wglCreateContext(s_fakeDC);
	if (fakeRC == 0) {
		MessageBoxA(fakewindow->hwnd, "wglCreateContext() failed.", "Try again later", MB_ICONERROR);
		return fakewindow;
	}
	if (wglMakeCurrent(s_fakeDC, fakeRC) == 0) {
		MessageBoxA(fakewindow->hwnd, "wglMakeCurrent() failed.", "Try again later", MB_ICONERROR);
		return fakewindow;
	}
	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;
	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)(wglGetProcAddress("wglChoosePixelFormatARB"));
	if (wglChoosePixelFormatARB == NULL) {
		MessageBoxA(fakewindow->hwnd, "wglGetProcAddress() failed.", "Try again later", MB_ICONERROR);
		return fakewindow;
	}
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)(wglGetProcAddress("wglCreateContextAttribsARB"));
	if (wglCreateContextAttribsARB == NULL) {
		MessageBoxA(fakewindow->hwnd, "wglGetProcAddress() failed.", "Try again later", MB_ICONERROR);
		return fakewindow;
	}
	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if (wglSwapIntervalEXT == NULL) {
		MessageBoxA(fakewindow->hwnd, "wglGetProcAddress() failed.", "Try again later", MB_ICONERROR);
		return fakewindow;
	}

	WNDCLASSEXA wc = RegisterWindowClass();
	pal_window* window = (pal_window*)malloc(sizeof(pal_window));
	window->hwnd = CreateWindowExA(
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

	if (window->hwnd == NULL) {
		return window;
	}

	window->hdc = GetDC(window->hwnd);

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
	uint8_t status = wglChoosePixelFormatARB(window->hdc, pixelAttribs, NULL, 1, &pixelFormatID, &numFormats);
	if (status == 0 || numFormats == 0) {
		MessageBoxA(window->hwnd, "wglChoosePixelFormatARB() failed.", "Try again later", MB_ICONERROR);
		return window;
	}

	PIXELFORMATDESCRIPTOR PFD;
	PFD.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | s_doubleBuffer;
	DescribePixelFormat(window->hdc, pixelFormatID, sizeof(PFD), &PFD);
	SetPixelFormat(window->hdc, pixelFormatID, &PFD);

	int contextAttribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, s_glVersionMajor,
		WGL_CONTEXT_MINOR_VERSION_ARB, s_glVersionMinor,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
		WGL_CONTEXT_PROFILE_MASK_ARB, s_glProfile,
		0
	};

	window->hglrc = wglCreateContextAttribsARB(window->hdc, 0, contextAttribs);
	if (window->hglrc) {

		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(fakeRC);
		ReleaseDC(fakewindow->hwnd, s_fakeDC);
		DestroyWindow(fakewindow->hwnd);

		ShowWindow(window->hwnd, SW_SHOWNORMAL);
		SetForegroundWindow(window->hwnd);
		SetFocus(window->hwnd);
		OutputDebugStringA("INFO: Using modern OpenGL Context.");
		return window;
	}
	else {

		ShowWindow(fakewindow->hwnd, SW_SHOW);
		SetForegroundWindow(fakewindow->hwnd);
		SetFocus(fakewindow->hwnd);
		OutputDebugStringA("INFO: Using old OpenGL Context.");
		return fakewindow;
	}

}

static int platform_make_context_current(pal_window* window) {
	if (!wglMakeCurrent(window->hdc, window->hglrc)) {
		MessageBoxA(window->hwnd, "wglMakeCurrent() failed.", "Try again later", MB_ICONERROR);
		return 1;
	}
	return 0;
}

static uint8_t platform_poll_events(pal_event* event) {
	platform_get_raw_input_buffer();
	platform_poll_gamepads();

	while (PeekMessageA(&s_msg, NULL , 0, 0, PM_REMOVE)) {
		if (s_msg.message == WM_QUIT) {
			exit(0);
		}
		TranslateMessage(&s_msg);
		DispatchMessageA(&s_msg);
		return 1;
	}
	return 0;
}



static uint8_t platform_set_window_title(pal_window* window, const char* string) {
	return SetWindowTextA(window->hwnd, string);
}

static VideoMode* platform_get_video_mode(pal_monitor* monitor) {

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

static pal_monitor* platform_get_primary_monitor() {
	pal_monitor* monitor = malloc(sizeof(pal_monitor));

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

void platform_end_drawing(pal_window* window) {
	SwapBuffers(window->hdc);
}

#define MAX_RAW_INPUTS 16

// Handler function signatures
typedef void (*RawInputHandler)(const RAWINPUT*);

// Mouse input handler
void Win32HandleMouse(const RAWINPUT* raw) {
	LONG dx = raw->data.mouse.lLastX;
	LONG dy = raw->data.mouse.lLastY;
	USHORT buttons = raw->data.mouse.usButtonFlags;

	input.mouse = (v2){
		(float)dx,
		(float)dy,
	};

	for (int i = 0; i < 16; ++i) {
		uint16_t down = (buttons >> (i * 2)) & 1;
		uint16_t up = (buttons >> (i * 2 + 1)) & 1;

		// If down is 1, set to 1; if up is 1, set to 0; otherwise leave unchanged
		input.mouse_buttons[i] = (input.mouse_buttons[i] & ~up) | down;
	}

	printf("Mouse: dx=%ld dy=%ld buttons=0x%04x\n", dx, dy, buttons);
}

v2 platform_get_mouse_position(pal_window* window) {
	POINT cursor_pos = { 0 };
	GetCursorPos(&cursor_pos);

	ScreenToClient(window->hwnd, &cursor_pos);     // Convert to client-area coordinates
	return (v2) {
		(float)cursor_pos.x,
		(float)cursor_pos.y
	};
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

int platform_register_raw_input_devices(pal_window* window) {
	RAWINPUTDEVICE rid[3];

	// 1. Keyboard
	rid[0].usUsagePage = 0x01; // Generic desktop controls
	rid[0].usUsage = 0x06;     // Keyboard
	rid[0].dwFlags = RIDEV_INPUTSINK | RIDEV_DEVNOTIFY; // Receive input even when not focused
	rid[0].hwndTarget = window->hwnd;

	// 2. Mouse
	rid[1].usUsagePage = 0x01; // Generic desktop controls
	rid[1].usUsage = 0x02;     // Mouse
	rid[1].dwFlags = RIDEV_INPUTSINK | RIDEV_DEVNOTIFY;
	rid[1].hwndTarget = window->hwnd;

	// 3. Joystick/Gamepad (Note: Not all controllers appear as HIDs)
	rid[2].usUsagePage = 0x01; // Generic desktop controls
	rid[2].usUsage = 0x04;     // Joystick
	rid[2].dwFlags = RIDEV_INPUTSINK | RIDEV_DEVNOTIFY;
	rid[2].hwndTarget = window->hwnd;

	if (!RegisterRawInputDevices(rid, 3, sizeof(RAWINPUTDEVICE))) {
		DWORD error = GetLastError();
		printf("RegisterRawInputDevices failed. Error code: %lu\n", error);
		return -1;
	}

	printf("Raw input devices registered successfully.\n");
	return 0;
}

void platform_poll_gamepads(void) {
    for (int i = 0; i < MAX_CONTROLLERS; ++i) {
        input.controller_prev_state[i] = input.controller_state[i];
        DWORD res = XInputGetState(i, &input.controller_state[i]);
        if (res == ERROR_SUCCESS) {
            if (!input.controller_connected[i]) {
                ZeroMemory(&input.controller_prev_state[i], sizeof(XINPUT_STATE));
            }
            input.controller_connected[i] = 1;
        } else {
            if (input.controller_connected[i]) {
                ZeroMemory(&input.controller_state[i], sizeof(XINPUT_STATE));
                ZeroMemory(&input.controller_prev_state[i], sizeof(XINPUT_STATE));
            }
            input.controller_connected[i] = 0;
        }
    }
}

int platform_is_button_down(int controller_id, unsigned short button) {
    if (controller_id < 0 || controller_id >= MAX_CONTROLLERS) return 0;
    if (!input.controller_connected[controller_id]) return 0;
    return (input.controller_state[controller_id].Gamepad.wButtons & button) != 0;
}

int platform_is_button_pressed(int controller_id, unsigned short button) {
    if (controller_id < 0 || controller_id >= MAX_CONTROLLERS) return 0;
    if (!input.controller_connected[controller_id]) return 0;

	unsigned short curr = input.controller_state[controller_id].Gamepad.wButtons;
	unsigned short prev = input.controller_prev_state[controller_id].Gamepad.wButtons;
    return ((curr & button) != 0) && ((prev & button) == 0);
}

int platform_is_button_released(int controller_id, unsigned short button) {
    if (controller_id < 0 || controller_id >= MAX_CONTROLLERS) return 0;
    if (!input.controller_connected[controller_id]) return 0;

	unsigned short curr = input.controller_state[controller_id].Gamepad.wButtons;
	unsigned short prev = input.controller_prev_state[controller_id].Gamepad.wButtons;
    return ((curr & button) == 0) && ((prev & button) != 0);
}

// Helper to normalize and apply deadzone
static v2 platform_process_thumbstick(short x, short y, int deadzone) {
    v2 v = {0};

    float fx = (float)x;
    float fy = (float)y;

    if (x > deadzone || x < -deadzone)
        fx = (fx > 0) ? (fx - deadzone) : (fx + deadzone);
    else
        fx = 0;

    if (y > deadzone || y < -deadzone)
        fy = (fy > 0) ? (fy - deadzone) : (fy + deadzone);
    else
        fy = 0;

    const float max_val = 32767.0f - deadzone;

    v.x = fx / max_val;
    v.y = fy / max_val;

    return v;
}

v2 platform_get_left_stick(int controller_id) {
    if (controller_id < 0 || controller_id >= MAX_CONTROLLERS) return (v2){0};
    if (!input.controller_connected[controller_id]) return (v2){0};

	short x = input.controller_state[controller_id].Gamepad.sThumbLX;
	short y = input.controller_state[controller_id].Gamepad.sThumbLY;

    return platform_process_thumbstick(x, y, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
}

v2 platform_get_right_stick(int controller_id) {
    if (controller_id < 0 || controller_id >= MAX_CONTROLLERS) return (v2){0};
    if (!input.controller_connected[controller_id]) return (v2){0};

	short x = input.controller_state[controller_id].Gamepad.sThumbRX;
	short y = input.controller_state[controller_id].Gamepad.sThumbRY;

    return platform_process_thumbstick(x, y, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
}

float platform_get_right_trigger(int controller_id) {
	if (controller_id < 0 || controller_id >= MAX_CONTROLLERS) return 0.0f;
	if (!input.controller_connected[controller_id]) return 0.0f;

	BYTE raw = input.controller_state[controller_id].Gamepad.bRightTrigger;

	return (raw > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		? (float)raw / 255.0f
		: 0.0f;
}

float platform_get_left_trigger(int controller_id) {
	if (controller_id < 0 || controller_id >= MAX_CONTROLLERS) return 0.0f;
	if (!input.controller_connected[controller_id]) return 0.0f;

	BYTE raw = input.controller_state[controller_id].Gamepad.bLeftTrigger;

	// Apply threshold (XInput docs recommend 30 as a deadzone)
	return (raw > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		? (float)raw / 255.0f
		: 0.0f;
}

void platform_set_controller_vibration(int controller_id, float left_motor, float right_motor) {
	if (controller_id < 0 || controller_id >= MAX_CONTROLLERS) return;
	if (!input.controller_connected[controller_id]) return;

	// Clamp values between 0.0f and 1.0f
	if (left_motor < 0.0f) left_motor = 0.0f;
	if (left_motor > 1.0f) left_motor = 1.0f;
	if (right_motor < 0.0f) right_motor = 0.0f;
	if (right_motor > 1.0f) right_motor = 1.0f;

	XINPUT_VIBRATION vibration = { 0 };
	vibration.wLeftMotorSpeed = (WORD)(left_motor * 65535.0f);
	vibration.wRightMotorSpeed = (WORD)(right_motor * 65535.0f);

	XInputSetState(controller_id, &vibration);
}

void platform_stop_controller_vibration(int controller_id) {
	platform_set_controller_vibration(controller_id, 0.0f, 0.0f);
}

#define RAW_INPUT_BUFFER_CAPACITY (64 * 1024) // 8 KB

static BYTE g_rawInputBuffer[RAW_INPUT_BUFFER_CAPACITY];

static int platform_get_raw_input_buffer() {
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

static int platform_play_sound(const Sound* sound, float volume) {
	if (!g_xaudio2 || !g_mastering_voice) {
		return E_FAIL;
	}

	static const GUID KSDATAFORMAT_SUBTYPE_PCM = {
		0x00000001, 0x0000, 0x0010,
		{ 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 }
	};

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

	HRESULT hr = g_xaudio2->lpVtbl->CreateSourceVoice(
		g_xaudio2, &source_voice, (const WAVEFORMATEX*)&wfex,
		0, XAUDIO2_DEFAULT_FREQ_RATIO,
		NULL, NULL, NULL);
	if (FAILED(hr)) {
		return hr;
	}

	// Set the volume
	source_voice->lpVtbl->SetVolume(source_voice, volume, 0);

	XAUDIO2_BUFFER buffer = {
		.AudioBytes = (UINT32)sound->dataSize,
		.pAudioData = sound->data,
		.Flags = XAUDIO2_END_OF_STREAM
	};

	hr = source_voice->lpVtbl->SubmitSourceBuffer(source_voice, &buffer, NULL);
	if (FAILED(hr)) {
		source_voice->lpVtbl->DestroyVoice(source_voice);
		return hr;
	}

	hr = source_voice->lpVtbl->Start(source_voice, 0, 0);
	if (FAILED(hr)) {
		source_voice->lpVtbl->DestroyVoice(source_voice);
		return hr;
	}

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
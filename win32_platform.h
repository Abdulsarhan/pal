#ifndef WIN32_PLATFORM_H
#define WIN32_PLATFORM_H

#define _CRT_SECURE_NO_WARNINGS
// Windows system headers
#include <Windows.h>
#include <windowsx.h> // Useful macros (e.g., GET_X_LPARAM)
#include <xaudio2.h>  // For Sound.
#include <Xinput.h> // For gamepad input.
#include <hidsdi.h> // Link with hid.lib

// for file permissions.
#include <aclapi.h>

// for PathIsDirectoryW
#include <Shlwapi.h>

// OpenGL
#include <gl/gl.h>
#include <GL/wglext.h>

// C Standard Library
#include <assert.h>

// Global function pointers
static DWORD(WINAPI* XinputGetstate_fn)(DWORD dwUserIndex, XINPUT_STATE* pState) = NULL;
static DWORD(WINAPI* XInputSetState_fn)(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration) = NULL;
static DWORD(WINAPI* XInputGetCapabilities_fn)(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES* pCapabilities) = NULL;
static void(WINAPI* XInputEnable_fn)(BOOL enable) = NULL;

typedef unsigned __int64 QWORD;

// Global state
static HMODULE g_xinput_dll = NULL;
static pal_bool g_has_trigger_motors = pal_false;

static HDC s_fakeDC = {0};
pal_window* g_current_window;

IXAudio2* g_xaudio2 = NULL;
IXAudio2MasteringVoice* g_mastering_voice = NULL;

// on windows, the message pump is not specific to any window, it's specific to the thread.
// this is false initially because windows sends messages to the window as soon as
// Create_WindowExA() is called.
pal_bool g_message_pump_drained = pal_false;
struct pal_window {
    HWND hwnd;
    HDC hdc;
    HGLRC hglrc;
    LONG windowedStyle;
    pal_bool confine_mouse;
    float width;
    float height;
};

struct pal_monitor {
    HMONITOR handle;
};

struct pal_sound {
    // Core audio data
    unsigned char* data; // Raw PCM audio data (initial buffer)
    size_t data_size;    // Size in bytes of initial buffer
    int sample_rate;     // Samples per second (e.g., 44100)
    int channels;        // Number of audio channels (e.g., 2 for stereo)
    int bits_per_sample; // Usually 16 or 32
    int is_float;        // 0 = PCM, 1 = IEEE float

    // XAudio2
    IXAudio2SourceVoice* source_voice;
    IXAudio2VoiceCallback* voice_callback;

    // Streaming - OGG
    void* decoder;  // stb_vorbis* (using void* to avoid header dependency)
    char* filename; // Filename for reopening OGG decoder

    // Streaming - WAV
    FILE* source_file;
    size_t total_data_size; // Total size of audio data in file
    size_t bytes_streamed;  // How many bytes we've read so far
    size_t data_offset;     // Offset in file where audio data starts

    // Streaming control
    float preload_seconds; // How many seconds were preloaded
    int is_streaming;      // 1 if this is a streaming sound
    int stream_finished;   // 1 when streaming is complete
};

struct pal_mutex {
    CRITICAL_SECTION cs;
};

// Keyboard & Mouse Input

#define MAX_MOUSEBUTTONS 32
#define MAX_XINPUT_CONTROLLERS 4
#define PAL_MAX_GAMEPADS 16
#define PAL_MAX_BUTTONS 32
#define PAL_MAX_AXES 16
#define PAL_MAX_MAPPINGS 256

static const uint8_t win32_button_to_pal_button[] = {
    [0] = PAL_MOUSE_LEFT,
    [1] = PAL_MOUSE_RIGHT,
    [2] = PAL_MOUSE_MIDDLE,
    [3] = PAL_MOUSE_4,
    [4] = PAL_MOUSE_5,
};

// clang-format off
static const uint16_t win32_key_to_pal_key[] = {
    // unassigned.
    [0x00] = 0x00, [0x01] = 0x00, [0x02] = 0x00, [0x03] = 0x00, [0x04] = 0x00,
    [0x05] = 0x00, [0x06] = 0x00, [0x07] = 0x00,

    [0x08] = 0x08, // Backspace
    [0x09] = 0x09, // Tab
    [0x0A] = 0x00, [0x0B] = 0x00, // unassigned.
    [0x0C] = 0x0C, // Clear
    [0x0D] = 0x0D, // Return
    [0x0E] = 0x00, [0x0F] = 0x00, // unassigned.
    [0x10] = 0x10, // Shift
    [0x11] = 0x11, // Control
    [0x12] = 0x12, // Menu (Alt)
    [0x13] = 0x13, // Pause
    [0x14] = 0x14, // Caps Lock
    [0x15] = 0x15, [0x16] = 0x16, [0x17] = 0x17,
    [0x18] = 0x18, [0x19] = 0x19, [0x1A] = 0x1A, // Japanese keys (?)
    [0x1B] = 0x1B, [0x1C] = 0x1C, [0x1D] = 0x1D,
    [0x1E] = 0x1E, [0x1F] = 0x1F,
    [0x20] = 0x20, [0x21] = 0x21, [0x22] = 0x22,
    [0x23] = 0x23, [0x24] = 0x24, [0x25] = 0x25,
    [0x26] = 0x26, [0x27] = 0x27, [0x28] = 0x28,
    [0x29] = 0x29, [0x2A] = 0x2A, [0x2B] = 0x2B,
    [0x2C] = 0x2C, [0x2D] = 0x2D, [0x2E] = 0x2E,
    [0x2F] = 0x2F,
    // 0 - 9 
    [0x30] = 0x30, [0x31] = 0x31, [0x32] = 0x32, [0x33] = 0x33,
    [0x34] = 0x34, [0x35] = 0x35, [0x36] = 0x36, [0x37] = 0x37,
    [0x38] = 0x38, [0x39] = 0x39,
    // Unassigned.
    [0x3A] = 0x3A, [0x3B] = 0x3B, [0x3C] = 0x3C, [0x3D] = 0x3D,
    [0x3E] = 0x3E, [0x3F] = 0x3F, [0x40] = 0x40,

    // A - Z
    [0x41] = 0x41, [0x42] = 0x42, [0x43] = 0x43, [0x44] = 0x44,
    [0x45] = 0x45, [0x46] = 0x46, [0x47] = 0x47, [0x48] = 0x48,
    [0x49] = 0x49, [0x4A] = 0x4A,
    [0x4B] = 0x4B, [0x4C] = 0x4C,
    [0x4D] = 0x4D, [0x4E] = 0x4E, [0x4F] = 0x4F,
    [0x50] = 0x50, [0x51] = 0x51, [0x52] = 0x52, [0x53] = 0x53,
    [0x54] = 0x54, [0x55] = 0x55, [0x56] = 0x56, [0x57] = 0x57,
    [0x58] = 0x58, [0x59] = 0x59, [0x5A] = 0x5A,

    // LWin, RWin, Apps (The Apps key is supposed to open up a context menu. Most keyboards don't have this.)
    [0x5B] = 0x5B, [0x5C] = 0x5C, [0x5D] = 0x5D, 

    [0x5E] = 0x5E, // Unassigned

    [0x5F] = 0x5F, // Sleep

    // Numpad 0 - 9
    [0x60] = 0x60, [0x61] = 0x61, [0x62] = 0x62,
    [0x63] = 0x63, [0x64] = 0x64, [0x65] = 0x65,
    [0x66] = 0x66, [0x67] = 0x67, [0x68] = 0x68,
    [0x69] = 0x69, 

    // numpad operands
    [0x6A] = 0x6A, [0x6B] = 0x6B, [0x6C] = 0x6C,
    [0x6D] = 0x6D, [0x6E] = 0x6E, [0x6F] = 0x6F, 

    // F1 - F24.
    [0x70] = 0x70, [0x71] = 0x71, [0x72] = 0x72,
    [0x73] = 0x73, [0x74] = 0x74, [0x75] = 0x75,
    [0x76] = 0x76, [0x77] = 0x77, [0x78] = 0x78,
    [0x79] = 0x79, [0x7A] = 0x7A, [0x7B] = 0x7B,
    [0x7C] = 0x7C, [0x7D] = 0x7D, [0x7E] = 0x7E,
    [0x7F] = 0x7F, [0x80] = 0x80, [0x81] = 0x81,
    [0x82] = 0x82, [0x83] = 0x83, [0x84] = 0x84,
    [0x85] = 0x85, [0x86] = 0x86, [0x87] = 0x87,

    // Reserved
    [0x88] = 0x88, [0x89] = 0x89, [0x8A] = 0x8A, [0x8B] = 0x8B,
    [0x8C] = 0x8C, [0x8D] = 0x8D, [0x8E] = 0x8E, [0x8F] = 0x8F, 

    [0x90] = 0x90, [0x91] = 0x91, // NumLock, ScrollLock
};

// clang-format on
static int win32_makecode_to_pal_scancode[256] = {
    [0x00] = 0,                       // Invalid
    [0x01] = PAL_SCAN_ESCAPE,         // Escape
    [0x02] = PAL_SCAN_1,              // 1
    [0x03] = PAL_SCAN_2,              // 2
    [0x04] = PAL_SCAN_3,              // 3
    [0x05] = PAL_SCAN_4,              // 4
    [0x06] = PAL_SCAN_5,              // 5
    [0x07] = PAL_SCAN_6,              // 6
    [0x08] = PAL_SCAN_7,              // 7
    [0x09] = PAL_SCAN_8,              // 8
    [0x0A] = PAL_SCAN_9,              // 9
    [0x0B] = PAL_SCAN_0,              // 0
    [0x0C] = PAL_SCAN_MINUS,          // -
    [0x0D] = PAL_SCAN_EQUALS,         // =
    [0x0E] = PAL_SCAN_BACKSPACE,      // Backspace
    [0x0F] = PAL_SCAN_TAB,            // Tab
    [0x10] = PAL_SCAN_Q,              // Q
    [0x11] = PAL_SCAN_W,              // W
    [0x12] = PAL_SCAN_E,              // E
    [0x13] = PAL_SCAN_R,              // R
    [0x14] = PAL_SCAN_T,              // T
    [0x15] = PAL_SCAN_Y,              // Y
    [0x16] = PAL_SCAN_U,              // U
    [0x17] = PAL_SCAN_I,              // I
    [0x18] = PAL_SCAN_O,              // O
    [0x19] = PAL_SCAN_P,              // P
    [0x1A] = PAL_SCAN_LEFTBRACKET,    // [
    [0x1B] = PAL_SCAN_RIGHTBRACKET,   // ]
    [0x1C] = PAL_SCAN_RETURN,         // Enter
    [0x1D] = PAL_SCAN_LCTRL,          // Left Ctrl
    [0x1E] = PAL_SCAN_A,              // A
    [0x1F] = PAL_SCAN_S,              // S
    [0x20] = PAL_SCAN_D,              // D
    [0x21] = PAL_SCAN_F,              // F
    [0x22] = PAL_SCAN_G,              // G
    [0x23] = PAL_SCAN_H,              // H
    [0x24] = PAL_SCAN_J,              // J
    [0x25] = PAL_SCAN_K,              // K
    [0x26] = PAL_SCAN_L,              // L
    [0x27] = PAL_SCAN_SEMICOLON,      // ;
    [0x28] = PAL_SCAN_APOSTROPHE,     // '
    [0x29] = PAL_SCAN_GRAVE,          // `
    [0x2A] = PAL_SCAN_LSHIFT,         // Left Shift
    [0x2B] = PAL_SCAN_BACKSLASH,      // \ (backslash)
    [0x2C] = PAL_SCAN_Z,              // Z
    [0x2D] = PAL_SCAN_X,              // X
    [0x2E] = PAL_SCAN_C,              // C
    [0x2F] = PAL_SCAN_V,              // V
    [0x30] = PAL_SCAN_B,              // B
    [0x31] = PAL_SCAN_N,              // N
    [0x32] = PAL_SCAN_M,              // M
    [0x33] = PAL_SCAN_COMMA,          // ,
    [0x34] = PAL_SCAN_PERIOD,         // .
    [0x35] = PAL_SCAN_SLASH,          // /
    [0x36] = PAL_SCAN_RSHIFT,         // Right Shift
    [0x37] = PAL_SCAN_KP_MULTIPLY,    // Keypad *
    [0x38] = PAL_SCAN_LALT,           // Left Alt
    [0x39] = PAL_SCAN_SPACE,          // Space
    [0x3A] = PAL_SCAN_CAPSLOCK,       // Caps Lock
    [0x3B] = PAL_SCAN_F1,             // F1
    [0x3C] = PAL_SCAN_F2,             // F2
    [0x3D] = PAL_SCAN_F3,             // F3
    [0x3E] = PAL_SCAN_F4,             // F4
    [0x3F] = PAL_SCAN_F5,             // F5
    [0x40] = PAL_SCAN_F6,             // F6
    [0x41] = PAL_SCAN_F7,             // F7
    [0x42] = PAL_SCAN_F8,             // F8
    [0x43] = PAL_SCAN_F9,             // F9
    [0x44] = PAL_SCAN_F10,            // F10
    [0x45] = PAL_SCAN_NUMCLEAR,       // Num Lock
    [0x46] = PAL_SCAN_SCROLLLOCK,     // Scroll Lock
    [0x47] = PAL_SCAN_KP_7,           // Keypad 7 / Home
    [0x48] = PAL_SCAN_KP_8,           // Keypad 8 / Up
    [0x49] = PAL_SCAN_KP_9,           // Keypad 9 / Page Up
    [0x4A] = PAL_SCAN_KP_MINUS,       // Keypad -
    [0x4B] = PAL_SCAN_KP_4,           // Keypad 4 / Left
    [0x4C] = PAL_SCAN_KP_5,           // Keypad 5
    [0x4D] = PAL_SCAN_KP_6,           // Keypad 6 / Right
    [0x4E] = PAL_SCAN_KP_PLUS,        // Keypad +
    [0x4F] = PAL_SCAN_KP_1,           // Keypad 1 / End
    [0x50] = PAL_SCAN_KP_2,           // Keypad 2 / Down
    [0x51] = PAL_SCAN_KP_3,           // Keypad 3 / Page Down
    [0x52] = PAL_SCAN_KP_0,           // Keypad 0 / Insert
    [0x53] = PAL_SCAN_KP_PERIOD,      // Keypad . / Delete
    [0x56] = PAL_SCAN_NONUSBACKSLASH, // Non-US backslash (ISO layout)
    [0x57] = PAL_SCAN_F11,            // F11
    [0x58] = PAL_SCAN_F12,            // F12
    [0x64] = PAL_SCAN_F13,            // F13
    [0x65] = PAL_SCAN_F14,            // F14
    [0x66] = PAL_SCAN_F15,            // F15
    [0x67] = PAL_SCAN_F16,            // F16
    [0x68] = PAL_SCAN_F17,            // F17
    [0x69] = PAL_SCAN_F18,            // F18
    [0x6A] = PAL_SCAN_F19,            // F19
    [0x6B] = PAL_SCAN_F20,            // F20
    [0x6C] = PAL_SCAN_F21,            // F21
    [0x6D] = PAL_SCAN_F22,            // F22
    [0x6E] = PAL_SCAN_F23,            // F23
    [0x6F] = PAL_SCAN_F24,            // F24
    [0x70] = PAL_SCAN_INTERNATIONAL2, // Katakana/Hiragana
    [0x73] = PAL_SCAN_INTERNATIONAL1, // Ro
    [0x79] = PAL_SCAN_INTERNATIONAL4, // Henkan
    [0x7B] = PAL_SCAN_INTERNATIONAL5, // Muhenkan
    [0x7D] = PAL_SCAN_INTERNATIONAL3, // Yen

};

// Translation table for extended makecodes (E0 prefix keys)
static int win32_extended_makecode_to_pal_scancode[256] = {
    [0x1C] = PAL_SCAN_KP_ENTER,     // Keypad Enter
    [0x1D] = PAL_SCAN_RCTRL,        // Right Ctrl
    [0x35] = PAL_SCAN_KP_DIVIDE,    // Keypad /
    [0x37] = PAL_SCAN_PRINTSCREEN,  // Print Screen
    [0x38] = PAL_SCAN_RALT,         // Right Alt / AltGr
    [0x46] = PAL_SCAN_PAUSE,        // Pause/Break
    [0x47] = PAL_SCAN_HOME,         // Home
    [0x48] = PAL_SCAN_UP,           // Up Arrow
    [0x49] = PAL_SCAN_PAGEUP,       // Page Up
    [0x4B] = PAL_SCAN_LEFT,         // Left Arrow
    [0x4D] = PAL_SCAN_RIGHT,        // Right Arrow
    [0x4F] = PAL_SCAN_END,          // End
    [0x50] = PAL_SCAN_DOWN,         // Down Arrow
    [0x51] = PAL_SCAN_PAGEDOWN,     // Page Down
    [0x52] = PAL_SCAN_INSERT,       // Insert
    [0x53] = PAL_SCAN_DELETE,       // Delete
    [0x5B] = PAL_SCAN_LGUI,         // Left Windows/Super
    [0x5C] = PAL_SCAN_RGUI,         // Right Windows/Super
    [0x5D] = PAL_SCAN_APPLICATION,  // Menu/Application
    [0x5F] = PAL_SCAN_SLEEP,        // Sleep
    [0x63] = PAL_SCAN_WAKE,         // Wake
    [0x65] = PAL_SCAN_AC_SEARCH,    // Search
    [0x66] = PAL_SCAN_AC_BOOKMARKS, // Favorites
    [0x67] = PAL_SCAN_AC_REFRESH,   // Refresh
    [0x68] = PAL_SCAN_AC_STOP,      // Stop
    [0x69] = PAL_SCAN_AC_FORWARD,   // Forward
    [0x6A] = PAL_SCAN_AC_BACK,      // Back
    [0x6B] = PAL_SCAN_AC_HOME,      // My Computer
    [0x6C] = PAL_SCAN_AC_OPEN,      // Mail
    [0x6D] = PAL_SCAN_MEDIA_SELECT, // Media Select
};

typedef struct {
    uint8_t usage;
    float value;
} win32_gamepad_button;

typedef struct {
    uint8_t usage;
    float value;
    int32_t min, max;
    pal_bool inverted;
} win32_gamepad_axis;

typedef struct {
    uint16_t vendor_id;
    uint16_t product_id;
    char name[128];
    uint8_t button_map[15];
    struct {
        uint8_t usage;
        pal_bool inverted;
    } axis_map[6];
} win32_gamepad_mapping;

typedef struct {
    uint8_t report[64];
    uint8_t report_size;
    pal_bool has_report;
    OVERLAPPED overlapped;
} win32_dualsense_state;

// Global State
typedef struct hid_device {
    HANDLE handle;
    PHIDP_PREPARSED_DATA pp_data;
    uint16_t vendor_id;
    uint16_t product_id;
    char name[128];
    win32_gamepad_button buttons[PAL_MAX_BUTTONS];
    uint8_t button_count;
    win32_gamepad_axis axes[PAL_MAX_AXES];
    uint8_t axis_count;
    pal_bool connected;
} hid_device;

typedef struct win32_gamepad_context {
    uint8_t xinput_connected[MAX_XINPUT_CONTROLLERS];
    XINPUT_STATE xinput_state[MAX_XINPUT_CONTROLLERS];

    uint8_t raw_input_buffer[1024]; // <-- THIS IS THE BUFFER

    struct hid_device hid_devices[PAL_MAX_GAMEPADS];
    struct { // DualSense/DS4
        HANDLE handle;
        win32_dualsense_state state;
        uint16_t vendor_id;
        uint16_t product_id;
        char name[128];
        pal_bool connected;
    } ds_devices[PAL_MAX_GAMEPADS];

    win32_gamepad_mapping mappings[PAL_MAX_MAPPINGS];
    int mapping_count;
    pal_bool initialized;
    HWND hwnd;
} win32_gamepad_context;
win32_gamepad_context win32_gamepad_ctx = {0};

#pragma pack(push, 1)
typedef struct {
    uint16_t idReserved; // Must be 0
    uint16_t idType;     // Must be 1 for icons
    uint16_t idCount;    // Number of images
} ICONDIR;

typedef struct {
    uint8_t bWidth;         // Width in pixels
    uint8_t bHeight;        // Height in pixels
    uint8_t bColorCount;    // 0 if >= 8bpp
    uint8_t bReserved;      // Must be 0
    uint16_t wPlanes;       // Should be 1
    uint16_t wBitCount;     // Usually 32
    uint32_t dwBytesInRes;  // Size of PNG data
    uint32_t dwImageOffset; // Offset to PNG data (after header)
} ICONDIRENTRY;
#pragma pack(pop)

PALAPI pal_bool pal_make_window_fullscreen_ex(pal_window* window, int width, int height, int refresh_rate) {
    window->windowedStyle = GetWindowLongA(window->hwnd, GWL_STYLE);

    DEVMODEA dm = {0};
    dm.dmSize = sizeof(dm);
    dm.dmPelsWidth = width;
    dm.dmPelsHeight = height;
    dm.dmDisplayFrequency = refresh_rate;
    dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;

    if (ChangeDisplaySettingsExA(NULL, &dm, NULL, CDS_FULLSCREEN, NULL) != DISP_CHANGE_SUCCESSFUL) {
        MessageBoxA(window->hwnd, "Failed to switch display mode", "Error", MB_OK);
        return pal_false;
    }

    SetWindowLongA(window->hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
    SetWindowPos(window->hwnd, HWND_TOP, 0, 0, width, height, SWP_FRAMECHANGED | SWP_NOOWNERZORDER);

    return pal_true;
}

PALAPI pal_bool pal_make_window_fullscreen(pal_window* window) {
    window->windowedStyle = GetWindowLongA(window->hwnd, GWL_STYLE);
    int width = (int)window->width;
    int height = (int)window->height;
    DEVMODEA dm = {0};
    EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &dm);
    dm.dmSize = sizeof(dm);
    dm.dmPelsWidth = (DWORD)window->width;
    dm.dmPelsHeight = (DWORD)window->height;
    dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;

    if (ChangeDisplaySettingsExA(NULL, &dm, NULL, CDS_FULLSCREEN, NULL) != DISP_CHANGE_SUCCESSFUL) {
        MessageBoxA(window->hwnd, "Failed to switch display mode", "Error", MB_OK);
        return pal_false;
    }

    SetWindowLongA(window->hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
    SetWindowPos(window->hwnd, HWND_TOP, 0, 0, dm.dmPelsWidth, dm.dmPelsHeight, SWP_FRAMECHANGED | SWP_NOOWNERZORDER);

    return pal_true;
}

PALAPI pal_bool pal_make_window_fullscreen_windowed(pal_window* window) {
    // Save the current window style and rect
    window->windowedStyle = GetWindowLongA(window->hwnd, GWL_STYLE);

    // Get the monitor bounds
    HMONITOR monitor = MonitorFromWindow(window->hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO mi = {.cbSize = sizeof(mi)};
    if (!GetMonitorInfo(monitor, &mi)) {
        MessageBoxA(window->hwnd, "Failed to get monitor info.", "Error", MB_OK);
        return pal_false;
    }

    // Set the window to borderless fullscreen
    SetWindowLongA(window->hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
    if (!SetWindowPos(window->hwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_FRAMECHANGED | SWP_NOOWNERZORDER)) {
        MessageBoxA(window->hwnd, "Failed to resize window.", "Error", MB_OK);
        return pal_false;
    }

    return pal_true;
}

PALAPI pal_bool pal_make_window_windowed(pal_window* window) {
    // Restore display mode (in case exclusive mode was used)
    ChangeDisplaySettings(NULL, 0);

    // Restore the window style
    if (SetWindowLongA(window->hwnd, GWL_STYLE, window->windowedStyle) == 0) {
        MessageBoxA(window->hwnd, "Failed to restore window style.", "Error", MB_OK);
        return pal_false;
    }
    RECT rect;
    GetWindowRect(window->hwnd, &rect);
    // Restore the window's size and position
    if (!SetWindowPos(window->hwnd, NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER | SWP_FRAMECHANGED)) {
        MessageBoxA(window->hwnd, "Failed to restore window position.", "Error", MB_OK);
        return pal_false;
    }

    return pal_true;
}

PALAPI void pal_set_cursor(pal_window* window, const char* filepath, int size, int hotspot_x, int hotspot_y) {
    if (size <= 0)
        size = 32;
    if (size > 256)
        size = 256;

    FILE* f = fopen(filepath, "rb");
    if (!f) {
        MessageBoxA(window->hwnd, "Failed to open cursor file.", "SetCustomCursor Error", MB_ICONERROR);
        return;
    }

    unsigned char header[12] = {0};
    fread(header, 1, sizeof(header), f);
    fclose(f);

    HCURSOR hCursor = NULL;

    if (memcmp(header, "RIFF", 4) == 0 && memcmp(header + 8, "ACON", 4) == 0) {
        hCursor = (HCURSOR)LoadImageA(NULL, filepath, IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);
        if (!hCursor) {
            MessageBoxA(window->hwnd, "Failed to load .ani cursor.", "SetCustomCursor Error", MB_ICONERROR);
            return;
        }
    } else if (header[0] == 0x00 && header[1] == 0x00 && header[2] == 0x02 && header[3] == 0x00) {
        hCursor = (HCURSOR)LoadImageA(NULL, filepath, IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);
        if (!hCursor) {
            MessageBoxA(window->hwnd, "Failed to load .cur cursor.", "SetCustomCursor Error", MB_ICONERROR);
            return;
        }
    } else {
        int width, height, channels;
        unsigned char* pixels = stbi_load(filepath, &width, &height, &channels, 4);
        if (!pixels) {
            MessageBoxA(window->hwnd, "Failed to load image with stb_image.", "SetCustomCursor Error", MB_ICONERROR);
            return;
        }

        unsigned char* resized = malloc(size * size * 4);
        if (!resized) {
            stbi_image_free(pixels);
            MessageBoxA(window->hwnd, "Failed to allocate memory for resized image.", "SetCustomCursor Error", MB_ICONERROR);
            return;
        }

        stbir_resize_uint8_srgb(
            pixels, width, height, width * 4, resized, size, size, size * 4, STBIR_RGBA);
        stbi_image_free(pixels);

        HDC hdc = GetDC(NULL);

        BITMAPV5HEADER bi = {0};
        bi.bV5Size = sizeof(BITMAPV5HEADER);
        bi.bV5Width = size;
        bi.bV5Height = -size;
        bi.bV5Planes = 1;
        bi.bV5BitCount = 32;
        bi.bV5Compression = BI_BITFIELDS;
        bi.bV5RedMask = 0x00FF0000;
        bi.bV5GreenMask = 0x0000FF00;
        bi.bV5BlueMask = 0x000000FF;
        bi.bV5AlphaMask = 0xFF000000;

        void* bitmapData = NULL;
        HBITMAP hBitmap = CreateDIBSection(hdc, (BITMAPINFO*)&bi, DIB_RGB_COLORS, &bitmapData, NULL, 0);
        ReleaseDC(NULL, hdc);

        if (!hBitmap || !bitmapData) {
            free(resized);
            MessageBoxA(window->hwnd, "Failed to create DIB section.", "SetCustomCursor Error", MB_ICONERROR);
            return;
        }

        for (int y = 0; y < size; ++y) {
            for (int x = 0; x < size; ++x) {
                unsigned char* src = &resized[(y * size + x) * 4];
                unsigned char* dst = (unsigned char*)bitmapData + (y * size + x) * 4;
                dst[0] = src[2]; // B
                dst[1] = src[1]; // G
                dst[2] = src[0]; // R
                dst[3] = src[3]; // A
            }
        }

        free(resized);

        ICONINFO ii = {0};
        ii.fIcon = pal_false;
        ii.xHotspot = hotspot_x;
        ii.yHotspot = hotspot_y;
        ii.hbmColor = hBitmap;
        ii.hbmMask = CreateBitmap(size, size, 1, 1, NULL);

        hCursor = CreateIconIndirect(&ii);

        DeleteObject(ii.hbmMask);
        DeleteObject(hBitmap);

        if (!hCursor) {
            MessageBoxA(window->hwnd, "Failed to create cursor from image.", "SetCustomCursor Error", MB_ICONERROR);
            return;
        }
    }

    SetClassLongPtr(window->hwnd, GCLP_HCURSOR, (LONG_PTR)hCursor);
    SetCursor(hCursor);
}
// older windows versions do not support pngs being embedded in .ico
// files directly, therefore, we have to decode the .png with stb_image
// and only then can we use it.
static HICON win32_load_icon_from_file(const char* image_path, BOOL legacy) {
    FILE* file = fopen(image_path, "rb");
    if (!file)
        return NULL;

    uint8_t header[8];
    if (fread(header, 1, sizeof(header), file) < sizeof(header)) {
        fclose(file);
        return NULL;
    }

    // Check for ICO header
    if (header[0] == 0x00 && header[1] == 0x00 &&
        header[2] == 0x01 && header[3] == 0x00) {
        fclose(file);
        return (HICON)LoadImageA(NULL, image_path, IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
    }

    // Check for PNG header
    const unsigned char png_sig[8] = {0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A};
    BOOL is_png = (memcmp(header, png_sig, 8) == 0);
    fseek(file, 0, SEEK_SET);

    if (is_png && !legacy) {
        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        rewind(file);
        if (size <= 0) {
            fclose(file);
            return NULL;
        }

        uint8_t* png_data = (uint8_t*)malloc(size);
        if (!png_data) {
            fclose(file);
            return NULL;
        }
        fread(png_data, 1, size, file);
        fclose(file);

        size_t header_size = sizeof(ICONDIR) + sizeof(ICONDIRENTRY);
        size_t total_size = header_size + size;
        uint8_t* ico_data = (uint8_t*)malloc(total_size);
        if (!ico_data) {
            free(png_data);
            return NULL;
        }

        ICONDIR* icon_dir = (ICONDIR*)ico_data;
        icon_dir->idReserved = 0;
        icon_dir->idType = 1;
        icon_dir->idCount = 1;

        ICONDIRENTRY* entry = (ICONDIRENTRY*)(ico_data + sizeof(ICONDIR));
        entry->bWidth = 0;
        entry->bHeight = 0;
        entry->bColorCount = 0;
        entry->bReserved = 0;
        entry->wPlanes = 1;
        entry->wBitCount = 32;
        entry->dwBytesInRes = (uint32_t)size;
        entry->dwImageOffset = (uint32_t)header_size;

        memcpy(ico_data + header_size, png_data, size);
        free(png_data);

        HICON hIcon = CreateIconFromResourceEx(
            ico_data + entry->dwImageOffset,
            entry->dwBytesInRes,
            pal_true,
            0x00030000,
            0,
            0,
            LR_DEFAULTCOLOR);

        free(ico_data);
        return hIcon;
    }

    // Fallback: decode image with stb_image (PNG in legacy mode, JPEG, BMP)
    fclose(file);
    int width, height, channels;
    uint8_t* rgba = stbi_load(image_path, &width, &height, &channels, 4);
    if (!rgba)
        return NULL;

    // Convert RGBA to BGRA
    for (int i = 0; i < width * height; ++i) {
        uint8_t r = rgba[i * 4 + 0];
        uint8_t g = rgba[i * 4 + 1];
        uint8_t b = rgba[i * 4 + 2];
        uint8_t a = rgba[i * 4 + 3];
        rgba[i * 4 + 0] = b;
        rgba[i * 4 + 1] = g;
        rgba[i * 4 + 2] = r;
        rgba[i * 4 + 3] = a;
    }

    BITMAPV5HEADER bi = {0};
    bi.bV5Size = sizeof(BITMAPV5HEADER);
    bi.bV5Width = width;
    bi.bV5Height = -height;
    bi.bV5Planes = 1;
    bi.bV5BitCount = 32;
    bi.bV5Compression = BI_BITFIELDS;
    bi.bV5RedMask = 0x00FF0000;
    bi.bV5GreenMask = 0x0000FF00;
    bi.bV5BlueMask = 0x000000FF;
    bi.bV5AlphaMask = 0xFF000000;

    void* dib_pixels = NULL;
    HDC hdc = GetDC(NULL);
    HBITMAP color_bitmap = CreateDIBSection(hdc, (BITMAPINFO*)&bi, DIB_RGB_COLORS, &dib_pixels, NULL, 0);
    ReleaseDC(NULL, hdc);

    if (!color_bitmap || !dib_pixels) {
        stbi_image_free(rgba);
        return NULL;
    }

    memcpy(dib_pixels, rgba, width * height * 4);
    stbi_image_free(rgba);

    HBITMAP mask_bitmap = CreateBitmap(width, height, 1, 1, NULL);

    ICONINFO ii = {0};
    ii.fIcon = pal_true;
    ii.hbmMask = mask_bitmap;
    ii.hbmColor = color_bitmap;

    HICON hIcon = CreateIconIndirect(&ii);

    DeleteObject(color_bitmap);
    DeleteObject(mask_bitmap);

    return hIcon;
}

PALAPI void pal_set_window_icon(pal_window* window, const char* image_path) {
    HICON hIcon = win32_load_icon_from_file(image_path, pal_false);
    if (hIcon) {
        SendMessage(window->hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        SendMessage(window->hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    } else {
        MessageBoxA(window->hwnd, "Failed to load window icon", "Error", MB_OK | MB_ICONERROR);
    }
}

PALAPI void pal_set_window_icon_legacy(pal_window* window, const char* image_path) {
    HICON hIcon = win32_load_icon_from_file(image_path, pal_true);
    if (hIcon) {
        SendMessage(window->hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        SendMessage(window->hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    } else {
        MessageBoxA(window->hwnd, "Failed to load window icon", "Error", MB_OK | MB_ICONERROR);
    }
}

PALAPI void pal_set_taskbar_icon(pal_window* window, const char* image_path) {
    HICON hIcon = win32_load_icon_from_file(image_path, pal_false);
    if (hIcon) {
        SetClassLongPtr(window->hwnd, GCLP_HICONSM, (LONG_PTR)hIcon);
        SetClassLongPtr(window->hwnd, GCLP_HICON, (LONG_PTR)hIcon);
    } else {
        MessageBoxA(window->hwnd, "Failed to load taskbar icon", "Error", MB_OK | MB_ICONERROR);
    }
}

PALAPI void pal_set_taskbar_icon_legacy(pal_window* window, const char* image_path) {
    HICON hIcon = win32_load_icon_from_file(image_path, pal_true);
    if (hIcon) {
        SetClassLongPtr(window->hwnd, GCLP_HICONSM, (LONG_PTR)hIcon);
        SetClassLongPtr(window->hwnd, GCLP_HICON, (LONG_PTR)hIcon);
    } else {
        MessageBoxA(window->hwnd, "Failed to load legacy taskbar icon", "Error", MB_OK | MB_ICONERROR);
    }
}

LRESULT CALLBACK win32_fake_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

void win32_handle_raw_input(HRAWINPUT raw_input) {
    // currently unused. This is for GetRawInputData().
    // we currently use GetRawInputBuffer(), because it's better for high-polling rate mice.
}

void win32_handle_device_change(HANDLE hDevice, DWORD dwChange) {
}

static void win32_update_xinput() {
}

// --- pal_get_gamepad_count ---

PALAPI int pal_get_gamepad_count(void) {
    // Poll remaining XInput slots
    for (DWORD i = 0; i < MAX_XINPUT_CONTROLLERS; ++i) {

        XINPUT_STATE state;
        if (XinputGetstate_fn(i, &state) == ERROR_SUCCESS) {
            win32_gamepad_ctx.xinput_connected[i] = pal_true;
            win32_gamepad_ctx.xinput_state[i] = state;
        } else {
            win32_gamepad_ctx.xinput_connected[i] = pal_false;
        }
    }

    int total_count = 0;
    for (int i = 0; i < MAX_XINPUT_CONTROLLERS; ++i) {
        if (win32_gamepad_ctx.xinput_connected[i])
            total_count++;
    }

    return total_count;
}

int win32_init_gamepads() {
    //---------------
    //     Xinput
    //---------------

    // Try XInput 1.4 first (Windows 8+, has trigger motors)
    g_xinput_dll = LoadLibraryW(L"xinput1_4.dll");
    if (g_xinput_dll) {
        g_has_trigger_motors = pal_true;
    } else {
        // Fallback to XInput 1.3 (Windows Vista/7)
        g_xinput_dll = LoadLibraryW(L"xinput1_3.dll");
        if (!g_xinput_dll) {
            // Last resort: XInput 9.1.0 (Windows 7 compatibility)
            g_xinput_dll = LoadLibraryW(L"xinput9_1_0.dll");
        }
        g_has_trigger_motors = pal_false;
    }

    if (!g_xinput_dll) {
        return pal_false;
    }
    // Load function pointers
    XinputGetstate_fn = (DWORD(WINAPI*)(DWORD, XINPUT_STATE*))GetProcAddress(g_xinput_dll, "XInputGetState");
    XInputSetState_fn = (DWORD(WINAPI*)(DWORD, XINPUT_VIBRATION*))GetProcAddress(g_xinput_dll, "XInputSetState");
    XInputGetCapabilities_fn = (DWORD(WINAPI*)(DWORD, DWORD, XINPUT_CAPABILITIES*))GetProcAddress(g_xinput_dll, "XInputGetCapabilities");
    XInputEnable_fn = (void(WINAPI*)(BOOL))GetProcAddress(g_xinput_dll, "XInputEnable");

    // Check if we got the essential functions
    if (!XinputGetstate_fn || !XInputSetState_fn) {
        FreeLibrary(g_xinput_dll);
        g_xinput_dll = NULL;
        return pal_false;
    }

    return pal_true;
}

void win32_shutdown_gamepads(void) {
    if (g_xinput_dll) {
        FreeLibrary(g_xinput_dll);
        g_xinput_dll = NULL;
    }

    XinputGetstate_fn = NULL;
    XInputSetState_fn = NULL;
    XInputGetCapabilities_fn = NULL;
    XInputEnable_fn = NULL;
    g_has_trigger_motors = pal_false;
}

PALAPI pal_bool pal_get_gamepad_state(int index, pal_gamepad_state* out_state) {
    memset(out_state, 0, sizeof(pal_gamepad_state));

    // XInput controllers only
    if (index >= MAX_XINPUT_CONTROLLERS) {
        return pal_false;
    }

    // Check if this specific controller slot is connected
    if (!win32_gamepad_ctx.xinput_connected[index]) {
        return pal_false;
    }

    const XINPUT_GAMEPAD* pad = &win32_gamepad_ctx.xinput_state[index].Gamepad;

    // Process analog sticks with proper deadzone handling
    float lx = (float)pad->sThumbLX;
    float ly = (float)pad->sThumbLY;
    float rx = (float)pad->sThumbRX;
    float ry = (float)pad->sThumbRY;

    // Apply circular deadzone for left stick
    float left_magnitude = sqrtf(lx * lx + ly * ly);
    if (left_magnitude < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
        lx = ly = 0;
    } else {
        // Normalize to remove deadzone
        float normalized = (left_magnitude - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) /
                           (32767.0f - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
        lx = (lx / left_magnitude) * normalized;
        ly = (ly / left_magnitude) * normalized;
    }

    // Apply circular deadzone for right stick
    float right_magnitude = sqrtf(rx * rx + ry * ry);
    if (right_magnitude < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
        rx = ry = 0;
    } else {
        // Normalize to remove deadzone
        float normalized = (right_magnitude - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) /
                           (32767.0f - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
        rx = (rx / right_magnitude) * normalized;
        ry = (ry / right_magnitude) * normalized;
    }

    out_state->axes.left_x = fmaxf(-1.0f, fminf(1.0f, lx));
    out_state->axes.left_y = fmaxf(-1.0f, fminf(1.0f, ly));
    out_state->axes.right_x = fmaxf(-1.0f, fminf(1.0f, rx));
    out_state->axes.right_y = fmaxf(-1.0f, fminf(1.0f, ry));

    // Process triggers with deadzone
    out_state->axes.left_trigger = (pad->bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? 0.0f : (pad->bLeftTrigger / 255.0f);
    out_state->axes.right_trigger = (pad->bRightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? 0.0f : (pad->bRightTrigger / 255.0f);

    // Process buttons
    WORD b = pad->wButtons;
    out_state->buttons.a = (b & XINPUT_GAMEPAD_A) != 0;
    out_state->buttons.b = (b & XINPUT_GAMEPAD_B) != 0;
    out_state->buttons.x = (b & XINPUT_GAMEPAD_X) != 0;
    out_state->buttons.y = (b & XINPUT_GAMEPAD_Y) != 0;
    out_state->buttons.back = (b & XINPUT_GAMEPAD_BACK) != 0;
    out_state->buttons.start = (b & XINPUT_GAMEPAD_START) != 0;
    out_state->buttons.left_stick = (b & XINPUT_GAMEPAD_LEFT_THUMB) != 0;
    out_state->buttons.right_stick = (b & XINPUT_GAMEPAD_RIGHT_THUMB) != 0;
    out_state->buttons.left_shoulder = (b & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0;
    out_state->buttons.right_shoulder = (b & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
    out_state->buttons.dpad_up = (b & XINPUT_GAMEPAD_DPAD_UP) != 0;
    out_state->buttons.dpad_down = (b & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
    out_state->buttons.dpad_left = (b & XINPUT_GAMEPAD_DPAD_LEFT) != 0;
    out_state->buttons.dpad_right = (b & XINPUT_GAMEPAD_DPAD_RIGHT) != 0;

    // Set controller info
    strncpy(out_state->name, "Xbox Controller", sizeof(out_state->name) - 1);
    out_state->name[sizeof(out_state->name) - 1] = '\0';
    out_state->vendor_id = 0x045E;                // Microsoft
    out_state->product_id = (uint16_t)0xDEADBEEF; // Since xinput supports xbox360 and various Xbone controllers, we don't know this.
    out_state->connected = pal_true;
    out_state->is_xinput = pal_true;

    return pal_true;
}

PALAPI void pal_set_gamepad_vibration(int controller_id, float left_motor, float right_motor, float left_trigger, float right_trigger) {
    if (!XInputSetState_fn || controller_id > 4)
        return;

    if (g_has_trigger_motors) {
        // Extended vibration structure with trigger motors (XInput 1.4+)
        struct {
            WORD wLeftMotorSpeed;
            WORD wRightMotorSpeed;
            WORD wLeftTriggerMotor;
            WORD wRightTriggerMotor;
        } vibration_ex; // we call it this to avoid potential naming conflicts.

        vibration_ex.wLeftMotorSpeed = (WORD)(left_motor * 65535.0f);
        vibration_ex.wRightMotorSpeed = (WORD)(right_motor * 65535.0f);
        vibration_ex.wLeftTriggerMotor = (WORD)(left_trigger * 65535.0f);
        vibration_ex.wRightTriggerMotor = (WORD)(right_trigger * 65535.0f);

        XInputSetState_fn(controller_id, (XINPUT_VIBRATION*)&vibration_ex);
    } else {
        // Standard vibration (XInput 9.1.0/1.3)
        XINPUT_VIBRATION vibration;
        vibration.wLeftMotorSpeed = (WORD)(left_motor * 65535.0f);
        vibration.wRightMotorSpeed = (WORD)(right_motor * 65535.0f);

        XInputSetState_fn(controller_id, &vibration);
    }
}

PALAPI void pal_stop_gamepad_vibration(int controller_id) {
    pal_set_gamepad_vibration(controller_id, 0.0f, 0.0f, 0.0f, 0.0f);
}

PALAPI pal_bool pal_close_window(pal_window* window) {
    return (pal_bool)DestroyWindow(window->hwnd);
}
#define MAX_MICE 8

typedef struct {
    HANDLE device_handle;
    uint8_t buttons[8];
    uint8_t buttons_processed[8];
    int32_t delta_x;
    int32_t delta_y;
    char device_name[256];
}pal_mouse_state;

static pal_mouse_state g_mice[MAX_MICE] = {0};
static int g_mouse_count = 0;
uint32_t g_cached_mouse_buttons = 0;
static int g_cached_modifiers = PAL_MOD_NONE; // both keyboard and mouse code need this so that you can do ctrl+click or ctrl+scroll

void win32_enumerate_mice(void) {
    UINT numDevices;
    GetRawInputDeviceList(NULL, &numDevices, sizeof(RAWINPUTDEVICELIST));

    PRAWINPUTDEVICELIST deviceList = malloc(numDevices * sizeof(RAWINPUTDEVICELIST));
    GetRawInputDeviceList(deviceList, &numDevices, sizeof(RAWINPUTDEVICELIST));

    g_mouse_count = 0;

    for (UINT i = 0; i < numDevices && g_mouse_count < MAX_MICE; i++) {
        if (deviceList[i].dwType == RIM_TYPEMOUSE) {
            pal_mouse_state* mouse = &g_mice[g_mouse_count];
            mouse->device_handle = deviceList[i].hDevice;

            UINT size = 0;
            GetRawInputDeviceInfo(mouse->device_handle, RIDI_DEVICENAME, NULL, &size);
            if (size < sizeof(mouse->device_name)) {
                GetRawInputDeviceInfo(mouse->device_handle, RIDI_DEVICENAME, mouse->device_name, &size);
            }

            g_mouse_count++;
        }
    }

    free(deviceList);
}

void win32_handle_mouse(const RAWINPUT* raw) {
    // Find mouse index
    int mouse_index = 0;
    for (int i = 0; i < g_mouse_count; i++) {
        if (g_mice[i].device_handle == raw->header.hDevice) {
            mouse_index = i;
            break;
        }
    }

    pal_mouse_state* mouse = &g_mice[mouse_index];

    pal_event event = {0};
    int32_t dx = raw->data.mouse.lLastX;
    int32_t dy = raw->data.mouse.lLastY;

    // Update per-mouse delta
    mouse->delta_x += dx;
    mouse->delta_y += dy;

    USHORT buttons = raw->data.mouse.usButtonFlags;
    POINT point = {0};
    GetCursorPos(&point);
    ScreenToClient(g_current_window->hwnd, &point);

    // Handle motion
    if (dx || dy) {
        event.type = PAL_EVENT_MOUSE_MOTION;
        event.motion = (pal_mouse_motion_event){
            .x = point.x,
            .y = point.y,
            .delta_x = dx,
            .delta_y = dy,
            .buttons = g_cached_mouse_buttons,
            .mouse_id = mouse_index};
        pal__eventq_push(&g_event_queue, event);
    }

    // Handle mouse wheel
    if (buttons & RI_MOUSE_WHEEL) {
        SHORT wheel_delta = (SHORT)HIWORD(raw->data.mouse.usButtonData);

        event.type = PAL_EVENT_MOUSE_WHEEL;
        event.wheel = (pal_mouse_wheel_event){
            .mouse_x = point.x,
            .mouse_y = point.y,
            .x = 0,
            .y = (float)(wheel_delta / WHEEL_DELTA),
            .wheel_direction = (wheel_delta > 0) ? PAL_MOUSEWHEEL_VERTICAL : PAL_MOUSEWHEEL_HORIZONTAL,
            .mouse_id = mouse_index};
        pal__eventq_push(&g_event_queue, event);
    }

    // Handle horizontal wheel
    if (buttons & RI_MOUSE_HWHEEL) {
        SHORT hwheel_delta = (SHORT)HIWORD(raw->data.mouse.usButtonData);

        event.type = PAL_EVENT_MOUSE_WHEEL;
        event.wheel = (pal_mouse_wheel_event){
            .mouse_x = point.x,
            .mouse_y = point.y,
            .x = (float)(hwheel_delta / WHEEL_DELTA),
            .y = 0,
            .wheel_direction = (hwheel_delta > 0) ? PAL_MOUSEWHEEL_VERTICAL : PAL_MOUSEWHEEL_HORIZONTAL,
            .mouse_id = mouse_index};
        pal__eventq_push(&g_event_queue, event);
    }

    // Handle button events
    for (int i = 0; i < 5; i++) {
        uint16_t down = (buttons >> (i * 2)) & 1;
        uint16_t up = (buttons >> (i * 2 + 1)) & 1;
        int pal_button = win32_button_to_pal_button[i];

        if (down) {
            mouse->buttons[pal_button] = 1; // FIXED: Use pal_button instead of i
            g_cached_mouse_buttons |= (1 << i);

            event.type = PAL_EVENT_MOUSE_BUTTON_DOWN;
            event.button = (pal_mouse_button_event){
                .x = point.x,
                .y = point.y,
                .pressed = 1,
                .clicks = 1,
                .modifiers = g_cached_modifiers,
                .button = pal_button,
                .mouse_id = mouse_index};
            pal__eventq_push(&g_event_queue, event);
        } else if (up) {
            mouse->buttons[pal_button] = 0;           // FIXED: Use pal_button instead of i
            mouse->buttons_processed[pal_button] = 0; // FIXED: Use pal_button instead of i
            g_cached_mouse_buttons &= ~(1 << i);

            event.type = PAL_EVENT_MOUSE_BUTTON_UP;
            event.button = (pal_mouse_button_event){
                .x = point.x,
                .y = point.y,
                .pressed = 0,
                .clicks = 1,
                .modifiers = g_cached_modifiers,
                .button = pal_button,
                .mouse_id = mouse_index};
            pal__eventq_push(&g_event_queue, event);
        }
    }
}

#define MAX_KEYBOARDS 8

typedef struct {
    HANDLE device_handle;
    uint8_t keys[256];
    uint8_t keys_processed[256];
    uint8_t key_is_down[256];
    char device_name[256];
} pal_keyboard_state;

static pal_keyboard_state g_keyboards[MAX_KEYBOARDS] = {0};
static int g_keyboard_count = 0;

// Call during init & WM_INPUT_DEVICE_CHANGE
void win32_enumerate_keyboards(void) {
    UINT numDevices;
    GetRawInputDeviceList(NULL, &numDevices, sizeof(RAWINPUTDEVICELIST));

    PRAWINPUTDEVICELIST deviceList = malloc(numDevices * sizeof(RAWINPUTDEVICELIST));
    GetRawInputDeviceList(deviceList, &numDevices, sizeof(RAWINPUTDEVICELIST));

    g_keyboard_count = 0;

    for (UINT i = 0; i < numDevices && g_keyboard_count < MAX_KEYBOARDS; i++) {
        if (deviceList[i].dwType == RIM_TYPEKEYBOARD) {
            pal_keyboard_state* kb = &g_keyboards[g_keyboard_count];
            kb->device_handle = deviceList[i].hDevice;

            // Get device name
            UINT size = 0;
            GetRawInputDeviceInfo(kb->device_handle, RIDI_DEVICENAME, NULL, &size);
            if (size < sizeof(kb->device_name)) {
                GetRawInputDeviceInfo(kb->device_handle, RIDI_DEVICENAME, kb->device_name, &size);
            }

            g_keyboard_count++;
        }
    }

    free(deviceList);
}

// Function to update modifier state based on raw input
static void update_modifier_state(USHORT vk, pal_bool is_key_released) {
    int modifier_flag = 0;

    // Map VK codes to modifier flags
    switch (vk) {
        case VK_LSHIFT:
            modifier_flag = PAL_MOD_LSHIFT;
            break;
        case VK_RSHIFT:
            modifier_flag = PAL_MOD_RSHIFT;
            break;
        case VK_LCONTROL:
            modifier_flag = PAL_MOD_LCTRL;
            break;
        case VK_RCONTROL:
            modifier_flag = PAL_MOD_RCTRL;
            break;
        case VK_LMENU:
            modifier_flag = PAL_MOD_LALT;
            break;
        case VK_RMENU:
            modifier_flag = PAL_MOD_RALT;
            // Also handle AltGr (right alt)
            // TODO: Assuming that right alt = altgr is probably wrong.
            if (is_key_released) {
                g_cached_modifiers &= ~PAL_MOD_ALTGR;
            } else {
                g_cached_modifiers |= PAL_MOD_ALTGR;
            }
            break;
        case VK_LWIN:
            modifier_flag = PAL_MOD_LSUPER;
            break;
        case VK_RWIN:
            modifier_flag = PAL_MOD_RSUPER;
            break;
        case VK_CAPITAL:
            // Toggle caps lock state
            if (!is_key_released) {
                g_cached_modifiers ^= PAL_MOD_CAPS;
            }
            return; // Don't process as regular modifier
        case VK_NUMLOCK:
            // Toggle num lock state
            if (!is_key_released) {
                g_cached_modifiers ^= PAL_MOD_NUM;
            }
            return; // Don't process as regular modifier
        case VK_SCROLL:
            // Toggle scroll lock state
            if (!is_key_released) {
                g_cached_modifiers ^= PAL_MOD_SCROLL;
            }
            return; // Don't process as regular modifier
        default:
            return; // Not a modifier key
    }

    // Update the cached modifier state
    if (is_key_released) {
        g_cached_modifiers &= ~modifier_flag;
    } else {
        g_cached_modifiers |= modifier_flag;
    }
}

void win32_handle_keyboard(const RAWINPUT* raw) {
    // Find keyboard index
    int kb_index = 0; // Fallback to first keyboard
    for (int i = 0; i < g_keyboard_count; i++) {
        if (g_keyboards[i].device_handle == raw->header.hDevice) {
            kb_index = i;
            break;
        }
    }

    pal_keyboard_state* kb = &g_keyboards[kb_index];

    USHORT vk = raw->data.keyboard.VKey;
    USHORT makecode = raw->data.keyboard.MakeCode;
    USHORT flags = raw->data.keyboard.Flags;
    pal_event event = {0};

    pal_bool is_key_released = (flags & RI_KEY_BREAK) != 0;
    pal_bool is_extended = (flags & RI_KEY_E0) != 0;

    pal_bool is_repeat = 0;
    if (vk < 256) {
        if (!is_key_released && kb->key_is_down[vk]) {
            is_repeat = 1;
        }
        kb->key_is_down[vk] = !is_key_released;
    }

    update_modifier_state(vk, is_key_released);

    int pal_key = (vk < 256) ? win32_key_to_pal_key[vk] : 0;
    int pal_scancode = 0;

    if (is_extended) {
        if (makecode < 256) {
            pal_scancode = win32_extended_makecode_to_pal_scancode[makecode];
        }
    } else {
        if (makecode < 256) {
            pal_scancode = win32_makecode_to_pal_scancode[makecode];
        }
    }

    if (is_key_released) {
        event.type = PAL_EVENT_KEY_UP;
        event.key = (pal_keyboard_event){
            .virtual_key = pal_key,
            .scancode = pal_scancode,
            .pressed = 0,
            .repeat = 0,
            .modifiers = g_cached_modifiers,
            .keyboard_id = kb_index};
        kb->keys[pal_key] = 0;
        kb->keys_processed[pal_key] = 0;
    } else {
        event.type = PAL_EVENT_KEY_DOWN;
        event.key = (pal_keyboard_event){
            .virtual_key = pal_key,
            .scancode = pal_scancode,
            .pressed = 1,
            .repeat = is_repeat,
            .modifiers = g_cached_modifiers,
            .keyboard_id = kb_index};
        kb->keys[pal_key] = 1;
    }

    pal__eventq_push(&g_event_queue, event);
}

static LRESULT CALLBACK win32_window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    pal_window* window = g_current_window;
    pal_event event = {0};
    switch (msg) {
        case WM_CLOSE:
            event.type = PAL_EVENT_WINDOW_CLOSE_REQUESTED;
            event.window = (pal_window_event){
                .x = LOWORD(lparam),
                .y = HIWORD(lparam),
                .width = 0,
                .height = 0,
                .focused = 1,
                .visible = 0};
            break;
        case WM_DESTROY:
            event.type = PAL_EVENT_WINDOW_CLOSED;
            event.window = (pal_window_event){
                .x = LOWORD(lparam),
                .y = HIWORD(lparam),
                .width = 0,
                .height = 0,
                .focused = 0,
                .visible = 0};
            break;
        case WM_QUIT: // we only get this when we call PostQuitMessage. This is fucking retarted. If we want to kill the app, we just break from the main loop. I think we should just make this event do nothing.
            event.type = PAL_EVENT_QUIT;
            event.quit = (pal_quit_event){.code = 0};
            break;
        case WM_MOVE:
            event.type = PAL_EVENT_WINDOW_MOVED;
            event.window = (pal_window_event){
                .x = LOWORD(lparam),
                .y = HIWORD(lparam),
                .width = 0,
                .height = 0,
                .focused = 1,
                .visible = 1};
            break;
        case WM_SIZE:
            event.type = PAL_EVENT_WINDOW_RESIZED;
            event.window = (pal_window_event){
                .event_code = WM_SIZE,
                .x = 0,
                .y = 0,
                .width = LOWORD(lparam),
                .height = HIWORD(lparam),
                .focused = 1,
                .visible = 1};
            break;

        case WM_MOUSEMOVE: {

            if (window->confine_mouse) {
                TRACKMOUSEEVENT tme = {
                    .cbSize = sizeof(tme),
                    .dwFlags = TME_LEAVE,
                    .hwndTrack = window->hwnd,
                    .dwHoverTime = HOVER_DEFAULT};
                TrackMouseEvent(&tme);
                RECT rect;
                GetClientRect(window->hwnd, &rect);
                POINT tl = {rect.left, rect.top};
                POINT br = {rect.right, rect.bottom};
                ClientToScreen(window->hwnd, &tl);
                ClientToScreen(window->hwnd, &br);
                rect.left = tl.x;
                rect.top = tl.y;
                rect.right = br.x;
                rect.bottom = br.y;
                ClipCursor(&rect);
            }
            // Mouse just entered the window
            event.type = PAL_EVENT_MOUSE_MOTION;
            event.motion = (pal_mouse_motion_event){
                .x = GET_X_LPARAM(lparam),
                .y = GET_Y_LPARAM(lparam),
                .buttons = (uint32_t)wparam};

        } break;

        case WM_MOUSELEAVE:
            ClipCursor(NULL); // we unclip the cursor to the window in case it was clipped before.
            // Mouse just left the window
            break;
        case WM_WINDOWPOSCHANGED:
        case WM_WINDOWPOSCHANGING:
            event.type = PAL_EVENT_WINDOW_MOVED;
            WINDOWPOS* pos = (WINDOWPOS*)lparam;
            event.window = (pal_window_event){
                .event_code = msg,
                .x = pos->x,
                .y = pos->y,
                .width = pos->cx,
                .height = pos->cy,
                .focused = 1, // This is wrong, fix.
                .visible = 1};
            break;

        case WM_INPUT: {

            win32_handle_raw_input((HRAWINPUT)lparam);
            event.type = PAL_EVENT_SENSOR_UPDATE;
            event.sensor = (pal_sensor_event){
                .device_id = 0,
                .x = 0,
                .y = 0,
                .z = 0,
                .sensor_type = 0};
        }; break;

        case WM_DROPFILES: {
            event.type = PAL_EVENT_DROP_FILE;
            HDROP hDrop = (HDROP)wparam;
            UINT count = DragQueryFileW(hDrop, 0xFFFFFFFF, NULL, 0);
            const char** paths = malloc(sizeof(char*) * count);
            for (UINT i = 0; i < count; ++i) {
                WCHAR buffer[MAX_PATH];
                DragQueryFileW(hDrop, i, buffer, MAX_PATH);
                int len = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, NULL, 0, NULL, NULL);
                char* utf8 = malloc(len);
                WideCharToMultiByte(CP_UTF8, 0, buffer, -1, utf8, len, NULL, NULL);
                paths[i] = utf8;
            }
            event.drop = (pal_drop_event){
                .paths = paths,
                .count = count};
            DragFinish(hDrop);
            break;
        }

        case WM_ACTIVATEAPP: {
            event.window = (pal_window_event){
                .event_code = WM_MOVE,
                .x = LOWORD(lparam),
                .y = HIWORD(lparam),
                .width = 0,
                .height = 0,
                .visible = 1};
            if ((BOOL)wparam == FALSE) {
                event.type = PAL_EVENT_WINDOW_LOST_FOCUS;
                event.window.focused = 0;
                printf("PAL: Lost Focus!\n");
            } else {
                event.type = PAL_EVENT_WINDOW_GAINED_FOCUS;
                event.window.focused = 1;
                printf("PAL: Gained Focus!\n");
            }
        }; break;

            // TODO: Make this return a pal_event of some kind.
        case WM_INPUT_DEVICE_CHANGE: {
            win32_enumerate_keyboards();
            win32_enumerate_mice();
            win32_handle_device_change((HANDLE)lparam, (DWORD)wparam);
            printf("Device Changed!\n");
        }; break;

        default:
            event.type = PAL_EVENT_NONE;
            return DefWindowProcA(hwnd, msg, wparam, lparam);
    }
    pal__eventq_push(&g_event_queue, event);
    return 0;
}

static PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
PALAPI pal_window* pal_create_window(int width, int height, const char* window_title, uint64_t window_flags) {
    DWORD ext_window_style = 0;
    DWORD window_style = 0;

    if (window_flags & PAL_WINDOW_NOT_FOCUSABLE) {
        ext_window_style |= WS_EX_NOACTIVATE;
    }
    if (window_flags & PAL_WINDOW_ALWAYS_ON_TOP) {
        ext_window_style |= WS_EX_TOPMOST;
    }

    if (window_flags & PAL_WINDOW_UTILITY) {

        ext_window_style |= WS_EX_TOOLWINDOW;
        window_style |= WS_SYSMENU;
    } else if (window_flags & PAL_WINDOW_POPUP_MENU) {
        window_style |= WS_POPUPWINDOW;
    } else if (window_flags & PAL_WINDOW_TOOLTIP) {
        ext_window_style |= WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
    } else if (window_flags & PAL_WINDOW_RESIZABLE) {
        window_style |= WS_OVERLAPPEDWINDOW;

    } else {
        window_style |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
    }

    if (window_flags & PAL_WINDOW_BORDERLESS) {
        window_style = WS_POPUP;
    }

    if (window_flags & PAL_WINDOW_FULLSCREEN) {

        DEVMODE dm = {0};
        dm.dmSize = sizeof(dm);
        dm.dmPelsWidth = width;
        dm.dmPelsHeight = height;
        dm.dmBitsPerPel = 32;
        dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

        LONG result = ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
        if (result != DISP_CHANGE_SUCCESSFUL) {
            MessageBoxA(NULL, "Failed to make window fullscreen!", "Error", MB_OK);
        }
        window_style = WS_POPUP;
    }

    // these variables are only
    // used when initializing opengl.
    pal_window* fakewindow = NULL;
    HGLRC fakeRC = 0;
    PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
    // we default to opengl.
    if (!(window_flags & PAL_WINDOW_OPENGL) || !(window_flags & PAL_WINDOW_VULKAN) || !(window_flags & PAL_WINDOW_METAL)) {
        window_flags |= PAL_WINDOW_OPENGL;
    }

    if (window_flags & PAL_WINDOW_OPENGL) {
        fakewindow = (pal_window*)malloc(sizeof(pal_window));
        WNDCLASSEXA fakewc = {0};
        fakewc.cbSize = sizeof(WNDCLASSEXA);
        fakewc.lpfnWndProc = win32_fake_window_proc;
        fakewc.hInstance = GetModuleHandleA(0);
        fakewc.lpszClassName = "Win32 Fake Window Class";
        fakewc.hCursor = LoadCursorW(NULL, IDC_ARROW);

        RegisterClassExA(&fakewc);

        fakewindow->hwnd = CreateWindowExA(
            ext_window_style,     // Optional window styles.
            fakewc.lpszClassName, // Window class
            window_title,   // Window text
            window_style,  // Window style

            // Size and position
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,

            NULL,             // Parent window
            NULL,             // Menu
            fakewc.hInstance, // Instance handle
            NULL              // Additional application data
        );

        if (fakewindow->hwnd == NULL) {
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

        fakeRC = wglCreateContext(s_fakeDC);
        if (fakeRC == 0) {
            MessageBoxA(fakewindow->hwnd, "wglCreateContext() failed.", "Try again later", MB_ICONERROR);
            return fakewindow;
        }
        if (wglMakeCurrent(s_fakeDC, fakeRC) == 0) {
            MessageBoxA(fakewindow->hwnd, "wglMakeCurrent() failed.", "Try again later", MB_ICONERROR);
            return fakewindow;
        }
        wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)(wglGetProcAddress("wglChoosePixelFormatARB"));
        if (wglChoosePixelFormatARB == NULL) {
            MessageBoxA(fakewindow->hwnd, "wglGetProcAddress() failed.", "Try again later", MB_ICONERROR);
            return fakewindow;
        }
        wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)(wglGetProcAddress("wglCreateContextAttribsARB"));
        if (wglCreateContextAttribsARB == NULL) {
            MessageBoxA(fakewindow->hwnd, "wglGetProcAddress() failed.", "Try again later", MB_ICONERROR);
            return fakewindow;
        }
        wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
        if (wglSwapIntervalEXT == NULL) {
            MessageBoxA(fakewindow->hwnd, "wglGetProcAddress() failed.", "Try again later", MB_ICONERROR);
            return fakewindow;
        }
    }

    WNDCLASSEXA wc = {0};

    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.lpfnWndProc = win32_window_proc;
    wc.hInstance = GetModuleHandleA(0);
    wc.lpszClassName = "Win32 Window Class";
    wc.hCursor = LoadCursorW(NULL, IDC_ARROW);

    RegisterClassExA(&wc);

    pal_window* window = (pal_window*)malloc(sizeof(pal_window));
    window->width = (float)width;
    window->height = (float)height;
    // -- CREATING EVENT QUEUE --
    // It's very important to set g_current_window to the pal_window
    // before we call CreateWindowExA() on window->hwnd, because if we don't,
    // windows will call win32_window_proc() as soon as the window is created,
    // and in that function, we rely on g_current_window for some things like
    // getting the event queue that belongs to the window, and if it's not set,
    // we will just crash. - Abdelrahman July 29, 2025
    g_current_window = window;
    // -- CREATING WINDOW --
    window->hwnd = CreateWindowExA(
        ext_window_style, // Optional window styles.
        wc.lpszClassName, // Window class
        window_title,     // Window text
        window_style,     // Window style

        // Size and position
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width,
        height,

        NULL,         // Parent window
        NULL,         // Menu
        wc.hInstance, // Instance handle
        NULL          // Additional application data
    );

    if (window->hwnd == NULL) {
        return window;
    }
    
    if (window_flags & PAL_WINDOW_OPENGL) {
		window->hdc = GetDC(window->hwnd);

		const int pixelAttribs[] = {
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE, WGL_SUPPORT_OPENGL_ARB, GL_TRUE, WGL_DOUBLE_BUFFER_ARB, GL_TRUE, WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB, WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB, WGL_COLOR_BITS_ARB, 32, WGL_ALPHA_BITS_ARB, 8, WGL_DEPTH_BITS_ARB, 24, WGL_STENCIL_BITS_ARB, 8, WGL_SAMPLE_BUFFERS_ARB, GL_TRUE, WGL_SAMPLES_ARB, 4, // NOTE: Maybe this is used for multisampling?
			0                                                                                                                                                                                                                                                                                                                                              // null terminator for attrib list.
		};

		int pixelFormatID;
		UINT numFormats;
		uint8_t status = wglChoosePixelFormatARB(window->hdc, pixelAttribs, NULL, 1, &pixelFormatID, &numFormats);
		if (status == 0 || numFormats == 0) {
			MessageBoxA(window->hwnd, "wglChoosePixelFormatARB() failed.", "Try again later", MB_ICONERROR);
			return window;
		}

		PIXELFORMATDESCRIPTOR PFD;
		PFD.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
		DescribePixelFormat(window->hdc, pixelFormatID, sizeof(PFD), &PFD);
		SetPixelFormat(window->hdc, pixelFormatID, &PFD);

		int contextAttribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3, WGL_CONTEXT_MINOR_VERSION_ARB, 3, WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB, WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB, 0};

		window->hglrc = wglCreateContextAttribsARB(window->hdc, 0, contextAttribs);
    }

    pal_window *final_window = NULL;
    if (window_flags & PAL_WINDOW_OPENGL) {
		if (window->hglrc) {
            final_window = window;
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(fakeRC);
			ReleaseDC(fakewindow->hwnd, s_fakeDC);
			DestroyWindow(fakewindow->hwnd);
			free(fakewindow);
		} else {
			// This is supposed to be a fallback in case we can't create the context that we want.
			// Ideally, this should never happen. - Abdelrahman june 13, 2024
            final_window = fakewindow;
            final_window->hglrc = fakeRC;
            final_window->hdc = s_fakeDC;
		}
    } else {
        final_window = window; 
    }

    RAWINPUTDEVICE rid[3];

    // 1. Keyboard
    rid[0].usUsagePage = 0x01;                          // Generic desktop controls
    rid[0].usUsage = 0x06;                              // Keyboard
    rid[0].dwFlags = RIDEV_INPUTSINK | RIDEV_DEVNOTIFY; // Receive input even when not focused
    rid[0].hwndTarget = final_window->hwnd;

    // 2. Mouse
    rid[1].usUsagePage = 0x01; // Generic desktop controls
    rid[1].usUsage = 0x02;     // Mouse
    rid[1].dwFlags = RIDEV_INPUTSINK | RIDEV_DEVNOTIFY;
    rid[1].hwndTarget = final_window->hwnd;

    // 3. Joystick/Gamepad (Note: Not all controllers appear as HIDs)
    rid[2].usUsagePage = 0x01; // Generic desktop controls
    rid[2].usUsage = 0x04;     // Joystick
    rid[2].dwFlags = RIDEV_INPUTSINK | RIDEV_DEVNOTIFY;
    rid[2].hwndTarget = final_window->hwnd;

    if (!RegisterRawInputDevices(rid, 3, sizeof(RAWINPUTDEVICE))) {
        DWORD error = GetLastError();
        printf("RegisterRawInputDevices failed. Error code: %lu\n", error);
    }

	if (!(window_flags & PAL_WINDOW_HIDDEN)) {
		if (window_flags & PAL_WINDOW_FULLSCREEN) {
			ShowWindow(final_window->hwnd, SW_SHOW);
		} else if (window_flags & PAL_WINDOW_MAXIMIZED) {
			ShowWindow(final_window->hwnd, SW_SHOWMAXIMIZED);
		} else if (window_flags & PAL_WINDOW_MINIMIZED) {
			ShowWindow(final_window->hwnd, SW_SHOWMINIMIZED);
		} else {
			ShowWindow(final_window->hwnd, SW_SHOWNORMAL);
		}
	} else {
		ShowWindow(final_window->hwnd, SW_HIDE);
	}
	if (window_flags & PAL_WINDOW_MOUSE_CONFINED) {
		RECT rect;
		GetClientRect(final_window->hwnd, &rect);
		POINT tl = {rect.left, rect.top};
		POINT br = {rect.right, rect.bottom};
		ClientToScreen(final_window->hwnd, &tl);
		ClientToScreen(final_window->hwnd, &br);
		rect.left = tl.x;
		rect.top = tl.y;
		rect.right = br.x;
		rect.bottom = br.y;
		ClipCursor(&rect);
		final_window->confine_mouse = pal_true;
	} else {
		final_window->confine_mouse = pal_false;
	}
	SetForegroundWindow(final_window->hwnd);
	SetFocus(final_window->hwnd);
	// save the final_window style and the final_window rect in case the user sets the final_window to windowed before setting it to fullscreen.
	// The fullscreen function is supposed to save this state whenever the user calls it,
	// but if the user doesn't, the pal_make__window_windowed() function uses a state that's all zeroes,
	// so we have to save it here. - Abdelrahman june 13, 2024
	final_window->windowedStyle = GetWindowLongA(final_window->hwnd, GWL_STYLE); // style of the final_window.
	return final_window;
}

PALAPI pal_ivec2 pal_get_window_border_size(pal_window* window) {
    RECT rect;
    GetClientRect(window->hwnd, &rect);

    HDC hdc = GetDC(window->hwnd);
    int dpiX = GetDeviceCaps(hdc, LOGPIXELSX);
    int dpiY = GetDeviceCaps(hdc, LOGPIXELSY);
    ReleaseDC(window->hwnd, hdc);

    // Convert logical pixels to physical pixels
    float scaleX = dpiX / 96.0f;
    float scaleY = dpiY / 96.0f;

    pal_ivec2 border_size;
    border_size.x = (int)((rect.right - rect.left) * scaleX);
    border_size.y = (int)((rect.bottom - rect.top) * scaleY);
    return border_size;
}

PALAPI void *pal_get_window_handle(pal_window *window) {
    return (void*)window->hwnd;
}

PALAPI int pal_make_context_current(pal_window* window) {
    if (!wglMakeCurrent(window->hdc, window->hglrc)) {
        MessageBoxA(window->hwnd, "wglMakeCurrent() failed.", "Try again later", MB_ICONERROR);
        return 1;
    }
    return 0;
}

PALAPI int pal_show_cursor(void) {
    int result = -1;
    while (result < 0) {
        result = ShowCursor(pal_true);
    }
    return result;
}

PALAPI int pal_hide_cursor(void) {
    int result = 1;
    while (result >= 0) {
        result = ShowCursor(FALSE);
    }
    return result;
}

PALAPI pal_bool pal_maximize_window(pal_window* window) {
    return ShowWindow(window->hwnd, SW_MAXIMIZE);
}

PALAPI pal_bool pal_minimize_window(pal_window* window) {
    return ShowWindow(window->hwnd, SW_MINIMIZE);
}

static int win32_get_raw_input_buffer(void);

PALAPI void pal__reset_mouse_deltas(void);
PALAPI pal_bool pal_poll_events(pal_event* event) {
    MSG msg = {0};
    if (!g_message_pump_drained) {
        pal__reset_mouse_deltas();
        win32_get_raw_input_buffer();
        while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE) != 0) {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
        g_message_pump_drained = pal_true;
    }

    pal_event_queue* queue = &g_event_queue;

    if (queue->size) { // if queue is not empty,

        // peek
        *event = queue->events[queue->front];

        // dequeue
        queue->front = (queue->front + 1) % queue->capacity;
        queue->size--;
        return 1;
    } else {
        g_message_pump_drained = pal_false;
        return 0;
    }
}

PALAPI pal_bool pal_set_window_title(pal_window* window, const char* string) {
    return SetWindowTextA(window->hwnd, string);
}

PALAPI pal_monitor* pal_get_primary_monitor(void) {
    // The point (0, 0) is guaranteed to be on the primary monitor
    pal_monitor* monitor = malloc(sizeof(pal_monitor));
    POINT pt = {0, 0};
    monitor->handle = MonitorFromPoint(pt, MONITOR_DEFAULTTOPRIMARY);
    return monitor;
}

PALAPI pal_video_mode* pal_get_video_mode(pal_monitor* monitor) {
    MONITORINFOEX mi = {.cbSize = sizeof(MONITORINFOEX)};
    if (!GetMonitorInfo(monitor->handle, (MONITORINFO*)&mi))
        return 0;

    DEVMODE dm = {.dmSize = sizeof(DEVMODE)};
    if (!EnumDisplaySettings(mi.szDevice, ENUM_CURRENT_SETTINGS, &dm))
        return 0;
    pal_video_mode* mode = (pal_video_mode*)malloc(sizeof(pal_video_mode));
    mode->width = dm.dmPelsWidth;
    mode->height = dm.dmPelsHeight;
    mode->refresh_rate = dm.dmDisplayFrequency;
    mode->bits_per_pixel = dm.dmBitsPerPel;

    return mode;
}

PALAPI pal_bool pal_set_video_mode(pal_video_mode* mode) {
    DEVMODEA dm = {.dmSize = sizeof(DEVMODEA)};
    if (mode == NULL) {
        if (ChangeDisplaySettingsA(NULL, 0)) {
            return 1;
        } else {
            return 0;
        }
    } else {
        dm.dmPelsWidth = mode->width;
        dm.dmPelsHeight = mode->height;
        dm.dmDisplayFrequency = mode->refresh_rate;
        dm.dmBitsPerPel = mode->bits_per_pixel;
        if (ChangeDisplaySettingsA(&dm, 0)) {
            return 1;
        } else {
            return 0;
        }
    }
}

PALAPI void* pal_gl_get_proc_address(const char* proc) {
    static HMODULE opengl_module = NULL; // Cached across all calls

    void* p = (void*)wglGetProcAddress(proc);
    if (p == NULL || p == (void*)0x1 || p == (void*)0x2 || p == (void*)0x3 || p == (void*)-1) {
        // Load opengl32.dll once on first call, reuse handle afterwards
        if (opengl_module == NULL) {
            opengl_module = LoadLibraryA("opengl32.dll");
        }

        if (opengl_module != NULL) {
            p = (void*)GetProcAddress(opengl_module, proc);
        } else {
            p = NULL;
        }
    }
    return p;
}

PALAPI void pal_swap_buffers(pal_window* window) {
    SwapBuffers(window->hdc);
}

PALAPI void pal_swap_interval(int interval) {
    wglSwapIntervalEXT(interval);
}

// Handler function signatures
typedef void (*RawInputHandler)(const RAWINPUT*);

// Helper struct to hold reusable buffers
PALAPI pal_vec2 pal_get_mouse_position(pal_window* window) {
    POINT cursor_pos = {0};
    GetCursorPos(&cursor_pos);

    ScreenToClient(window->hwnd, &cursor_pos); // Convert to client-area coordinates
    return (pal_vec2){
        (float)cursor_pos.x,
        (float)cursor_pos.y};
}

// Handles Gamepads, Joysticks, Steering wheels, etc...
void win32_handle_hid(const RAWINPUT* raw) {
    printf("%d", raw->data.hid.dwCount);
}

RawInputHandler Win32InputHandlers[3] = {
    win32_handle_mouse,
    win32_handle_keyboard,
    win32_handle_hid};

#define RAW_INPUT_BUFFER_CAPACITY (64 * 1024) // 64 KB

static BYTE g_raw_input_buffer[RAW_INPUT_BUFFER_CAPACITY];

static int win32_get_raw_input_buffer(void) {
    UINT bufferSize = RAW_INPUT_BUFFER_CAPACITY;
    UINT inputEventCount = GetRawInputBuffer((PRAWINPUT)g_raw_input_buffer, &bufferSize, sizeof(RAWINPUTHEADER));

    PRAWINPUT raw = (PRAWINPUT)g_raw_input_buffer;
    for (UINT i = 0; i < inputEventCount; ++i) {
        UINT type = raw->header.dwType;
        if (type == RIM_TYPEMOUSE) {
            win32_handle_mouse(raw);
        } else if (type == RIM_TYPEKEYBOARD) {
            win32_handle_keyboard(raw);
        } else {
            win32_handle_hid(raw);
        }
        raw = NEXTRAWINPUTBLOCK(raw);
    }
    return 0;
}

//----------------------------------------------------------------------------------
// File Functions.
//----------------------------------------------------------------------------------

// Helper function to convert UTF-8 to UTF-16
static wchar_t* win32_utf8_to_utf16(const char* utf8_str) {
    if (!utf8_str) return NULL;
    
    int len = MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1, NULL, 0);
    if (len == 0) return NULL;
    
    wchar_t* utf16_str = (wchar_t*)malloc(len * sizeof(wchar_t));
    if (!utf16_str) return NULL;
    
    if (MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1, utf16_str, len) == 0) {
        free(utf16_str);
        return NULL;
    }
    
    return utf16_str;
}

PALAPI pal_bool pal_does_file_exist(const char* file_path) {
    wchar_t* wide_path = win32_utf8_to_utf16(file_path);
    if (!wide_path) return 0;
    
    DWORD attrs = GetFileAttributesW(wide_path);
    free(wide_path);
    return (attrs != INVALID_FILE_ATTRIBUTES) && !(attrs & FILE_ATTRIBUTE_DIRECTORY);
}

PALAPI size_t pal_get_file_size(const char* file_path) {
    wchar_t* wide_path = win32_utf8_to_utf16(file_path);
    if (!wide_path) return 0;
    
    HANDLE file = CreateFileW(
        wide_path,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    
    free(wide_path);

    if (file == INVALID_HANDLE_VALUE) {
        return 0;
    }

    LARGE_INTEGER file_size;
    if (GetFileSizeEx(file, &file_size)) {
        CloseHandle(file);
        return file_size.QuadPart;
    }
    CloseHandle(file);
    return 0;
}

PALAPI size_t pal_get_last_write_time(const char* file) {
    wchar_t* wide_path = win32_utf8_to_utf16(file);
    if (!wide_path) return 0;
    
    WIN32_FILE_ATTRIBUTE_DATA fileInfo;
    if (!GetFileAttributesExW(wide_path, GetFileExInfoStandard, &fileInfo)) {
        free(wide_path);
        return 0;
    }
    free(wide_path);
    return ((uint64_t)fileInfo.ftLastWriteTime.dwHighDateTime << 32) |
           fileInfo.ftLastWriteTime.dwLowDateTime;
}

PALAPI size_t pal_get_last_read_time(const char* file) {
    wchar_t* wide_path = win32_utf8_to_utf16(file);
    if (!wide_path) return 0;
    
    WIN32_FILE_ATTRIBUTE_DATA fileInfo;
    if (!GetFileAttributesExW(wide_path, GetFileExInfoStandard, &fileInfo)) {
        free(wide_path);
        return 0;
    }
    free(wide_path);
    return ((uint64_t)fileInfo.ftLastAccessTime.dwHighDateTime << 32) |
           fileInfo.ftLastAccessTime.dwLowDateTime;
}

PALAPI uint32_t pal_get_file_permissions(const char* file_path) {
    if (!file_path) {
        return 0;
    }

    wchar_t* wide_path = win32_utf8_to_utf16(file_path);
    if (!wide_path) return 0;

    uint32_t permissions = 0;

    PACL pDacl = NULL;
    PSECURITY_DESCRIPTOR pSD = NULL;
    DWORD dwRes = GetNamedSecurityInfoW(
        wide_path,
        SE_FILE_OBJECT,
        DACL_SECURITY_INFORMATION,
        NULL,
        NULL,
        &pDacl,
        NULL,
        &pSD);

    free(wide_path);

    if (dwRes != ERROR_SUCCESS) {
        if (pSD)
            LocalFree(pSD);
        return 0;
    }

    HANDLE hToken;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        LocalFree(pSD);
        return 0;
    }

    GENERIC_MAPPING mapping = {0};
    mapping.GenericRead = FILE_GENERIC_READ;
    mapping.GenericWrite = FILE_GENERIC_WRITE;
    mapping.GenericExecute = FILE_GENERIC_EXECUTE;
    mapping.GenericAll = FILE_ALL_ACCESS;

    PRIVILEGE_SET privileges = {0};
    DWORD privSize = sizeof(privileges);
    BOOL accessStatus = FALSE;

    ACCESS_MASK accessRights = 0;
    dwRes = GetEffectiveRightsFromAclA(pDacl, NULL, &accessRights);

    if (dwRes == ERROR_SUCCESS) {
        if (accessRights & FILE_GENERIC_READ)
            permissions |= PAL_READ;
        if (accessRights & FILE_GENERIC_WRITE)
            permissions |= PAL_WRITE;
        if (accessRights & FILE_GENERIC_EXECUTE)
            permissions |= PAL_EXECUTE;
    }

    CloseHandle(hToken);
    LocalFree(pSD);

    return permissions;
}

PALAPI pal_bool pal_change_file_permissions(const char* file_path, uint32_t permission_flags) {
    if (!file_path) {
        return 0;
    }

    wchar_t* wide_path = win32_utf8_to_utf16(file_path);
    if (!wide_path) return 0;

    DWORD dwAccessRights = 0;
    if (permission_flags & PAL_READ)
        dwAccessRights |= GENERIC_READ;
    if (permission_flags & PAL_WRITE)
        dwAccessRights |= GENERIC_WRITE;
    if (permission_flags & PAL_EXECUTE)
        dwAccessRights |= GENERIC_EXECUTE;

    if (dwAccessRights == 0) {
        free(wide_path);
        return 0;
    }

    HANDLE hToken;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        free(wide_path);
        return 0;
    }

    DWORD dwSize = 0;
    GetTokenInformation(hToken, TokenUser, NULL, 0, &dwSize);
    if (dwSize == 0) {
        CloseHandle(hToken);
        free(wide_path);
        return 0;
    }

    PTOKEN_USER pTokenUser = (PTOKEN_USER)malloc(dwSize);
    if (!pTokenUser) {
        CloseHandle(hToken);
        free(wide_path);
        return 0;
    }

    if (!GetTokenInformation(hToken, TokenUser, pTokenUser, dwSize, &dwSize)) {
        free(pTokenUser);
        CloseHandle(hToken);
        free(wide_path);
        return 0;
    }

    CloseHandle(hToken);

    EXPLICIT_ACCESS_W ea = {0};
    ea.grfAccessPermissions = dwAccessRights;
    ea.grfAccessMode = SET_ACCESS;
    ea.grfInheritance = NO_INHERITANCE;
    ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea.Trustee.TrusteeType = TRUSTEE_IS_USER;
    ea.Trustee.ptstrName = (LPWSTR)pTokenUser->User.Sid;

    PACL pOldDACL = NULL, pNewDACL = NULL;
    PSECURITY_DESCRIPTOR pSD = NULL;

    DWORD dwRes = GetNamedSecurityInfoW(
        wide_path,
        SE_FILE_OBJECT,
        DACL_SECURITY_INFORMATION,
        NULL,
        NULL,
        &pOldDACL,
        NULL,
        &pSD);

    if (dwRes != ERROR_SUCCESS) {
        free(pTokenUser);
        free(wide_path);
        return 0;
    }

    dwRes = SetEntriesInAclW(1, &ea, pOldDACL, &pNewDACL);
    if (dwRes != ERROR_SUCCESS) {
        LocalFree(pSD);
        free(pTokenUser);
        free(wide_path);
        return 0;
    }

    dwRes = SetNamedSecurityInfoW(
        wide_path,
        SE_FILE_OBJECT,
        DACL_SECURITY_INFORMATION,
        NULL,
        NULL,
        pNewDACL,
        NULL);

    LocalFree(pNewDACL);
    LocalFree(pSD);
    free(pTokenUser);
    free(wide_path);

    return (dwRes == ERROR_SUCCESS) ? 1 : 0;
}

PALAPI unsigned char *pal_read_entire_file(const char *file_path, size_t *bytes_read) {
    wchar_t* wide_path = win32_utf8_to_utf16(file_path);
    if (!wide_path) return NULL;
    
    HANDLE file = CreateFileW(
        wide_path,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    free(wide_path);

    if (file == INVALID_HANDLE_VALUE) {
        return NULL;
    }

    LARGE_INTEGER file_size;
    if (!GetFileSizeEx(file, &file_size)) {
        CloseHandle(file);
        return NULL;
    }

    if (file_size.QuadPart > SIZE_MAX) {
        CloseHandle(file);
        return NULL;
    }

    size_t total_size = (size_t)file_size.QuadPart;
    char *buffer = (char *)malloc(total_size + 1);
    if (!buffer) {
        CloseHandle(file);
        return NULL;
    }

    size_t total_read = 0;
    while (total_read < total_size) {
        DWORD chunk = (DWORD)((total_size - total_read > MAXDWORD) ? MAXDWORD : (total_size - total_read));
        DWORD read_now = 0;

        if (!ReadFile(file, buffer + total_read, chunk, &read_now, NULL)) {
            free(buffer);
            CloseHandle(file);
            return NULL;
        }

        if (read_now == 0) break;

        total_read += read_now;
    }
    buffer[total_size] = '\0';
    CloseHandle(file);

    if (bytes_read)
        *bytes_read = total_read;

    return (unsigned char*)buffer;
}

PALAPI pal_bool pal_write_file(const char* file_path, size_t file_size, char* buffer) {
    wchar_t* wide_path = win32_utf8_to_utf16(file_path);
    if (!wide_path) return 1;
    
    HANDLE file = CreateFileW(
        wide_path,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    free(wide_path);

    if (file == INVALID_HANDLE_VALUE) {
        return 1;
    }

    size_t remaining = file_size;
    const char* current_pos = buffer;

    while (remaining > 0) {
        DWORD chunk = (remaining > MAXDWORD) ? MAXDWORD : (DWORD)remaining;
        DWORD bytes_written = 0;

        if (!WriteFile(file, current_pos, chunk, &bytes_written, NULL) ||
            bytes_written != chunk) {
            CloseHandle(file);
            return 1;
        }

        remaining -= chunk;
        current_pos += chunk;
    }

    CloseHandle(file);
    return 0;
}

PALAPI pal_bool pal_copy_file(const char* original_path, const char* copy_path) {
    wchar_t* wide_original = win32_utf8_to_utf16(original_path);
    wchar_t* wide_copy = win32_utf8_to_utf16(copy_path);
    
    if (!wide_original || !wide_copy) {
        if (wide_original) free(wide_original);
        if (wide_copy) free(wide_copy);
        return 1;
    }
    
    BOOL result = CopyFileW(wide_original, wide_copy, FALSE);
    
    free(wide_original);
    free(wide_copy);
    
    return result ? 0 : 1;
}

PALAPI pal_file* pal_open_file(const char* file_path) {
    wchar_t* wide_path = win32_utf8_to_utf16(file_path);
    if (!wide_path) return NULL;
    
    pal_file* file = NULL;
    if (!file) {
        free(wide_path);
        return NULL;
    }
    
    file = CreateFileW(
        wide_path,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    free(wide_path);

    if (file == INVALID_HANDLE_VALUE) {
        free(file);
        return NULL;
    }

    return file;
}

PALAPI pal_bool pal_read_from_open_file(pal_file* file, size_t offset, size_t bytes_to_read, char* buffer) {
    if (!file || file == INVALID_HANDLE_VALUE || !buffer) {
        return 0;
    }

    LARGE_INTEGER file_offset = {0};
    file_offset.QuadPart = (LONGLONG)offset;

    if (!SetFilePointerEx(file, file_offset, NULL, FILE_BEGIN)) {
        return 0;
    }

    size_t total_read = 0;
    DWORD to_read;
    while (total_read < bytes_to_read) {
        if ((DWORD)((bytes_to_read - total_read) > MAXDWORD)) {
            to_read = MAXDWORD;
        } else {
            to_read = (DWORD)(bytes_to_read - total_read);
        }
        DWORD bytesRead = 0;
        BOOL success = ReadFile(file, buffer + total_read, to_read, &bytesRead, NULL);
        if (!success || bytesRead != to_read) {
            return 0;
        }
        total_read += bytesRead;
    }

    return 1;
}

PALAPI pal_bool pal_close_file(pal_file* file) {
    if (!file) return 0;
    
    if (!CloseHandle(file)) {
        return 0;
    }
    free(file);
    return 1;
}

//----------------------------------------------------------------------------------
// Directory Listing.
//----------------------------------------------------------------------------------
PALAPI pal_bool pal_path_is_dir(const char *path) {
    wchar_t* wide_path = win32_utf8_to_utf16(path);
    if (!wide_path) return 1;

    pal_bool is_dir = PathIsDirectoryW(wide_path) ? pal_true : pal_false;

    free(wide_path);

    return is_dir;
}

//----------------------------------------------------------------------------------
// Random Number Generator.
//----------------------------------------------------------------------------------

PALAPI void pal_srand(uint64_t* state, uint64_t seed) {
    if (seed == 0) {
        seed = 1; // Avoid zero state which would produce all zeros
    }
    *state = seed;
}

PALAPI uint32_t pal_rand(uint64_t* state) {
    // SDL's well-tested LCG constants:
    // - Multiplier: 0xff1cd035 (32-bit for better performance on 32-bit archs)
    // - Increment: 0x05 (small odd number, generates smaller ARM code)
    // - These constants passed extensive testing with PractRand and TestU01
    *state = *state * 0xff1cd035ul + 0x05;

    // Return upper 32 bits - they have better statistical properties
    // and longer period than lower bits in an LCG
    return (uint32_t)(*state >> 32);
}

//----------------------------------------------------------------------------------
// Sound Functions.
//----------------------------------------------------------------------------------
int win32_init_sound(void) {
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
    hr = g_xaudio2->lpVtbl->CreateMasteringVoice(g_xaudio2, &g_mastering_voice, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, NULL, NULL, AudioCategory_GameEffects);

    return hr;
}

// XAudio2 callback for streaming
typedef struct {
    IXAudio2VoiceCallbackVtbl* lpVtbl;
    pal_sound* sound;
} StreamingVoiceCallback;

static size_t calculate_buffer_size_for_seconds(pal_sound* sound, float seconds) {
    // bytes_per_second = sample_rate * channels * (bits_per_sample / 8)
    size_t bytes_per_second = sound->sample_rate * sound->channels * (sound->bits_per_sample / 8);
    return (size_t)(bytes_per_second * seconds);
}

static size_t load_next_chunk(pal_sound* sound, unsigned char* buffer, size_t buffer_size) {
    size_t bytes_read = 0;

    if (sound->source_file) {
        size_t remaining = sound->total_data_size - sound->bytes_streamed;

        if (remaining == 0) {
            printf("WAV: No remaining data\n");
            return 0;
        }

        if (!sound->source_file) {
            printf("ERROR: WAV file handle is NULL!\n");
            return 0;
        }

        size_t to_read = (buffer_size < remaining) ? buffer_size : remaining;
        size_t seek_pos = sound->data_offset + sound->bytes_streamed;

        printf("WAV: Seeking to %zu (data_offset=%zu + bytes_streamed=%zu), reading %zu bytes\n",
               seek_pos,
               sound->data_offset,
               sound->bytes_streamed,
               to_read);

        if (fseek(sound->source_file, (long)seek_pos, SEEK_SET) != 0) {
            printf("WAV: Seek failed to position %zu\n", seek_pos);
            return 0;
        }

        bytes_read = fread(buffer, 1, to_read, sound->source_file);

        if (bytes_read != to_read) {
            printf("WAV: Read %zu bytes but expected %zu, feof=%d, ferror=%d\n",
                   bytes_read,
                   to_read,
                   feof(sound->source_file),
                   ferror(sound->source_file));
        }

        sound->bytes_streamed += bytes_read;
        printf("WAV: Read %zu bytes, new bytes_streamed=%zu\n", bytes_read, sound->bytes_streamed);

    } else if (sound->decoder) {
        stb_vorbis* vorbis = (stb_vorbis*)sound->decoder;

        // Calculate how many sample frames we can fit in the buffer
        size_t bytes_per_sample_frame = sound->channels * sizeof(short);
        size_t max_sample_frames = buffer_size / bytes_per_sample_frame;

        if (max_sample_frames <= 0) {
            printf("OGG: Buffer too small for samples\n");
            return 0;
        }

        // Calculate how many sample frames we should skip
        size_t sample_frames_streamed = sound->bytes_streamed / bytes_per_sample_frame;
        int current_decoder_pos = stb_vorbis_get_sample_offset(vorbis);

        printf("OGG: Loading chunk - streamed %zu sample frames, decoder at %d\n",
               sample_frames_streamed,
               current_decoder_pos);

        // If decoder is behind our streaming position, seek forward
        if (current_decoder_pos < (int)sample_frames_streamed) {
            printf("OGG: Seeking decoder from %d to %zu\n", current_decoder_pos, sample_frames_streamed);
            if (stb_vorbis_seek(vorbis, (unsigned int)sample_frames_streamed)) {
                current_decoder_pos = stb_vorbis_get_sample_offset(vorbis);
                printf("OGG: Seek successful, decoder now at %u\n", current_decoder_pos);
            } else {
                printf("OGG: Seek failed!\n");
                return 0;
            }
        }

        // Allocate temporary float buffers for non-interleaved data
        float** channel_buffers = (float**)malloc(sound->channels * sizeof(float*));
        for (int i = 0; i < sound->channels; i++) {
            channel_buffers[i] = (float*)malloc(max_sample_frames * sizeof(float));
        }

        // Read using non-interleaved API
        int total_sample_frames_read = stb_vorbis_get_samples_float(
            vorbis, sound->channels, channel_buffers, (int)max_sample_frames);

        if (total_sample_frames_read > 0) {
            // Convert float samples to interleaved 16-bit shorts
            short* output_ptr = (short*)buffer;

            for (int sample = 0; sample < total_sample_frames_read; sample++) {
                for (int ch = 0; ch < sound->channels; ch++) {
                    float f_sample = channel_buffers[ch][sample];
                    // Clamp and convert to 16-bit
                    if (f_sample > 1.0f)
                        f_sample = 1.0f;
                    if (f_sample < -1.0f)
                        f_sample = -1.0f;
                    short s_sample = (short)(f_sample * 32767.0f);
                    output_ptr[sample * sound->channels + ch] = s_sample;
                }
            }
        }

        // Clean up temporary buffers
        for (int i = 0; i < sound->channels; i++) {
            free(channel_buffers[i]);
        }
        free(channel_buffers);

        bytes_read = total_sample_frames_read * bytes_per_sample_frame;

        // Update bytes_streamed to track total bytes processed
        sound->bytes_streamed += bytes_read;

        int after_sample = stb_vorbis_get_sample_offset(vorbis);

        printf("OGG: Read %d sample frames (%zu bytes)\n", total_sample_frames_read, bytes_read);
        printf("OGG: Decoder position: %d -> %u (advanced %d samples)\n",
               current_decoder_pos,
               after_sample,
               after_sample - current_decoder_pos);
        printf("OGG: Total bytes streamed: %zu\n", sound->bytes_streamed);

        // Check if we're at the end
        if (total_sample_frames_read == 0) {
            unsigned int total_samples = stb_vorbis_stream_length_in_samples(vorbis);
            printf("OGG: End of stream - position %d of %u total samples\n", after_sample, total_samples);
        }
    }

    return bytes_read;
}

static void STDMETHODCALLTYPE OnBufferEnd(IXAudio2VoiceCallback* callback, void* pBufferContext) {
    StreamingVoiceCallback* cb = (StreamingVoiceCallback*)callback;
    pal_sound* sound = cb->sound;

    static int buffer_end_count = 0;
    printf("OnBufferEnd %d: buffer=%p\n", buffer_end_count++, pBufferContext);

    // Free the buffer that just finished playing
    // pBufferContext contains the buffer pointer we set in buffer.pContext
    if (pBufferContext) {
        free(pBufferContext);
    }

    if (!sound->is_streaming || sound->stream_finished) {
        return;
    }

    // Check how many buffers are queued
    XAUDIO2_VOICE_STATE state;
    sound->source_voice->lpVtbl->GetState(sound->source_voice, &state, 0);

    // If we have fewer than 2 buffers queued, queue another one
    if (state.BuffersQueued < 2) {
        float chunk_seconds = sound->preload_seconds;
        size_t buffer_chunk_size = calculate_buffer_size_for_seconds(sound, chunk_seconds);

        unsigned char* chunk_buffer = (unsigned char*)malloc(buffer_chunk_size);

        if (chunk_buffer) {
            size_t bytes_read = load_next_chunk(sound, chunk_buffer, buffer_chunk_size);

            if (bytes_read > 0) {
                XAUDIO2_BUFFER buffer = {0};
                buffer.AudioBytes = (UINT32)bytes_read;
                buffer.pAudioData = chunk_buffer;
                buffer.pContext = chunk_buffer; // For OnBufferEnd to free

                HRESULT hr = sound->source_voice->lpVtbl->SubmitSourceBuffer(
                    sound->source_voice, &buffer, NULL);

                if (FAILED(hr)) {
                    printf("ERROR: Failed to submit buffer in OnBufferEnd: 0x%08X\n", hr);
                    free(chunk_buffer);
                } else {
                    printf("Queued next buffer: %u bytes\n", buffer.AudioBytes);
                }
            } else {
                // End of stream reached
                free(chunk_buffer);
                sound->stream_finished = 1;
                printf("Stream finished - no more data\n");
            }
        }
    }
}

static void STDMETHODCALLTYPE OnVoiceProcessingPassEnd(IXAudio2VoiceCallback* callback) {
    // OnBufferEnd now handles buffer queuing, so this can be much simpler
    StreamingVoiceCallback* cb = (StreamingVoiceCallback*)callback;
    pal_sound* sound = cb->sound;

    if (!sound->is_streaming || sound->stream_finished) {
        return;
    }

    // Just log state for debugging
    XAUDIO2_VOICE_STATE state;
    sound->source_voice->lpVtbl->GetState(sound->source_voice, &state, 0);

    static int callback_count = 0;
    if (callback_count % 100 == 0) { // Log less frequently
        printf("ProcessingPass %d: BuffersQueued=%u, SamplesPlayed=%llu\n",
               callback_count,
               state.BuffersQueued,
               state.SamplesPlayed);
    }
    callback_count++;
}

static void STDMETHODCALLTYPE OnBufferStart(IXAudio2VoiceCallback* callback, void* pBufferContext) {
    // Called when XAudio2 starts processing a buffer
    // pBufferContext contains the buffer we passed in SubmitSourceBuffer
    // For streaming, we don't need to do anything special here
}

static void STDMETHODCALLTYPE OnLoopEnd(IXAudio2VoiceCallback* callback, void* pBufferContext) {
    // Called when a buffer with XAUDIO2_LOOP_INFINITE completes a loop
    // We don't use looping buffers in streaming, so this stays empty
}

static void STDMETHODCALLTYPE OnVoiceError(IXAudio2VoiceCallback* callback, void* pBufferContext, HRESULT error) {
    // Called when XAudio2 encounters an error
    StreamingVoiceCallback* cb = (StreamingVoiceCallback*)callback;
    pal_sound* sound = cb->sound;

    printf("XAudio2 Voice Error: 0x%08X\n", error);

    // Stop streaming on error
    sound->stream_finished = 1;
}

static void STDMETHODCALLTYPE OnVoiceProcessingPassStart(IXAudio2VoiceCallback* callback, UINT32 BytesRequired) {
    // Called when XAudio2 starts processing audio for this voice
    // BytesRequired tells us how much data XAudio2 needs
    StreamingVoiceCallback* cb = (StreamingVoiceCallback*)callback;
    pal_sound* sound = cb->sound;

    // We can use BytesRequired to be more intelligent about buffer management
    // For now, we'll handle this in OnBufferEnd instead
}

static void STDMETHODCALLTYPE OnStreamEnd(IXAudio2VoiceCallback* callback) {
    // Called when the last buffer with XAUDIO2_END_OF_STREAM finishes playing
    StreamingVoiceCallback* cb = (StreamingVoiceCallback*)callback;
    pal_sound* sound = cb->sound;

    printf("Audio stream ended\n");

    // Mark stream as finished
    sound->stream_finished = 1;

    // You could trigger a callback here to notify your game that the music finished
    // or automatically start the next track in a playlist
}

static IXAudio2VoiceCallbackVtbl StreamingCallbackVtbl = {
    OnVoiceProcessingPassStart,
    OnVoiceProcessingPassEnd,
    OnStreamEnd,
    OnBufferStart,
    OnBufferEnd,
    OnLoopEnd,
    OnVoiceError,
};

PALAPI int pal_play_music(pal_sound* sound, float volume) {
    if (!g_xaudio2 || !g_mastering_voice) {
        printf("ERROR: XAudio2 not initialized\n");
        return E_FAIL;
    }

    printf("Playing sound: streaming=%s, voice_callback=%p\n",
           sound->is_streaming ? "YES" : "NO",
           sound->voice_callback);

    // Set volume
    sound->source_voice->lpVtbl->SetVolume(sound->source_voice, volume, 0);

    // Submit initial buffer (the preloaded data)
    XAUDIO2_BUFFER buffer = {0};
    buffer.AudioBytes = (UINT32)sound->data_size;
    buffer.pAudioData = sound->data;
    buffer.pContext = NULL; // Don't free this buffer - it's owned by the sound object

    // For non-streaming sounds, set end of stream flag
    // For streaming sounds, let the callback handle subsequent buffers
    buffer.Flags = sound->is_streaming ? 0 : XAUDIO2_END_OF_STREAM;

    float initial_seconds = (float)sound->data_size /
                            (sound->sample_rate * sound->channels * (sound->bits_per_sample / 8));

    printf("Submitting initial buffer: %u bytes (%.3f seconds), flags=0x%x\n",
           buffer.AudioBytes,
           initial_seconds,
           buffer.Flags);

    HRESULT hr = sound->source_voice->lpVtbl->SubmitSourceBuffer(sound->source_voice, &buffer, NULL);
    if (FAILED(hr)) {
        printf("ERROR: Failed to submit source buffer: 0x%08X\n", hr);
        return hr;
    }

    // IMPROVEMENT: For streaming sounds, queue an additional buffer immediately
    if (sound->is_streaming && !sound->stream_finished) {
        float chunk_seconds = sound->preload_seconds;
        size_t buffer_chunk_size = calculate_buffer_size_for_seconds(sound, chunk_seconds);

        unsigned char* chunk_buffer = (unsigned char*)malloc(buffer_chunk_size);
        if (chunk_buffer) {
            size_t bytes_read = load_next_chunk(sound, chunk_buffer, buffer_chunk_size);

            if (bytes_read > 0) {
                XAUDIO2_BUFFER next_buffer = {0};
                next_buffer.AudioBytes = (UINT32)bytes_read;
                next_buffer.pAudioData = chunk_buffer;
                next_buffer.pContext = chunk_buffer; // For OnBufferEnd to free

                hr = sound->source_voice->lpVtbl->SubmitSourceBuffer(sound->source_voice, &next_buffer, NULL);
                if (FAILED(hr)) {
                    printf("ERROR: Failed to submit second buffer: 0x%08X\n", hr);
                    free(chunk_buffer);
                } else {
                    printf("Queued second buffer: %u bytes\n", next_buffer.AudioBytes);
                }
            } else {
                free(chunk_buffer);
                // If we can't load more data immediately, that's fine for short files
            }
        }
    }

    // Start playback
    hr = sound->source_voice->lpVtbl->Start(sound->source_voice, 0, 0);
    if (FAILED(hr)) {
        printf("ERROR: Failed to start voice: 0x%08X\n", hr);
        return hr;
    }

    printf("Playback started successfully\n");
    return S_OK;
}

static int pal__load_wav(const char* filename, pal_sound* out, float seconds);
static int pal__load_ogg(const char* filename, pal_sound* out, float seconds);

pal_sound* win32_load_sound(const char* filename, float seconds);
PALAPI pal_sound* pal_load_music(const char* filename) {
    // every loaded buffer will be this long.
    const float buffer_length_in_seconds = 2.0f;
    return win32_load_sound(filename, buffer_length_in_seconds);
}

PALAPI pal_sound* pal_load_sound(const char* filename) {
    return win32_load_sound(filename, 0.0f);
}

pal_sound* win32_load_sound(const char* filename, float seconds) {
    FILE* file = fopen(filename, "rb");

    if (!file)
        return NULL;

    char header[12];
    if (fread(header, 1, sizeof(header), file) < 12) {
        fclose(file);
        return NULL;
    }

    pal_sound* sound = (pal_sound*)malloc(sizeof(pal_sound));
    if (!sound) {
        fclose(file);
        printf("ERROR: %s(): Failed to allocate memory for sound!\n", __func__);
        return NULL;
    }
    *sound = (pal_sound){0};

    int result = 0;

    if (memcmp(header, "RIFF", 4) == 0 && memcmp(header + 8, "WAVE", 4) == 0) {
        fclose(file);
        result = pal__load_wav(filename, sound, seconds);
    } else if (memcmp(header, "OggS", 4) == 0) {
        fclose(file);
        result = pal__load_ogg(filename, sound, seconds);
    } else {
        fclose(file);
        free(sound);
        return NULL; // unsupported format
    }

    // we expect pal__load_wav and pal__load_ogg to return 1 for success.
    if (result != 1) {
        free(sound);
        return NULL;
    }

    static const GUID KSDATAFORMAT_SUBTYPE_PCM = {
        0x00000001, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}};

    static const GUID KSDATAFORMAT_SUBTYPE_IEEE_FLOAT = {
        0x00000003, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}};

    WAVEFORMATEXTENSIBLE wfex = {0};
    wfex.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
    wfex.Format.nChannels = sound->channels;
    wfex.Format.nSamplesPerSec = sound->sample_rate;
    wfex.Format.wBitsPerSample = sound->bits_per_sample;
    wfex.Format.nBlockAlign = (wfex.Format.nChannels * wfex.Format.wBitsPerSample) / 8;
    wfex.Format.nAvgBytesPerSec = wfex.Format.nSamplesPerSec * wfex.Format.nBlockAlign;
    wfex.Format.cbSize = 22;
    wfex.Samples.wValidBitsPerSample = (uint16_t)sound->bits_per_sample;

    wfex.SubFormat = (sound->is_float)
                         ? KSDATAFORMAT_SUBTYPE_IEEE_FLOAT
                         : KSDATAFORMAT_SUBTYPE_PCM;

    switch (sound->channels) {
        case 1:
            wfex.dwChannelMask = SPEAKER_FRONT_CENTER;
            break;
        case 2:
            wfex.dwChannelMask = SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT;
            break;
        case 4:
            wfex.dwChannelMask = SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT |
                                 SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT;
            break;
        case 6:
            wfex.dwChannelMask = SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT |
                                 SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY |
                                 SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT;
            break;
        case 8:
            wfex.dwChannelMask = SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT |
                                 SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY |
                                 SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT |
                                 SPEAKER_SIDE_LEFT | SPEAKER_SIDE_RIGHT;
            break;
        default:
            wfex.dwChannelMask = 0;
            break;
    }

    sound->source_voice = NULL;

    // Create streaming callback if this is a streaming sound
    if (seconds > 0.0f) {
        StreamingVoiceCallback* callback = (StreamingVoiceCallback*)malloc(sizeof(StreamingVoiceCallback));
        if (callback) {
            callback->lpVtbl = &StreamingCallbackVtbl;
            callback->sound = sound;
            sound->voice_callback = (IXAudio2VoiceCallback*)callback;
        }
        sound->preload_seconds = seconds;
        sound->is_streaming = 1;

        if (sound->source_file) {
            // WAV streaming setup
            long current_pos = ftell(sound->source_file);
            fseek(sound->source_file, 0, SEEK_END);
            long file_end = ftell(sound->source_file);
            fseek(sound->source_file, current_pos, SEEK_SET);

            sound->total_data_size = file_end - sound->data_offset;
            sound->bytes_streamed = sound->data_size;

            printf("WAV streaming setup: total_size=%zu bytes, preloaded=%zu bytes\n",
                   sound->total_data_size,
                   sound->data_size);

        } else if (sound->decoder) {
            stb_vorbis* vorbis = (stb_vorbis*)sound->decoder;

            unsigned int total_sample_frames = stb_vorbis_stream_length_in_samples(vorbis);
            size_t bytes_per_sample_frame = sound->channels * sizeof(short);

            printf("OGG streaming setup - CONTINUOUS APPROACH:\n");
            printf("  - Total sample frames: %u\n", total_sample_frames);
            printf("  - Preloaded data size: %zu bytes\n", sound->data_size);

            unsigned int current_position = stb_vorbis_get_sample_offset(vorbis);
            printf("  - Decoder position after preload: %u\n", current_position);

            sound->total_data_size = total_sample_frames * bytes_per_sample_frame;

            // Set bytes_streamed to match what we've already preloaded
            sound->bytes_streamed = sound->data_size;

            printf("  - Total estimated size: %zu bytes\n", sound->total_data_size);
            printf("  - Bytes already streamed (preloaded): %zu\n", sound->bytes_streamed);

            // Store filename for reopening decoder if needed
            size_t filename_len = strlen(filename);
            sound->filename = (char*)malloc(filename_len + 1);
            strcpy(sound->filename, filename);
        }
    } else {
        sound->preload_seconds = 0;
        sound->is_streaming = 0;
        sound->bytes_streamed = 0;
        sound->total_data_size = 0;
    }

    HRESULT hr = g_xaudio2->lpVtbl->CreateSourceVoice(
        g_xaudio2, &sound->source_voice, (const WAVEFORMATEX*)&wfex, 0, XAUDIO2_DEFAULT_FREQ_RATIO, sound->voice_callback, NULL, NULL);

    if (FAILED(hr)) {
        if (sound->voice_callback)
            free(sound->voice_callback);
        free(sound);
        return NULL;
    }

    return sound;
}

PALAPI void pal_free_music(pal_sound* sound) {
    if (sound->is_streaming) {
        sound->stream_finished = 1;
    }

    if (sound->source_file) {
        fclose(sound->source_file);
    }

    if (sound->decoder) {
        stb_vorbis_close((stb_vorbis*)sound->decoder);
    }

    if (sound->source_voice) {
        sound->source_voice->lpVtbl->DestroyVoice(sound->source_voice);
    }

    if (sound->voice_callback) {
        free(sound->voice_callback);
    }

    if (sound->filename) {
        free(sound->filename);
    }
    free(sound->data);
    free(sound);
}

PALAPI int pal_play_sound(pal_sound* sound, float volume) {
    if (!g_xaudio2 || !g_mastering_voice) {
        return E_FAIL;
    }
    
    if (sound == NULL) return S_FALSE;

    // Set the volume
    sound->source_voice->lpVtbl->SetVolume(sound->source_voice, volume, 0);

    XAUDIO2_BUFFER buffer = {
        .AudioBytes = (UINT32)sound->data_size,
        .pAudioData = sound->data,
        .Flags = XAUDIO2_END_OF_STREAM};

    HRESULT hr;

    hr = sound->source_voice->lpVtbl->SubmitSourceBuffer(sound->source_voice, &buffer, NULL);
    if (FAILED(hr)) {
        sound->source_voice->lpVtbl->DestroyVoice(sound->source_voice);
        return hr;
    }

    hr = sound->source_voice->lpVtbl->Start(sound->source_voice, 0, 0);
    if (FAILED(hr)) {
        sound->source_voice->lpVtbl->DestroyVoice(sound->source_voice);
        return hr;
    }

    return S_OK;
}

PALAPI void pal_free_sound(pal_sound* sound) {
    if (sound) {
        if (sound->source_voice) {
            sound->source_voice->lpVtbl->DestroyVoice(sound->source_voice);
            printf("ERROR: %s(): Source voice was not initialized. Was the sound played before? If not, you have to play it before destroying!\n", __func__);
        }
        if (sound->data) {
            free(sound->data);
            sound->data = NULL;
        } else {
            printf("ERROR: %s(): Pointer to data in pal_sound was null. Not going to free. Was the sound loaded?\n", __func__);
        }
        free(sound);
    } else {
        printf("ERROR: %s(): Pointer to pal_sound was null. Not going to free. Was it allocated to begin with?\n", __func__);
    }
}

PALAPI int pal_stop_sound(pal_sound* sound) {
    HRESULT hr = 0;
    hr = sound->source_voice->lpVtbl->Stop(sound->source_voice, 0, XAUDIO2_COMMIT_NOW);

    if (FAILED(hr)) {
        sound->source_voice->lpVtbl->DestroyVoice(sound->source_voice);
        return hr;
    }

    hr = sound->source_voice->lpVtbl->FlushSourceBuffers(sound->source_voice);

    if (FAILED(hr)) {
        sound->source_voice->lpVtbl->DestroyVoice(sound->source_voice);
        return hr;
    }
    return hr;
}

//----------------------------------------------------------------------------------
// Time Functions.
//----------------------------------------------------------------------------------
typedef struct _KSYSTEM_TIME {
    ULONG LowPart;  // Low 32 bits of the 64-bit time value
    LONG High1Time; // High 32 bits (first copy)
    LONG High2Time; // High 32 bits (second copy)
} KSYSTEM_TIME, *PKSYSTEM_TIME;

typedef struct _KUSER_SHARED_DATA {
    ULONG TickCountLowDeprecated;
    ULONG TickCountMultiplier;
    KSYSTEM_TIME InterruptTime;
    KSYSTEM_TIME SystemTime;
    KSYSTEM_TIME TimeZoneBias;
    // padding to get to right offsets.
    UCHAR Padding0[0x300 - 0x20];
    LONGLONG QpcFrequency; // Performance Counter Frequency at offset 0x300
    // padding to get to TickCount
    UCHAR Padding1[0x320 - 0x308];
    union {
        KSYSTEM_TIME TickCount;
        UINT64 TickCountQuad;
    };
} KUSER_SHARED_DATA, *PKUSER_SHARED_DATA;
#define KUSER_SHARED_DATA_ADDRESS 0x7FFE0000
static uint64_t g_app_start_time = 0;

PALAPI pal_time pal_get_date_and_time_utc(void) {
    PKUSER_SHARED_DATA kuser = (PKUSER_SHARED_DATA)KUSER_SHARED_DATA_ADDRESS;
    LARGE_INTEGER time = {0};
    do {
        time.HighPart = kuser->SystemTime.High1Time;
        time.LowPart = kuser->SystemTime.LowPart;
    } while (time.HighPart != kuser->SystemTime.High2Time);

    uint64_t total_100ns = time.QuadPart;
    uint64_t total_days = total_100ns / (10000000ULL * 60 * 60 * 24); // 100ns to days
    uint64_t remaining_100ns = total_100ns % (10000000ULL * 60 * 60 * 24);

    uint32_t year = 1601 + (uint32_t)(total_days / 365.25);

    uint64_t days_since_1601 = total_days;
    year = 1601;
    while (1) {
        uint32_t days_in_year = 365;
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            days_in_year = 366;
        }

        if (days_since_1601 < days_in_year)
            break;
        days_since_1601 -= days_in_year;
        year++;
    }

    uint32_t days_in_months[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        days_in_months[1] = 29;
    }

    uint32_t month = 1;
    while (month <= 12 && days_since_1601 >= days_in_months[month - 1]) {
        days_since_1601 -= days_in_months[month - 1];
        month++;
    }

    uint32_t day = (uint32_t)days_since_1601 + 1;

    uint64_t total_seconds = remaining_100ns / 10000000ULL;
    uint32_t hours = (uint32_t)(total_seconds / 3600);
    total_seconds %= 3600;
    uint32_t minutes = (uint32_t)(total_seconds / 60);
    uint32_t seconds = (uint32_t)(total_seconds % 60);

    pal_time result = {0};
    result.year = year;
    result.month = month;
    result.day = day;
    result.weeks = 0; // Unused for system time
    result.hours = hours;
    result.minutes = minutes;
    result.seconds = seconds;

    return result;
}

PALAPI pal_time pal_get_date_and_time_local(void) {
    PKUSER_SHARED_DATA kuser = (PKUSER_SHARED_DATA)KUSER_SHARED_DATA_ADDRESS;

    LARGE_INTEGER system_time = {0};
    do {
        system_time.HighPart = kuser->SystemTime.High1Time;
        system_time.LowPart = kuser->SystemTime.LowPart;
    } while (system_time.HighPart != kuser->SystemTime.High2Time);

    LARGE_INTEGER timezone_bias = {0};
    do {
        timezone_bias.HighPart = kuser->TimeZoneBias.High1Time;
        timezone_bias.LowPart = kuser->TimeZoneBias.LowPart;
    } while (timezone_bias.HighPart != kuser->TimeZoneBias.High2Time);

    uint64_t local_time_100ns = system_time.QuadPart - timezone_bias.QuadPart;

    uint64_t total_days = local_time_100ns / (10000000ULL * 60 * 60 * 24); // 100ns to days
    uint64_t remaining_100ns = local_time_100ns % (10000000ULL * 60 * 60 * 24);

    uint32_t year = 1601 + (uint32_t)(total_days / 365.25);

    uint64_t days_since_1601 = total_days;
    year = 1601;
    while (1) {
        uint32_t days_in_year = 365;

        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            days_in_year = 366;
        }

        if (days_since_1601 < days_in_year)
            break;
        days_since_1601 -= days_in_year;
        year++;
    }

    uint32_t days_in_months[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        days_in_months[1] = 29;
    }

    uint32_t month = 1;
    while (month <= 12 && days_since_1601 >= days_in_months[month - 1]) {
        days_since_1601 -= days_in_months[month - 1];
        month++;
    }

    uint32_t day = (uint32_t)days_since_1601 + 1;

    uint64_t total_seconds = remaining_100ns / 10000000ULL;
    uint32_t hours = (uint32_t)(total_seconds / 3600);
    total_seconds %= 3600;
    uint32_t minutes = (uint32_t)(total_seconds / 60);
    uint32_t seconds = (uint32_t)(total_seconds % 60);

    pal_time result = {0};
    result.year = year;
    result.month = month;
    result.day = day;
    result.weeks = 0; // Unused for system time
    result.hours = hours;
    result.minutes = minutes;
    result.seconds = seconds;

    return result;
}

PALAPI pal_time pal_get_time_since_boot(void) {
    PKUSER_SHARED_DATA kuser = (PKUSER_SHARED_DATA)KUSER_SHARED_DATA_ADDRESS;
    LARGE_INTEGER time = {0};

    do {
        time.HighPart = kuser->TickCount.High1Time;
        time.LowPart = kuser->TickCount.LowPart;
    } while (time.HighPart != kuser->TickCount.High2Time);

    uint64_t tick_ms = ((uint64_t)time.QuadPart * kuser->TickCountMultiplier) >> 24;
    uint64_t total_seconds = tick_ms / 1000;
    uint32_t total_days = (uint32_t)(total_seconds / (24 * 60 * 60));
    uint32_t remaining_seconds = (uint32_t)(total_seconds % (24 * 60 * 60));

    uint32_t years = total_days / 365;
    uint32_t remaining_days = total_days % 365;

    uint32_t leap_days = years / 4 - years / 100 + years / 400;
    if (remaining_days >= leap_days && years > 0) {
        remaining_days -= leap_days;
    }

    uint32_t months = remaining_days / 30;
    remaining_days %= 30;

    uint32_t weeks = remaining_days / 7;
    remaining_days %= 7;

    uint32_t hours = remaining_seconds / 3600;
    remaining_seconds %= 3600;
    uint32_t minutes = remaining_seconds / 60;
    uint32_t seconds = remaining_seconds % 60;

    pal_time result = {0};
    result.year = years;
    result.month = months;
    result.weeks = weeks;
    result.day = remaining_days;
    result.hours = hours;
    result.minutes = minutes;
    result.seconds = seconds;

    return result;
}

void win32_init_timer(void) {
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    g_app_start_time = counter.QuadPart;
    assert(g_app_start_time != 0);
}

PALAPI double pal_get_time_since_pal_started(void) {
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);

    uint64_t elapsed_ticks = counter.QuadPart - g_app_start_time;

    // Get frequency from KUSER_SHARED_DATA (Windows 8+) or fall back to API
    PKUSER_SHARED_DATA kuser = (PKUSER_SHARED_DATA)KUSER_SHARED_DATA_ADDRESS;
    uint64_t frequency = kuser->QpcFrequency;

    // Fallback to API if frequency is 0 (older Windows versions)
    if (frequency == 0) {
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        frequency = freq.QuadPart;
    }

    return (double)elapsed_ticks / (double)frequency;
}

PALAPI uint64_t pal_get_timer(void) {
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return counter.QuadPart;
}

// Gets the frequency of the raw timer that is used by pal, not including any time the computer
// is sleeping while pal is running.
PALAPI uint64_t pal_get_timer_frequency(void) {
    PKUSER_SHARED_DATA kuser = (PKUSER_SHARED_DATA)KUSER_SHARED_DATA_ADDRESS;
    uint64_t frequency = kuser->QpcFrequency;
    // Fallback to API if frequency is 0 (older Windows versions)
    if (frequency == 0) {
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        frequency = freq.QuadPart;
    }

    return frequency;
}
//----------------------------------------------------------------------------------
// Clip Board Functions.
//----------------------------------------------------------------------------------

PALAPI char* pal_clipboard_get(void) {
    if (!OpenClipboard(NULL))
        return NULL;

    HANDLE hData = GetClipboardData(CF_UNICODETEXT);
    if (!hData) {
        CloseClipboard();
        return NULL;
    }

    wchar_t* wtext = GlobalLock(hData);
    if (!wtext) {
        CloseClipboard();
        return NULL;
    }

    // Convert wide char text to UTF-8
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wtext, -1, NULL, 0, NULL, NULL);
    char* text = (char*)malloc(size_needed);
    if (text)
        WideCharToMultiByte(CP_UTF8, 0, wtext, -1, text, size_needed, NULL, NULL);

    GlobalUnlock(hData);
    CloseClipboard();

    return text; // caller must free()
}

PALAPI void pal_clipboard_set(const char* text) {
    if (text == NULL || *text == '\0')
        return;

    // Calculate the size of the text, including the null terminator
    size_t len = strlen(text) + 1;
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
    if (!hMem)
        return;

    // Copy the text into the allocated memory
    memcpy(GlobalLock(hMem), text, len);
    GlobalUnlock(hMem);

    // Open the clipboard and set the data
    if (OpenClipboard(NULL)) {
        EmptyClipboard();
        SetClipboardData(CF_TEXT, hMem);
        CloseClipboard();
    } else {
        GlobalFree(hMem);
    }
}

//----------------------------------------------------------------------------------
// Mouse Warp Functions.
//----------------------------------------------------------------------------------

void pal_mouse_warp(int x, int y) {
    SetCursorPos(x, y);
}

void pal_mouse_warp_relative(int dx, int dy) {
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    input.mi.dx = dx;
    input.mi.dy = dy;
    SendInput(1, &input, sizeof(INPUT));
}

//----------------------------------------------------------------------------------
// Url Launch Function.
//----------------------------------------------------------------------------------
PALAPI void pal_url_launch(char* url) {
    if (!url || !*url)
        return;

    // ShellExecuteA automatically opens the URL with the default app (e.g., browser)
    HINSTANCE result = ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);

    // Optional: check if it failed
    if ((INT_PTR)result <= 32) {
        MessageBoxA(NULL, "Failed to open URL.", "Error", MB_ICONERROR);
    }
}

//----------------------------------------------------------------------------------
// File Requester Functions.
//----------------------------------------------------------------------------------

#include <commdlg.h>

typedef struct PalRequester {
    char path[MAX_PATH];
} PalRequester;

static PalRequester g_requesters[16]; // simple static pool, indexed by `id`

static PalRequester* win32_get_requester(void* id) {
    uintptr_t index = (uintptr_t)id;
    if (index >= 16)
        return NULL;
    return &g_requesters[index];
}

static void win32_build_filter_string(char** types, uint32_t type_count, char* out, size_t out_size) {
    // Builds Windows filter string like: "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0"
    out[0] = '\0';
    size_t pos = 0;
    for (uint32_t i = 0; i < type_count; i++) {
        const char* ext = types[i];
        int written = snprintf(out + pos, out_size - pos, "%s files (*.%s)%c*.%s%c", ext, ext, '\0', ext, '\0');
        pos += written;
        if (pos >= out_size)
            break;
    }
    // Add final double null terminator
    out[pos++] = '\0';
}

void pal_create_save_dialog(char** types, uint32_t type_count, void* id) {
    PalRequester* req = win32_get_requester(id);
    if (!req)
        return;

    OPENFILENAMEA ofn = {0};
    char filter[512];
    win32_build_filter_string(types, type_count, filter, sizeof(filter));
    char path[MAX_PATH] = {0};

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = filter[0] ? filter : "All Files (*.*)\0*.*\0";
    ofn.lpstrFile = path;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
    ofn.lpstrDefExt = type_count > 0 ? types[0] : "";

    if (GetSaveFileNameA(&ofn)) {
        strcpy_s(req->path, MAX_PATH, path);
    } else {
        req->path[0] = '\0';
    }
}

void pal_create_load_dialog(char** types, uint32_t type_count, void* id) {
    PalRequester* req = win32_get_requester(id);
    if (!req)
        return;

    OPENFILENAMEA ofn = {0};
    char filter[512];
    win32_build_filter_string(types, type_count, filter, sizeof(filter));
    char path[MAX_PATH] = {0};

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = filter[0] ? filter : "All Files (*.*)\0*.*\0";
    ofn.lpstrFile = path;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
    ofn.lpstrDefExt = type_count > 0 ? types[0] : "";

    if (GetOpenFileNameA(&ofn)) {
        strcpy_s(req->path, MAX_PATH, path);
    } else {
        req->path[0] = '\0';
    }
}

char* pal_show_save_dialog(void* id) {
    PalRequester* req = win32_get_requester(id);
    return (req && req->path[0]) ? req->path : NULL;
}

char* pal_show_load_dialog(void* id) {
    PalRequester* req = win32_get_requester(id);
    return (req && req->path[0]) ? req->path : NULL;
}

//----------------------------------------------------------------------------------
// Multi-threadding functions.
//----------------------------------------------------------------------------------
PALAPI pal_mutex *pal_create_mutex() {
    pal_mutex *mutex = malloc(sizeof(*mutex));
    if (!mutex) return NULL;
    InitializeCriticalSection(&mutex->cs);
    return mutex;
}

PALAPI void pal_lock_mutex(pal_mutex *mutex) {
    EnterCriticalSection(&mutex->cs);
}

PALAPI pal_bool pal_lock_mutex_try(pal_mutex *mutex) {
    return TryEnterCriticalSection(&mutex->cs) ? 1 : 0;
}

PALAPI void pal_unlock_mutex(pal_mutex *mutex) {
    LeaveCriticalSection(&mutex->cs);
}

PALAPI void pal_destroy_mutex(pal_mutex *mutex) {
    DeleteCriticalSection(&mutex->cs);
    free(mutex);
}

PALAPI pal_signal *pal_create_signal(void) {
    // Manual-reset event, initially non-signaled
    return (pal_signal *)CreateEventW(NULL, TRUE, FALSE, NULL);
}

PALAPI pal_bool pal_wait_for_signal(pal_signal *signal, pal_mutex *mutex) {
    if (!signal)
        return pal_false;

    // Release the mutex so other threads can activate the signal
    if (mutex)
        pal_unlock_mutex(mutex);

    // Wait for the signal to be activated
    DWORD result = WaitForSingleObject((HANDLE)signal, INFINITE);

    // Reacquire the mutex before returning
    if (mutex)
        pal_lock_mutex(mutex);

    return (result == WAIT_OBJECT_0);
}

PALAPI pal_bool pal_activate_signal(pal_signal *signal) {
    if (!signal)
        return pal_false;

    return SetEvent((HANDLE)signal) ? pal_true : pal_false;
}

PALAPI pal_bool pal_deactivate_signal(pal_signal *signal) {
    if (!signal)
        return pal_false;

    return ResetEvent((HANDLE)signal) ? pal_true : pal_false;
}

PALAPI void pal_destroy_signal(pal_signal *signal) {
    if (signal)
        CloseHandle((HANDLE)signal);
}

typedef struct {
    pal_thread_func func;
    void *arg;
} thread_wrapper_arg;

// Wrapper to adapt pal_thread_func to Windows signature
DWORD WINAPI thread_wrapper(LPVOID param) {
    thread_wrapper_arg *wrapper = (thread_wrapper_arg *)param;
    wrapper->func(wrapper->arg);
    HeapFree(GetProcessHeap(), 0, wrapper);
    return 0;
}

PALAPI pal_thread *pal_create_thread(pal_thread_func func, void *arg) {
    thread_wrapper_arg *wrapper = (thread_wrapper_arg *)HeapAlloc(GetProcessHeap(), 0, sizeof(thread_wrapper_arg));
    if (!wrapper) return NULL;
    wrapper->func = func;
    wrapper->arg = arg;

    HANDLE thread = CreateThread(NULL, 0, thread_wrapper, wrapper, CREATE_SUSPENDED, NULL);
    return (pal_thread *)thread;
}

PALAPI pal_bool pal_start_thread(pal_thread *thread) {
    if (!thread) return pal_false;
    return ResumeThread((HANDLE)thread) != (DWORD)-1;
}

PALAPI pal_bool pal_join_thread(pal_thread *thread) {
    if (!thread) return pal_false;
    return WaitForSingleObject((HANDLE)thread, INFINITE) == WAIT_OBJECT_0;
}

PALAPI void pal_destroy_thread(pal_thread *thread) {
    if (thread) CloseHandle((HANDLE)thread);
}

//----------------------------------------------------------------------------------
// Dynamic Library Functions.
//----------------------------------------------------------------------------------
PALAPI void* pal_load_dynamic_library(const char* dll) {
    HMODULE result = LoadLibraryA(dll);
    assert(result);
    return (void*)result;
}

PALAPI void* pal_load_dynamic_function(void* dll, char* func_name) {
    FARPROC proc = GetProcAddress(dll, func_name);
    assert(proc);
    return (void*)proc;
}

PALAPI pal_bool pal_free_dynamic_library(void* dll) {
    pal_bool free_result = FreeLibrary(dll);
    assert(free_result);
    return (pal_bool)free_result;
}

PALAPI void pal_init(void) {
    pal__init_eventq();
    win32_enumerate_keyboards();
    win32_enumerate_mice();
    win32_init_timer();
    win32_init_sound();
    if (!win32_init_gamepads()) {
        printf("ERROR: %s: win32_init_gamepads failed\n", __func__);
    }
}

PALAPI void pal_shutdown(void) {
    win32_shutdown_gamepads();
    pal__eventq_free(g_event_queue);
}

#endif // WIN32_PLATFORM_H

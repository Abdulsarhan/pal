#ifndef WIN32_PLATFORM_H
#define WIN32_PLATFORM_H

// Windows system headers
#include <Windows.h>
#include <windowsx.h>         // Useful macros (e.g., GET_X_LPARAM)
#include <xaudio2.h>          // XAudio2
#include <mmreg.h>            // WAVEFORMATEX
#include <Xinput.h>
#include <hidsdi.h>   // Link with hid.lib

// C Standard Library
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

// OpenGL
#include <gl/gl.h>
#include <GL/glext.h>
#include <GL/wglext.h>

// Windows Gaming Input (For Xbone / Series S Controllers)
#define COBJMACROS
#include <roapi.h>
#include <winstring.h>

#include "pal_platform.h"

typedef unsigned __int64 QWORD;

static HDC s_fakeDC = { 0 };

IXAudio2* g_xaudio2 = NULL;
IXAudio2MasteringVoice* g_mastering_voice = NULL;

struct pal_window {
	uint32_t id;
	HWND hwnd;
	HDC hdc;
	HGLRC hglrc;
    DWORD windowedStyle;
    RECT windowedRect;
    pal_event_queue queue;
    pal_bool message_pump_drained;
};

struct pal_monitor {
	HMONITOR handle;
};

struct pal_sound {
    // Core audio data
    unsigned char* data;        // Raw PCM audio data (initial buffer)
    uint32_t data_size;        // Size in bytes of initial buffer
    int sample_rate;           // Samples per second (e.g., 44100)
    int channels;              // Number of audio channels (e.g., 2 for stereo)
    int bits_per_sample;       // Usually 16 or 32
    int is_float;              // 0 = PCM, 1 = IEEE float
    
    // XAudio2
    IXAudio2SourceVoice* source_voice;
    IXAudio2VoiceCallback* voice_callback;
    
    // Streaming - OGG
    void* decoder;             // stb_vorbis* (using void* to avoid header dependency)
    
    // Streaming - WAV
    FILE* source_file;
    uint32_t total_data_size;  // Total size of audio data in file
    uint32_t bytes_streamed;   // How many bytes we've read so far
    uint32_t data_offset;      // Offset in file where audio data starts
    
    // Streaming control
    float preload_seconds;     // How many seconds were preloaded
    int is_streaming;          // 1 if this is a streaming sound
    int stream_finished;       // 1 when streaming is complete
};

// Keyboard & Mouse Input

#define MAX_KEYS 256
#define MAX_MOUSEBUTTONS 32
#define MAX_XINPUT_CONTROLLERS 4
#define PAL_MAX_GAMEPADS 16
#define PAL_MAX_BUTTONS 32
#define PAL_MAX_AXES 16
#define PAL_MAX_MAPPINGS 256

typedef struct pal_input {
	uint8_t keys[MAX_KEYS];
	uint8_t keys_processed[MAX_KEYS];
	uint8_t mouse_buttons[MAX_MOUSEBUTTONS];
	uint8_t mouse_buttons_processed[MAX_MOUSEBUTTONS];
	v2 mouse_position;
	v2 mouse_delta;
}pal_input;
pal_input input = { 0 };

static const uint8_t win32_button_to_pal_button[] = {
    [0] = PAL_MOUSE_LEFT,
    [1] = PAL_MOUSE_LEFT,
    [2] = PAL_MOUSE_LEFT,

    [3] = PAL_MOUSE_RIGHT,
    [4] = PAL_MOUSE_RIGHT,
    [5] = PAL_MOUSE_RIGHT,

    [6] = PAL_MOUSE_MIDDLE,
    [7] = PAL_MOUSE_MIDDLE,
    [8] = PAL_MOUSE_MIDDLE,

    [9]  = PAL_MOUSE_4,
    [10] = PAL_MOUSE_5,
};

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

static const uint16_t win32_scancode_to_pal[256] = {
	[0x01] = PAL_SCAN_ESCAPE,
	[0x02] = PAL_SCAN_1,
	[0x03] = PAL_SCAN_2,
	[0x04] = PAL_SCAN_3,
	[0x05] = PAL_SCAN_4,
	[0x06] = PAL_SCAN_5,
	[0x07] = PAL_SCAN_6,
	[0x08] = PAL_SCAN_7,
	[0x09] = PAL_SCAN_8,
	[0x0A] = PAL_SCAN_9,
	[0x0B] = PAL_SCAN_0,
	[0x0C] = PAL_SCAN_MINUS,
	[0x0D] = PAL_SCAN_EQUALS,
	[0x0E] = PAL_SCAN_BACKSPACE,
	[0x0F] = PAL_SCAN_TAB,
	[0x10] = PAL_SCAN_Q,
	[0x11] = PAL_SCAN_W,
	[0x12] = PAL_SCAN_E,
	[0x13] = PAL_SCAN_R,
	[0x14] = PAL_SCAN_T,
	[0x15] = PAL_SCAN_Y,
	[0x16] = PAL_SCAN_U,
	[0x17] = PAL_SCAN_I,
	[0x18] = PAL_SCAN_O,
	[0x19] = PAL_SCAN_P,
	[0x1A] = PAL_SCAN_LEFTBRACKET,
	[0x1B] = PAL_SCAN_RIGHTBRACKET,
	[0x1C] = PAL_SCAN_RETURN,
	[0x1D] = PAL_SCAN_LCTRL,
	[0x1E] = PAL_SCAN_A,
	[0x1F] = PAL_SCAN_S,
	[0x20] = PAL_SCAN_D,
	[0x21] = PAL_SCAN_F,
	[0x22] = PAL_SCAN_G,
	[0x23] = PAL_SCAN_H,
	[0x24] = PAL_SCAN_J,
	[0x25] = PAL_SCAN_K,
	[0x26] = PAL_SCAN_L,
	[0x27] = PAL_SCAN_SEMICOLON,
	[0x28] = PAL_SCAN_APOSTROPHE,
	[0x29] = PAL_SCAN_GRAVE,
	[0x2A] = PAL_SCAN_LSHIFT,
	[0x2B] = PAL_SCAN_BACKSLASH,
	[0x2C] = PAL_SCAN_Z,
	[0x2D] = PAL_SCAN_X,
	[0x2E] = PAL_SCAN_C,
	[0x2F] = PAL_SCAN_V,
	[0x30] = PAL_SCAN_B,
	[0x31] = PAL_SCAN_N,
	[0x32] = PAL_SCAN_M,
	[0x33] = PAL_SCAN_COMMA,
	[0x34] = PAL_SCAN_PERIOD,
	[0x35] = PAL_SCAN_SLASH,
	[0x36] = PAL_SCAN_RSHIFT,
	[0x37] = PAL_SCAN_KP_MULTIPLY,
	[0x38] = PAL_SCAN_LALT,
	[0x39] = PAL_SCAN_SPACE,
	[0x3A] = PAL_SCAN_CAPSLOCK,
	[0x3B] = PAL_SCAN_F1,
	[0x3C] = PAL_SCAN_F2,
	[0x3D] = PAL_SCAN_F3,
	[0x3E] = PAL_SCAN_F4,
	[0x3F] = PAL_SCAN_F5,
	[0x40] = PAL_SCAN_F6,
	[0x41] = PAL_SCAN_F7,
	[0x42] = PAL_SCAN_F8,
	[0x43] = PAL_SCAN_F9,
	[0x44] = PAL_SCAN_F10,
	[0x45] = PAL_SCAN_NUMCLEAR,
	[0x46] = PAL_SCAN_SCROLLLOCK,
	[0x47] = PAL_SCAN_KP_7,
	[0x48] = PAL_SCAN_KP_8,
	[0x49] = PAL_SCAN_KP_9,
	[0x4A] = PAL_SCAN_KP_MINUS,
	[0x4B] = PAL_SCAN_KP_4,
	[0x4C] = PAL_SCAN_KP_5,
	[0x4D] = PAL_SCAN_KP_6,
	[0x4E] = PAL_SCAN_KP_PLUS,
	[0x4F] = PAL_SCAN_KP_1,
	[0x50] = PAL_SCAN_KP_2,
	[0x51] = PAL_SCAN_KP_3,
	[0x52] = PAL_SCAN_KP_0,
	[0x53] = PAL_SCAN_KP_PERIOD,
	[0x57] = PAL_SCAN_F11,
	[0x58] = PAL_SCAN_F12,
};

static const uint16_t win32_scancode_e0_to_pal[256] = {
	[0x1C] = PAL_SCAN_KP_ENTER,
	[0x1D] = PAL_SCAN_RCTRL,
	[0x35] = PAL_SCAN_KP_DIVIDE,
	[0x38] = PAL_SCAN_RALT,
	[0x47] = PAL_SCAN_HOME,
	[0x48] = PAL_SCAN_UP,
	[0x49] = PAL_SCAN_PAGEUP,
	[0x4B] = PAL_SCAN_LEFT,
	[0x4D] = PAL_SCAN_RIGHT,
	[0x4F] = PAL_SCAN_END,
	[0x50] = PAL_SCAN_DOWN,
	[0x51] = PAL_SCAN_PAGEDOWN,
	[0x52] = PAL_SCAN_INSERT,
	[0x53] = PAL_SCAN_DELETE,
	[0x5B] = PAL_SCAN_LGUI,
	[0x5C] = PAL_SCAN_RGUI,
	[0x5D] = PAL_SCAN_APPLICATION,
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
typedef struct hid_device{
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

}hid_device;

#pragma pack(push, 1)
typedef struct {
    uint16_t idReserved;   // Must be 0
    uint16_t idType;       // Must be 1 for icons
    uint16_t idCount;      // Number of images
} ICONDIR;

typedef struct {
    uint8_t  bWidth;        // Width in pixels
    uint8_t  bHeight;       // Height in pixels
    uint8_t  bColorCount;   // 0 if >= 8bpp
    uint8_t  bReserved;     // Must be 0
    uint16_t wPlanes;       // Should be 1
    uint16_t wBitCount;     // Usually 32
    uint32_t dwBytesInRes;  // Size of PNG data
    uint32_t dwImageOffset; // Offset to PNG data (after header)
} ICONDIRENTRY;
#pragma pack(pop)

pal_bool platform_make_window_fullscreen(pal_window* window) {
    window->windowedStyle = GetWindowLongA(window->hwnd, GWL_STYLE);
    GetWindowRect(window->hwnd, &window->windowedRect);

    DEVMODE dm = {0};
    dm.dmSize = sizeof(dm);

    HMONITOR monitor = MonitorFromWindow(window->hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFOEXA mi = { .cbSize = sizeof(mi) };
    if (!GetMonitorInfoA(monitor, (MONITORINFO*)&mi)) {
        MessageBoxA(window->hwnd, "Failed to get monitor info.", "Error", MB_OK);
        return FALSE;
    }

    if (!EnumDisplaySettingsA(mi.szDevice, ENUM_CURRENT_SETTINGS, &dm)) {
        MessageBoxA(window->hwnd, "Failed to get current monitor settings.", "Error", MB_OK);
        return FALSE;
    }

    dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;

    if (ChangeDisplaySettingsExA(NULL, &dm, NULL, CDS_FULLSCREEN, NULL) != DISP_CHANGE_SUCCESSFUL) {
        MessageBoxA(window->hwnd, "Failed to switch display mode", "Error", MB_OK);
        return FALSE;
    }

    SetWindowLongA(window->hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
    SetWindowPos(window->hwnd, HWND_TOP, 0, 0,
        dm.dmPelsWidth, dm.dmPelsHeight,
        SWP_FRAMECHANGED | SWP_NOOWNERZORDER);

    return TRUE;
}
pal_bool platform_make_window_fullscreen_ex(pal_window* window, int width, int height, int refreshRate) {
    window->windowedStyle = GetWindowLongA(window->hwnd, GWL_STYLE);
    GetWindowRect(window->hwnd, &window->windowedRect);

    DEVMODE dm = {0};
    dm.dmSize = sizeof(dm);
    dm.dmPelsWidth = width;
    dm.dmPelsHeight = height;
    dm.dmBitsPerPel = 32;
    dm.dmDisplayFrequency = refreshRate;
    dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;

    if (ChangeDisplaySettingsExA(NULL, &dm, NULL, CDS_FULLSCREEN, NULL) != DISP_CHANGE_SUCCESSFUL) {
        MessageBoxA(window->hwnd, "Failed to switch display mode", "Error", MB_OK);
        return FALSE;
    }

    SetWindowLongA(window->hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
    SetWindowPos(window->hwnd, HWND_TOP, 0, 0,
        width, height,
        SWP_FRAMECHANGED | SWP_NOOWNERZORDER);

    return TRUE;
}
pal_bool platform_make_window_fullscreen_windowed(pal_window* window) {
    // Save the current window style and rect
    window->windowedStyle = GetWindowLongA(window->hwnd, GWL_STYLE);
    GetWindowRect(window->hwnd, &window->windowedRect);

    // Get the monitor bounds
    HMONITOR monitor = MonitorFromWindow(window->hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO mi = { .cbSize = sizeof(mi) };
    if (!GetMonitorInfo(monitor, &mi)) {
        MessageBoxA(window->hwnd, "Failed to get monitor info.", "Error", MB_OK);
        return FALSE;
    }

    // Set the window to borderless fullscreen
    SetWindowLongA(window->hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
    if (!SetWindowPos(window->hwnd, HWND_TOP,
        mi.rcMonitor.left, mi.rcMonitor.top,
        mi.rcMonitor.right - mi.rcMonitor.left,
        mi.rcMonitor.bottom - mi.rcMonitor.top,
        SWP_FRAMECHANGED | SWP_NOOWNERZORDER)) {
        MessageBoxA(window->hwnd, "Failed to resize window.", "Error", MB_OK);
        return FALSE;
    }

    return TRUE;
}
pal_bool platform_make_window_windowed(pal_window* window) {
    // Restore display mode (in case exclusive mode was used)
    ChangeDisplaySettings(NULL, 0);

    // Restore the window style
    if (SetWindowLongA(window->hwnd, GWL_STYLE, window->windowedStyle) == 0) {
        MessageBoxA(window->hwnd, "Failed to restore window style.", "Error", MB_OK);
        return FALSE;
    }

    // Restore the window's size and position
    if (!SetWindowPos(window->hwnd, NULL,
        window->windowedRect.left, window->windowedRect.top,
        window->windowedRect.right - window->windowedRect.left,
        window->windowedRect.bottom - window->windowedRect.top,
        SWP_NOZORDER | SWP_FRAMECHANGED)) {
        MessageBoxA(window->hwnd, "Failed to restore window position.", "Error", MB_OK);
        return FALSE;
    }

    return TRUE;
}

void platform_set_cursor(pal_window *window, const char *filepath, int size) {
    // Clamp size to reasonable max, e.g. 256 (you can adjust)
    if (size <= 0) size = 32;
    if (size > 256) size = 256;

    FILE *f = fopen(filepath, "rb");
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
    }
    else if (header[0] == 0x00 && header[1] == 0x00 && header[2] == 0x02 && header[3] == 0x00) {
        hCursor = (HCURSOR)LoadImageA(NULL, filepath, IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);
        if (!hCursor) {
            MessageBoxA(window->hwnd, "Failed to load .cur cursor.", "SetCustomCursor Error", MB_ICONERROR);
            return;
        }
    }
    else {
        int width, height, channels;
        unsigned char *pixels = stbi_load(filepath, &width, &height, &channels, 4);
        if (!pixels) {
            MessageBoxA(window->hwnd, "Failed to load image with stb_image.", "SetCustomCursor Error", MB_ICONERROR);
            return;
        }

        unsigned char *resized = malloc(size * size * 4);
        if (!resized) {
            stbi_image_free(pixels);
            MessageBoxA(window->hwnd, "Failed to allocate memory for resized image.", "SetCustomCursor Error", MB_ICONERROR);
            return;
        }

        stbir_resize_uint8_srgb(
            pixels, width, height, width * 4,
            resized, size, size, size * 4,
            STBIR_RGBA
        );
        stbi_image_free(pixels);

        HDC hdc = GetDC(NULL);

        BITMAPV5HEADER bi = {0};
        bi.bV5Size = sizeof(BITMAPV5HEADER);
        bi.bV5Width = size;
        bi.bV5Height = -size;
        bi.bV5Planes = 1;
        bi.bV5BitCount = 32;
        bi.bV5Compression = BI_BITFIELDS;
        bi.bV5RedMask   = 0x00FF0000;
        bi.bV5GreenMask = 0x0000FF00;
        bi.bV5BlueMask  = 0x000000FF;
        bi.bV5AlphaMask = 0xFF000000;

        void *bitmapData = NULL;
        HBITMAP hBitmap = CreateDIBSection(hdc, (BITMAPINFO *)&bi, DIB_RGB_COLORS, &bitmapData, NULL, 0);
        ReleaseDC(NULL, hdc);

        if (!hBitmap || !bitmapData) {
            free(resized);
            MessageBoxA(window->hwnd, "Failed to create DIB section.", "SetCustomCursor Error", MB_ICONERROR);
            return;
        }

        for (int y = 0; y < size; ++y) {
            for (int x = 0; x < size; ++x) {
                unsigned char *src = &resized[(y * size + x) * 4];
                unsigned char *dst = (unsigned char *)bitmapData + (y * size + x) * 4;
                dst[0] = src[2]; // B
                dst[1] = src[1]; // G
                dst[2] = src[0]; // R
                dst[3] = src[3]; // A
            }
        }

        free(resized);

        ICONINFO ii = {0};
        ii.fIcon = FALSE;
        ii.xHotspot = 0;
        ii.yHotspot = 0;
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
static HICON load_icon_from_file(const char* image_path, BOOL legacy) {
    FILE* file = fopen(image_path, "rb");
    if (!file) return NULL;

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
            TRUE,
            0x00030000,
            0, 0,
            LR_DEFAULTCOLOR
        );

        free(ico_data);
        return hIcon;
    }

    // Fallback: decode image with stb_image (PNG in legacy mode, JPEG, BMP)
    fclose(file);
    int width, height, channels;
    uint8_t* rgba = stbi_load(image_path, &width, &height, &channels, 4);
    if (!rgba) return NULL;

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
    bi.bV5RedMask   = 0x00FF0000;
    bi.bV5GreenMask = 0x0000FF00;
    bi.bV5BlueMask  = 0x000000FF;
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
    ii.fIcon = TRUE;
    ii.hbmMask = mask_bitmap;
    ii.hbmColor = color_bitmap;

    HICON hIcon = CreateIconIndirect(&ii);

    DeleteObject(color_bitmap);
    DeleteObject(mask_bitmap);

    return hIcon;
}

int platform_translate_message(MSG msg, pal_window* window) {
    pal_event event;
    // test WM_QUIT, WM_DESTORY, and WM_CLOSE
    switch (msg.message) {
        case WM_DESTROY:
            PostQuitMessage(0);
        case WM_QUIT:
        case WM_CLOSE:
            event.type = PAL_QUIT;
            event.quit = (pal_quit_event){ .code = 0 };
            break;
		case WM_MOVE:
			event.type = PAL_WINDOW_EVENT;
			event.window = (pal_window_event){
				.windowid = window->id,
				.event_code = WM_MOVE,
				.x = LOWORD(msg.lParam),
				.y = HIWORD(msg.lParam),
				.width = 0,
				.height = 0,
				.focused = 1,
				.visible = 1
			};
			break;
		case WM_SIZE:
			event.type = PAL_WINDOW_EVENT;
			event.window = (pal_window_event){
				.windowid = window->id,
				.event_code = WM_SIZE,
				.x = 0,
				.y = 0,
				.width = LOWORD(msg.lParam),
				.height = HIWORD(msg.lParam),
				.focused = 1,
				.visible = 1
			};
			break;
        case WM_WINDOWPOSCHANGED:
        case WM_WINDOWPOSCHANGING:
            event.type = PAL_WINDOW_EVENT;
            WINDOWPOS* pos = (WINDOWPOS*)msg.lParam;
            event.window = (pal_window_event){
                .windowid = window->id,
                .event_code = msg.message,
                .x = pos->x,
                .y = pos->y,
                .width = pos->cx,
                .height = pos->cy,
                .focused = 1, // guess; could adjust later
                .visible = 1
            };
            break;

        case WM_MOUSEMOVE:
            event.type = PAL_MOUSE_MOTION;
            event.motion = (pal_mouse_motion_event){
                .x = GET_X_LPARAM(msg.lParam),
                .y = GET_Y_LPARAM(msg.lParam),
                .delta_x = input.mouse_delta.x, // this should be assigned when we get raw input from the mouse.
                .delta_y = input.mouse_delta.y,
                .buttons = msg.wParam
            };
            break;

        case WM_LBUTTONDOWN: 
        case WM_RBUTTONDOWN: 
        case WM_MBUTTONDOWN: 
        case WM_XBUTTONDOWN: {
            event.type = PAL_MOUSE_BUTTON_DOWN;
            event.button = (pal_mouse_button_event){
                .x = GET_X_LPARAM(msg.lParam),
                .y = GET_Y_LPARAM(msg.lParam),
                .pressed = 1,
                .clicks = 1,
                .modifiers = msg.wParam,
                .button = win32_button_to_pal_button[msg.message - WM_LBUTTONDOWN]
            };

            if (msg.message == WM_XBUTTONDOWN) {
                WORD xButton = GET_XBUTTON_WPARAM(msg.wParam);
                if (xButton == XBUTTON1) {
                    event.button.button = PAL_MOUSE_4;
                    input.mouse_buttons[PAL_MOUSE_4] = 1;
                } else if (xButton == XBUTTON2) {
                    event.button.button = PAL_MOUSE_5;
                    input.mouse_buttons[PAL_MOUSE_5] = 1;
                }
            } else {
                input.mouse_buttons[event.button.button] = 1;
            }
        } break;

        case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDBLCLK:
        case WM_XBUTTONDBLCLK: {
            event.type = PAL_MOUSE_BUTTON_DOWN;
            event.button = (pal_mouse_button_event){
                .x = GET_X_LPARAM(msg.lParam),
                .y = GET_Y_LPARAM(msg.lParam),
                .pressed = 1,
                .clicks = 2,
                .modifiers = msg.wParam,
                .button = win32_button_to_pal_button[msg.message - WM_LBUTTONDOWN]
            };

            if (msg.message == WM_XBUTTONDBLCLK) {
                WORD xButton = GET_XBUTTON_WPARAM(msg.wParam);
                if (xButton == XBUTTON1) {
                    event.button.button = PAL_MOUSE_4;
                    input.mouse_buttons[PAL_MOUSE_4] = 1;
                } else if (xButton == XBUTTON2) {
                    event.button.button = PAL_MOUSE_5;
                    input.mouse_buttons[PAL_MOUSE_5] = 1;
                }
            } else {
                input.mouse_buttons[event.button.button] = 1;
            }
        } break;

        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        case WM_XBUTTONUP: {
            event.type = PAL_MOUSE_BUTTON_UP;
            event.button = (pal_mouse_button_event){
                .x = GET_X_LPARAM(msg.lParam),
                .y = GET_Y_LPARAM(msg.lParam),
                .pressed = 0,
                .modifiers = msg.wParam,
                .button = win32_button_to_pal_button[msg.message - WM_LBUTTONDOWN]
            };

            if (msg.message == WM_XBUTTONUP) {
                WORD xButton = GET_XBUTTON_WPARAM(msg.wParam);
                if (xButton == XBUTTON1) {
                    event.button.button = PAL_MOUSE_4;
                    input.mouse_buttons[PAL_MOUSE_4] = 0;
                    input.mouse_buttons_processed[PAL_MOUSE_4] = 0;
                } else if (xButton == XBUTTON2) {
                    event.button.button = PAL_MOUSE_5;
                    input.mouse_buttons[PAL_MOUSE_5] = 0;
                    input.mouse_buttons_processed[PAL_MOUSE_5] = 0;
                }
            } else {
                input.mouse_buttons[event.button.button] = 0;
                input.mouse_buttons_processed[event.button.button] = 0;
            }
        } break;

        case WM_MOUSEWHEEL:
        case WM_MOUSEHWHEEL: {
            int delta = GET_WHEEL_DELTA_WPARAM(msg.wParam);
            event.type = PAL_MOUSE_WHEEL;
            event.wheel = (pal_mouse_wheel_event){
                .x = GET_X_LPARAM(msg.lParam),
                .y = GET_Y_LPARAM(msg.lParam),
                .delta_x = (msg.message == WM_MOUSEHWHEEL) ? (float)delta / WHEEL_DELTA : 0.0f,
                .delta_y = (msg.message == WM_MOUSEWHEEL) ? (float)delta / WHEEL_DELTA : 0.0f,
                .modifiers = GET_KEYSTATE_WPARAM(msg.wParam)
            };
            break;
        }
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN: {
			uint32_t vk = (uint32_t)msg.wParam;
			uint8_t scancode = (msg.lParam >> 16) & 0xFF;
			uint8_t extended = (msg.lParam >> 24) & 1;

			uint16_t modifiers = 0;

			switch (vk) {
				case VK_SHIFT:
					if (scancode == MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC))
						modifiers |= PAL_MOD_LSHIFT;
					else
						modifiers |= PAL_MOD_RSHIFT;
					break;

				case VK_CONTROL:
					modifiers |= extended ? PAL_MOD_RCTRL : PAL_MOD_LCTRL;
					break;

				case VK_MENU:
					modifiers |= extended ? PAL_MOD_RALT : PAL_MOD_LALT;
					break;

				case VK_LWIN:
					modifiers |= PAL_MOD_LWINDOWS;
					break;

				case VK_RWIN:
					modifiers |= PAL_MOD_RWINDOWS;
					break;

				case VK_NUMLOCK:
					modifiers |= PAL_MOD_NUM;
					break;

				case VK_SCROLL:
					modifiers |= PAL_MOD_SCROLL;
					break;

				case VK_PROCESSKEY:
					modifiers |= PAL_MOD_ALTGR;
					break;

				case VK_SHIFT | 0x100: // Level 5 shift (user-defined)
					modifiers |= PAL_MOD_LEVEL5SHIFT;
					break;
			}

			// Check toggle states:
			if ((GetKeyState(VK_NUMLOCK) & 1) != 0)   modifiers |= PAL_MOD_NUM;
			if ((GetKeyState(VK_SCROLL) & 1) != 0)    modifiers |= PAL_MOD_SCROLL;
			if ((GetKeyState(VK_CAPITAL) & 1) != 0)   modifiers |= PAL_MOD_CAPS;

			// AltGr detection: Right Alt + Ctrl
			if ((GetKeyState(VK_RMENU) & 0x8000) && (GetKeyState(VK_CONTROL) & 0x8000)) {
				modifiers |= PAL_MOD_ALTGR;
			}

            uint16_t pal_scancode = extended ? win32_scancode_e0_to_pal[scancode] : win32_scancode_to_pal[scancode];

			event.type = PAL_KEY_DOWN;
			event.key = (pal_keyboard_event){
				.virtual_key = win32_key_to_pal_key[vk],
				.scancode = pal_scancode,
				.pressed = 1,
				.repeat = (msg.lParam >> 30) & 1,
				.modifiers = modifiers
			};
			input.keys[win32_key_to_pal_key[vk]] = 1;
			break;
		}

		case WM_KEYUP:
		case WM_SYSKEYUP: {
			uint32_t vk = (uint32_t)msg.wParam;
			uint8_t scancode = (msg.lParam >> 16) & 0xFF;
			uint8_t extended = (msg.lParam >> 24) & 1;

			uint16_t modifiers = 0;

			switch (vk) {
				case VK_SHIFT:
					if (scancode == MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC))
						modifiers |= PAL_MOD_LSHIFT;
					else
						modifiers |= PAL_MOD_RSHIFT;
					break;

				case VK_CONTROL:
					modifiers |= extended ? PAL_MOD_RCTRL : PAL_MOD_LCTRL;
					break;

				case VK_MENU:
					modifiers |= extended ? PAL_MOD_RALT : PAL_MOD_LALT;
					break;

				case VK_LWIN:
					modifiers |= PAL_MOD_LWINDOWS;
					break;

				case VK_RWIN:
					modifiers |= PAL_MOD_RWINDOWS;
					break;

				case VK_NUMLOCK:
					modifiers |= PAL_MOD_NUM;
					break;

				case VK_SCROLL:
					modifiers |= PAL_MOD_SCROLL;
					break;

				case VK_PROCESSKEY:
					modifiers |= PAL_MOD_ALTGR;
					break;

				case VK_SHIFT | 0x100:
					modifiers |= PAL_MOD_LEVEL5SHIFT;
					break;
			}

			if ((GetKeyState(VK_NUMLOCK) & 1) != 0)   modifiers |= PAL_MOD_NUM;
			if ((GetKeyState(VK_SCROLL) & 1) != 0)    modifiers |= PAL_MOD_SCROLL;
			if ((GetKeyState(VK_CAPITAL) & 1) != 0)   modifiers |= PAL_MOD_CAPS;

			if ((GetKeyState(VK_RMENU) & 0x8000) && (GetKeyState(VK_CONTROL) & 0x8000)) {
				modifiers |= PAL_MOD_ALTGR;
			}

            uint16_t pal_scancode = extended ? win32_scancode_e0_to_pal[scancode] : win32_scancode_to_pal[scancode];

			event.type = PAL_KEY_UP;
			event.key = (pal_keyboard_event){
				.virtual_key = win32_key_to_pal_key[vk],
				.scancode = pal_scancode,
				.pressed = 0,
				.repeat = 0,
				.modifiers = modifiers
			};
			input.keys[win32_key_to_pal_key[vk]] = 0;
			input.keys_processed[win32_key_to_pal_key[vk]] = 0;
			break;
		}
        case WM_CHAR:
        case WM_UNICHAR:
            event.type = PAL_TEXT_INPUT;
            event.text = (pal_text_input_event){
                .utf8_text = {0}
            };
            {
                char utf8[8] = {0};
                int len = WideCharToMultiByte(CP_UTF8, 0, (WCHAR*)&msg.wParam, 1, utf8, sizeof(utf8), NULL, NULL);
                memcpy(event.text.utf8_text, utf8, len);
            }
            break;

        case WM_INPUT:
            event.type = PAL_SENSOR_UPDATE;
            event.sensor = (pal_sensor_event){
                .device_id = 0,
                .x = 0, .y = 0, .z = 0,
                .sensor_type = 0
            };
            break;

        case WM_DROPFILES: {
            event.type = PAL_DROP_FILE;
            HDROP hDrop = (HDROP)msg.wParam;
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
                .count = count
            };
            DragFinish(hDrop);
            break;
        }

        default:
            event.type = PAL_NONE;
            DispatchMessage(&msg);
            break;
    }

    pal_event_queue* queue = &window->queue;
    if (queue->size == queue->capacity) {
        fprintf(stderr, "ERROR: pal_eventq_enqueue(): Event queue size has reached capacity. Not going to enqueue.\n");
        return;
    }
    queue->events[queue->back] = event;
    queue->back = (queue->back + 1) % queue->capacity;
    queue->size++;
    return 0;
}

void platform_set_window_icon(pal_window* window, const char* image_path) {
    HICON hIcon = load_icon_from_file(image_path, FALSE);
    if (hIcon) {
        SendMessage(window->hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        SendMessage(window->hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    } else {
        MessageBoxA(window->hwnd, "Failed to load window icon", "Error", MB_OK | MB_ICONERROR);
    }
}

void platform_set_window_icon_legacy(pal_window* window, const char* image_path) {
    HICON hIcon = load_icon_from_file(image_path, TRUE);
    if (hIcon) {
        SendMessage(window->hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        SendMessage(window->hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    } else {
        MessageBoxA(window->hwnd, "Failed to load window icon", "Error", MB_OK | MB_ICONERROR);
    }
}

void platform_set_taskbar_icon(pal_window* window, const char* image_path) {
    HICON hIcon = load_icon_from_file(image_path, FALSE);
    if (hIcon) {
        SetClassLongPtr(window->hwnd, GCLP_HICONSM, (LONG_PTR)hIcon);
        SetClassLongPtr(window->hwnd, GCLP_HICON,   (LONG_PTR)hIcon);
    } else {
        MessageBoxA(window->hwnd, "Failed to load taskbar icon", "Error", MB_OK | MB_ICONERROR);
    }
}

void platform_set_taskbar_icon_legacy(pal_window* window, const char* image_path) {
    HICON hIcon = load_icon_from_file(image_path, TRUE);
    if (hIcon) {
        SetClassLongPtr(window->hwnd, GCLP_HICONSM, (LONG_PTR)hIcon);
        SetClassLongPtr(window->hwnd, GCLP_HICON,   (LONG_PTR)hIcon);
    } else {
        MessageBoxA(window->hwnd, "Failed to load legacy taskbar icon", "Error", MB_OK | MB_ICONERROR);
    }
}

LRESULT CALLBACK win32_fake_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
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

// --- COM interface IDs (IIDs) ---

static const IID IID_IGamepadStatics = { 0x6a44c408, 0x6ff0, 0x46ab, {0xb6, 0x22, 0xd9, 0x18, 0x19, 0x7b, 0x03, 0x23} };
static const IID IID_IGamepad = {0x3C1689BD, 0x5915, 0x4489, 0x83, 0x13, 0xF6, 0xE3, 0xFC, 0xBF, 0xE7, 0x9C};
static const IID IID_IGamepad2 = { 0x0e2b57cc, 0x4a5d, 0x4b66, {0xb6, 0xbb, 0x38, 0xbb, 0x8b, 0x3c, 0xb7, 0x0a} };
static const IID IID_IVectorView = { 0xbbe1fa4c, 0x7110, 0x4af6, {0xba, 0x5b, 0xd8, 0x2f, 0xe0, 0x9f, 0x0c, 0x1f} };

// --- COM interfaces (IGamepad, IGamepadStatics, IVectorView) ---

typedef struct IGamepad IGamepad;
typedef struct IGamepadVtbl IGamepadVtbl;
typedef struct IGamepadStatics IGamepadStatics;
typedef struct IGamepadStaticsVtbl IGamepadStaticsVtbl;
typedef struct IVectorView IVectorView;
typedef struct IVectorViewVtbl IVectorViewVtbl;

static IGamepadStatics* g_statics = NULL;
typedef struct GamepadReading {
    uint16_t LeftThumbstickX;
    uint16_t LeftThumbstickY;
    uint16_t RightThumbstickX;
    uint16_t RightThumbstickY;
    float LeftTrigger;
    float RightTrigger;
    uint16_t Buttons;
} GamepadReading;

typedef struct GamepadVibration {
    float LeftMotor;
    float RightMotor;
    float LeftTrigger;
    float RightTrigger;
} GamepadVibration;

struct IGamepadVtbl {
    HRESULT(STDMETHODCALLTYPE* QueryInterface)(IGamepad* This, REFIID riid, void** ppvObject);
    ULONG(STDMETHODCALLTYPE* AddRef)(IGamepad* This);
    ULONG(STDMETHODCALLTYPE* Release)(IGamepad* This);
    HRESULT(STDMETHODCALLTYPE* GetIids)(IGamepad* This, ULONG* iidCount, IID** iids);
    HRESULT(STDMETHODCALLTYPE* GetRuntimeClassName)(IGamepad* This, HSTRING* className);
    HRESULT(STDMETHODCALLTYPE* GetTrustLevel)(IGamepad* This, int* trustLevel);
    HRESULT(STDMETHODCALLTYPE* GetCurrentReading)(IGamepad* This, GamepadReading* reading);
    HRESULT(STDMETHODCALLTYPE* get_Vibration)(IGamepad* This, GamepadVibration* vibration);
    HRESULT(STDMETHODCALLTYPE* put_Vibration)(IGamepad* This, GamepadVibration vibration);
    HRESULT(STDMETHODCALLTYPE* add_GamepadRemoved)(IGamepad* This, IUnknown* handler, void* token);
    HRESULT(STDMETHODCALLTYPE* remove_GamepadRemoved)(IGamepad* This, void* token);
    HRESULT(STDMETHODCALLTYPE* add_GamepadAdded)(IGamepad* This, IUnknown* handler, void* token);
    HRESULT(STDMETHODCALLTYPE* remove_GamepadAdded)(IGamepad* This, void* token);
};

struct IGamepad {
    IGamepadVtbl* lpVtbl;
};

struct IVectorViewVtbl {
    HRESULT(STDMETHODCALLTYPE* QueryInterface)(IVectorView* This, REFIID riid, void** ppvObject);
    ULONG(STDMETHODCALLTYPE* AddRef)(IVectorView* This);
    ULONG(STDMETHODCALLTYPE* Release)(IVectorView* This);
    HRESULT(STDMETHODCALLTYPE* GetIids)(IVectorView* This, ULONG* iidCount, IID** iids);
    HRESULT(STDMETHODCALLTYPE* GetRuntimeClassName)(IVectorView* This, HSTRING* className);
    HRESULT(STDMETHODCALLTYPE* GetTrustLevel)(IVectorView* This, int* trustLevel);
    HRESULT(STDMETHODCALLTYPE* GetAt)(IVectorView* This, UINT32 index, IGamepad** item);
    HRESULT(STDMETHODCALLTYPE* get_Size)(IVectorView* This, UINT32* size);
    HRESULT(STDMETHODCALLTYPE* IndexOf)(IVectorView* This, IGamepad* value, UINT32* index, BOOL* found);
    HRESULT(STDMETHODCALLTYPE* GetMany)(IVectorView* This, UINT32 startIndex, UINT32 capacity, IGamepad** items, UINT32* actual);
};

struct IVectorView {
    IVectorViewVtbl* lpVtbl;
};

struct IGamepadStaticsVtbl {
    HRESULT(STDMETHODCALLTYPE* QueryInterface)(IGamepadStatics* This, REFIID riid, void** ppvObject);
    ULONG(STDMETHODCALLTYPE* AddRef)(IGamepadStatics* This);
    ULONG(STDMETHODCALLTYPE* Release)(IGamepadStatics* This);
    HRESULT(STDMETHODCALLTYPE* GetIids)(IGamepadStatics* This, ULONG* iidCount, IID** iids);
    HRESULT(STDMETHODCALLTYPE* GetRuntimeClassName)(IGamepadStatics* This, HSTRING* className);
    HRESULT(STDMETHODCALLTYPE* GetTrustLevel)(IGamepadStatics* This, int* trustLevel);
    HRESULT(STDMETHODCALLTYPE* get_Gamepads)(IGamepadStatics* This, IVectorView** gamepads);
    HRESULT(STDMETHODCALLTYPE* add_GamepadAdded)(IGamepadStatics* This, IUnknown* handler, void* token);
    HRESULT(STDMETHODCALLTYPE* remove_GamepadAdded)(IGamepadStatics* This, void* token);
    HRESULT(STDMETHODCALLTYPE* add_GamepadRemoved)(IGamepadStatics* This, IUnknown* handler, void* token);
    HRESULT(STDMETHODCALLTYPE* remove_GamepadRemoved)(IGamepadStatics* This, void* token);
};

struct IGamepadStatics {
    IGamepadStaticsVtbl* lpVtbl;
};

typedef struct IGamepad2Vtbl {
    HRESULT (STDMETHODCALLTYPE* QueryInterface)(void* This, REFIID riid, void** ppvObject);
    ULONG (STDMETHODCALLTYPE* AddRef)(void* This);
    ULONG (STDMETHODCALLTYPE* Release)(void* This);
    HRESULT (STDMETHODCALLTYPE* GetIids)(void* This, ULONG* iidCount, IID** iids);
    HRESULT (STDMETHODCALLTYPE* GetRuntimeClassName)(void* This, HSTRING* className);
    HRESULT (STDMETHODCALLTYPE* GetTrustLevel)(void* This, TrustLevel* trustLevel);
    HRESULT (STDMETHODCALLTYPE* get_Vibration)(void* This, GamepadVibration* value);
    HRESULT (STDMETHODCALLTYPE* put_Vibration)(void* This, GamepadVibration value);
} IGamepad2Vtbl;

typedef struct IGamepad2 {
    IGamepad2Vtbl* lpVtbl;
} IGamepad2;


// --- Helper macros ---
#define SAFE_RELEASE(p) do { if (p) { (p)->lpVtbl->Release(p); (p) = NULL; } } while (0)

// --- pal_gamepad_state (simplified) ---
typedef struct win32_gamepad_context{
    uint8_t xinput_connected[MAX_XINPUT_CONTROLLERS];
    XINPUT_STATE xinput_state[MAX_XINPUT_CONTROLLERS];

    // windows gaming input.
    IVectorView* wgi_gamepads;
    UINT32 wgi_count;

    uint8_t raw_input_buffer[1024];  // <-- THIS IS THE BUFFER

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
}win32_gamepad_context;
win32_gamepad_context win32_gamepad_ctx = {0};

static const wchar_t* RuntimeClass_Gamepad = L"Windows.Gaming.Input.Gamepad";

int init_windows_gaming_input(void) {
    HRESULT hr = RoInitialize(RO_INIT_MULTITHREADED);
    if (FAILED(hr) && hr != RPC_E_CHANGED_MODE) return 0;

    HSTRING class_str;
    hr = WindowsCreateString(RuntimeClass_Gamepad, (UINT32)wcslen(RuntimeClass_Gamepad), &class_str);
    if (FAILED(hr)) return 0;

    hr = RoGetActivationFactory(class_str, &IID_IGamepadStatics, (void**)&g_statics);
    WindowsDeleteString(class_str);

    return SUCCEEDED(hr);
}

// --- platform_get_gamepad_count ---

int platform_get_gamepad_count(void) {
    UINT32 wgi_count = 0;
    IVectorView* gamepads = NULL;

    if (g_statics && SUCCEEDED(g_statics->lpVtbl->get_Gamepads(g_statics, (void**)&gamepads)) && gamepads) {
        gamepads->lpVtbl->get_Size(gamepads, &wgi_count);
    }
    SAFE_RELEASE(gamepads);

    win32_gamepad_ctx.wgi_count = wgi_count;

    // Track XInput slots used by WGI
    pal_bool xinput_slot_used[MAX_XINPUT_CONTROLLERS] = { FALSE };
    for (UINT32 i = 0; i < wgi_count && i < MAX_XINPUT_CONTROLLERS; ++i) {
        xinput_slot_used[i] = TRUE;
    }

    // Poll remaining XInput slots
    for (DWORD i = 0; i < MAX_XINPUT_CONTROLLERS; ++i) {
        if (xinput_slot_used[i]) {
            // we check xinput_connected when polling input to ensure that the same gamepad is not polled twice when it's in xinput and wgi.
            win32_gamepad_ctx.xinput_connected[i] = FALSE; 
            continue;
        }

        XINPUT_STATE state;
        if (XInputGetState(i, &state) == ERROR_SUCCESS) {
            win32_gamepad_ctx.xinput_connected[i] = TRUE;
            win32_gamepad_ctx.xinput_state[i] = state;
        } else {
            win32_gamepad_ctx.xinput_connected[i] = FALSE;
        }
    }

    int total_count = (int)wgi_count;
    for (int i = 0; i < MAX_XINPUT_CONTROLLERS; ++i) {
        if (win32_gamepad_ctx.xinput_connected[i]) total_count++;
    }

    return total_count;
}
// --- platform_gamepad_get_state ---

#define GamepadButtons_None             0x0000
#define GamepadButtons_Menu             0x0001  // Start button
#define GamepadButtons_View             0x0002  // Back button
#define GamepadButtons_DPadUp           0x0004
#define GamepadButtons_DPadDown         0x0008
#define GamepadButtons_DPadLeft         0x0010
#define GamepadButtons_DPadRight        0x0020
#define GamepadButtons_LeftThumbstick  0x0040
#define GamepadButtons_RightThumbstick 0x0080
#define GamepadButtons_LeftShoulder    0x0100
#define GamepadButtons_RightShoulder   0x0200
#define GamepadButtons_A                0x1000
#define GamepadButtons_B                0x2000
#define GamepadButtons_X                0x4000
#define GamepadButtons_Y                0x8000

pal_bool platform_gamepad_get_state(int index, pal_gamepad_state* out_state) {
    memset(out_state, 0, sizeof(pal_gamepad_state));

    // WGI controllers
    if (g_statics && index < (int)win32_gamepad_ctx.wgi_count) {
        IVectorView* gamepads = NULL;
        if (FAILED(g_statics->lpVtbl->get_Gamepads(g_statics, (void**)&gamepads)) || !gamepads)
            return FALSE;

        IGamepad* pad = NULL;
        HRESULT hr = gamepads->lpVtbl->GetAt(gamepads, (UINT32)index, &pad);
        SAFE_RELEASE(gamepads);
        if (FAILED(hr) || !pad) return FALSE;

        GamepadReading reading = {0};
        hr = pad->lpVtbl->GetCurrentReading(pad, &reading);
        SAFE_RELEASE(pad);
        if (FAILED(hr)) return FALSE;

        // Normalize & deadzone
        const float DEADZONE = 0.25f;
        float lx = reading.LeftThumbstickX;
        float ly = reading.LeftThumbstickY;
        float rx = reading.RightThumbstickX;
        float ry = reading.RightThumbstickY;

        if (sqrtf(lx*lx + ly*ly) < DEADZONE) lx = ly = 0;
        if (sqrtf(rx*rx + ry*ry) < DEADZONE) rx = ry = 0;

        out_state->axes.left_x = lx;
        out_state->axes.left_y = ly;
        out_state->axes.right_x = rx;
        out_state->axes.right_y = ry;
        out_state->axes.left_trigger = reading.LeftTrigger;
        out_state->axes.right_trigger = reading.RightTrigger;

        out_state->buttons.a = (reading.Buttons & GamepadButtons_A) != 0;
        out_state->buttons.b = (reading.Buttons & GamepadButtons_B) != 0;
        out_state->buttons.x = (reading.Buttons & GamepadButtons_X) != 0;
        out_state->buttons.y = (reading.Buttons & GamepadButtons_Y) != 0;
        out_state->buttons.back = (reading.Buttons & GamepadButtons_View) != 0;
        out_state->buttons.start = (reading.Buttons & GamepadButtons_Menu) != 0;
        out_state->buttons.left_stick = (reading.Buttons & GamepadButtons_LeftThumbstick) != 0;
        out_state->buttons.right_stick = (reading.Buttons & GamepadButtons_RightThumbstick) != 0;
        out_state->buttons.left_shoulder = (reading.Buttons & GamepadButtons_LeftShoulder) != 0;
        out_state->buttons.right_shoulder = (reading.Buttons & GamepadButtons_RightShoulder) != 0;
        out_state->buttons.dpad_up = (reading.Buttons & GamepadButtons_DPadUp) != 0;
        out_state->buttons.dpad_down = (reading.Buttons & GamepadButtons_DPadDown) != 0;
        out_state->buttons.dpad_left = (reading.Buttons & GamepadButtons_DPadLeft) != 0;
        out_state->buttons.dpad_right = (reading.Buttons & GamepadButtons_DPadRight) != 0;

        strncpy(out_state->name, "Windows Gaming Input Controller", sizeof(out_state->name));
        out_state->vendor_id = 0x045E;
        out_state->product_id = 0;
        out_state->connected = TRUE;
        out_state->is_xinput = FALSE;
        return TRUE;
    }

    // XInput controllers
    index -= (int)win32_gamepad_ctx.wgi_count;

    for (int i = 0; i < MAX_XINPUT_CONTROLLERS; ++i) {
        if (win32_gamepad_ctx.xinput_connected[i]) {
            if (index == 0) {
                const XINPUT_GAMEPAD* pad = &win32_gamepad_ctx.xinput_state[i].Gamepad;

                float lx = pad->sThumbLX;
                float ly = pad->sThumbLY;
                float rx = pad->sThumbRX;
                float ry = pad->sThumbRY;

                if (abs(lx) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) lx = 0;
                if (abs(ly) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ly = 0;
                if (abs(rx) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) rx = 0;
                if (abs(ry) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) ry = 0;

                out_state->axes.left_x = fmaxf(-1.0f, lx / 32767.0f);
                out_state->axes.left_y = fmaxf(-1.0f, ly / 32767.0f);
                out_state->axes.right_x = fmaxf(-1.0f, rx / 32767.0f);
                out_state->axes.right_y = fmaxf(-1.0f, ry / 32767.0f);

                out_state->axes.left_trigger = (pad->bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? 0.0f : (pad->bLeftTrigger / 255.0f);
                out_state->axes.right_trigger = (pad->bRightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? 0.0f : (pad->bRightTrigger / 255.0f);

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

                strncpy(out_state->name, "Xbox Controller", sizeof(out_state->name));
                out_state->vendor_id = 0x045E;
                out_state->product_id = 0xDEADBEEF;
                out_state->connected = TRUE;
                out_state->is_xinput = TRUE;
                return TRUE;
            }
            --index;
        }
    }

    return FALSE;
}
// --- platform_set_controller_vibration ---

void platform_set_gamepad_vibration(int controller_id, float left_motor, float right_motor, float left_trigger, float right_trigger) {
    // Clamp all values to [0.0f, 1.0f]
    if (left_motor < 0.0f) left_motor = 0.0f;
    if (left_motor > 1.0f) left_motor = 1.0f;
    if (right_motor < 0.0f) right_motor = 0.0f;
    if (right_motor > 1.0f) right_motor = 1.0f;

    // --- WGI Gamepads ---
    if (controller_id >= 0 && controller_id < (int)win32_gamepad_ctx.wgi_count && win32_gamepad_ctx.wgi_gamepads) {
        IGamepad* gamepad = NULL;
        HRESULT hr = win32_gamepad_ctx.wgi_gamepads->lpVtbl->GetAt(
            win32_gamepad_ctx.wgi_gamepads,
            (UINT32)controller_id,
            &gamepad
        );

        if (SUCCEEDED(hr) && gamepad) {
            // Try to query IGamepad2 for trigger vibration support
            IGamepad2* gamepad2 = NULL;
            hr = gamepad->lpVtbl->QueryInterface(gamepad, &IID_IGamepad2, (void**)&gamepad2);
            if (SUCCEEDED(hr) && gamepad2) {
                GamepadVibration vibration = {
                    .LeftMotor = left_motor,
                    .RightMotor = right_motor,
                    .LeftTrigger = left_trigger,
                    .RightTrigger = right_trigger
                };
                gamepad2->lpVtbl->put_Vibration(gamepad2, vibration);
                gamepad2->lpVtbl->Release(gamepad2);
                gamepad->lpVtbl->Release(gamepad);
                return;
            }

            // Fallback: if only IGamepad (no trigger vibration support)
            GamepadVibration vib = { left_motor, right_motor, 0.0f, 0.0f };
            gamepad->lpVtbl->put_Vibration(gamepad, vib);
            gamepad->lpVtbl->Release(gamepad);
            return;
        }
    }

    // --- XInput Gamepads ---
    int xinput_index = controller_id - (int)win32_gamepad_ctx.wgi_count;
    if (xinput_index >= 0 && xinput_index < MAX_XINPUT_CONTROLLERS && win32_gamepad_ctx.xinput_connected[xinput_index]) {
        XINPUT_VIBRATION vibration = {
            .wLeftMotorSpeed = (WORD)(left_motor * 65535.0f),
            .wRightMotorSpeed = (WORD)(right_motor * 65535.0f)
        };
        XInputSetState(xinput_index, &vibration);
    }
}

void platform_stop_gamepad_vibration(int controller_id) {
    platform_set_gamepad_vibration(controller_id, 0.0f, 0.0f, 0.0f, 0.0f);
}

static LRESULT CALLBACK win32_window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

    switch (msg) {
        case WM_CLOSE:
            DestroyWindow(hwnd); // This is required to get WM_DESTROY
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0); // Posts WM_QUIT to the message queue
            return 0;
        case WM_INPUT:
            win32_handle_raw_input((HRAWINPUT)lparam);
            return 0;
            
        case WM_INPUT_DEVICE_CHANGE:
            win32_handle_device_change((HANDLE)lparam, (DWORD)wparam);
            printf("Device Changed!\n");
            return 0;
    }
      return DefWindowProc(hwnd, msg, wparam, lparam);
}

static pal_window* platform_create_window(int width, int height, const char* windowTitle, uint64_t window_flags) {
    // these variables are only
    // used when initializing opengl.
    pal_window* fakewindow = NULL;
    HGLRC fakeRC = 0;
	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
    // we default to opengl.
    if (!(window_flags & PAL_WINDOW_OPENGL) || !(window_flags & PAL_WINDOW_VULKAN) || !(window_flags & PAL_WINDOW_METAL)) {
        window_flags |= PAL_WINDOW_OPENGL;
    }
    if (window_flags & PAL_WINDOW_OPENGL) {
		fakewindow = (pal_window*)malloc(sizeof(pal_window));
		WNDCLASSEXA fakewc = { 0 };
		fakewc.cbSize = sizeof(WNDCLASSEXA);
		fakewc.lpfnWndProc = win32_fake_window_proc;
		fakewc.hInstance = GetModuleHandleA(0);
		fakewc.lpszClassName = "Win32 Fake Window Class";
		fakewc.hCursor = LoadCursorA(NULL, IDC_ARROW);

		RegisterClassExA(&fakewc);

		fakewindow->hwnd = CreateWindowExA(
			0,                    // Optional window styles.
			fakewc.lpszClassName, // Window class
			"Fake Ass Window.",   // Window text
			WS_OVERLAPPEDWINDOW,  // Window style

			                      // Size and position
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

			NULL,                 // Parent window    
			NULL,                 // Menu
			fakewc.hInstance,     // Instance handle
			NULL                  // Additional application data
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
	wc.hCursor = LoadCursorA(NULL, IDC_ARROW);

	RegisterClassExA(&wc);

	pal_window* window = (pal_window*)malloc(sizeof(pal_window));
    DWORD ext_window_style = 0;
    DWORD window_style = 0;

    if(window_flags & PAL_WINDOW_NOT_FOCUSABLE) {
        ext_window_style |= WS_EX_NOACTIVATE;
    }
    if (window_flags & PAL_WINDOW_ALWAYS_ON_TOP) {
        ext_window_style |= WS_EX_TOPMOST;
    }

    if (window_flags & PAL_WINDOW_UTILITY) {

        ext_window_style |= WS_EX_TOOLWINDOW;
        window_style |= WS_SYSMENU;
    }
    else if (window_flags & PAL_WINDOW_POPUP_MENU) {
        window_style |= WS_POPUPWINDOW;
    }
    else if (window_flags & PAL_WINDOW_TOOLTIP) {
        ext_window_style |= WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
    }
    else if (window_flags & PAL_WINDOW_RESIZABLE) {
        window_style |= WS_OVERLAPPEDWINDOW;

    }
    else {
        window_style |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
    }

    if (window_flags & PAL_WINDOW_BORDERLESS) {
        window_style = WS_POPUP;
    }

    if (window_flags & PAL_WINDOW_FULLSCREEN) {
		// Desired fullscreen resolution

		DEVMODE dm = {0};
		dm.dmSize = sizeof(dm);
		dm.dmPelsWidth = width;
		dm.dmPelsHeight = height;
		dm.dmBitsPerPel = 32;
		dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

		LONG result = ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
		if (result != DISP_CHANGE_SUCCESSFUL) {
			MessageBox(NULL, "Failed to change resolution", "Error", MB_OK);
			return 1;
		}
        window_style = WS_POPUP;
    }

	window->hwnd = CreateWindowExA(
		ext_window_style,           // Optional window styles.
		wc.lpszClassName,     // Window class
		windowTitle,          // Window text
		window_style,          // Window style

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
    if (window_flags & PAL_WINDOW_OPENGL)
        PFD.dwFlags |= PFD_SUPPORT_OPENGL;

	PFD.dwFlags |= PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	DescribePixelFormat(window->hdc, pixelFormatID, sizeof(PFD), &PFD);
	SetPixelFormat(window->hdc, pixelFormatID, &PFD);

	int contextAttribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	window->hglrc = wglCreateContextAttribsARB(window->hdc, 0, contextAttribs);

    size_t capacity = 10000;
    pal_event* events = (pal_event*)malloc((capacity * sizeof(pal_event)));

    if(events == NULL) {
        fprintf(stderr, "ERROR: platform_create_window(): failed to allocate memory for events!\n");
    }

    pal_event_queue queue = {
        // size and capacity are measured in pal_events, not bytes.
        .size = 0,
        .capacity = capacity,
        .front = 0,
        .back = 0,
        .events = events
    };

    window->queue = queue;

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
	}

	if (window->hglrc) {

		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(fakeRC);
		ReleaseDC(fakewindow->hwnd, s_fakeDC);
		DestroyWindow(fakewindow->hwnd);
        free(fakewindow);



		if (!(window_flags & PAL_WINDOW_HIDDEN)) {
			if (window_flags & PAL_WINDOW_FULLSCREEN) {
				ShowWindow(window->hwnd, SW_SHOW);
			} else if (window_flags & PAL_WINDOW_MAXIMIZED) {
				ShowWindow(window->hwnd, SW_SHOWMAXIMIZED);
			} else if (window_flags & PAL_WINDOW_MINIMIZED) {
				ShowWindow(window->hwnd, SW_SHOWMINIMIZED);
			} else {
				ShowWindow(window->hwnd, SW_SHOWNORMAL);
			}
		} else {
			ShowWindow(window->hwnd, SW_HIDE);
		}

		SetForegroundWindow(window->hwnd);
		SetFocus(window->hwnd);
		OutputDebugStringA("INFO: Using modern OpenGL Context.");
        // save the window style and the window rect in case the user sets the window to windowed before setting it to fullscreen.
        // The fullscreen function is supposed to save this state whenever the user calls it,
        // but if the user doesn't, the make_window_windowed() function uses a state that's all zeroes
        //, so we have to save it here. - Abdelrahman june 13, 2024
		window->windowedStyle = GetWindowLongA(window->hwnd, GWL_STYLE); // style of the window.
		GetWindowRect(window->hwnd, &window->windowedRect); // size and pos of the window.
		return window;
	}
	else {
        // This is supposed to be a fallback in case we can't create the context that we want.
        // Ideally, this should never happen. - Abdelrahman june 13, 2024
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

static uint8_t platform_poll_events(pal_event* event, pal_window* window) {

	MSG msg = {0};
    if (!window->message_pump_drained) {

		platform_get_raw_input_buffer();
		while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE) != 0) {

			TranslateMessage(&msg);

			platform_translate_message(msg, window);

		}
        window->message_pump_drained = TRUE;
    }
 
    pal_event_queue* queue = &window->queue;

    if (queue->size) { // if queue is not empty,

		// peek
		*event = queue->events[queue->front];
		
		// dequeue
		queue->front = (queue->front + 1) % queue->capacity;
		queue->size--;
		return 1;
    }
    else {
        window->message_pump_drained = FALSE;
        input.mouse_delta = (v2){.x = 0.0f, .y = 0.0f};
        return 0;
    }
    assert(0); // UNREACHABLE! Just crash if we get here somehow.
    return 0;
}

static uint8_t platform_set_window_title(pal_window* window, const char* string) {
	return SetWindowTextA(window->hwnd, string);
}


static pal_monitor* platform_get_primary_monitor(void) {
    // The point (0, 0) is guaranteed to be on the primary monitor
	pal_monitor* monitor = malloc(sizeof(pal_monitor));
    POINT pt = { 0, 0 };
    monitor->handle = MonitorFromPoint(pt, MONITOR_DEFAULTTOPRIMARY);
    return monitor;
}

static pal_video_mode* platform_get_video_mode(pal_monitor* monitor) {
    MONITORINFOEX mi = { .cbSize = sizeof(MONITORINFOEX) };
    if (!GetMonitorInfo(monitor->handle, (MONITORINFO*)&mi))
        return 0;

    DEVMODE dm = { .dmSize = sizeof(DEVMODE) };
    if (!EnumDisplaySettings(mi.szDevice, ENUM_CURRENT_SETTINGS, &dm))
        return 0;
    pal_video_mode* mode = (pal_video_mode*)malloc(sizeof(pal_video_mode));
    mode->width = dm.dmPelsWidth;
    mode->height = dm.dmPelsHeight;
    mode->refresh_rate = dm.dmDisplayFrequency;
    mode->bits_per_pixel = dm.dmBitsPerPel;

    return mode;
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

#define MAX_MOUSE_BUTTONS 64
#define MAX_BUTTON_CAPS 32

// Helper struct to hold reusable buffers
typedef struct {
    PHIDP_PREPARSED_DATA prep_data;
    HIDP_BUTTON_CAPS button_caps[MAX_BUTTON_CAPS];
    USHORT num_button_caps;
} MouseHIDBuffers;

MouseHIDBuffers hid_buffer = {0};

pal_bool InitMouseHIDBuffers(HANDLE device_handle) {
    UINT prep_size = 0;
    if (GetRawInputDeviceInfo(device_handle, RIDI_PREPARSEDDATA, NULL, &prep_size) == (UINT)-1)
        return FALSE;

    if (hid_buffer.prep_data) {
        HidD_FreePreparsedData(hid_buffer.prep_data);
        hid_buffer.prep_data = NULL;
    }

    hid_buffer.prep_data = (PHIDP_PREPARSED_DATA)malloc(prep_size);
    if (!hid_buffer.prep_data)
        return FALSE;

    if (GetRawInputDeviceInfo(device_handle, RIDI_PREPARSEDDATA, hid_buffer.prep_data, &prep_size) == (UINT)-1) {
        free(hid_buffer.prep_data);
        hid_buffer.prep_data = NULL;
        return FALSE;
    }

    hid_buffer.num_button_caps = MAX_BUTTON_CAPS;
    NTSTATUS status = HidP_GetButtonCaps(
        HidP_Input,
        hid_buffer.button_caps,
        &hid_buffer.num_button_caps,
        hid_buffer.prep_data
    );

    if (status != HIDP_STATUS_SUCCESS) {
        HidD_FreePreparsedData(hid_buffer.prep_data);
        free(hid_buffer.prep_data);
        hid_buffer.prep_data = NULL;
        return FALSE;
    }

    return TRUE;
}

pal_bool get_device_handle() {
    UINT device_count = 0;
    if (GetRawInputDeviceList(NULL, &device_count, sizeof(RAWINPUTDEVICELIST)) != 0 || device_count == 0)
        return FALSE;

    RAWINPUTDEVICELIST* device_list = (RAWINPUTDEVICELIST*)malloc(sizeof(RAWINPUTDEVICELIST) * device_count);
    if (!device_list)
        return FALSE;

    if (GetRawInputDeviceList(device_list, &device_count, sizeof(RAWINPUTDEVICELIST)) == (UINT)-1) {
        free(device_list);
        return FALSE;
    }

    pal_bool initialized = FALSE;
    for (UINT i = 0; i < device_count; ++i) {
        if (device_list[i].dwType == RIM_TYPEMOUSE)
            continue; // skip traditional mice

        RID_DEVICE_INFO info = {0};
        info.cbSize = sizeof(info);
        UINT size = sizeof(info);
        if (GetRawInputDeviceInfo(device_list[i].hDevice, RIDI_DEVICEINFO, &info, &size) == (UINT)-1)
            continue;

        if (info.dwType == RIM_TYPEHID && info.hid.usUsagePage == 0x01 && info.hid.usUsage == 0x02) {
            if (InitMouseHIDBuffers(device_list[i].hDevice)) {
                initialized = TRUE;
                break;
            }
        }
    }

    free(device_list);
    return initialized;
}

void Win32HandleMouse(const RAWINPUT* raw) {
	LONG dx = raw->data.mouse.lLastX;
	LONG dy = raw->data.mouse.lLastY;
    input.mouse_delta.x += dx;
    input.mouse_delta.y += dy;
 
    /* We don't save mouse buttons
	USHORT buttons = raw->data.mouse.usButtonFlags;
	for (int i = 0; i < 16; ++i) {
		uint16_t down = (buttons >> (i * 2)) & 1;
		uint16_t up = (buttons >> (i * 2 + 1)) & 1;
 
		// If down is 1, set to 1; if up is 1, set to 0; otherwise leave unchanged
		input.mouse_buttons[i] = (input.mouse_buttons[i] & ~up) | down;
	}
    */
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
    /*
    // we used to save this in the input struct, not anymore.

	USHORT key = raw->data.keyboard.VKey;
	input.keys[key] = ~(raw->data.keyboard.Flags) & RI_KEY_BREAK; 
    printf("Key %u %s\n", key, input.keys[key] ? "up" : "down"); // FOR DEBUGGING NOCHECKIN!
    
    
    */

}

// Handles Gamepads, Joysticks, Steering wheels, etc...
void Win32HandleHID(const RAWINPUT* raw) {
	printf("%d", raw->data.hid.dwCount);
}

// Handler function table indexed by dwType (0 = mouse, 1 = keyboard, 2 = HID)
RawInputHandler Win32InputHandlers[3] = {
	Win32HandleMouse,      // RIM_TYPEMOUSE (0)
	Win32HandleKeyboard,   // RIM_TYPEKEYBOARD (1)
	Win32HandleHID        // RIM_TYPEHID (2) This is for joysticks, gamepads, and steering wheels.
};

#define RAW_INPUT_BUFFER_CAPACITY (64 * 1024) // 64 KB

static BYTE g_rawInputBuffer[RAW_INPUT_BUFFER_CAPACITY];

static int platform_get_raw_input_buffer() {
	UINT bufferSize = RAW_INPUT_BUFFER_CAPACITY;
	UINT inputEventCount = GetRawInputBuffer((PRAWINPUT)g_rawInputBuffer, &bufferSize, sizeof(RAWINPUTHEADER));

	PRAWINPUT raw = (PRAWINPUT)g_rawInputBuffer;
	for (UINT i = 0; i < inputEventCount; ++i) {
		UINT type = raw->header.dwType;
        if (type == RIM_TYPEMOUSE) {
            Win32HandleMouse(raw);
        }
        else if (type == RIM_TYPEKEYBOARD) {
            Win32HandleKeyboard(raw);
        }
        else {
            Win32HandleHID(raw);
        }
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

// XAudio2 callback for streaming
typedef struct {
    IXAudio2VoiceCallbackVtbl* lpVtbl;
    pal_sound* sound;
} StreamingVoiceCallback;

// Fixed OnBufferEnd - properly free the allocated buffer
static void STDMETHODCALLTYPE OnBufferEnd(IXAudio2VoiceCallback* callback, void* pBufferContext) {
    StreamingVoiceCallback* cb = (StreamingVoiceCallback*)callback;
    pal_sound* sound = cb->sound;
    
    // Free the buffer that just finished playing
    // pBufferContext contains the buffer pointer we set in buffer.pContext
    if (pBufferContext) {
        free(pBufferContext);
    }
}

static size_t calculate_buffer_size_for_seconds(pal_sound* sound, float seconds) {
    // bytes_per_second = sample_rate * channels * (bits_per_sample / 8)
    size_t bytes_per_second = sound->sample_rate * sound->channels * (sound->bits_per_sample / 8);
    return (size_t)(bytes_per_second * seconds);
}

// Load next chunk from file/decoder into a new buffer
// Returns bytes read, or 0 if end of stream
static size_t load_next_chunk(pal_sound* sound, unsigned char* buffer, size_t buffer_size) {
    size_t bytes_read = 0;
    
    if (sound->source_file) {
        // WAV streaming
        uint32_t remaining = sound->total_data_size - sound->bytes_streamed;
        if (remaining == 0) {
            return 0; // End of file
        }
        
        size_t to_read = (buffer_size < remaining) ? buffer_size : remaining;
        long seek_pos = sound->data_offset + sound->bytes_streamed;
        
        if (fseek(sound->source_file, seek_pos, SEEK_SET) == 0) {
            bytes_read = fread(buffer, 1, to_read, sound->source_file);
            sound->bytes_streamed += bytes_read;
        }
    } else if (sound->decoder) {
        // OGG streaming
        stb_vorbis* vorbis = (stb_vorbis*)sound->decoder;
        int samples_per_chunk = buffer_size / (sound->channels * sizeof(short));
        short* pcm_buffer = (short*)buffer;
        
        int samples_read = stb_vorbis_get_samples_short_interleaved(
            vorbis, sound->channels, pcm_buffer, samples_per_chunk);
        
        bytes_read = samples_read * sound->channels * sizeof(short);
    }
    
    return bytes_read;
}

// Fixed OnVoiceProcessingPassEnd - queue new buffers as needed
static void STDMETHODCALLTYPE OnVoiceProcessingPassEnd(IXAudio2VoiceCallback* callback) {
    StreamingVoiceCallback* cb = (StreamingVoiceCallback*)callback;
    pal_sound* sound = cb->sound;
    
    if (!sound->is_streaming || sound->stream_finished) {
        return;
    }
    
    // Check how many buffers are queued
    XAUDIO2_VOICE_STATE state;
    sound->source_voice->lpVtbl->GetState(sound->source_voice, &state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
    
    static int callback_count = 0;
    if (callback_count < 10) {
        printf("Callback %d: BuffersQueued=%u\n", callback_count, state.BuffersQueued);
    }
    callback_count++;
    
    // If we have fewer than 2 buffers queued, try to queue another one
    if (state.BuffersQueued < 2) {
        // Use the same chunk duration as the preload (e.g., 0.5 seconds)
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
                
                // Set end of stream flag only if we got no data (true end of stream)
                if (bytes_read == 0) {
                    buffer.Flags = XAUDIO2_END_OF_STREAM;
                    sound->stream_finished = 1;
                    printf("End of stream reached - no more data\n");
                }
                
                HRESULT hr = sound->source_voice->lpVtbl->SubmitSourceBuffer(
                    sound->source_voice, &buffer, NULL);
                    
                if (FAILED(hr)) {
                    printf("ERROR: Failed to submit buffer in callback: 0x%08X\n", hr);
                    free(chunk_buffer);
                } else if (callback_count < 10) {
                    float buffer_duration = (float)bytes_read / 
                        (sound->sample_rate * sound->channels * (sound->bits_per_sample / 8));
                    printf("Submitted buffer: %u bytes (%.3f seconds), flags=0x%x\n", 
                           buffer.AudioBytes, buffer_duration, buffer.Flags);
                }
            } else {
                free(chunk_buffer);
                sound->stream_finished = 1;
                if (callback_count < 10) {
                    printf("No more data available - stream finished\n");
                }
            }
        }
    }
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

static void STDMETHODCALLTYPE OnVoiceError(IXAudio2VoiceCallback* callback, void* pBufferContext, HRESULT Error) {
    // Called when XAudio2 encounters an error
    StreamingVoiceCallback* cb = (StreamingVoiceCallback*)callback;
    pal_sound* sound = cb->sound;
    
    printf("XAudio2 Voice Error: 0x%08X\n", Error);
    
    // Stop streaming on error
    sound->stream_finished = 1;
}

static void STDMETHODCALLTYPE OnVoiceProcessingPassStart(IXAudio2VoiceCallback* callback, UINT32 BytesRequired) {
    // Called when XAudio2 starts processing audio for this voice
    // BytesRequired tells us how much data XAudio2 needs
    StreamingVoiceCallback* cb = (StreamingVoiceCallback*)callback;
    pal_sound* sound = cb->sound;
    
    // We can use BytesRequired to be more intelligent about buffer management
    // For now, we'll handle this in OnVoiceProcessingPassEnd instead
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
    OnBufferStart,
    OnBufferEnd,
    OnLoopEnd,
    OnVoiceError,
    OnStreamEnd
};

// it would be cleaner if we did fopen just to check the file format
// and then used a separate fopen call to actually parse the formats.

pal_sound* platform_load_sound(const char* filename, float seconds) {
    volatile float secs = seconds;
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
		result = load_wav(filename, sound, &secs);
	}
	else if (memcmp(header, "OggS", 4) == 0) {
        fclose(file);
		result = load_ogg(filename, sound, &secs);
	}
	else {
		fclose(file);
		free(sound);
		return NULL; // unsupported format
	}

    // we expect load_wav and load_ogg to return 1 for success.
	if (result != 1) {
		free(sound);
		return NULL;
	}

	static const GUID KSDATAFORMAT_SUBTYPE_PCM = {
		0x00000001, 0x0000, 0x0010,
		{ 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 }
	};

	static const GUID KSDATAFORMAT_SUBTYPE_IEEE_FLOAT = {
		0x00000003, 0x0000, 0x0010,
		{ 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 }
	};

	WAVEFORMATEXTENSIBLE wfex = { 0 };
	wfex.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
	wfex.Format.nChannels = sound->channels;
	wfex.Format.nSamplesPerSec = sound->sample_rate;
	wfex.Format.wBitsPerSample = sound->bits_per_sample;
	wfex.Format.nBlockAlign = (wfex.Format.nChannels * wfex.Format.wBitsPerSample) / 8;
	wfex.Format.nAvgBytesPerSec = wfex.Format.nSamplesPerSec * wfex.Format.nBlockAlign;
	wfex.Format.cbSize = 22; // Yes, this is correct according to the docs.
	wfex.Samples.wValidBitsPerSample = (uint16_t)sound->bits_per_sample;

	wfex.SubFormat = (sound->is_float)
		? KSDATAFORMAT_SUBTYPE_IEEE_FLOAT
		: KSDATAFORMAT_SUBTYPE_PCM;

	switch (sound->channels) {
		case 1: wfex.dwChannelMask = SPEAKER_FRONT_CENTER; break;

		case 2: wfex.dwChannelMask = SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT; break;

		case 4: wfex.dwChannelMask = SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT |
                                     SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT; break;

		case 6: wfex.dwChannelMask = SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT |
			      					 SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY |
                                     SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT; break;

		case 8: wfex.dwChannelMask = SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT |
								     SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY |
								     SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT |
                                     SPEAKER_SIDE_LEFT | SPEAKER_SIDE_RIGHT; break;

		default: wfex.dwChannelMask = 0; break;
	}

	sound->source_voice = NULL;

	// Create streaming callback if this is a streaming sound
	if (seconds > 0.0f) { // This is a streaming sound
		StreamingVoiceCallback* callback = (StreamingVoiceCallback*)malloc(sizeof(StreamingVoiceCallback));
		if (callback) {
			callback->lpVtbl = &StreamingCallbackVtbl;
			callback->sound = sound;
			sound->voice_callback = (IXAudio2VoiceCallback*)callback;
		}
        sound->preload_seconds = seconds;
        sound->is_streaming = 1;
        
        // For streaming sounds, store the total data size
        if (sound->source_file) {
            fseek(sound->source_file, 0, SEEK_END);
            long file_end = ftell(sound->source_file);
            sound->total_data_size = file_end - sound->data_offset;
            sound->bytes_streamed = sound->data_size; // Already loaded portion
            
            float total_seconds = (float)sound->total_data_size / 
                (sound->sample_rate * sound->channels * (sound->bits_per_sample / 8));
            
            printf("WAV streaming: total_size=%u bytes (%.1f seconds), preloaded=%zu bytes (%.3f seconds)\n",
                   sound->total_data_size, total_seconds, sound->data_size,
                   (float)sound->data_size / (sound->sample_rate * sound->channels * (sound->bits_per_sample / 8)));
        }
    }
    else {
        sound->preload_seconds = 0;
        sound->is_streaming = 0;
    }
	
	HRESULT hr = g_xaudio2->lpVtbl->CreateSourceVoice(
		g_xaudio2, &sound->source_voice, (const WAVEFORMATEX*)&wfex,
		0, XAUDIO2_DEFAULT_FREQ_RATIO,
		sound->voice_callback, NULL, NULL);

	if (FAILED(hr)) {
		if (sound->voice_callback) free(sound->voice_callback);
		free(sound);
		return NULL;
	}

	return sound;
}

static int platform_play_music(pal_sound* sound, float volume) {
    if (!g_xaudio2 || !g_mastering_voice) {
        printf("ERROR: XAudio2 not initialized\n");
        return E_FAIL;
    }
    
    printf("Playing sound: streaming=%s, voice_callback=%p\n", 
           sound->is_streaming ? "YES" : "NO", sound->voice_callback);
    
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
           buffer.AudioBytes, initial_seconds, buffer.Flags);
    
    HRESULT hr = sound->source_voice->lpVtbl->SubmitSourceBuffer(sound->source_voice, &buffer, NULL);
    if (FAILED(hr)) {
        printf("ERROR: Failed to submit source buffer: 0x%08X\n", hr);
        return hr;
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

static void platform_free_music(pal_sound* sound) {
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
    
    // Free the callback we allocated
    if (sound->voice_callback) {
        free(sound->voice_callback);
    }
    
    free(sound->data);
    free(sound);
}
static int platform_play_sound(pal_sound* sound, float volume) {
	if (!g_xaudio2 || !g_mastering_voice) {
		return E_FAIL;
	}

	// Set the volume
	sound->source_voice->lpVtbl->SetVolume(sound->source_voice, volume, 0);

	XAUDIO2_BUFFER buffer = {
		.AudioBytes = (UINT32)sound->data_size,
		.pAudioData = sound->data,
		.Flags = XAUDIO2_END_OF_STREAM 
	};

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

void platform_free_sound(pal_sound* sound) {
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

int platform_stop_sound(pal_sound* sound) {
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
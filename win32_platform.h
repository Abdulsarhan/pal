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
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

// OpenGL
#include <gl/gl.h>
#include <GL/glext.h>
#include <GL/wglext.h>

// Windows Gaming Input (For Xbone / Series S Controllers)
#define COBJMACROS
#include <roapi.h>
#include <winstring.h>

// Project headers (always last)
#include "pal_platform.h"

#define KEY_DOWN_BIT 0x80
#define KEY_REPEAT_BIT 0b1

#define MB(x)((size_t)(x) * 1024 * 1024)

typedef unsigned __int64 QWORD;

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

/*
#define WM_LBUTTONDOWN                  0x0201
#define WM_LBUTTONUP                    0x0202
#define WM_LBUTTONDBLCLK                0x0203
#define WM_RBUTTONDOWN                  0x0204
#define WM_RBUTTONUP                    0x0205
#define WM_RBUTTONDBLCLK                0x0206
#define WM_MBUTTONDOWN                  0x0207
#define WM_MBUTTONUP                    0x0208
#define WM_MBUTTONDBLCLK                0x0209

#define WM_XBUTTONDOWN                  0x020B
#define WM_XBUTTONUP                    0x020C
#define WM_XBUTTONDBLCLK                0x020D
*/

// this is supposed to be indexed by subtracting
// 201  from the event that windows gives us.
uint32_t win32_button_to_pal_button[] = {

0x0, 0x0, 0x0, // left mouse button
0x1, 0x1, 0x1, // right mouse button
0x2, 0x2, 0x2, // middle mouse button
0x3, // mouse4
0x4, // mouse5
};

uint32_t win32_key_to_pal_key[] = {
// first 7 values are just bullshit.
0x00, 0x00, 0x00, 0x00, 0x00, // Unassigned
0x00, 0x00, 0x00, // Unassigned
0x08, 0x09, 0x00, 0x00, // backspace, tab, unassigned, unassigned.
0x0C, 0x0D, 0x00, 0x00, // clear, return, unassigned, unassigned.
0x10, 0x11, 0x12, 0x13, 0x14,  // shift, control, menu(key next to alt), pause, capslock.
0x15, 0x16, 0x17, 0x18,0x19,0x1A, // japanese keyboard shit. Probably not going to use this.
0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20,
0x21, 0x22, 0x23, 0x24, 0x25,
0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,
0x2E, 0x2F, 

// Uppercase A - Z.
0x30, 0x31, 0x32, 0x33,
0x34, 0x35, 0x36, 0x37,
0x38, 0x39,

//Unassigned
0x3A, 0x3B, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 

// Lowercase a - z.
0x41, 0x42, 0x43, 0x44,
0x45, 0x46, 0x47, 0x48, 0x49,
0x4A, 0x4B, 0x4C, 0x4D, 0x4E,
0x4F, 0x50, 0x51, 0x52, 0x53,
0x54, 0x55, 0x56, 0x57, 0x58,
0x59, 0x5A, 

0x5B, 0x5C, 0x5D, // Lwin, Rwin, Apps

0x5E, // Unassigned

0x5f, // sleep

// Numpad 0 - 9
0x60, 0x61, 0x62, 0x63, 0x64,
0x65, 0x66, 0x67, 0x68, 0x69,

// Numpad operands
0x6A, 0x6B, 0x6C, 0x6D, 0x6E,
0x6F,

// Function Keys (supports 24 function keys, even though most keyboards just have 12)
0x70, 0x71, 0x72, 0x73,
0x74, 0x75, 0x76, 0x77,
0x78, 0x79, 0x7A, 0x7B,
0x7C, 0x7D, 0x7E, 0x7F,
0x80, 0x81, 0x82, 0x83,
0x84, 0x85, 0x86, 0x87,

// reserved
0x88, 0x89, 0x8A, 0x8B, 
0x8C, 0x8D, 0x8E, 0x8F,

0x90, 0x91 // Numlock, scroll lock

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
                    event.button.button = SIDE_MOUSE_BUTTON1;
                    input.mouse_buttons[SIDE_MOUSE_BUTTON1] = 1;
                } else if (xButton == XBUTTON2) {
                    event.button.button = SIDE_MOUSE_BUTTON2;
                    input.mouse_buttons[SIDE_MOUSE_BUTTON2] = 1;
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
                    event.button.button = SIDE_MOUSE_BUTTON1;
                    input.mouse_buttons[SIDE_MOUSE_BUTTON1] = 1;
                } else if (xButton == XBUTTON2) {
                    event.button.button = SIDE_MOUSE_BUTTON2;
                    input.mouse_buttons[SIDE_MOUSE_BUTTON2] = 1;
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
                    event.button.button = SIDE_MOUSE_BUTTON1;
                    input.mouse_buttons[SIDE_MOUSE_BUTTON1] = 0;
                    input.mouse_buttons_processed[SIDE_MOUSE_BUTTON1] = 0;
                } else if (xButton == XBUTTON2) {
                    event.button.button = SIDE_MOUSE_BUTTON2;
                    input.mouse_buttons[SIDE_MOUSE_BUTTON2] = 0;
                    input.mouse_buttons_processed[SIDE_MOUSE_BUTTON2] = 0;
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

			event.type = PAL_KEY_DOWN;
			event.key = (pal_keyboard_event){
				.virtual_key = win32_key_to_pal_key[vk],
				.scancode = scancode,
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

			event.type = PAL_KEY_UP;
			event.key = (pal_keyboard_event){
				.virtual_key = win32_key_to_pal_key[vk],
				.scancode = scancode,
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

// Window Hints
static void platform_set_window_hint(int type, int value) {
    // This is very jank-tastic. There is probably a better way of doing this. 
    // maybe some of this should become window flags that are passed into the create_window() function?
	switch (type) {
    case GL_VERSION_MAJOR: s_glVersionMajor = value; break;
    case GL_VERSION_MINOR: s_glVersionMinor = value; break;
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

static pal_window* platform_create_window(int width, int height, const char* windowTitle) {
	pal_window* fakewindow = (pal_window*)malloc(sizeof(pal_window));
    WNDCLASSEXA fakewc = { 0 };
	fakewc.cbSize = sizeof(WNDCLASSEXA);
	fakewc.lpfnWndProc = win32_fake_window_proc;
	fakewc.hInstance = GetModuleHandleA(0);
	fakewc.lpszClassName = "Win32 Fake Window Class";
	fakewc.hCursor = LoadCursorA(NULL, IDC_ARROW);

	RegisterClassExA(&fakewc);

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

    WNDCLASSEXA wc = {0};

	wc.cbSize = sizeof(WNDCLASSEXA);
	wc.lpfnWndProc = win32_window_proc;
	wc.hInstance = GetModuleHandleA(0);
	wc.lpszClassName = "Win32 Window Class";
	wc.hCursor = LoadCursorA(NULL, IDC_ARROW);

	RegisterClassExA(&wc);

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
    // enough space for 10,000 events.
    size_t capacity = 10000 * sizeof(pal_event);
    pal_event* events = (pal_event*)malloc((capacity));

    if(events == NULL) {
        fprintf(stderr, "ERROR: platform_create_window(): failed to allocate memory for events!\n");
    }

    pal_event_queue queue = {
        .size = 0,
        .capacity = capacity,
        .front = 0,
        .back = 0,
        .events = events
    };

    window->queue = queue;

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
        // save the window style and the window rect in case the user sets the window to windowed before setting it to fullscreen.
        // The fullscreen function is supposed to save this state whenever the user calls it,
        // but if the user doesn't, the make_window_windowed() function uses a state that's all zeroes
        //, so we have to save it here. - Abdelrahman june 13, 2024
		window->windowedStyle = GetWindowLongA(window->hwnd, GWL_STYLE); // style of the window.
		GetWindowRect(window->hwnd, &window->windowedRect); // size and pos of the window.
        free(fakewindow);
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
        return 0;
    }
    assert(0); // UNREACHABLE! Just crash if we get here somehow.
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
// Helper struct to hold reusable buffers

#define MAX_MOUSE_BUTTONS 64
#define MAX_BUTTON_CAPS 32

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
	input.mouse_delta = (v2){
		(float)dx,
		(float)dy,
	};
 
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
	wfex.Format.cbSize = sizeof(wfex);

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
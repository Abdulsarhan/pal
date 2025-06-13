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

// Project headers (always last)
#include "pal_platform.h"

#define KEY_DOWN_BIT 0x80
#define KEY_REPEAT_BIT 0b1

#define MB(x)((size_t)(x) * 1024 * 1024)

typedef unsigned __int64 QWORD;

// TODO: it might be a good idea to move all of this shit into the window structure
// because the lifetime of all this shit is related to the lifetime of the window.
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
};

struct pal_monitor {
	HMONITOR handle;
};

// Keyboard & Mouse Input
//TODO: @fix We should not have this here and just have the user call
//the functions that fill up the input struct.

#define MAX_KEYS 256
#define MAX_MOUSEBUTTONS 33 // 0 is reserved as a default value, and we want 32 buttons, so we do 33.
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

int platform_translate_message(MSG msg, pal_window* window, pal_event* event) {
    switch (msg.message) {
        case WM_DESTROY:
        case WM_QUIT:
        case WM_CLOSE:
            event->type = PAL_QUIT;
            event->quit = (pal_quit_event){ .code = 0 };
            break;
		case WM_MOVE:
			event->type = PAL_WINDOW_EVENT;
			event->window = (pal_window_event){
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
			event->type = PAL_WINDOW_EVENT;
			event->window = (pal_window_event){
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
            event->type = PAL_WINDOW_EVENT;
            event->window = (pal_window_event){
                .windowid = window->id,
                .event_code = msg.message,
                .x = LOWORD(msg.lParam),
                .y = HIWORD(msg.lParam),
                .width = LOWORD(msg.lParam),
                .height = HIWORD(msg.lParam),
                .focused = 1, // guess; could adjust later
                .visible = 1
            };
            break;

        case WM_MOUSEMOVE:
            event->type = PAL_MOUSE_MOTION;
            event->motion = (pal_mouse_motion_event){
                .x = GET_X_LPARAM(msg.lParam),
                .y = GET_Y_LPARAM(msg.lParam),
                .delta_x = 0, // you could track previous pos elsewhere
                .delta_y = 0,
                .buttons = msg.wParam
            };
            break;

        case WM_LBUTTONDOWN: 
			printf("Left Mouse down!\n");
            event->type = PAL_MOUSE_BUTTON_DOWN;
            event->button = (pal_mouse_button_event){
                .x = GET_X_LPARAM(msg.lParam), // xpos of the mouse
                .y = GET_Y_LPARAM(msg.lParam), // ypos of the mouse.
                .pressed = 1, // pressed state of the mouse, might remove.
                .button = LEFT_MOUSE_BUTTON, // optionally remap to your enum
				.clicks = 1,
                .modifiers = msg.wParam
            };
            break;

        case WM_RBUTTONDOWN: 
            event->type = PAL_MOUSE_BUTTON_DOWN;
            event->button = (pal_mouse_button_event){
                .x = GET_X_LPARAM(msg.lParam), // xpos of the mouse
                .y = GET_Y_LPARAM(msg.lParam), // ypos of the mouse.
                .pressed = 1, // pressed state of the mouse, might remove.
                .button = RIGHT_MOUSE_BUTTON, // optionally remap to your enum
				.clicks = 1,
                .modifiers = msg.wParam
            };
            break;

        case WM_MBUTTONDOWN: 

            event->type = PAL_MOUSE_BUTTON_DOWN;
            event->button = (pal_mouse_button_event){
                .x = GET_X_LPARAM(msg.lParam), // xpos of the mouse
                .y = GET_Y_LPARAM(msg.lParam), // ypos of the mouse.
                .pressed = 1, // pressed state of the mouse, might remove.
                .button = MIDDLE_MOUSE_BUTTON, // optionally remap to your enum
				.clicks = 1,
                .modifiers = msg.wParam
            };
            break;

        case WM_XBUTTONDOWN: 

            event->type = PAL_MOUSE_BUTTON_DOWN;
            event->button = (pal_mouse_button_event){
                .x = GET_X_LPARAM(msg.lParam), // xpos of the mouse
                .y = GET_Y_LPARAM(msg.lParam), // ypos of the mouse.
                .pressed = 1, // pressed state of the mouse, might remove.
				.clicks = 1,
                .modifiers = msg.wParam
            };
			if (GET_XBUTTON_WPARAM(msg.wParam) == MK_XBUTTON1)
				event->button.button = SIDE_MOUSE_BUTTON1;
			else 
				event->button.button = SIDE_MOUSE_BUTTON2;
			break;

        case WM_LBUTTONDBLCLK:
            event->type = PAL_MOUSE_BUTTON_DOWN;
			event->button = (pal_mouse_button_event){
				.x = GET_X_LPARAM(msg.lParam), // xpos of the mouse
				.y = GET_Y_LPARAM(msg.lParam), // ypos of the mouse.
				.pressed = 1, // pressed state of the mouse, might remove.
				.button = LEFT_MOUSE_BUTTON, // optionally remap to your enum
				.clicks = 2,
                .modifiers = msg.wParam
            };
            break;
		case WM_RBUTTONDBLCLK:
            event->type = PAL_MOUSE_BUTTON_DOWN;
            event->button = (pal_mouse_button_event){
                .x = GET_X_LPARAM(msg.lParam), // xpos of the mouse
                .y = GET_Y_LPARAM(msg.lParam), // ypos of the mouse.
                .pressed = 1, // pressed state of the mouse, might remove.
                .button = RIGHT_MOUSE_BUTTON, // optionally remap to your enum
				.clicks = 2,
                .modifiers = msg.wParam
            };
            break;
		case WM_MBUTTONDBLCLK:
            event->type = PAL_MOUSE_BUTTON_DOWN;
            event->button = (pal_mouse_button_event){
                .x = GET_X_LPARAM(msg.lParam), // xpos of the mouse
                .y = GET_Y_LPARAM(msg.lParam), // ypos of the mouse.
                .pressed = 1, // pressed state of the mouse, might remove.
                .button = MIDDLE_MOUSE_BUTTON, // optionally remap to your enum
				.clicks = 2,
                .modifiers = msg.wParam
            };
            break;

		case WM_XBUTTONDBLCLK:
            event->type = PAL_MOUSE_BUTTON_DOWN;
            event->button = (pal_mouse_button_event){
                .x = GET_X_LPARAM(msg.lParam), // xpos of the mouse
                .y = GET_Y_LPARAM(msg.lParam), // ypos of the mouse.
                .pressed = 1, // pressed state of the mouse, might remove.
				.clicks = 2,
                .modifiers = msg.wParam
            };

			if (GET_XBUTTON_WPARAM(msg.wParam) == MK_XBUTTON1)
				event->button.button = SIDE_MOUSE_BUTTON1;
			else 
				event->button.button = SIDE_MOUSE_BUTTON2;
            break;

        case WM_LBUTTONUP:
            event->type = PAL_MOUSE_BUTTON_UP;
            event->button = (pal_mouse_button_event){
                .x = GET_X_LPARAM(msg.lParam),
                .y = GET_Y_LPARAM(msg.lParam),
                .pressed = 0,
                .button = LEFT_MOUSE_BUTTON,
                .modifiers = msg.wParam
            };
            break;
        case WM_RBUTTONUP:
            event->type = PAL_MOUSE_BUTTON_UP;
            event->button = (pal_mouse_button_event){
                .x = GET_X_LPARAM(msg.lParam),
                .y = GET_Y_LPARAM(msg.lParam),
                .pressed = 0,
                .button = RIGHT_MOUSE_BUTTON,
                .modifiers = msg.wParam
            };
            break;
        case WM_MBUTTONUP:
            event->type = PAL_MOUSE_BUTTON_UP;
            event->button = (pal_mouse_button_event){
                .x = GET_X_LPARAM(msg.lParam),
                .y = GET_Y_LPARAM(msg.lParam),
                .pressed = 0,
                .button = MIDDLE_MOUSE_BUTTON,
                .modifiers = msg.wParam
            };
            break;

        case WM_XBUTTONUP:
            event->type = PAL_MOUSE_BUTTON_UP;
            event->button = (pal_mouse_button_event){
                .x = GET_X_LPARAM(msg.lParam),
                .y = GET_Y_LPARAM(msg.lParam),
                .pressed = 0,
                .modifiers = msg.wParam
            };
			if (GET_XBUTTON_WPARAM(msg.wParam) == MK_XBUTTON1)
				event->button.button = SIDE_MOUSE_BUTTON1;
			else 
				event->button.button = SIDE_MOUSE_BUTTON2;
            break;

        case WM_MOUSEWHEEL:
        case WM_MOUSEHWHEEL: {
            int delta = GET_WHEEL_DELTA_WPARAM(msg.wParam);
            event->type = PAL_MOUSE_WHEEL;
            event->wheel = (pal_mouse_wheel_event){
                .x = GET_X_LPARAM(msg.lParam),
                .y = GET_Y_LPARAM(msg.lParam),
                .delta_x = (msg.message == WM_MOUSEHWHEEL) ? (float)delta / WHEEL_DELTA : 0.0f,
                .delta_y = (msg.message == WM_MOUSEWHEEL) ? (float)delta / WHEEL_DELTA : 0.0f,
                .modifiers = GET_KEYSTATE_WPARAM(msg.wParam)
            };
            break;
        }

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            event->type = PAL_KEY_DOWN;
            event->key = (pal_keyboard_event){
                .virtual_key = (uint32_t)msg.wParam,
                .scancode = (uint32_t)((msg.lParam >> 16) & 0xFF),
                .pressed = 1,
                .repeat = (msg.lParam >> 30) & 1,
                .modifiers = GetKeyState(VK_SHIFT) < 0 ? 1 : 0 // or more bits
            };
            break;

        case WM_KEYUP:
        case WM_SYSKEYUP:
            event->type = PAL_KEY_UP;
            event->key = (pal_keyboard_event){
                .virtual_key = (uint32_t)msg.wParam,
                .scancode = (uint32_t)((msg.lParam >> 16) & 0xFF),
                .pressed = 0,
                .repeat = 0,
                .modifiers = GetKeyState(VK_SHIFT) < 0 ? 1 : 0
            };
            break;

        case WM_CHAR:
        case WM_UNICHAR:
            event->type = PAL_TEXT_INPUT;
            event->text = (pal_text_input_event){
                .utf8_text = {0}
            };
            {
                char utf8[8] = {0};
                int len = WideCharToMultiByte(CP_UTF8, 0, (WCHAR*)&msg.wParam, 1, utf8, sizeof(utf8), NULL, NULL);
                memcpy(event->text.utf8_text, utf8, len);
            }
            break;

        case WM_INPUT:
            event->type = PAL_SENSOR_UPDATE;
            event->sensor = (pal_sensor_event){
                .device_id = 0,
                .x = 0, .y = 0, .z = 0,
                .sensor_type = 0
            };
            break;

        case WM_DROPFILES: {
            event->type = PAL_DROP_FILE;
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
            event->drop = (pal_drop_event){
                .paths = paths,
                .count = count
            };
            DragFinish(hDrop);
            break;
        }

        default:
            event->type = PAL_NONE;
            DispatchMessage(&msg);
            break;
    }

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

LRESULT CALLBACK win32_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UINT button = GET_XBUTTON_WPARAM(wParam);
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);
	int width = LOWORD(lParam);
	int height = HIWORD(lParam);


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
	platform_get_raw_input_buffer();
	platform_poll_gamepads();

	//TODO: Handle all the messages in one go.
// this function is supposed to return 0 if there are no more events in peekmessage,
	// and 1 if there are events.

	MSG msg = {0};
	if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE) != 0) {
	
		if (msg.message == WM_DESTROY) {
			PostQuitMessage(0);
		}

		TranslateMessage(&msg);

        platform_translate_message(msg, window, event);

		return 1;
	}
	else {
		return 0;
	}

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

void FreeMouseHIDBuffers() {
    if (hid_buffer.prep_data) {
        HidD_FreePreparsedData(hid_buffer.prep_data);
        free(hid_buffer.prep_data);
        hid_buffer.prep_data = NULL;
    }
    hid_buffer.num_button_caps = 0;
}

void Win32HandleHidMouse(const RAWINPUT* raw) {
    if (raw->header.dwType != RIM_TYPEHID)
        return;

    input.mouse = (v2){ (float)raw->data.mouse.lLastX, (float)raw->data.mouse.lLastY };

    if (!hid_buffer.prep_data)
        return;

    for (int i = 0; i < MAX_MOUSE_BUTTONS; ++i)
        input.mouse_buttons[i] = 0;

    for (ULONG i = 0; i < hid_buffer.num_button_caps; ++i) {
        ULONG usage_count = MAX_MOUSE_BUTTONS;
        USAGE usages[MAX_MOUSE_BUTTONS] = {0};

        NTSTATUS status = HidP_GetUsages(
            HidP_Input,
            hid_buffer.button_caps[i].UsagePage,
            0,
            usages,
            &usage_count,
            hid_buffer.prep_data,
            (PCHAR)raw->data.hid.bRawData,
            raw->data.hid.dwSizeHid
        );

        if (status == HIDP_STATUS_SUCCESS || status == HIDP_STATUS_BUFFER_TOO_SMALL) {
            for (ULONG j = 0; j < usage_count; ++j) {
                USAGE usage_id = usages[j];
                if (usage_id < MAX_MOUSE_BUTTONS)
                    input.mouse_buttons[usage_id] = 1;
            }
        }
    }
}

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
    if (controller_id < 0 || controller_id >= MAX_CONTROLLERS) return (v2) {0};
    if (!input.controller_connected[controller_id]) return (v2) {0};

	short x = input.controller_state[controller_id].Gamepad.sThumbLX;
	short y = input.controller_state[controller_id].Gamepad.sThumbLY;

    return platform_process_thumbstick(x, y, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
}

v2 platform_get_right_stick(int controller_id) {
    if (controller_id < 0 || controller_id >= MAX_CONTROLLERS) return (v2) {0};
    if (!input.controller_connected[controller_id]) return (v2) {0};

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
        if (type == RIM_TYPEMOUSE) {
            Win32HandleMouse(raw);
        }
        else if (type == RIM_TYPEKEYBOARD) {
            Win32HandleKeyboard(raw);
        }
        else {
            Win32HandleHidMouse(raw);
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
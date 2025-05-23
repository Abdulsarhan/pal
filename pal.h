#ifndef PAL_H
#define PAL_H

#include <stdint.h> // For Clearly Defined Types.
#include <sys/stat.h> // For time_t and stat.

typedef struct VideoMode {
	int width;
	int height;
}VideoMode;

typedef struct {
	unsigned char* data;   // Raw PCM audio data
	uint32_t dataSize;       // Size in bytes

	int sampleRate;        // Samples per second (e.g., 44100)
	int channels;          // Number of audio channels (e.g., 2 for stereo)
	int bitsPerSample;     // Usually 16 or 32
    int isFloat; // 0 = PCM, 1 = IEEE float

} Sound;

typedef struct Window Window;
typedef struct Monitor Monitor;
typedef struct SoundInitInfo SoundInitInfo;

#if defined(_WIN32)
#if defined(__TINYC__)
#define __declspec(x) __attribute__((x))
#endif
#if defined(BUILD_LIBTYPE_SHARED)
#define PALAPI __declspec(dllexport)     // We are building the library as a Win32 shared library (.dll)
#elif defined(USE_LIBTYPE_SHARED)
#define PALAPI __declspec(dllimport)     // We are using the library as a Win32 shared library (.dll)
#endif
#else
#if defined(BUILD_LIBTYPE_SHARED)
#define PALAPI __attribute__((visibility("default"))) // We are building as a Unix shared library (.so/.dylib)
#endif
#endif

#ifndef PALAPI
#define PALAPI       // Functions defined as 'extern' by default (implicit specifiers)
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

// Vector2, 2 components
typedef struct Vector2 {
    float x;                // Vector x component
    float y;                // Vector y component
} Vector2;

// Vector3, 3 components
typedef struct Vector3 {
    float x;                // Vector x component
    float y;                // Vector y component
    float z;                // Vector z component
} Vector3;

// Vector4, 4 components
typedef struct Vector4 {
    float x;                // Vector x component
    float y;                // Vector y component
    float z;                // Vector z component
    float w;                // Vector w component
} Vector4;

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

//----------------------------------------------------------------------------------
// Keyboard and Mouse Buttons
//----------------------------------------------------------------------------------
#define LEFT_MOUSE_BUTTON 0x0
#define RIGHT_MOUSE_BUTTON 0x1
#define MIDDLE_MOUSE_BUTTON 0x2
#define SIDE_MOUSE_BUTTON1 0x3
#define SIDE_MOUSE_BUTTON2 0x4
#define EXTRA_MOUSE_BUTTON1 0x05
#define EXTRA_MOUSE_BUTTON2 0x06
#define EXTRA_MOUSE_BUTTON3 0x07
#define EXTRA_MOUSE_BUTTON4 0x08
#define EXTRA_MOUSE_BUTTON5 0x09
#define EXTRA_MOUSE_BUTTON6 0x0A
#define EXTRA_MOUSE_BUTTON7 0x0B
#define EXTRA_MOUSE_BUTTON8 0x0C
#define EXTRA_MOUSE_BUTTON9 0x0D
#define EXTRA_MOUSE_BUTTON10 0x0E
#define EXTRA_MOUSE_BUTTON11 0x0F
#define EXTRA_MOUSE_BUTTON12 0x10
#define EXTRA_MOUSE_BUTTON13 0x11
#define EXTRA_MOUSE_BUTTON14 0x12
#define EXTRA_MOUSE_BUTTON15 0x13
#define EXTRA_MOUSE_BUTTON16 0x14
#define EXTRA_MOUSE_BUTTON17 0x15
#define EXTRA_MOUSE_BUTTON18 0x16
#define EXTRA_MOUSE_BUTTON19 0x17
#define EXTRA_MOUSE_BUTTON20 0x18
#define EXTRA_MOUSE_BUTTON21 0x19
#define EXTRA_MOUSE_BUTTON22 0x1A
#define EXTRA_MOUSE_BUTTON23 0x1B
#define EXTRA_MOUSE_BUTTON24 0x1C
#define EXTRA_MOUSE_BUTTON25 0x1D
#define EXTRA_MOUSE_BUTTON26 0x1E
#define EXTRA_MOUSE_BUTTON27 0x1F
#define EXTRA_MOUSE_BUTTON28 0x20
#define EXTRA_MOUSE_BUTTON29 0x21
#define EXTRA_MOUSE_BUTTON30 0x22

#define KEY_BACKSPACE     0x08
#define KEY_TAB           0x09
#define KEY_ENTER         0x0D
#define KEY_SHIFT         0x10
#define KEY_CONTROL       0x11
#define KEY_ALT           0x12
#define KEY_PAUSE         0x13
#define KEY_CAPSLOCK      0x14
#define KEY_ESCAPE        0x1B
#define KEY_SPACE         0x20
#define KEY_PAGEUP        0x21
#define KEY_PAGEDOWN      0x22
#define KEY_END           0x23
#define KEY_HOME          0x24
#define KEY_LEFT          0x25
#define KEY_UP            0x26
#define KEY_RIGHT         0x27
#define KEY_DOWN          0x28
#define KEY_PRINTSCREEN   0x2C
#define KEY_INSERT        0x2D
#define KEY_DELETE        0x2E

#define KEY_0             0x30
#define KEY_1             0x31
#define KEY_2             0x32
#define KEY_3             0x33
#define KEY_4             0x34
#define KEY_5             0x35
#define KEY_6             0x36
#define KEY_7             0x37
#define KEY_8             0x38
#define KEY_9             0x39

#define KEY_A             0x41
#define KEY_B             0x42
#define KEY_C             0x43
#define KEY_D             0x44
#define KEY_E             0x45
#define KEY_F             0x46
#define KEY_G             0x47
#define KEY_H             0x48
#define KEY_I             0x49
#define KEY_J             0x4A
#define KEY_K             0x4B
#define KEY_L             0x4C
#define KEY_M             0x4D
#define KEY_N             0x4E
#define KEY_O             0x4F
#define KEY_P             0x50
#define KEY_Q             0x51
#define KEY_R             0x52
#define KEY_S             0x53
#define KEY_T             0x54
#define KEY_U             0x55
#define KEY_V             0x56
#define KEY_W             0x57
#define KEY_X             0x58
#define KEY_Y             0x59
#define KEY_Z             0x5A

#define KEY_LWIN          0x5B // This is also known as the super key on linux.
#define KEY_RWIN          0x5C
#define KEY_APPS          0x5D

#define KEY_NUMPAD0       0x60
#define KEY_NUMPAD1       0x61
#define KEY_NUMPAD2       0x62
#define KEY_NUMPAD3       0x63
#define KEY_NUMPAD4       0x64
#define KEY_NUMPAD5       0x65
#define KEY_NUMPAD6       0x66
#define KEY_NUMPAD7       0x67
#define KEY_NUMPAD8       0x68
#define KEY_NUMPAD9       0x69
#define KEY_MULTIPLY      0x6A
#define KEY_ADD           0x6B
#define KEY_SEPARATOR     0x6C // Most modern keyboards don't have this.
#define KEY_SUBTRACT      0x6D
#define KEY_DECIMAL       0x6E
#define KEY_DIVIDE        0x6F

#define KEY_F1            0x70
#define KEY_F2            0x71
#define KEY_F3            0x72
#define KEY_F4            0x73
#define KEY_F5            0x74
#define KEY_F6            0x75
#define KEY_F7            0x76
#define KEY_F8            0x77
#define KEY_F9            0x78
#define KEY_F10           0x79
#define KEY_F11           0x7A
#define KEY_F12           0x7B

#define KEY_NUMLOCK       0x90
#define KEY_SCROLLLOCK    0x91


#if defined(__cplusplus)
#define CLITERAL(type)      type
#else
#define CLITERAL(type)      (type)
#endif

#ifdef __cplusplus
extern "C" {
#endif

PALAPI void init_pal();
PALAPI Window* init_window(int width, int height, const char* windowTitle);
PALAPI void set_window_title(Window* window, const char* string);
PALAPI void set_window_hint(int type, int value);
PALAPI VideoMode* get_video_mode(Monitor* monitor);
PALAPI Monitor* get_primary_monitor(void);
PALAPI void* gl_get_proc_address(const char* proc);
PALAPI uint8_t window_should_close(void);
PALAPI void poll_events(void);
PALAPI int make_context_current(Window* window);

PALAPI int register_input_devices(Window* window);
PALAPI uint8_t is_key_pressed(int key);
PALAPI uint8_t is_key_down(int key);
PALAPI uint8_t is_key_processed(int key);
PALAPI uint8_t set_key_processed(int key);

// Mouse input
PALAPI uint8_t is_mouse_pressed(int button);
PALAPI uint8_t is_mouse_down(int button);
PALAPI uint8_t is_mouse_processed(int button);
PALAPI void set_mouse_processed(int button);

PALAPI void begin_drawing(void);
PALAPI void DrawTriangle(void);
PALAPI void end_drawing(void);

// Sound
PALAPI int load_sound(const char* filename, Sound* out);
PALAPI int play_sound(Sound* sound);

PALAPI uint8_t does_file_exist(const char* file_path);
PALAPI time_t get_file_timestamp(const char* file);
PALAPI long get_file_size(const char* file_path);
PALAPI char* read_file(const char* filePath, int* fileSize, char* buffer);
PALAPI void write_file(const char* filePath, char* buffer, int size);
PALAPI uint8_t copy_file(const char* fileName, const char* outputName, char* buffer);

PALAPI uint8_t is_upper_case(char ch);
PALAPI uint8_t is_lower_case(char ch);
PALAPI uint8_t is_letter(char ch);
PALAPI uint8_t is_end_of_line(char ch);
PALAPI uint8_t is_whitespace(char ch);
PALAPI uint8_t is_number(char ch);
PALAPI uint8_t is_underscore(char ch);
PALAPI uint8_t is_hyphen(char ch);
PALAPI uint8_t is_dot(char ch);
PALAPI uint8_t are_chars_equal(char ch1, char ch2);
PALAPI uint8_t are_strings_equal(int count, char* str1, char* str2);

void* load_dynamic_library(char* dll);
void* load_dynamic_function(void* dll, char* func_name);
uint8_t free_dynamic_library(void* dll);


#ifdef __cplusplus
}
#endif

#endif //PAL_H

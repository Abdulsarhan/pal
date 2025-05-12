#ifndef SAL_H
#define SAL_H

#include <stdint.h> // For Clearly Defined Types.
#include <sys/stat.h> // For time_t and stat.

typedef struct VideoMode {
	int width;
	int height;
}VideoMode;

typedef struct Window Window;
typedef struct Monitor Monitor;
typedef struct ProcAddress ProcAddress;

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

#if defined(__cplusplus)
#define CLITERAL(type)      type
#else
#define CLITERAL(type)      (type)
#endif

#ifdef __cplusplus
extern "C" {
#endif

SALAPI void init_sal();
SALAPI Window* init_window(int width, int height, const char* windowTitle);
SALAPI void set_window_title(Window* window, const char* string);
SALAPI void set_window_hint(int type, int value);
SALAPI VideoMode* set_video_mode(Monitor* monitor);
SALAPI Monitor* get_primary_monitor(void);
SALAPI ProcAddress* gl_get_proc_address(const char* proc);
SALAPI int register_input_devices(Window* window);
SALAPI uint8_t window_should_close(void);
SALAPI void poll_events(void);
SALAPI int make_context_current(Window* window);

SALAPI void begin_drawing(void);
SALAPI void DrawTriangle(void);
SALAPI void end_drawing(void);

SALAPI uint8_t does_file_exist(const char* file_path);
SALAPI time_t get_file_timestamp(const char* file);
SALAPI long get_file_size(const char* file_path);
SALAPI char* read_file(const char* filePath, int* fileSize, char* buffer);
SALAPI void write_file(const char* filePath, char* buffer, int size);
SALAPI uint8_t copy_file(const char* fileName, const char* outputName, char* buffer);

SALAPI uint8_t is_upper_case(char ch);
SALAPI uint8_t is_lower_case(char ch);
SALAPI uint8_t is_letter(char ch);
SALAPI uint8_t is_end_of_line(char ch);
SALAPI uint8_t is_whitespace(char ch);
SALAPI uint8_t is_number(char ch);
SALAPI uint8_t is_underscore(char ch);
SALAPI uint8_t is_hyphen(char ch);
SALAPI uint8_t is_dot(char ch);
SALAPI uint8_t are_chars_equal(char ch1, char ch2);
SALAPI uint8_t are_strings_equal(int count, char* str1, char* str2);

void* LoadDynamicLibrary(char* dll);
void* LoadDynamicFunction(void* dll, char* func_name);
uint8_t FreeDynamicLibrary(void* dll);


#ifdef __cplusplus
}
#endif

#endif //SAL_H

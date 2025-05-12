#ifndef SAL_H
#define SAL_H

#include <stdint.h>

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

SALAPI void InitSal();
SALAPI Window* InitWindow(int width, int height, const char* windowTitle);
SALAPI void SetWindowTitle(Window* window, const char* string);
SALAPI void SetWindowHint(int type, int value);
SALAPI VideoMode* GetVideoMode(Monitor* monitor);
SALAPI Monitor* GetPrimaryMonitor(void);
SALAPI ProcAddress* GlGetProcAddress(const char* proc);
SALAPI int RegisterInputDevices(Window* window);
SALAPI uint8_t WindowShouldClose(void);
SALAPI void PollEvents(void);
SALAPI int MakeContextCurrent(Window* window);

SALAPI void BeginDrawing(void);
SALAPI void DrawTriangle(void);
SALAPI void EndDrawing(void);

SALAPI uint8_t IsCapitalLetter(char ch);
SALAPI uint8_t IsLowerCaseLetter(char ch);
SALAPI uint8_t IsLetter(char ch);
SALAPI uint8_t IsEndOfLine(char ch);
SALAPI uint8_t IsWhiteSpace(char ch);
SALAPI uint8_t IsNumber(char ch);
SALAPI uint8_t IsUnderscore(char ch);
SALAPI uint8_t IsHyphen(char ch);
SALAPI uint8_t IsDot(char ch);
SALAPI uint8_t AreCharsEqual(char ch1, char ch2);
SALAPI uint8_t AreStringsEqual(int count, char* str1, char* str2);
void* LoadDynamicLibrary(char* dll);
void* LoadDynamicFunction(void* dll, char* func_name);
uint8_t FreeDynamicLibrary(void* dll);

#ifdef __cplusplus
}
#endif

#endif //SAL_H

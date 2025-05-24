#ifndef PAL_PLATFORM_H
#define PAL_PLATFORM_H

#ifdef __cplusplus
#define extern "C" {
#endif

static void platform_set_window_hint(int type, int value);
static Window* platform_init_window(int width, int height, const char* windowTitle);
static VideoMode* platform_get_video_mode(Monitor* monitor);
static Monitor* platform_get_primary_monitor(void);
static uint8_t platform_window_should_close(void);
static void platform_poll_events(void);
static void* platform_gl_get_proc_address(const char* proc);
static int platform_init_sound(SoundInitInfo* info);
static int platform_make_context_current(Window* window);
static int platform_play_sound( const Sound* sound);
static int platform_register_raw_input_devices(Window* window);

static void platform_begin_drawing();
static void platform_end_drawing();
static void Win32WindowResizeCallback(HWND hwnd, UINT flag, int width, int height); // This probably shouldn't be here.
static void* platform_load_dynamic_library(char* dll);
static void* platform_load_dynamic_function(void* dll, char* func_name);
static uint8_t platform_free_dynamic_library(void* dll);

#ifdef __cplusplus
}
#endif


#endif // PAL_PLATFORM_H
#ifndef PAL_PLATFORM_H
#define PAL_PLATFORM_H

//TODO: THIS WHOLE FILE MIGHT BE A BAD IDEA.
#ifdef __cplusplus
#define extern "C" {
#endif

static void platform_set_window_hint(int type, int value);
static pal_window* platform_init_window(int width, int height, const char* windowTitle);
static VideoMode* platform_get_video_mode(pal_monitor* monitor);
static pal_monitor* platform_get_primary_monitor(void);
static uint8_t platform_set_window_title(pal_window* window, const char* string);
static uint8_t platform_poll_events(pal_event* event, pal_window* window);
static void* platform_gl_get_proc_address(const char* proc);
static int platform_init_sound();
static int platform_make_context_current(pal_window* window);
static int platform_play_sound(const Sound* sound, float volume);
static int platform_register_raw_input_devices(pal_window* window);
static int platform_get_raw_input_buffer();
void platform_poll_gamepads(void);
v2 platform_get_mouse_position(pal_window* window);
static void platform_begin_drawing();
static void platform_end_drawing(pal_window* window);
static void Win32WindowResizeCallback(HWND hwnd, UINT flag, int width, int height); // This function probably shouldn't be here.
static void* platform_load_dynamic_library(char* dll);
static void* platform_load_dynamic_function(void* dll, char* func_name);
static uint8_t platform_free_dynamic_library(void* dll);

#ifdef __cplusplus
}
#endif


#endif // PAL_PLATFORM_H
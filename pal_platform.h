#ifndef PAL_PLATFORM_H
#define PAL_PLATFORM_H

// TODO: THIS WHOLE FILE MIGHT BE A BAD IDEA.
#ifdef __cplusplus
#define extern "C" {
#endif

static pal_window* platform_create_window(int width, int height, const char* windowTitle, uint64_t window_flags);
static pal_video_mode* platform_get_video_mode(pal_monitor* monitor);
static pal_bool platform_set_video_mode(pal_video_mode* mode);
static pal_monitor* platform_get_primary_monitor(void);
static uint8_t platform_set_window_title(pal_window* window, const char* string);
static uint8_t platform_poll_events(pal_event* event);
static void* platform_gl_get_proc_address(const char* proc);
static int platform_init_sound();
static int platform_make_context_current(pal_window* window);
static int platform_play_sound(pal_sound* sound, float volume);
static int platform_stop_sound(pal_sound* sound);
static int platform_get_raw_input_buffer();
pal_vec2 platform_get_mouse_position(pal_window* window);
static void platform_swap_buffers(pal_window* window);
static void* platform_load_dynamic_library(char* dll);
static void* platform_load_dynamic_function(void* dll, char* func_name);
static uint8_t platform_free_dynamic_library(void* dll);
void platform_set_window_icon(pal_window* window, const char* image_path);
void platform_set_window_icon_legacy(pal_window* window, const char* image_path);
pal_bool platform_make_window_windowed(pal_window* window);
pal_bool platform_make_window_fullscreen(pal_window* window);
pal_bool platform_minimize_window(pal_window* window);
pal_bool platform_maximize_window(pal_window* window);

#ifdef __cplusplus
}
#endif

#endif // PAL_PLATFORM_H

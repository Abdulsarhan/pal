#include "pal.h"
#define STB_VORBIS_IMPLEMENTATION
#include "stb_vorbis.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h"

#ifdef _WIN32
#include "win32_platform.h"
#elif __LINUX__
#include "linux_x11_platform.h"
#endif

PALAPI void pal_init() {
	platform_init_sound();
	get_device_handle();
    if (!platform_init_gamepads()) {
        printf("ERROR: %s: platform_init_gamepads failed\n", __func__);
    }
}

PALAPI void pal_shutdown() {
    platform_shutdown_gamepads();
}

/*

###########################################
	       WINDOW & MONITOR STUFF
###########################################

*/

PALAPI pal_window* pal_create_window(int width, int height, const char* window_title, uint64_t window_flags) {
	 return platform_create_window(width, height, window_title, window_flags);
}

PALAPI int pal_show_cursor() {
    return platform_show_cursor();
}

PALAPI int pal_hide_cursor() {
    return platform_hide_cursor();
}

PALAPI uint8_t pal_set_window_title(pal_window* window, const char* string) {
	return platform_set_window_title(window, string);
}

PALAPI pal_bool pal_make_window_fullscreen(pal_window* window) {
	return platform_make_window_fullscreen(window);
}

PALAPI pal_bool pal_make_window_fullscreen_ex(pal_window* window, int width, int height, int refresh_rate) {
	return platform_make_window_fullscreen_ex(window, width, height, refresh_rate);
}

PALAPI pal_bool pal_make_window_fullscreen_windowed(pal_window* window) {
	return platform_make_window_fullscreen_windowed(window);
}

PALAPI pal_bool pal_make_window_windowed(pal_window* window) {
	return platform_make_window_windowed(window);
}

PALAPI void pal_set_window_icon(pal_window* window, const char* image_path) {
	(void)platform_set_window_icon(window, image_path);
}

PALAPI void pal_set_window_icon_legacy(pal_window* window, const char* image_path) {
	(void)platform_set_window_icon_legacy(window, image_path);
}

PALAPI void pal_set_taskbar_icon(pal_window* taskbar, const char* image_path) {
	(void)platform_set_taskbar_icon(taskbar, image_path);
}

PALAPI void pal_set_taskbar_icon_legacy(pal_window* taskbar, const char* image_path) {
	(void)platform_set_taskbar_icon_legacy(taskbar, image_path);
}

PALAPI void pal_set_cursor(pal_window* window, const char* image_path, int size) {
	(void)platform_set_cursor(window, image_path, size);
}

PALAPI pal_video_mode* pal_get_video_mode(pal_monitor* monitor) {
	return platform_get_video_mode(monitor);
}

PALAPI pal_monitor* pal_get_primary_monitor(void) {
	return platform_get_primary_monitor();
}

PALAPI void* pal_gl_get_proc_address(const unsigned char* proc) {
	return platform_gl_get_proc_address(proc);
}

// Keyboard input
PALAPI uint8_t is_key_pressed(int key) {

	if (is_key_down(key) && !is_key_processed(key)) {
		set_key_processed(key);
		return 1;
	}
	else {
		return 0;
	}

}

PALAPI uint8_t is_key_down(int key) {
	return input.keys[key];

}

PALAPI uint8_t is_key_processed(int key) {
	return input.keys_processed[key];
}

PALAPI void set_key_processed(int key) {
	input.keys_processed[key] = 1;  // Mark as processed
}

// Mouse input
PALAPI pal_vec2 get_mouse_position(pal_window* window) {
	return platform_get_mouse_position(window);

}

PALAPI uint8_t is_mouse_pressed(int button) {

	if (is_mouse_down(button) && !is_mouse_processed(button)) {
		set_mouse_processed(button);
		return 1;
	}
	else {
		return 0;
	}
}

PALAPI uint8_t is_mouse_down(int button) {
	return input.mouse_buttons[button];
}

PALAPI uint8_t is_mouse_processed(int button) {
	return input.mouse_buttons_processed[button];
}

PALAPI void set_mouse_processed(int button) {
	input.mouse_buttons_processed[button] = 1; // Mark as processed
}

PALAPI int pal_get_gamepad_count() {
	return platform_get_gamepad_count();
}
PALAPI pal_bool pal_get_gamepad_state(int index, pal_gamepad_state* out_state) {
	return platform_gamepad_get_state(index, out_state);
}
PALAPI void pal_set_gamepad_vibration(int controller_id, float left_motor, float right_motor, float left_trigger, float right_trigger) {
	(void)platform_set_gamepad_vibration(controller_id, left_motor, right_motor, left_trigger, right_trigger);
}

PALAPI void pal_stop_gamepad_vibration(int controller_id) {
	(void)platform_stop_gamepad_vibration(controller_id);
}

PALAPI uint8_t pal_poll_events(pal_event* event, pal_window* window) {
	return platform_poll_events(event, window);
}

PALAPI int pal_make_context_current(pal_window* window) {
	return platform_make_context_current(window);
}

/*

###########################################
		   RENDERING FUNCTIONS.
###########################################

*/

PALAPI void pal_swap_buffers(pal_window* window) {
	(void)platform_swap_buffers(window);
}

/*

###########################################
		   SOUND FUNCTIONS.
###########################################

*/

pal_sound* pal_load_sound(const char* filename) {
	return platform_load_sound(filename, 0.0f);
}

PALAPI int pal_play_sound(pal_sound* sound, float volume) {
	return platform_play_sound(sound, volume);
}

PALAPI int pal_stop_sound(pal_sound* sound) {
	return platform_stop_sound(sound);
}

void pal_free_sound(pal_sound* sound) {
	platform_free_sound(sound);
}

pal_sound* pal_load_music(const char* filename) {
    // every loaded buffer will be this long.
    const float buffer_length_in_seconds = 2.0f;
	return platform_load_sound(filename, buffer_length_in_seconds);
}

PALAPI int pal_play_music(pal_sound* sound, float volume) {
	return platform_play_music(sound, volume);
}

/*
PALAPI int pal_stop_music(pal_sound* sound) {
	return platform_stop_music(sound);
}

*/
void pal_free_music(pal_sound* sound) {
	platform_free_music(sound);
}

// TODO: @fix This loads uncompressed .wav files only!
static int load_wav(const char* filename, pal_sound* out, float seconds) {
    FILE* file = fopen(filename, "rb");
    static const int WAV_FMT_PCM = 0x0001, WAV_FMT_IEEE_FLOAT = 0x0003, WAV_FMT_EXTENSIBLE = 0xFFFE;
    static const uint8_t SUBFORMAT_PCM[16] = {
        0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00,
        0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71, 0x00, 0x00
    };
    static const uint8_t SUBFORMAT_IEEE_FLOAT[16] = {
        0x03, 0x00, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00,
        0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71, 0x00, 0x00
    };
    char riffHeader[4];
    uint32_t riffSize;
    char waveID[4];
    if (fread(riffHeader, 1, 4, file) != 4 ||
        fread(&riffSize, 4, 1, file) != 1 ||
        fread(waveID, 1, 4, file) != 4) {
        return -1;
    }
    if (memcmp(riffHeader, "RIFF", 4) != 0 || memcmp(waveID, "WAVE", 4) != 0) {
        return -1;
    }
    int audioFormat = 0;
    int numChannels = 0;
    int sample_rate = 0;
    int bits_per_sample = 0;
    int is_float = 0;
    uint32_t data_size = 0;
    uint32_t data_offset = 0;
    while (!feof(file)) {
        char chunkID[4];
        uint32_t chunkSize;
        if (fread(chunkID, 1, 4, file) != 4) break;
        if (fread(&chunkSize, 4, 1, file) != 1) break;
        if (memcmp(chunkID, "fmt ", 4) == 0) {
            uint16_t formatTag;
            if (fread(&formatTag, 2, 1, file) != 1) return -1;
            if (fread(&numChannels, 2, 1, file) != 1) return -1;
            if (fread(&sample_rate, 4, 1, file) != 1) return -1;
            if (fseek(file, 6, SEEK_CUR) != 0) return -1;
            if (fread(&bits_per_sample, 2, 1, file) != 1) return -1;
            if (formatTag == WAV_FMT_EXTENSIBLE && chunkSize >= 40) {
                uint16_t cbSize;
                if (fread(&cbSize, sizeof(cbSize), 1, file) != 1) return -1;
                if (cbSize < 22) return -1;
                if (fseek(file, 6, SEEK_CUR) != 0) return -1;
                uint8_t subFormat[16];
                if (fread(subFormat, 1, 16, file) != 16) return -1;
                if (memcmp(subFormat, SUBFORMAT_PCM, 16) == 0) {
                    audioFormat = WAV_FMT_PCM;
                    is_float = 0;
                } else if (memcmp(subFormat, SUBFORMAT_IEEE_FLOAT, 16) == 0) {
                    audioFormat = WAV_FMT_IEEE_FLOAT;
                    is_float = 1;
                } else {
                    return -1;
                }
            } else {
                audioFormat = formatTag;
                is_float = (audioFormat == WAV_FMT_IEEE_FLOAT) ? 1 : 0;
                if (audioFormat != WAV_FMT_PCM && audioFormat != WAV_FMT_IEEE_FLOAT) return -1;
                if (chunkSize > 16) {
                    if (fseek(file, chunkSize - 16, SEEK_CUR) != 0) return -1;
                }
            }
        }
        else if (memcmp(chunkID, "data", 4) == 0) {
            data_size = chunkSize;
            data_offset = (uint32_t)ftell(file);
            uint32_t bytes_per_sample = (bits_per_sample / 8) * numChannels;
            uint32_t preload_bytes;
            if (seconds == 0.0f) {
                preload_bytes = chunkSize;
            } else {
                preload_bytes = (uint32_t)(seconds * sample_rate * bytes_per_sample);
                if (preload_bytes > chunkSize) {
                    preload_bytes = chunkSize;
                }
            }
            void* audioData = malloc(preload_bytes);
            if (!audioData) return -1;
            if (fread(audioData, 1, preload_bytes, file) != preload_bytes) {
                free(audioData);
                return -1;
            }
            out->data = audioData;
            out->data_size = preload_bytes;
            
            // Debug: Show first 8 bytes of initial buffer
            if (preload_bytes >= 8) {
                unsigned char* debug_data = (unsigned char*)audioData;
                printf("Initial buffer first 8 bytes: %02X %02X %02X %02X %02X %02X %02X %02X\n",
                       debug_data[0], debug_data[1], debug_data[2], debug_data[3],
                       debug_data[4], debug_data[5], debug_data[6], debug_data[7]);
            }
        }
        else {
            if (fseek(file, (chunkSize + 1) & ~1, SEEK_CUR) != 0) return -1;
        }
    }
    if (out->data == NULL || data_size == 0 ||
        (audioFormat != WAV_FMT_PCM && audioFormat != WAV_FMT_IEEE_FLOAT)) {
        free(out->data);
        return -1;
    }
    out->sample_rate = sample_rate;
    out->channels = numChannels;
    out->bits_per_sample = bits_per_sample;
    out->is_float = is_float;
    out->data_offset = data_offset;
    
    // Debug: Show the audio format information
    printf("WAV format: audioFormat=%d, channels=%d, sample_rate=%d, bits_per_sample=%d, is_float=%d\n",
           audioFormat, numChannels, sample_rate, bits_per_sample, is_float);
    
    if (seconds > 0.0f) { // Streaming mode - keep file open and set up streaming metadata
        out->source_file = file;
        out->total_data_size = data_size;           // Total size of audio data in file
        out->bytes_streamed = out->data_size;      // How many bytes already loaded into initial buffer
        
        printf("WAV streaming setup: total=%zu bytes, preloaded=%zu bytes, data_offset=%zu\n",
               out->total_data_size, out->data_size, out->data_offset);
    }
    else { // Non-streaming mode - close file
        fclose(file);
        out->source_file = NULL;
        out->total_data_size = 0;
        out->bytes_streamed = 0;
    }
    return 1;
}
static int load_ogg(const char* filename, pal_sound* out, float seconds) {
    int channels, sample_rate;
    int error;
    stb_vorbis* vorbis = stb_vorbis_open_filename(filename, &error, NULL);
    if (!vorbis) {
        printf("Failed to open Ogg file (error code %d).\n", error);
        return -1;
    }
    
    stb_vorbis_info info = stb_vorbis_get_info(vorbis);
    channels = info.channels;
    sample_rate = info.sample_rate;
    
    // Calculate how many sample frames to preload based on seconds
    size_t total_sample_frames = stb_vorbis_stream_length_in_samples(vorbis);
    size_t target_sample_frames = (size_t)(sample_rate * (seconds));
    
    if (seconds <= 0.0f || target_sample_frames > total_sample_frames) {
        target_sample_frames = total_sample_frames;
    }
    
    printf("OGG Load: Requesting %lld sample frames (%.3f seconds) from %lld total\n",
           target_sample_frames, (float)target_sample_frames / sample_rate, total_sample_frames);
    
    // Allocate buffer for preload samples (16-bit signed shorts, interleaved)
    short* pcm_data = (short*)malloc((size_t)(target_sample_frames * channels * sizeof(short)));
    if (!pcm_data) {
        stb_vorbis_close(vorbis);
        return -1;
    }
    
    // Use the non-interleaved API which is more predictable
    float** channel_buffers = (float**)malloc(channels * sizeof(float*));
    for (int i = 0; i < channels; i++) {
        channel_buffers[i] = (float*)malloc(target_sample_frames * sizeof(float));
    }
    
    size_t total_decoded = 0;
    size_t start_position = stb_vorbis_get_sample_offset(vorbis);
    
    while (total_decoded < target_sample_frames) {
        int samples_to_read = (int)(target_sample_frames - total_decoded);
        
        // Use non-interleaved API
        int samples_read = stb_vorbis_get_samples_float(
            vorbis, channels, channel_buffers, samples_to_read);
        
        if (samples_read <= 0) {
            printf("OGG Load: Decoder returned %d sample frames, stopping\n", samples_read);
            break;
        }
        
        // Convert float samples to interleaved 16-bit shorts
        for (int sample = 0; sample < samples_read; sample++) {
            for (int ch = 0; ch < channels; ch++) {
                float f_sample = channel_buffers[ch][sample];
                // Clamp and convert to 16-bit
                if (f_sample > 1.0f) f_sample = 1.0f;
                if (f_sample < -1.0f) f_sample = -1.0f;
                short s_sample = (short)(f_sample * 32767.0f);
                pcm_data[(total_decoded + sample) * channels + ch] = s_sample;
            }
        }
        
        total_decoded += samples_read;
        
        unsigned int current_position = stb_vorbis_get_sample_offset(vorbis);
        printf("OGG Load: Read %d sample frames, total: %lld/%lld, decoder: %u\n", 
               samples_read, total_decoded, target_sample_frames, current_position);
    }
    
    // Clean up temporary buffers
    for (int i = 0; i < channels; i++) {
        free(channel_buffers[i]);
    }
    free(channel_buffers);
    
    // Verify final decoder position
    int final_position = stb_vorbis_get_sample_offset(vorbis);
    size_t expected_position = start_position + total_decoded;
    
    printf("OGG Load: Finished - loaded %lld sample frames\n", total_decoded);
    printf("OGG Load: Decoder position: %zu -> %d (expected %zu)\n", 
           start_position, final_position, expected_position);
    
    if (final_position != (int)expected_position) {
        printf("WARNING: Decoder position mismatch! Expected %zu, got %d\n", 
               expected_position, final_position);
    }
    
    out->data = (unsigned char*)pcm_data;
    out->data_size = (size_t)(total_decoded * channels * sizeof(short));
    out->channels = channels;
    out->sample_rate = sample_rate;
    out->bits_per_sample = 16;
    out->is_float = 0;
    
    // Store decoder for streaming later
    out->decoder = vorbis;
    
    printf("OGG Load: Final data_size = %zu bytes (%zu sample frames)\n", 
           out->data_size, total_decoded);
    
    return 1;
}

/*

###########################################
	     LIBRARY LOADING FUNCTIONS
###########################################

*/

void* load_dynamic_library(char* dll) {
	return platform_load_dynamic_library(dll);

}

void* load_dynamic_function(void* dll, char* func_name) {
	return platform_load_dynamic_function(dll, func_name);

}

uint8_t free_dynamic_library(void* dll) {
	return platform_free_dynamic_library(dll);

}

/*

###########################################
				 File I/O
###########################################

*/

uint8_t pal_does_file_exist(const char* file_path) {
    return platform_does_file_exist(file_path);
}

size_t pal_get_last_read_time(const char* file_path) {
    return platform_get_last_read_time(file_path);
}

size_t pal_get_last_write_time(const char* file_path) {
    return platform_get_last_write_time(file_path);
}

size_t pal_get_file_size(const char* file_path) {
    return platform_get_file_size(file_path);
}

uint32_t pal_get_file_permissions(const char* file_path) {
    return platform_get_file_permissions(file_path);
}

uint8_t pal_change_file_permissions(const char* file_path, uint32_t permission_flags) {
    return platform_change_file_permissions(file_path, permission_flags);
}

uint8_t pal_read_file(const char* file_path, char* buffer) {
    return platform_read_file(file_path, buffer);
}

uint8_t pal_write_file(const char* file_path, size_t file_size, char* buffer) {
    return platform_write_file(file_path, file_size, buffer);
}

uint8_t pal_copy_file(const char* original_path, const char* copy_path) {
    return platform_copy_file(original_path, copy_path);
}

/*

###########################################
		   STRING PARSING HELPERS
###########################################

*/
enum {
    PAL_UPPER_BIT    = (1 << 0),  // A-Z
    PAL_LOWER_BIT    = (1 << 1),  // a-z
    PAL_DIGIT_BIT    = (1 << 2),  // 0-9
    PAL_UNDER_BIT    = (1 << 3),  // _
    PAL_HYPHEN_BIT   = (1 << 4),  // -
    PAL_DOT_BIT      = (1 << 5),  // .
    PAL_EOL_BIT      = (1 << 6),  // \r, \n (included in whitespace)
    PAL_WHITESPACE_BIT = (1 << 7) // All whitespace chars
};

static const uint8_t pal_char_masks[128] = {
    // Control characters (0-31)
    [0] = 0, [1] = 0, [2] = 0, [3] = 0, [4] = 0, [5] = 0, [6] = 0, [7] = 0,
    [8] = 0, 
    ['\t'] = PAL_WHITESPACE_BIT,    // tab
    ['\n'] = PAL_WHITESPACE_BIT,    // new line
    ['\v'] = PAL_WHITESPACE_BIT,    // vertical tab (not used anymore)
    ['\f'] = PAL_WHITESPACE_BIT,    // form feed (not used anymore)
    ['\r'] = PAL_WHITESPACE_BIT,    // carriage return
    [14] = 0, [15] = 0, [16] = 0, [17] = 0, [18] = 0, [19] = 0, [20] = 0, [21] = 0,
    [22] = 0, [23] = 0, [24] = 0, [25] = 0, [26] = 0, [27] = 0, [28] = 0, [29] = 0,
    [30] = 0, [31] = 0,
    
    // Printable characters (32-127)
    [' '] = PAL_WHITESPACE_BIT,   // Space
    
    ['!'] = 0, ['"'] = 0, ['#'] = 0, ['$'] = 0, ['%'] = 0, ['&'] = 0, ['\''] = 0,
    ['('] = 0, [')'] = 0, ['*'] = 0, ['+'] = 0, [','] = 0,
    ['-'] = PAL_HYPHEN_BIT,
    ['.'] = PAL_DOT_BIT,
    ['/'] = 0,
    
    // Numbers (0-9)
    ['0'] = PAL_DIGIT_BIT,
    ['1'] = PAL_DIGIT_BIT,
    ['2'] = PAL_DIGIT_BIT,
    ['3'] = PAL_DIGIT_BIT,
    ['4'] = PAL_DIGIT_BIT,
    ['5'] = PAL_DIGIT_BIT,
    ['6'] = PAL_DIGIT_BIT,
    ['7'] = PAL_DIGIT_BIT,
    ['8'] = PAL_DIGIT_BIT,
    ['9'] = PAL_DIGIT_BIT,
    
    [':'] = 0, [';'] = 0, ['<'] = 0, ['='] = 0, ['>'] = 0, ['?'] = 0, ['@'] = 0,
    
    // Uppercase (A-Z)
    ['A'] = PAL_UPPER_BIT, ['B'] = PAL_UPPER_BIT, ['C'] = PAL_UPPER_BIT,
    ['D'] = PAL_UPPER_BIT, ['E'] = PAL_UPPER_BIT, ['F'] = PAL_UPPER_BIT,
    ['G'] = PAL_UPPER_BIT, ['H'] = PAL_UPPER_BIT, ['I'] = PAL_UPPER_BIT,
    ['J'] = PAL_UPPER_BIT, ['K'] = PAL_UPPER_BIT, ['L'] = PAL_UPPER_BIT,
    ['M'] = PAL_UPPER_BIT, ['N'] = PAL_UPPER_BIT, ['O'] = PAL_UPPER_BIT,
    ['P'] = PAL_UPPER_BIT, ['Q'] = PAL_UPPER_BIT, ['R'] = PAL_UPPER_BIT,
    ['S'] = PAL_UPPER_BIT, ['T'] = PAL_UPPER_BIT, ['U'] = PAL_UPPER_BIT,
    ['V'] = PAL_UPPER_BIT, ['W'] = PAL_UPPER_BIT, ['X'] = PAL_UPPER_BIT,
    ['Y'] = PAL_UPPER_BIT, ['Z'] = PAL_UPPER_BIT,
    
    ['['] = 0, ['\\'] = 0, [']'] = 0, ['^'] = 0,
    ['_'] = PAL_UNDER_BIT,
    ['`'] = 0,
    
    // Lowercase (a-z)
    ['a'] = PAL_LOWER_BIT, ['b'] = PAL_LOWER_BIT, ['c'] = PAL_LOWER_BIT,
    ['d'] = PAL_LOWER_BIT, ['e'] = PAL_LOWER_BIT, ['f'] = PAL_LOWER_BIT,
    ['g'] = PAL_LOWER_BIT, ['h'] = PAL_LOWER_BIT, ['i'] = PAL_LOWER_BIT,
    ['j'] = PAL_LOWER_BIT, ['k'] = PAL_LOWER_BIT, ['l'] = PAL_LOWER_BIT,
    ['m'] = PAL_LOWER_BIT, ['n'] = PAL_LOWER_BIT, ['o'] = PAL_LOWER_BIT,
    ['p'] = PAL_LOWER_BIT, ['q'] = PAL_LOWER_BIT, ['r'] = PAL_LOWER_BIT,
    ['s'] = PAL_LOWER_BIT, ['t'] = PAL_LOWER_BIT, ['u'] = PAL_LOWER_BIT,
    ['v'] = PAL_LOWER_BIT, ['w'] = PAL_LOWER_BIT, ['x'] = PAL_LOWER_BIT,
    ['y'] = PAL_LOWER_BIT, ['z'] = PAL_LOWER_BIT,
    
    ['{'] = 0, ['|'] = 0, ['}'] = 0, ['~'] = 0,
    [127] = 0  // DEL
};

// Character classification functions
PALAPI uint8_t pal_is_uppercase(char ch) {
    return pal_char_masks[(uint8_t)ch] & PAL_UPPER_BIT;
}

PALAPI uint8_t pal_is_lowercase(char ch) {
    return pal_char_masks[(uint8_t)ch] & PAL_LOWER_BIT;
}

PALAPI uint8_t pal_is_letter(char ch) {
    return pal_char_masks[(uint8_t)ch] & (PAL_UPPER_BIT | PAL_LOWER_BIT);
}

PALAPI uint8_t pal_is_number(char ch) {
    return pal_char_masks[(uint8_t)ch] & PAL_DIGIT_BIT;
}

PALAPI uint8_t pal_is_end_of_line(char ch) {
    return pal_char_masks[(uint8_t)ch] & PAL_EOL_BIT;
}

PALAPI uint8_t pal_is_underscore(char ch) {
    return pal_char_masks[(uint8_t)ch] & PAL_UNDER_BIT;
}

PALAPI uint8_t pal_is_hyphen(char ch) {
    return pal_char_masks[(uint8_t)ch] & PAL_HYPHEN_BIT;
}

PALAPI uint8_t pal_is_dot(char ch) {
    return pal_char_masks[(uint8_t)ch] & PAL_DOT_BIT;
}
PALAPI uint8_t pal_is_whitespace(char ch) {
    return pal_char_masks[(uint8_t)ch] & PAL_WHITESPACE_BIT;
}

// Character comparison
PALAPI uint8_t pal_are_chars_equal(char ch1, char ch2) {
    return (uint8_t)ch1 == (uint8_t)ch2;
}

// String comparison
PALAPI uint8_t pal_are_strings_equal(int count, const char* str1, const char* str2) {
    if (str1 == NULL || str2 == NULL) {
        return 0;
    }
    
    for (int i = 0; i < count; i++) {
        if (!pal_are_chars_equal(str1[i], str2[i])) {
            return 0;
        }
    }
    return 1;
}

pal_bool pal_eventq_free(pal_event_queue* queue);

pal_bool pal_eventq_free(pal_event_queue* queue) {
    if(queue->events) {
        free(queue->events);
        return 1;
    }
    else {
        fprintf(stderr, "ERROR: pal_eventq_free(): Tried to free a queue that was already freed!\n");
        queue->events = NULL;
        return 0;
    }
}

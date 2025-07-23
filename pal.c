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
}

/*

###########################################
	       WINDOW & MONITOR STUFF
###########################################

*/

PALAPI pal_window* pal_create_window(int width, int height, const char* windowTitle, uint64_t window_flags) {
	 return platform_create_window(width, height, windowTitle, window_flags);
}

PALAPI uint8_t pal_set_window_title(pal_window* window, const char* string) {
	return platform_set_window_title(window, string);
}

PALAPI pal_bool pal_make_window_fullscreen(pal_window* window) {
	return platform_make_window_fullscreen(window);
}

PALAPI pal_bool pal_make_window_fullscreen_ex(pal_window* window, int width, int height, int refreshrate) {
	return platform_make_window_fullscreen_ex(window, width, height, refreshrate);
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

PALAPI void* gl_get_proc_address(const unsigned char* proc) {
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
PALAPI v2 get_mouse_position(pal_window* window) {
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

PALAPI void begin_drawing(void) {
	(void)platform_begin_drawing();
}

PALAPI void DrawTriangle() {

}

PALAPI void end_drawing(pal_window* window) {
	(void)platform_end_drawing(window);
}

/*

###########################################
		   SOUND FUNCTIONS.
###########################################

*/

// loader for uncompressed .wav and ogg vorbis files.
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

// loader for uncompressed .wav and ogg vorbis files.
pal_sound* pal_load_music(const char* filename) {
	return platform_load_sound(filename, 2.0f);
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
static int load_wav(const char* filename, pal_sound* out, float* seconds) {
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
            if (*seconds == 0.0f) {
                preload_bytes = chunkSize;
            } else {
                preload_bytes = (uint32_t)(*seconds * sample_rate * bytes_per_sample);
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
    
    if (*seconds > 0.0f) { // Streaming mode - keep file open and set up streaming metadata
        out->source_file = file;
        out->total_data_size = data_size;           // Total size of audio data in file
        out->bytes_streamed = out->data_size;      // How many bytes already loaded into initial buffer
        
        printf("WAV streaming setup: total=%u bytes, preloaded=%u bytes, data_offset=%u\n",
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

static int load_ogg(const char* filename, pal_sound* out, float* seconds) {
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

    // Calculate how many samples to preload based on seconds
    int64_t total_samples = stb_vorbis_stream_length_in_samples(vorbis);
    int64_t target_samples = (int64_t)(sample_rate * *seconds);
    if (*seconds <= 0.0f || target_samples > total_samples) {
        target_samples = total_samples;
    }

    // Allocate buffer for preload samples (16-bit signed shorts)
    short* pcm_data = (short*)malloc((size_t)(target_samples * channels * sizeof(short)));
    if (!pcm_data) {
        stb_vorbis_close(vorbis);
        return -1;
    }

    int64_t total_decoded = 0;
    while (total_decoded < target_samples) {
        int samples_to_read = (int)(target_samples - total_decoded);
        int samples_read = stb_vorbis_get_samples_short_interleaved(
            vorbis,
            channels,
            pcm_data + total_decoded * channels,
            samples_to_read
        );

        if (samples_read <= 0) break;
        total_decoded += samples_read;
    }

    out->data = (unsigned char*)pcm_data;
    out->data_size = (size_t)(total_decoded * channels * sizeof(short));
    out->channels = channels;
    out->sample_rate = sample_rate;
    out->bits_per_sample = 16;
    out->is_float = 0;

    // Store decoder and preload state for streaming later
    out->decoder = vorbis;

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

uint8_t does_file_exist(const char* file_path) {

	assert(file_path != NULL);
	FILE* file = fopen(file_path, "rb");
	if (!file)
	{
		return 0;
	}
	fclose(file);

	return 1;
}

time_t get_file_timestamp(const char* file) {
	struct stat file_stat = { 0 };
	stat(file, &file_stat);
	return file_stat.st_mtime;
}

long get_file_size(const char* file_path) {
	assert(file_path != NULL);
	long fileSize = 0;
	FILE* file = fopen(file_path, "rb");
	if (!file)
	{
		fprintf(stderr, "ERROR: Failed to open file!\n");
		return 0;
	}

	fseek(file, 0, SEEK_END);
	fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	fclose(file);

	return fileSize;
}

char* read_file(const char* filePath, int* fileSize, char* buffer) {
	assert(filePath != NULL);
	assert(fileSize != NULL);
	assert(buffer != NULL);

	*fileSize = 0;
	FILE* file = fopen(filePath, "rb");
	if (!file)
	{
		fprintf(stderr, "ERROR: Failed to open file!\n");
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	*fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	memset(buffer, 0, *fileSize + 1);
	fread(buffer, sizeof(char), *fileSize, file);

	fclose(file);

	return buffer;
}

void write_file(const char* filePath, char* buffer, int size) {
	assert(filePath != NULL);
	assert(buffer != NULL);

	FILE* file = fopen(filePath, "wb");
	if (!file)
	{
		fprintf(stderr, "ERROR: Failed to open file!\n");
		return;
	}

	fwrite(buffer, sizeof(char), size, file);
	fclose(file);
}

uint8_t copy_file(const char* fileName, const char* outputName, char* buffer) {
	int fileSize = 0;
	char* data = read_file(fileName, &fileSize, buffer);

	FILE* outputFile = fopen(outputName, "wb");
	if (!outputFile)
	{
		fprintf(stderr, "ERROR: Failed to open file!\n");
		return 0;
	}

	size_t result = fwrite(data, sizeof(char), fileSize, outputFile);
	if (!result)
	{
		fprintf(stderr, "ERROR: Failed to open file!\n");
		return 0;
	}

	fclose(outputFile);

	return 1;
}

/*

###########################################
		   STRING PARSING HELPERS
###########################################

*/

PALAPI uint8_t is_upper_case(char ch) {
	return ((ch >= 'A') && (ch <= 'Z'));
}

PALAPI uint8_t is_lower_case(char ch) {
	return ((ch >= 'a') && (ch <= 'z'));
}

PALAPI uint8_t is_letter(char ch) {
	return (is_upper_case(ch) || is_lower_case(ch));
}

PALAPI uint8_t is_end_of_line(char ch) {
	return ((ch == '\r') || (ch == '\n'));
}

PALAPI uint8_t is_whitespace(char ch) {
	return ((ch == ' ') || (ch == '\t') || (ch == '\v') || (ch == '\f'));
}

PALAPI uint8_t is_number(char ch) {
	return ((ch >= '0') && (ch <= '9'));
}

PALAPI uint8_t is_underscore(char ch) {
	return (ch == '_');
}

PALAPI uint8_t is_hyphen(char ch) {
	return (ch == '-');
}

PALAPI uint8_t is_dot(char ch) {
	return (ch == '.');
}

PALAPI uint8_t are_chars_equal(char ch1, char ch2) {
	return (ch1 == ch2);
}

PALAPI uint8_t are_strings_equal(int count, char* str1, char* str2) {
	for (int i = 0; i < count; i++) {
		if (str1 == NULL || str2 == NULL)
			return 0;
		if (*str1 != *str2) {
			return 0;
		}
		str1++;
		str2++;
	}
	return 1;
}

// capacity and size are in bytes,
// not events.

// There is only a single event queue, and it should be isntan
size_t pal_eventq_get_size(pal_event_queue* queue);
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

// ---------------> back
// [0][1][2][3][4]
// <--------------- front


size_t pal_eventq_get_size(pal_event_queue* queue) {
    return queue->size;
}



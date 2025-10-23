#pragma warning(disable : 4996)
#include "pal.h"
#include "stb_vorbis.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h"

pal_event_queue g_event_queue = {0};

pal_bool pal__init_eventq() {

    // -- CREATE QUEUE FOR THE WINDOW --
    size_t capacity = 10000;
    pal_event* events = (pal_event*)malloc((capacity * sizeof(pal_event)));

    if (events == NULL) {
        fprintf(stderr, "ERROR: %s: failed to allocate memory for events!\n", __func__);
        return 0;
    }

    g_event_queue = (pal_event_queue){
        // size and capacity are measured in pal_events, not bytes.
        .size = 0,
        .capacity = capacity,
        .front = 0,
        .back = 0,
        .events = events
    };

	return 1;
}

// enqueue
void pal__eventq_push(pal_event_queue* queue, pal_event event) {
	if (queue->size == queue->capacity) {
		fprintf(stderr, "ERROR: %s(): Event queue->size has reached capacity. Not going to enqueue->\n", __func__);
	}
	queue->events[queue->back] = event;
	queue->back = (queue->back + 1) % queue->capacity;
	queue->size++;
}

pal_bool pal__eventq_free(pal_event_queue queue) {
    if (queue.events) {
        free(queue.events);
        return 1;
    } else {
        fprintf(stderr, "ERROR: %s(): Tried to free a queue that was already freed!\n", __func__);
        queue.events = NULL;
        return 0;
    }
}

#ifdef _WIN32
#include "win32_platform.h"
#elif __LINUX__
#include "linux_x11_platform.h"
#endif

PALAPI void pal_init(void) {
    pal__init_eventq();
    win32_enumerate_keyboards();
    win32_enumerate_mice();
    win32_init_timer();
    win32_init_sound();
    if (!win32_init_gamepads()) {
        printf("ERROR: %s: win32_init_gamepads failed\n", __func__);
    }
}

PALAPI void pal_shutdown(void) {
    win32_shutdown_gamepads();
    pal__eventq_free(g_event_queue);
}

/*

###########################################
           WINDOW & MONITOR STUFF
###########################################

*/

// Keyboard input
PALAPI int pal_get_keyboard_count(void) {
    return g_keyboard_count;
}

PALAPI int pal_get_keyboard_indices(int key, int *keyboard_indices) {
    int count = 0;
    
    if (key == -1) {
        // Check if ANY key is pressed on each keyboard
        for (int i = 0; i < g_keyboard_count && count < MAX_KEYBOARDS; i++) {
            pal_keyboard_state *kb = &g_keyboards[i];
            for (int k = 0; k < 256; k++) {
                if (kb->keys[k] && !kb->keys_processed[k]) {
                    keyboard_indices[count++] = i;
                    kb->keys_processed[k] = 1;  // Mark as processed
                    break;  // Found a key on this keyboard, move to next keyboard
                }
            }
        }
    } else {
        // Check specific key
        for (int i = 0; i < g_keyboard_count && count < MAX_KEYBOARDS; i++) {
            pal_keyboard_state *kb = &g_keyboards[i];
            if (kb->keys[key] && !kb->keys_processed[key]) {
                keyboard_indices[count++] = i;
                kb->keys_processed[key] = 1;  // Mark as processed
            }
        }
    }
    
    return count;
}

PALAPI const char* pal_get_keyboard_name(int keyboard_id) {
    if (keyboard_id < 0 || keyboard_id >= g_keyboard_count) return NULL;
    return g_keyboards[keyboard_id].device_name;
}

PALAPI pal_bool pal_is_key_down(int keyboard_id, int key) {
    // Check all keyboards if keyboard_id is -1
    if (keyboard_id == -1) {
        for (int i = 0; i < g_keyboard_count; i++) {
            if (g_keyboards[i].keys[key]) return 1;
        }
        return 0;
    }
    
    if (keyboard_id < 0 || keyboard_id >= g_keyboard_count) return 0;
    return g_keyboards[keyboard_id].keys[key];
}

PALAPI pal_bool pal_is_key_pressed(int keyboard_id, int key) {
    // Check all keyboards if keyboard_id is -1
    if (keyboard_id == -1) {
        for (int i = 0; i < g_keyboard_count; i++) {
            pal_keyboard_state *kb = &g_keyboards[i];
            if (kb->keys[key] && !kb->keys_processed[key]) {
                kb->keys_processed[key] = 1;
                return 1;
            }
        }
        return 0;
    }
    
    if (keyboard_id < 0 || keyboard_id >= g_keyboard_count) return 0;
    pal_keyboard_state *kb = &g_keyboards[keyboard_id];
    if (kb->keys[key] && !kb->keys_processed[key]) {
        kb->keys_processed[key] = 1;
        return 1;
    }
    return 0;
}

PALAPI int pal_get_mouse_count(void) {
    return g_mouse_count;
}

PALAPI const char* pal_get_mouse_name(int mouse_id) {
    if (mouse_id < 0 || mouse_id >= g_mouse_count) return NULL;
    return g_mice[mouse_id].device_name;
}

PALAPI int pal_get_mouse_indices(int *mouse_indices) {
    int count = 0;
    
    for (int i = 0; i < g_mouse_count && count < MAX_MICE; i++) {
        mouse_state *m = &g_mice[i];
        
        // Check if mouse has delta movement
        if (m->delta_x != 0 || m->delta_y != 0) {
            mouse_indices[count++] = i;
            continue;
        }
        
        // Check if any button is pressed
        for (int b = 0; b < 8; b++) {
            if (m->buttons[b] && !m->buttons_processed[b]) {
                mouse_indices[count++] = i;
                m->buttons_processed[b] = 1;
                break;
            }
        }
    }
    
    return count;
}

PALAPI pal_bool pal_is_mouse_down(int mouse_id, int button) {
    if (mouse_id == -1) {
        for (int i = 0; i < g_mouse_count; i++) {
            if (g_mice[i].buttons[button]) return 1;
        }
        return 0;
    }
    
    if (mouse_id < 0 || mouse_id >= g_mouse_count) return 0;
    return g_mice[mouse_id].buttons[button];
}
PALAPI pal_bool pal_is_mouse_pressed(int mouse_id, int button) {
    if (mouse_id == -1) {
        for (int i = 0; i < g_mouse_count; i++) {
            mouse_state *m = &g_mice[i];
            if (m->buttons[button] && !m->buttons_processed[button]) {
                m->buttons_processed[button] = 1;
                return 1;
            }
        }
        return 0;
    }
    
    if (mouse_id < 0 || mouse_id >= g_mouse_count) return 0;
    mouse_state *m = &g_mice[mouse_id];
    if (m->buttons[button] && !m->buttons_processed[button]) {
        m->buttons_processed[button] = 1;
        return 1;
    }
    return 0;
}

// Get delta for specific mouse (useful if you want per-mouse tracking)
PALAPI pal_vec2 pal_get_mouse_delta(int mouse_id) {
    if (mouse_id < 0 || mouse_id >= g_mouse_count) {
        return (pal_vec2){0, 0};  // Return zero vector for invalid ID
    }
    
    return (pal_vec2){
        (float)g_mice[mouse_id].delta_x,
        (float)g_mice[mouse_id].delta_y
    };
}

PALAPI void pal__reset_mouse_deltas(void) {
    for (int i = 0; i < g_mouse_count; i++) {
        g_mice[i].delta_x = 0;
        g_mice[i].delta_y = 0;
    }
}

// TODO: @fix This loads uncompressed .wav files only!
static int pal__load_wav(const char* filename, pal_sound* out, float seconds) {
    FILE* file = fopen(filename, "rb");
    static const int WAV_FMT_PCM = 0x0001, WAV_FMT_IEEE_FLOAT = 0x0003, WAV_FMT_EXTENSIBLE = 0xFFFE;
    static const uint8_t SUBFORMAT_PCM[16] = {
        0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71, 0x00, 0x00};
    static const uint8_t SUBFORMAT_IEEE_FLOAT[16] = {
        0x03, 0x00, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71, 0x00, 0x00};
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
        if (fread(chunkID, 1, 4, file) != 4)
            break;
        if (fread(&chunkSize, 4, 1, file) != 1)
            break;
        if (memcmp(chunkID, "fmt ", 4) == 0) {
            uint16_t formatTag;
            if (fread(&formatTag, 2, 1, file) != 1)
                return -1;
            if (fread(&numChannels, 2, 1, file) != 1)
                return -1;
            if (fread(&sample_rate, 4, 1, file) != 1)
                return -1;
            if (fseek(file, 6, SEEK_CUR) != 0)
                return -1;
            if (fread(&bits_per_sample, 2, 1, file) != 1)
                return -1;
            if (formatTag == WAV_FMT_EXTENSIBLE && chunkSize >= 40) {
                uint16_t cbSize;
                if (fread(&cbSize, sizeof(cbSize), 1, file) != 1)
                    return -1;
                if (cbSize < 22)
                    return -1;
                if (fseek(file, 6, SEEK_CUR) != 0)
                    return -1;
                uint8_t subFormat[16];
                if (fread(subFormat, 1, 16, file) != 16)
                    return -1;
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
                if (audioFormat != WAV_FMT_PCM && audioFormat != WAV_FMT_IEEE_FLOAT)
                    return -1;
                if (chunkSize > 16) {
                    if (fseek(file, chunkSize - 16, SEEK_CUR) != 0)
                        return -1;
                }
            }
        } else if (memcmp(chunkID, "data", 4) == 0) {
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
            if (!audioData)
                return -1;
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
                       debug_data[0],
                       debug_data[1],
                       debug_data[2],
                       debug_data[3],
                       debug_data[4],
                       debug_data[5],
                       debug_data[6],
                       debug_data[7]);
            }
        } else {
            if (fseek(file, (chunkSize + 1) & ~1, SEEK_CUR) != 0)
                return -1;
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
           audioFormat,
           numChannels,
           sample_rate,
           bits_per_sample,
           is_float);

    if (seconds > 0.0f) { // Streaming mode - keep file open and set up streaming metadata
        out->source_file = file;
        out->total_data_size = data_size;     // Total size of audio data in file
        out->bytes_streamed = out->data_size; // How many bytes already loaded into initial buffer

        printf("WAV streaming setup: total=%zu bytes, preloaded=%zu bytes, data_offset=%zu\n",
               out->total_data_size,
               out->data_size,
               out->data_offset);
    } else { // Non-streaming mode - close file
        fclose(file);
        out->source_file = NULL;
        out->total_data_size = 0;
        out->bytes_streamed = 0;
    }
    return 1;
}

static int pal__load_ogg(const char* filename, pal_sound* out, float seconds) {
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
           target_sample_frames,
           (float)target_sample_frames / sample_rate,
           total_sample_frames);

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
                if (f_sample > 1.0f)
                    f_sample = 1.0f;
                if (f_sample < -1.0f)
                    f_sample = -1.0f;
                short s_sample = (short)(f_sample * 32767.0f);
                pcm_data[(total_decoded + sample) * channels + ch] = s_sample;
            }
        }

        total_decoded += samples_read;

        unsigned int current_position = stb_vorbis_get_sample_offset(vorbis);
        printf("OGG Load: Read %d sample frames, total: %lld/%lld, decoder: %u\n",
               samples_read,
               total_decoded,
               target_sample_frames,
               current_position);
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
           start_position,
           final_position,
           expected_position);

    if (final_position != (int)expected_position) {
        printf("WARNING: Decoder position mismatch! Expected %zu, got %d\n",
               expected_position,
               final_position);
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
           out->data_size,
           total_decoded);

    return 1;
}

// clang-format off
enum {
    PAL_UPPER_BIT = (1 << 0),     // A-Z
    PAL_LOWER_BIT = (1 << 1),     // a-z
    PAL_DIGIT_BIT = (1 << 2),     // 0-9
    PAL_UNDER_BIT = (1 << 3),     // _
    PAL_HYPHEN_BIT = (1 << 4),    // -
    PAL_DOT_BIT = (1 << 5),       // .
    PAL_EOL_BIT = (1 << 6),       // \r, \n (included in whitespace)
    PAL_WHITESPACE_BIT = (1 << 7) // All whitespace chars
};

static const uint8_t pal_char_masks[128] = {
    // Control characters (0-31)
    [0] = 0, [1] = 0, [2] = 0, [3] = 0, [4] = 0,
    [5] = 0, [6] = 0, [7] = 0, [8] = 0,

    ['\t'] = PAL_WHITESPACE_BIT, // tab
    ['\n'] = PAL_WHITESPACE_BIT, // new line
    ['\v'] = PAL_WHITESPACE_BIT, // vertical tab (not used anymore)
    ['\f'] = PAL_WHITESPACE_BIT, // form feed (not used anymore)
    ['\r'] = PAL_WHITESPACE_BIT, // carriage return

    [14] = 0, [15] = 0, [16] = 0, [17] = 0, [18] = 0,
    [19] = 0, [20] = 0, [21] = 0, [22] = 0, [23] = 0,
    [24] = 0, [25] = 0, [26] = 0, [27] = 0, [28] = 0,
    [29] = 0, [30] = 0, [31] = 0,

    // Printable characters (32-127)
    [' '] = PAL_WHITESPACE_BIT, // Space
    ['!'] = 0, ['"'] = 0, ['#'] = 0, ['$'] = 0,
    ['%'] = 0, ['&'] = 0, ['\''] = 0, ['('] = 0,
    [')'] = 0, ['*'] = 0, ['+'] = 0, [','] = 0,
    ['-'] = PAL_HYPHEN_BIT, ['.'] = PAL_DOT_BIT,
    ['/'] = 0,

    // Numbers (0-9)
    ['0'] = PAL_DIGIT_BIT, ['1'] = PAL_DIGIT_BIT,
    ['2'] = PAL_DIGIT_BIT, ['3'] = PAL_DIGIT_BIT,
    ['4'] = PAL_DIGIT_BIT, ['5'] = PAL_DIGIT_BIT,
    ['6'] = PAL_DIGIT_BIT, ['7'] = PAL_DIGIT_BIT,
    ['8'] = PAL_DIGIT_BIT, ['9'] = PAL_DIGIT_BIT,

    [':'] = 0, [';'] = 0, ['<'] = 0, ['='] = 0,
    ['>'] = 0, ['?'] = 0, ['@'] = 0,

    // Uppercase (A-Z)
    ['A'] = PAL_UPPER_BIT, ['B'] = PAL_UPPER_BIT,
    ['C'] = PAL_UPPER_BIT, ['D'] = PAL_UPPER_BIT,
    ['E'] = PAL_UPPER_BIT, ['F'] = PAL_UPPER_BIT,
    ['G'] = PAL_UPPER_BIT, ['H'] = PAL_UPPER_BIT,
    ['I'] = PAL_UPPER_BIT, ['J'] = PAL_UPPER_BIT,
    ['K'] = PAL_UPPER_BIT, ['L'] = PAL_UPPER_BIT,
    ['M'] = PAL_UPPER_BIT, ['N'] = PAL_UPPER_BIT,
    ['O'] = PAL_UPPER_BIT, ['P'] = PAL_UPPER_BIT,
    ['Q'] = PAL_UPPER_BIT, ['R'] = PAL_UPPER_BIT,
    ['S'] = PAL_UPPER_BIT, ['T'] = PAL_UPPER_BIT,
    ['U'] = PAL_UPPER_BIT, ['V'] = PAL_UPPER_BIT,
    ['W'] = PAL_UPPER_BIT, ['X'] = PAL_UPPER_BIT,
    ['Y'] = PAL_UPPER_BIT, ['Z'] = PAL_UPPER_BIT,

    ['['] = 0, ['\\'] = 0, [']'] = 0, ['^'] = 0,
    ['_'] = PAL_UNDER_BIT, ['`'] = 0,

    // Lowercase (a-z)
    ['a'] = PAL_LOWER_BIT, ['b'] = PAL_LOWER_BIT,
    ['c'] = PAL_LOWER_BIT, ['d'] = PAL_LOWER_BIT,
    ['e'] = PAL_LOWER_BIT, ['f'] = PAL_LOWER_BIT,
    ['g'] = PAL_LOWER_BIT, ['h'] = PAL_LOWER_BIT,
    ['i'] = PAL_LOWER_BIT, ['j'] = PAL_LOWER_BIT,
    ['k'] = PAL_LOWER_BIT, ['l'] = PAL_LOWER_BIT,
    ['m'] = PAL_LOWER_BIT, ['n'] = PAL_LOWER_BIT,
    ['o'] = PAL_LOWER_BIT, ['p'] = PAL_LOWER_BIT,
    ['q'] = PAL_LOWER_BIT, ['r'] = PAL_LOWER_BIT,
    ['s'] = PAL_LOWER_BIT, ['t'] = PAL_LOWER_BIT,
    ['u'] = PAL_LOWER_BIT, ['v'] = PAL_LOWER_BIT,
    ['w'] = PAL_LOWER_BIT, ['x'] = PAL_LOWER_BIT,
    ['y'] = PAL_LOWER_BIT, ['z'] = PAL_LOWER_BIT,

    ['{'] = 0, ['|'] = 0, ['}'] = 0, ['~'] = 0,
    [127] = 0 // DEL
};

// clang-format on
// String Parsing functions
PALAPI pal_bool pal_is_uppercase(char ch) {
    return pal_char_masks[(pal_bool)ch] & PAL_UPPER_BIT;
}

PALAPI pal_bool pal_is_lowercase(char ch) {
    return pal_char_masks[(pal_bool)ch] & PAL_LOWER_BIT;
}

PALAPI pal_bool pal_is_letter(char ch) {
    return pal_char_masks[(pal_bool)ch] & (PAL_UPPER_BIT | PAL_LOWER_BIT);
}

PALAPI pal_bool pal_is_number(char ch) {
    return pal_char_masks[(pal_bool)ch] & PAL_DIGIT_BIT;
}

PALAPI pal_bool pal_is_alphanumeric(char ch) {
    return pal_is_number(ch) || pal_is_letter(ch);
}

PALAPI pal_bool pal_is_end_of_line(char ch) {
    return pal_char_masks[(pal_bool)ch] & PAL_EOL_BIT;
}

PALAPI pal_bool pal_is_underscore(char ch) {
    return pal_char_masks[(pal_bool)ch] & PAL_UNDER_BIT;
}

PALAPI pal_bool pal_is_hyphen(char ch) {
    return pal_char_masks[(pal_bool)ch] & PAL_HYPHEN_BIT;
}

PALAPI pal_bool pal_is_dot(char ch) {
    return pal_char_masks[(pal_bool)ch] & PAL_DOT_BIT;
}
PALAPI pal_bool pal_is_whitespace(char ch) {
    return pal_char_masks[(pal_bool)ch] & PAL_WHITESPACE_BIT;
}

PALAPI pal_bool pal_are_chars_equal(char ch1, char ch2) {
    return (pal_bool)ch1 == (pal_bool)ch2;
}

PALAPI pal_bool pal_are_strings_equal(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *s1 == *s2;
}

// String comparison
PALAPI int pal_strcmp(const char* s1, const char* s2) {
	while (*s1 && (*s1 == *s2)) {
		s1++;
		s2++;
	}
	// Cast to unsigned char to match standard strcmp behavior
	return (unsigned char)*s1 - (unsigned char)*s2;
}

PALAPI int pal_strncmp(const char* s1, const char* s2, size_t n) {
    while (n > 0 && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
        n--;
    }

    if (n == 0)
        return 0;

    return (unsigned char)*s1 - (unsigned char)*s2;
}

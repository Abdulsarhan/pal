#include "sal.h"

#ifdef _WIN32
#include "win32_platform.h"
#elif __LINUX__
#include "linux_platform.h"
#endif

SALAPI void init_sal() {
}

#ifdef _WIN32

/*

###########################################
	       WINDOW & MONITOR STUFF
###########################################

*/

SALAPI Window* init_window(int width, int height, const char* windowTitle) {
	 return Win32InitWindow(width, height, windowTitle);
}

SALAPI uint8_t window_should_close(void) {
	return Win32WindowShouldClose();
}

SALAPI void set_window_title(Window* window, const char* string) {
	(void)SetWindowTextA(window->handle, string);
}

SALAPI void set_window_hint(int type, int value) {
	(void)Win32SetWindowHint(type, value);
}

SALAPI VideoMode* set_video_mode(Monitor* monitor) {
	return Win32GetVideoMode(monitor->handle);
}
SALAPI Monitor* get_primary_monitor(void) {
	return Win32GetPrimaryMonitor();
}

SALAPI ProcAddress* gl_get_proc_address(const char* proc) {
	return wglGetProcAddress(proc);
}

SALAPI int register_input_devices(Window* window) {
	return Win32RegisterRawInputDevices(window->handle);
}

// Keyboard input
SALAPI uint8_t is_key_pressed(int key) {

	if (is_key_down(key) && !is_key_processed(key)) {
		set_key_processed(key);
		return 1;
	}
	else {
		return 0;
	}

}

SALAPI uint8_t is_key_down(int key) {
	return input.keys[key];

}

SALAPI uint8_t is_key_processed(int key) {
	return input.keys_processed[key];
}

SALAPI uint8_t set_key_processed(int key) {
	input.keys_processed[key] = 1;  // Mark as processed
}

// Mouse input
SALAPI uint8_t is_mouse_pressed(int button) {

	if (is_mouse_down(button) && !is_mouse_pressed(button)) {
		set_mouse_processed(button);
		return 1;
	}
	else {
		return 0;
	}
}

SALAPI uint8_t is_mouse_down(int button) {
	return input.mouse_buttons[button];
}

SALAPI uint8_t is_mouse_processed(int button) {
	return input.mouse_buttons_processed[button];
}

SALAPI void set_mouse_processed(int button) {
	input.mouse_buttons_processed[button] = 1; // Mark as processed
}

SALAPI void poll_events(void) {
	(void)Win32PollEvents();
}

SALAPI int make_context_current(Window* window) {
	return Win32MakeContextCurrent(window->handle);
}

/*

###########################################
		   RENDERING FUNCTIONS.
###########################################

*/

SALAPI void begin_drawing(void) {
	(void)Win32BeginDrawing();
}

SALAPI void DrawTriangle() {

}

SALAPI void end_drawing(void) {
	(void)Win32EndDrawing();
}


/*

###########################################
	     LIBRARY LOADING FUNCTIONS
###########################################

*/

void* LoadDynamicLibrary(char* dll) {
	return Win32LoadDynamicLibrary(dll);

}

void* LoadDynamicFunction(void* dll, char* func_name) {
	return Win32LoadDynamicFunction(dll, func_name);

}

uint8_t FreeDynamicLibrary(void* dll) {
	return Win32FreeDynamicLibrary(dll);

}
#endif


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

	int result = fwrite(data, sizeof(char), fileSize, outputFile);
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

SALAPI uint8_t is_upper_case(char ch) {
	return ((ch >= 'A') && (ch <= 'Z'));
}

SALAPI uint8_t is_lower_case(char ch) {
	return ((ch >= 'a') && (ch <= 'z'));
}

SALAPI uint8_t is_letter(char ch) {
	return (is_upper_case(ch) || is_lower_case(ch));
}

SALAPI uint8_t is_end_of_line(char ch) {
	return ((ch == '\r') || (ch == '\n'));
}

SALAPI uint8_t is_whitespace(char ch) {
	return ((ch == ' ') || (ch == '\t') || (ch == '\v') || (ch == '\f'));
}

SALAPI uint8_t is_number(char ch) {
	return ((ch >= '0') && (ch <= '9'));
}

SALAPI uint8_t is_underscore(char ch) {
	return (ch == '_');
}

SALAPI uint8_t is_hyphen(char ch) {
	return (ch == '-');
}

SALAPI uint8_t is_dot(char ch) {
	return (ch == '.');
}

SALAPI uint8_t are_chars_equal(char ch1, char ch2) {
	return (ch1 == ch2);
}

SALAPI uint8_t are_strings_equal(int count, char* str1, char* str2) {
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

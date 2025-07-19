#ifndef PAL_H
#define PAL_H

#include <stdint.h> // For Clearly Defined Types.
#include <sys/stat.h> // For time_t and stat.
typedef uint8_t pal_bool;

typedef struct pal_video_mode {
	int width;
	int height;
    int refresh_rate;
    int bits_per_pixel;
}pal_video_mode;

typedef struct {
	unsigned char* data;   // Raw PCM audio data
	uint32_t dataSize;       // Size in bytes

	int sampleRate;        // Samples per second (e.g., 44100)
	int channels;          // Number of audio channels (e.g., 2 for stereo)
	int bitsPerSample;     // Usually 16 or 32
    int isFloat; // 0 = PCM, 1 = IEEE float

}Sound;

#define PAL_MAX_TOUCHES 2
typedef struct {
    // Standard gamepad controls
    struct {
        float left_x, left_y;
        float right_x, right_y;
        float left_trigger, right_trigger;
    } axes;
    
    struct {
        pal_bool a, b, x, y;
        pal_bool back, start, guide;
        pal_bool left_stick, right_stick;
        pal_bool left_shoulder, right_shoulder;
        pal_bool dpad_up, dpad_down, dpad_left, dpad_right;
        pal_bool touchpad_button;
    } buttons;
    
    // Identification
    char name[128];
    uint16_t vendor_id;
    uint16_t product_id;
    pal_bool connected;
    pal_bool is_xinput;

    // Battery information
    float battery_level;        // 0.0-1.0
    pal_bool is_charging;

    // Motion sensors
    float accel_x, accel_y, accel_z;  // In G's
    float gyro_x, gyro_y, gyro_z;     // In degrees/second

    // Touchpad
    struct {
        int touch_count;
        struct {
            int id;            // Touch ID
            float x, y;        // Normalized coordinates (0-1)
            pal_bool down;     // Is touch active
        } touches[PAL_MAX_TOUCHES]; // Two fingers on the touch pad at the same time. Kinky. commonly used for scrolling, zooming and rotating.
    } touchpad;
} pal_gamepad_state;



// Window stuff.
typedef struct pal_window pal_window;
typedef struct pal_monitor pal_monitor;

// events.

typedef enum pal_event_type
{
    PAL_NONE = 0x0,
    PAL_QUIT = 0x100,

    PAL_WINDOW_EVENT = 0x200,
    PAL_SYSWM_EVENT,

    PAL_KEY_DOWN = 0x300,
    PAL_KEY_UP,
    PAL_TEXT_EDITING,
    PAL_TEXT_INPUT,
    PAL_KEYMAP_CHANGED,
    PAL_TEXT_EDITING_EXT,

    PAL_MOUSE_MOTION = 0x400,
    PAL_MOUSE_BUTTON_DOWN,
    PAL_MOUSE_BUTTON_UP,
    PAL_MOUSE_WHEEL,

    PAL_JOY_AXIS_MOTION = 0x600,
    PAL_JOY_BALL_MOTION,
    PAL_JOY_HAT_MOTION,
    PAL_JOY_BUTTON_DOWN,
    PAL_JOY_BUTTON_UP,
    PAL_JOY_DEVICE_ADDED,
    PAL_JOY_DEVICE_REMOVED,
    PAL_JOY_BATTERY_UPDATED,

    PAL_GAMEPAD_AXIS_MOTION = 0x650,
    PAL_GAMEPAD_BUTTON_DOWN,
    PAL_GAMEPAD_BUTTON_UP,
    PAL_GAMEPAD_DEVICE_ADDED,
    PAL_GAMEPAD_DEVICE_REMOVED,
    PAL_GAMEPAD_REMAPPED,
    PAL_GAMEPAD_TOUCHPAD_DOWN,
    PAL_GAMEPAD_TOUCHPAD_MOTION,
    PAL_GAMEPAD_TOUCHPAD_UP,
    PAL_GAMEPAD_SENSOR_UPDATE,

    PAL_FINGER_DOWN = 0x700,
    PAL_FINGER_UP,
    PAL_FINGER_MOTION,

    PAL_DOLLAR_GESTURE = 0x800,
    PAL_DOLLAR_RECORD,
    PAL_MULTI_GESTURE,

    PAL_CLIPBOARD_EVENT = 0x900,

    PAL_DROP_FILE = 0x1000,
    PAL_DROP_TEXT,
    PAL_DROP_BEGIN,
    PAL_DROP_COMPLETE,

    PAL_AUDIO_DEVICE_ADDED = 0x1100,
    PAL_AUDIO_DEVICE_REMOVED,

    PAL_SENSOR_UPDATE = 0x1200,

    PAL_RENDER_TARGETS_RESET = 0x2000,
    PAL_RENDER_DEVICE_RESET,

    PAL_PEN_PROXIMITY = 0x1400,
    PAL_PEN_TOUCH,
    PAL_PEN_MOTION,
    PAL_PEN_BUTTON,
    PAL_PEN_AXIS,

    PAL_CAMERA_DEVICE_ADDED = 0x1500,
    PAL_CAMERA_DEVICE_REMOVED,

    PAL_TEXT_EDITING_CANDIDATES = 0x1600,

    PAL_USER_EVENT = 0x8000,

    PAL_LAST_EVENT = 0xFFFF
} pal_event_type;

typedef struct pal_common_event {
    int32_t dummy;
} pal_common_event;

typedef struct pal_display_event {
    int32_t display_index;
    int32_t width;
    int32_t height;
    float dpi;
} pal_display_event;

typedef struct pal_window_event {
    uint32_t windowid;
    int32_t event_code;
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
    uint8_t focused;
    uint8_t visible;
} pal_window_event;

typedef struct pal_keyboard_device_event {
    int32_t device_id;
    uint8_t connected;
} pal_keyboard_device_event;

typedef struct pal_keyboard_event {
    uint32_t virtual_key;
    uint32_t scancode;
    uint8_t pressed;
    uint8_t repeat;
    uint32_t modifiers;
} pal_keyboard_event;

typedef struct pal_text_editing_event {
    char text[32];
    int32_t start;
    int32_t length;
} pal_text_editing_event;

typedef struct pal_text_editing_candidates_event {
    char candidates[8][32];
    int32_t count;
} pal_text_editing_candidates_event;

typedef struct pal_text_input_event {
    char utf8_text[8];
} pal_text_input_event;

typedef struct pal_mouse_device_event {
    int32_t device_id;
    uint8_t connected;
} pal_mouse_device_event;

typedef struct pal_mouse_motion_event {
    int32_t x;
    int32_t y;
    int32_t delta_x;
    int32_t delta_y;
    uint32_t buttons;
} pal_mouse_motion_event;

typedef struct pal_mouse_button_event {
    int32_t x;
    int32_t y;
    uint8_t pressed;
    uint32_t button;
    uint8_t clicks;
    uint32_t modifiers;
} pal_mouse_button_event;

typedef struct pal_mouse_wheel_event {
    int32_t x;
    int32_t y;
    float delta_x;
    float delta_y;
    uint32_t modifiers;
} pal_mouse_wheel_event;

typedef struct pal_joy_device_event {
    int32_t device_id;
    uint8_t connected;
} pal_joy_device_event;

typedef struct pal_joy_axis_event {
    int32_t device_id;
    uint8_t axis;
    float value;
} pal_joy_axis_event;

typedef struct pal_joy_ball_event {
    int32_t device_id;
    uint8_t ball;
    int32_t delta_x;
    int32_t delta_y;
} pal_joy_ball_event;

typedef struct pal_joy_hat_event {
    int32_t device_id;
    uint8_t hat;
    uint8_t value;
} pal_joy_hat_event;

typedef struct pal_joy_button_event {
    int32_t device_id;
    uint8_t button;
    uint8_t pressed;
} pal_joy_button_event;

typedef struct pal_joy_battery_event {
    int32_t device_id;
    uint8_t level;      // 0-100
    uint8_t charging;
} pal_joy_battery_event;

typedef struct pal_gamepad_device_event {
    int32_t device_id;
    uint8_t connected;
} pal_gamepad_device_event;

typedef struct pal_gamepad_axis_event {
    int32_t device_id;
    uint8_t axis;
    float value;
} pal_gamepad_axis_event;

typedef struct pal_gamepad_button_event {
    int32_t device_id;
    uint8_t button;
    uint8_t pressed;
} pal_gamepad_button_event;

typedef struct pal_gamepad_touchpad_event {
    int32_t device_id;
    int32_t x;
    int32_t y;
    uint8_t pressed;
} pal_gamepad_touchpad_event;

typedef struct pal_gamepad_sensor_event {
    int32_t device_id;
    float x;
    float y;
    float z;
    uint8_t sensor_type;
} pal_gamepad_sensor_event;

typedef struct pal_audio_device_event {
    int32_t device_id;
    uint8_t input;
    uint8_t connected;
} pal_audio_device_event;

typedef struct pal_camera_device_event {
    int32_t device_id;
    uint8_t connected;
} pal_camera_device_event;

typedef struct pal_sensor_event {
    int32_t device_id;
    float x;
    float y;
    float z;
    uint8_t sensor_type;
} pal_sensor_event;

typedef struct pal_quit_event {
    int32_t code;
} pal_quit_event;

typedef struct pal_user_event {
    int32_t code;
    void* data1;
    void* data2;
} pal_user_event;

typedef struct pal_touch_finger_event {
    int64_t touch_id;
    int64_t finger_id;
    float x;
    float y;
    float pressure;
    uint8_t pressed;
} pal_touch_finger_event;

typedef struct pal_pen_proximity_event {
    int32_t device_id;
    uint8_t in_range;
} pal_pen_proximity_event;

typedef struct pal_pen_touch_event {
    int32_t x;
    int32_t y;
    float pressure;
    uint8_t pressed;
} pal_pen_touch_event;

typedef struct pal_pen_motion_event {
    int32_t x;
    int32_t y;
    float pressure;
} pal_pen_motion_event;

typedef struct pal_pen_button_event {
    uint8_t button;
    uint8_t pressed;
} pal_pen_button_event;

typedef struct pal_pen_axis_event {
    float tilt_x;
    float tilt_y;
    float rotation;
} pal_pen_axis_event;

typedef struct pal_render_event {
    uint32_t windowid;
} pal_render_event;

typedef struct pal_drop_event {
    const char** paths;
    int32_t count;
} pal_drop_event;

typedef struct pal_clipboard_event {
    const char* text;
} pal_clipboard_event;

typedef struct pal_event
{
    uint32_t type;                              /**< Event type, shared with all events, Uint32 to cover user events which are not in the pal_event_type enumeration */
	// Event data:
    union {
    pal_common_event common;                  /**< Common event data */
    pal_display_event display;                /**< Display event data */
    pal_window_event window;                  /**< Window event data */
    pal_keyboard_device_event kdevice;       /**< Keyboard device change event data */
    pal_keyboard_event key;                   /**< Keyboard event data */
    pal_text_editing_event edit;              /**< Text editing event data */
    pal_text_editing_candidates_event edit_candidates; /**< Text editing candidates event data */
    pal_text_input_event text;                /**< Text input event data */
    pal_mouse_device_event mdevice;           /**< Mouse device change event data */
    pal_mouse_motion_event motion;            /**< Mouse motion event data */
    pal_mouse_button_event button;            /**< Mouse button event data */
    pal_mouse_wheel_event wheel;              /**< Mouse wheel event data */
    pal_joy_device_event jdevice;             /**< Joystick device change event data */
    pal_joy_axis_event jaxis;                  /**< Joystick axis event data */
    pal_joy_ball_event jball;                  /**< Joystick ball event data */
    pal_joy_hat_event jhat;                    /**< Joystick hat event data */
    pal_joy_button_event jbutton;              /**< Joystick button event data */
    pal_joy_battery_event jbattery;            /**< Joystick battery event data */
    pal_gamepad_device_event gdevice;          /**< Gamepad device event data */
    pal_gamepad_axis_event gaxis;               /**< Gamepad axis event data */
    pal_gamepad_button_event gbutton;           /**< Gamepad button event data */
    pal_gamepad_touchpad_event gtouchpad;       /**< Gamepad touchpad event data */
    pal_gamepad_sensor_event gsensor;           /**< Gamepad sensor event data */
    pal_audio_device_event adevice;              /**< Audio device event data */
    pal_camera_device_event cdevice;             /**< Camera device event data */
    pal_sensor_event sensor;                     /**< Sensor event data */
    pal_quit_event quit;                         /**< Quit request event data */
    pal_user_event user;                         /**< Custom event data */
    pal_touch_finger_event tfinger;             /**< Touch finger event data */
    pal_pen_proximity_event pproximity;         /**< Pen proximity event data */
    pal_pen_touch_event ptouch;                   /**< Pen tip touching event data */
    pal_pen_motion_event pmotion;                 /**< Pen motion event data */
    pal_pen_button_event pbutton;                 /**< Pen button event data */
    pal_pen_axis_event paxis;                     /**< Pen axis event data */
    pal_render_event render;                       /**< Render event data */
    pal_drop_event drop;                           /**< Drag and drop event data */
    pal_clipboard_event clipboard;                 /**< Clipboard event data */
    };

    uint8_t padding[128];
} pal_event;

typedef struct pal_event_queue {
    size_t size;
    size_t capacity;
    int front;
    int back;
    pal_event* events;
}pal_event_queue;

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
#define PALAPI extern // extern is default, but it doesn't hurt to be explicit.
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

// v2, 2 components
typedef struct v2 {
    float x;                // Vector x component
    float y;                // Vector y component
} v2;

// v3, 3 components
typedef struct v3 {
    float x;                // Vector x component
    float y;                // Vector y component
    float z;                // Vector z component
} v3;

// v4, 4 components
typedef struct v4 {
    float x;                // Vector x component
    float y;                // Vector y component
    float z;                // Vector z component
    float w;                // Vector w component
} v4;

// v2, 2 components
typedef struct iv2 {
	int x;                // Vector x component
	int y;                // Vector y component
} iv2;

// v3, 3 components
typedef struct iv3 {
	int x;                // Vector x component
	int y;                // Vector y component
	int z;                // Vector z component
} iv3;

// v4, 4 components
typedef struct iv4 {
	int x;                // Vector x component
	int y;                // Vector y component
	int z;                // Vector z component
	int w;                // Vector w component
} iv4;

//----------------------------------------------------------------------------------
// Window Flags 
//----------------------------------------------------------------------------------
#define GL_PROFILE 0x1
#define GL_VERSION_MAJOR 0x2
#define GL_VERSION_MINOR 0x3
#define RESIZABLE 0x4
#define DOUBLE_BUFFER 0x5
#define FLOATING 0x6

// window visibility
#define PAL_WINDOW_FULLSCREEN 0x1
#define PAL_WINDOW_BORDERLESS 0x10
#define PAL_WINDOW_MINIMIZED 0x40
#define PAL_WINDOW_MAXIMIZED 0x80
#define PAL_WINDOW_RESIZABLE 0x20
#define PAL_WINDOW_OCCLUDED 0x4
#define PAL_WINDOW_HIDDEN 0x8
#define PAL_WINDOW_TRANSPARENT 0x400000
#define PAL_WINDOW_ALWAYS_ON_TOP 0x8000

// graphics APIs
#define PAL_WINDOW_OPENGL 0x2
#define PAL_WINDOW_VULKAN 0x100000
#define PAL_WINDOW_METAL 0x200000

// window type
#define PAL_WINDOW_MODAL 0x1000
#define PAL_WINDOW_UTILITY 0x10000
#define PAL_WINDOW_TOOLTIP 0x20000
#define PAL_WINDOW_POPUP_MENU 0x40000

// keyboard / mouse input
#define PAL_WINDOW_INPUT_FOCUS 0x200
#define PAL_WINDOW_MOUSE_FOCUS 0x400
#define PAL_WINDOW_MOUSE_GRABBED 0x100
#define PAL_WINDOW_KEYBOARD_GRABBED 0x80000
#define PAL_WINDOW_NOT_FOCUSABLE 0x800000
#define PAL_WINDOW_MOUSE_CAPTURE 0x4000

// unreleated stuff.
#define PAL_WINDOW_EXTERNAL 0x800
#define PAL_WINDOW_HIGH_PIXEL_DENSITY 0x2000

//----------------------------------------------------------------------------------
// Window Hint Values
//----------------------------------------------------------------------------------
#define GL_PROFILE_CORE 0x7
#define GL_PROFILE_COMPAT 0x8

//----------------------------------------------------------------------------------
// Keyboard and Mouse Buttons
//----------------------------------------------------------------------------------
#define PAL_MOUSE_LEFT 0x0
#define PAL_MOUSE_RIGHT 0x1
#define PAL_MOUSE_MIDDLE 0x2
#define PAL_MOUSE_4 0x3
#define PAL_MOUSE_5 0x4
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

#define PAL_BACKSPACE     0x08
#define PAL_TAB           0x09
#define PAL_ENTER         0x0D
#define PAL_SHIFT         0x10
#define PAL_CONTROL       0x11
#define PAL_ALT           0x12
#define PAL_PAUSE         0x13
#define PAL_CAPSLOCK      0x14
#define PAL_ESCAPE        0x1B
#define PAL_SPACE         0x20
#define PAL_PAGEUP        0x21
#define PAL_PAGEDOWN      0x22
#define PAL_END           0x23
#define PAL_HOME          0x24
#define PAL_LEFT          0x25
#define PAL_UP            0x26
#define PAL_RIGHT         0x27
#define PAL_DOWN          0x28
#define PAL_PRINTSCREEN   0x2C
#define PAL_INSERT        0x2D
#define PAL_DELETE        0x2E

#define PAL_0             0x30
#define PAL_1             0x31
#define PAL_2             0x32
#define PAL_3             0x33
#define PAL_4             0x34
#define PAL_5             0x35
#define PAL_6             0x36
#define PAL_7             0x37
#define PAL_8             0x38
#define PAL_9             0x39

#define PAL_A             0x41
#define PAL_B             0x42
#define PAL_C             0x43
#define PAL_D             0x44
#define PAL_E             0x45
#define PAL_F             0x46
#define PAL_G             0x47
#define PAL_H             0x48
#define PAL_I             0x49
#define PAL_J             0x4A
#define PAL_K             0x4B
#define PAL_L             0x4C
#define PAL_M             0x4D
#define PAL_N             0x4E
#define PAL_O             0x4F
#define PAL_P             0x50
#define PAL_Q             0x51
#define PAL_R             0x52
#define PAL_S             0x53
#define PAL_T             0x54
#define PAL_U             0x55
#define PAL_V             0x56
#define PAL_W             0x57
#define PAL_X             0x58
#define PAL_Y             0x59
#define PAL_Z             0x5A

#define PAL_LWIN          0x5B // This is also known as the super key on linux.
#define PAL_RWIN          0x5C
#define PAL_APPS          0x5D

#define PAL_NUMPAD_0      0x60
#define PAL_NUMPAD_1      0x61
#define PAL_NUMPAD_2      0x62
#define PAL_NUMPAD_3      0x63
#define PAL_NUMPAD_4      0x64
#define PAL_NUMPAD_5      0x65
#define PAL_NUMPAD_6      0x66
#define PAL_NUMPAD_7      0x67
#define PAL_NUMPAD_8      0x68
#define PAL_NUMPAD_9      0x69
#define PAL_MULTIPLY      0x6A
#define PAL_ADD           0x6B
#define PAL_SEPARATOR     0x6C // Most modern Keyboards don't have this.
#define PAL_SUBTRACT      0x6D
#define PAL_DECIMAL       0x6E
#define PAL_DIVIDE        0x6F

#define PAL_F1            0x70
#define PAL_F2            0x71
#define PAL_F3            0x72
#define PAL_F4            0x73
#define PAL_F5            0x74
#define PAL_F6            0x75
#define PAL_F7            0x76
#define PAL_F8            0x77
#define PAL_F9            0x78
#define PAL_F10           0x79
#define PAL_F11           0x7A
#define PAL_F12           0x7B

#define PAL_NUMLOCK       0x90
#define PAL_SCROLLLOCK    0x91

//---------------------------------------------------------------------------------
// Modifier Keys
//---------------------------------------------------------------------------------
#define PAL_MOD_NONE 0x0
#define PAL_MOD_LSHIFT 0x1
#define PAL_MOD_RSHIFT 0x2
#define PAL_MOD_LEVEL5SHIFT 0x4
#define PAL_MOD_LCTRL 0x8
#define PAL_MOD_RCTRL 0x10
#define PAL_MOD_LALT 0x20
#define PAL_MOD_RALT 0x40
#define PAL_MOD_LSUPER 0x80
#define PAL_MOD_RSUPER 0x100
#define PAL_MOD_LWINDOWS 0x200
#define PAL_MOD_RWINDOWS 0x400
#define PAL_MOD_ALTGR 0x800
#define PAL_MOD_CAPS 0x1000
#define PAL_MOD_NUM 0x2000
#define PAL_MOD_SCROLL 0x4000

//----------------------------------------------------------------------------------
// Controller Buttons
//----------------------------------------------------------------------------------
#define GAMEPAD_DPAD_UP       0x0001
#define GAMEPAD_DPAD_DOWN     0x0002
#define GAMEPAD_DPAD_LEFT     0x0004
#define GAMEPAD_DPAD_RIGHT    0x0008
#define GAMEPAD_START         0x0010
#define GAMEPAD_BACK          0x0020
#define GAMEPAD_LEFT_THUMB    0x0040
#define GAMEPAD_RIGHT_THUMB   0x0080
#define GAMEPAD_LEFT_BUMPER   0x0100
#define GAMEPAD_RIGHT_BUMPER  0x0200
#define GAMEPAD_A              0x1000  // Cross on Playstation Controllers
#define GAMEPAD_B              0x2000  // Circle on Playstation Controllers
#define GAMEPAD_X              0x4000  // Square on Playstation Controllers
#define GAMEPAD_Y              0x8000  // Triangle on Playstation Controllers
#define GAMEPAD_CROSS          GAMEPAD_A
#define GAMEPAD_CIRCLE         GAMEPAD_B
#define GAMEPAD_SQUARE         GAMEPAD_X
#define GAMEPAD_TRIANGLE       GAMEPAD_Y

#if defined(__cplusplus)
#define CLITERAL(type)      type
#else
#define CLITERAL(type)      (type)
#endif

#ifdef __cplusplus
extern "C" {
#endif

PALAPI void pal_init();
PALAPI pal_window* pal_create_window(int width, int height, const char* windowTitle, uint64_t window_flags);
PALAPI uint8_t pal_set_window_title(pal_window* window, const char* string);
PALAPI pal_bool pal_make_window_fullscreen(pal_window* window);
PALAPI pal_bool pal_make_window_fullscreen_ex(pal_window* window, int width, int height, int refreshrate);
PALAPI pal_bool pal_make_window_fullscreen_windowed(pal_window* window);
PALAPI pal_bool pal_make_window_windowed(pal_window* window);
PALAPI void pal_set_window_icon(pal_window* window, const char* image_path);
PALAPI void pal_set_window_icon_legacy(pal_window* window, const char* image_path);
PALAPI void pal_set_taskbar_icon(pal_window* taskbar, const char* image_path);
PALAPI void pal_set_taskbar_icon_legacy(pal_window* taskbar, const char* image_path);
PALAPI void pal_set_cursor(pal_window* window, const char* image_path, int size);
PALAPI pal_video_mode* pal_get_video_mode(pal_monitor* monitor);
PALAPI pal_monitor* pal_get_primary_monitor(void);
PALAPI void* gl_get_proc_address(const unsigned char* proc);
PALAPI uint8_t pal_poll_events(pal_event* event, pal_window* window);
PALAPI int pal_make_context_current(pal_window* window);
PALAPI int register_input_devices(pal_window* window);
PALAPI uint8_t is_key_pressed(int key);
PALAPI uint8_t is_key_down(int key);
PALAPI uint8_t is_key_processed(int key);
PALAPI void set_key_processed(int key);

// Mouse input
PALAPI uint8_t is_mouse_pressed(int button);
PALAPI uint8_t is_mouse_down(int button);
PALAPI uint8_t is_mouse_processed(int button);
PALAPI void set_mouse_processed(int button);
PALAPI v2 get_mouse_position(pal_window* window);

// Gamepad Input
PALAPI int pal_get_gamepad_count();
PALAPI pal_bool pal_get_gamepad_state(int index, pal_gamepad_state* out_state);
PALAPI void pal_set_gamepad_vibration(int controller_id, float left_motor, float right_motor, float left_trigger, float right_trigger);
PALAPI void pal_stop_gamepad_vibration(int controller_id);

PALAPI void begin_drawing(void);
PALAPI void DrawTriangle(void);
PALAPI void end_drawing(pal_window* window);

// Sound
PALAPI int load_sound(const char* filename, Sound* out);
PALAPI int play_sound(Sound* sound, float volume);

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

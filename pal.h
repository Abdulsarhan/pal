#ifndef PAL_H
#define PAL_H

#include <stdint.h> // For Clearly Defined Types.
#include <sys/stat.h> // For time_t and stat.
typedef uint8_t pal_bool;

typedef struct VideoMode {
	int width;
	int height;
}VideoMode;

typedef struct {
	unsigned char* data;   // Raw PCM audio data
	uint32_t dataSize;       // Size in bytes

	int sampleRate;        // Samples per second (e.g., 44100)
	int channels;          // Number of audio channels (e.g., 2 for stereo)
	int bitsPerSample;     // Usually 16 or 32
    int isFloat; // 0 = PCM, 1 = IEEE float

}Sound;

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
	// Event type, This is just an enum that contains all the types of events. The user will have these in the event loop
	// To check what kinds of events they have received.
    uint32_t type;                              /**< Event type, shared with all events, Uint32 to cover user events which are not in the SDL_EventType enumeration */
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

//----------------------------------------------------------------------------------
// Keyboard and Mouse Buttons
//----------------------------------------------------------------------------------
#define LEFT_MOUSE_BUTTON 0x0
#define RIGHT_MOUSE_BUTTON 0x1
#define MIDDLE_MOUSE_BUTTON 0x2
#define SIDE_MOUSE_BUTTON1 0x3
#define SIDE_MOUSE_BUTTON2 0x4
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

#define KEY_BACKSPACE     0x08
#define KEY_TAB           0x09
#define KEY_ENTER         0x0D
#define KEY_SHIFT         0x10
#define KEY_CONTROL       0x11
#define KEY_ALT           0x12
#define KEY_PAUSE         0x13
#define KEY_CAPSLOCK      0x14
#define KEY_ESCAPE        0x1B
#define KEY_SPACE         0x20
#define KEY_PAGEUP        0x21
#define KEY_PAGEDOWN      0x22
#define KEY_END           0x23
#define KEY_HOME          0x24
#define KEY_LEFT          0x25
#define KEY_UP            0x26
#define KEY_RIGHT         0x27
#define KEY_DOWN          0x28
#define KEY_PRINTSCREEN   0x2C
#define KEY_INSERT        0x2D
#define KEY_DELETE        0x2E

#define KEY_0             0x30
#define KEY_1             0x31
#define KEY_2             0x32
#define KEY_3             0x33
#define KEY_4             0x34
#define KEY_5             0x35
#define KEY_6             0x36
#define KEY_7             0x37
#define KEY_8             0x38
#define KEY_9             0x39

#define KEY_A             0x41
#define KEY_B             0x42
#define KEY_C             0x43
#define KEY_D             0x44
#define KEY_E             0x45
#define KEY_F             0x46
#define KEY_G             0x47
#define KEY_H             0x48
#define KEY_I             0x49
#define KEY_J             0x4A
#define KEY_K             0x4B
#define KEY_L             0x4C
#define KEY_M             0x4D
#define KEY_N             0x4E
#define KEY_O             0x4F
#define KEY_P             0x50
#define KEY_Q             0x51
#define KEY_R             0x52
#define KEY_S             0x53
#define KEY_T             0x54
#define KEY_U             0x55
#define KEY_V             0x56
#define KEY_W             0x57
#define KEY_X             0x58
#define KEY_Y             0x59
#define KEY_Z             0x5A

#define KEY_LWIN          0x5B // This is also known as the super key on linux.
#define KEY_RWIN          0x5C
#define KEY_APPS          0x5D

#define KEY_NUMPAD_0       0x60
#define KEY_NUMPAD_1       0x61
#define KEY_NUMPAD_2       0x62
#define KEY_NUMPAD_3       0x63
#define KEY_NUMPAD_4       0x64
#define KEY_NUMPAD_5       0x65
#define KEY_NUMPAD_6       0x66
#define KEY_NUMPAD_7       0x67
#define KEY_NUMPAD_8       0x68
#define KEY_NUMPAD_9       0x69
#define KEY_MULTIPLY      0x6A
#define KEY_ADD           0x6B
#define KEY_SEPARATOR     0x6C // Most modern keyboards don't have this.
#define KEY_SUBTRACT      0x6D
#define KEY_DECIMAL       0x6E
#define KEY_DIVIDE        0x6F

#define KEY_F1            0x70
#define KEY_F2            0x71
#define KEY_F3            0x72
#define KEY_F4            0x73
#define KEY_F5            0x74
#define KEY_F6            0x75
#define KEY_F7            0x76
#define KEY_F8            0x77
#define KEY_F9            0x78
#define KEY_F10           0x79
#define KEY_F11           0x7A
#define KEY_F12           0x7B

#define KEY_NUMLOCK       0x90
#define KEY_SCROLLLOCK    0x91
typedef enum PAL_Scancode
{
    PAL_SCANCODE_UNKNOWN = 0,

    /**
     *  \name Usage page 0x07
     *
     *  These values are from usage page 0x07 (USB keyboard page).
     */
    /* @{ */

    PAL_SCANCODE_A = 4,
    PAL_SCANCODE_B = 5,
    PAL_SCANCODE_C = 6,
    PAL_SCANCODE_D = 7,
    PAL_SCANCODE_E = 8,
    PAL_SCANCODE_F = 9,
    PAL_SCANCODE_G = 10,
    PAL_SCANCODE_H = 11,
    PAL_SCANCODE_I = 12,
    PAL_SCANCODE_J = 13,
    PAL_SCANCODE_K = 14,
    PAL_SCANCODE_L = 15,
    PAL_SCANCODE_M = 16,
    PAL_SCANCODE_N = 17,
    PAL_SCANCODE_O = 18,
    PAL_SCANCODE_P = 19,
    PAL_SCANCODE_Q = 20,
    PAL_SCANCODE_R = 21,
    PAL_SCANCODE_S = 22,
    PAL_SCANCODE_T = 23,
    PAL_SCANCODE_U = 24,
    PAL_SCANCODE_V = 25,
    PAL_SCANCODE_W = 26,
    PAL_SCANCODE_X = 27,
    PAL_SCANCODE_Y = 28,
    PAL_SCANCODE_Z = 29,

    PAL_SCANCODE_1 = 30,
    PAL_SCANCODE_2 = 31,
    PAL_SCANCODE_3 = 32,
    PAL_SCANCODE_4 = 33,
    PAL_SCANCODE_5 = 34,
    PAL_SCANCODE_6 = 35,
    PAL_SCANCODE_7 = 36,
    PAL_SCANCODE_8 = 37,
    PAL_SCANCODE_9 = 38,
    PAL_SCANCODE_0 = 39,

    PAL_SCANCODE_RETURN = 40,
    PAL_SCANCODE_ESCAPE = 41,
    PAL_SCANCODE_BACKSPACE = 42,
    PAL_SCANCODE_TAB = 43,
    PAL_SCANCODE_SPACE = 44,

    PAL_SCANCODE_MINUS = 45,
    PAL_SCANCODE_EQUALS = 46,
    PAL_SCANCODE_LEFTBRACKET = 47,
    PAL_SCANCODE_RIGHTBRACKET = 48,
    PAL_SCANCODE_BACKSLASH = 49, /**< Located at the lower left of the return
                                  *   key on ISO keyboards and at the right end
                                  *   of the QWERTY row on ANSI keyboards.
                                  *   Produces REVERSE SOLIDUS (backslash) and
                                  *   VERTICAL LINE in a US layout, REVERSE
                                  *   SOLIDUS and VERTICAL LINE in a UK Mac
                                  *   layout, NUMBER SIGN and TILDE in a UK
                                  *   Windows layout, DOLLAR SIGN and POUND SIGN
                                  *   in a Swiss German layout, NUMBER SIGN and
                                  *   APOSTROPHE in a German layout, GRAVE
                                  *   ACCENT and POUND SIGN in a French Mac
                                  *   layout, and ASTERISK and MICRO SIGN in a
                                  *   French Windows layout.
                                  */
    PAL_SCANCODE_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
                                  *   instead of 49 for the same key, but all
                                  *   OSes I've seen treat the two codes
                                  *   identically. So, as an implementor, unless
                                  *   your keyboard generates both of those
                                  *   codes and your OS treats them differently,
                                  *   you should generate PAL_SCANCODE_BACKSLASH
                                  *   instead of this code. As a user, you
                                  *   should not rely on this code because SDL
                                  *   will never generate it with most (all?)
                                  *   keyboards.
                                  */
    PAL_SCANCODE_SEMICOLON = 51,
    PAL_SCANCODE_APOSTROPHE = 52,
    PAL_SCANCODE_GRAVE = 53, /**< Located in the top left corner (on both ANSI
                              *   and ISO keyboards). Produces GRAVE ACCENT and
                              *   TILDE in a US Windows layout and in US and UK
                              *   Mac layouts on ANSI keyboards, GRAVE ACCENT
                              *   and NOT SIGN in a UK Windows layout, SECTION
                              *   SIGN and PLUS-MINUS SIGN in US and UK Mac
                              *   layouts on ISO keyboards, SECTION SIGN and
                              *   DEGREE SIGN in a Swiss German layout (Mac:
                              *   only on ISO keyboards), CIRCUMFLEX ACCENT and
                              *   DEGREE SIGN in a German layout (Mac: only on
                              *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
                              *   French Windows layout, COMMERCIAL AT and
                              *   NUMBER SIGN in a French Mac layout on ISO
                              *   keyboards, and LESS-THAN SIGN and GREATER-THAN
                              *   SIGN in a Swiss German, German, or French Mac
                              *   layout on ANSI keyboards.
                              */
    PAL_SCANCODE_COMMA = 54,
    PAL_SCANCODE_PERIOD = 55,
    PAL_SCANCODE_SLASH = 56,

    PAL_SCANCODE_CAPSLOCK = 57,

    PAL_SCANCODE_F1 = 58,
    PAL_SCANCODE_F2 = 59,
    PAL_SCANCODE_F3 = 60,
    PAL_SCANCODE_F4 = 61,
    PAL_SCANCODE_F5 = 62,
    PAL_SCANCODE_F6 = 63,
    PAL_SCANCODE_F7 = 64,
    PAL_SCANCODE_F8 = 65,
    PAL_SCANCODE_F9 = 66,
    PAL_SCANCODE_F10 = 67,
    PAL_SCANCODE_F11 = 68,
    PAL_SCANCODE_F12 = 69,

    PAL_SCANCODE_PRINTSCREEN = 70,
    PAL_SCANCODE_SCROLLLOCK = 71,
    PAL_SCANCODE_PAUSE = 72,
    PAL_SCANCODE_INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
                                   does send code 73, not 117) */
    PAL_SCANCODE_HOME = 74,
    PAL_SCANCODE_PAGEUP = 75,
    PAL_SCANCODE_DELETE = 76,
    PAL_SCANCODE_END = 77,
    PAL_SCANCODE_PAGEDOWN = 78,
    PAL_SCANCODE_RIGHT = 79,
    PAL_SCANCODE_LEFT = 80,
    PAL_SCANCODE_DOWN = 81,
    PAL_SCANCODE_UP = 82,

    PAL_SCANCODE_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                                     */
    PAL_SCANCODE_KP_DIVIDE = 84,
    PAL_SCANCODE_KP_MULTIPLY = 85,
    PAL_SCANCODE_KP_MINUS = 86,
    PAL_SCANCODE_KP_PLUS = 87,
    PAL_SCANCODE_KP_ENTER = 88,
    PAL_SCANCODE_KP_1 = 89,
    PAL_SCANCODE_KP_2 = 90,
    PAL_SCANCODE_KP_3 = 91,
    PAL_SCANCODE_KP_4 = 92,
    PAL_SCANCODE_KP_5 = 93,
    PAL_SCANCODE_KP_6 = 94,
    PAL_SCANCODE_KP_7 = 95,
    PAL_SCANCODE_KP_8 = 96,
    PAL_SCANCODE_KP_9 = 97,
    PAL_SCANCODE_KP_0 = 98,
    PAL_SCANCODE_KP_PERIOD = 99,

    PAL_SCANCODE_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                                        *   keyboards have over ANSI ones,
                                        *   located between left shift and Y.
                                        *   Produces GRAVE ACCENT and TILDE in a
                                        *   US or UK Mac layout, REVERSE SOLIDUS
                                        *   (backslash) and VERTICAL LINE in a
                                        *   US or UK Windows layout, and
                                        *   LESS-THAN SIGN and GREATER-THAN SIGN
                                        *   in a Swiss German, German, or French
                                        *   layout. */
    PAL_SCANCODE_APPLICATION = 101, /**< windows contextual menu, compose */
    PAL_SCANCODE_POWER = 102, /**< The USB document says this is a status flag,
                               *   not a physical key - but some Mac keyboards
                               *   do have a power key. */
    PAL_SCANCODE_KP_EQUALS = 103,
    PAL_SCANCODE_F13 = 104,
    PAL_SCANCODE_F14 = 105,
    PAL_SCANCODE_F15 = 106,
    PAL_SCANCODE_F16 = 107,
    PAL_SCANCODE_F17 = 108,
    PAL_SCANCODE_F18 = 109,
    PAL_SCANCODE_F19 = 110,
    PAL_SCANCODE_F20 = 111,
    PAL_SCANCODE_F21 = 112,
    PAL_SCANCODE_F22 = 113,
    PAL_SCANCODE_F23 = 114,
    PAL_SCANCODE_F24 = 115,
    PAL_SCANCODE_EXECUTE = 116,
    PAL_SCANCODE_HELP = 117,    /**< AL Integrated Help Center */
    PAL_SCANCODE_MENU = 118,    /**< Menu (show menu) */
    PAL_SCANCODE_SELECT = 119,
    PAL_SCANCODE_STOP = 120,    /**< AC Stop */
    PAL_SCANCODE_AGAIN = 121,   /**< AC Redo/Repeat */
    PAL_SCANCODE_UNDO = 122,    /**< AC Undo */
    PAL_SCANCODE_CUT = 123,     /**< AC Cut */
    PAL_SCANCODE_COPY = 124,    /**< AC Copy */
    PAL_SCANCODE_PASTE = 125,   /**< AC Paste */
    PAL_SCANCODE_FIND = 126,    /**< AC Find */
    PAL_SCANCODE_MUTE = 127,
    PAL_SCANCODE_VOLUMEUP = 128,
    PAL_SCANCODE_VOLUMEDOWN = 129,
/* not sure whether there's a reason to enable these */
/*     PAL_SCANCODE_LOCKINGCAPSLOCK = 130,  */
/*     PAL_SCANCODE_LOCKINGNUMLOCK = 131, */
/*     PAL_SCANCODE_LOCKINGSCROLLLOCK = 132, */
    PAL_SCANCODE_KP_COMMA = 133,
    PAL_SCANCODE_KP_EQUALSAS400 = 134,

    PAL_SCANCODE_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                            footnotes in USB doc */
    PAL_SCANCODE_INTERNATIONAL2 = 136,
    PAL_SCANCODE_INTERNATIONAL3 = 137, /**< Yen */
    PAL_SCANCODE_INTERNATIONAL4 = 138,
    PAL_SCANCODE_INTERNATIONAL5 = 139,
    PAL_SCANCODE_INTERNATIONAL6 = 140,
    PAL_SCANCODE_INTERNATIONAL7 = 141,
    PAL_SCANCODE_INTERNATIONAL8 = 142,
    PAL_SCANCODE_INTERNATIONAL9 = 143,
    PAL_SCANCODE_LANG1 = 144, /**< Hangul/English toggle */
    PAL_SCANCODE_LANG2 = 145, /**< Hanja conversion */
    PAL_SCANCODE_LANG3 = 146, /**< Katakana */
    PAL_SCANCODE_LANG4 = 147, /**< Hiragana */
    PAL_SCANCODE_LANG5 = 148, /**< Zenkaku/Hankaku */
    PAL_SCANCODE_LANG6 = 149, /**< reserved */
    PAL_SCANCODE_LANG7 = 150, /**< reserved */
    PAL_SCANCODE_LANG8 = 151, /**< reserved */
    PAL_SCANCODE_LANG9 = 152, /**< reserved */

    PAL_SCANCODE_ALTERASE = 153,    /**< Erase-Eaze */
    PAL_SCANCODE_SYSREQ = 154,
    PAL_SCANCODE_CANCEL = 155,      /**< AC Cancel */
    PAL_SCANCODE_CLEAR = 156,
    PAL_SCANCODE_PRIOR = 157,
    PAL_SCANCODE_RETURN2 = 158,
    PAL_SCANCODE_SEPARATOR = 159,
    PAL_SCANCODE_OUT = 160,
    PAL_SCANCODE_OPER = 161,
    PAL_SCANCODE_CLEARAGAIN = 162,
    PAL_SCANCODE_CRSEL = 163,
    PAL_SCANCODE_EXSEL = 164,

    PAL_SCANCODE_KP_00 = 176,
    PAL_SCANCODE_KP_000 = 177,
    PAL_SCANCODE_THOUSANDSSEPARATOR = 178,
    PAL_SCANCODE_DECIMALSEPARATOR = 179,
    PAL_SCANCODE_CURRENCYUNIT = 180,
    PAL_SCANCODE_CURRENCYSUBUNIT = 181,
    PAL_SCANCODE_KP_LEFTPAREN = 182,
    PAL_SCANCODE_KP_RIGHTPAREN = 183,
    PAL_SCANCODE_KP_LEFTBRACE = 184,
    PAL_SCANCODE_KP_RIGHTBRACE = 185,
    PAL_SCANCODE_KP_TAB = 186,
    PAL_SCANCODE_KP_BACKSPACE = 187,
    PAL_SCANCODE_KP_A = 188,
    PAL_SCANCODE_KP_B = 189,
    PAL_SCANCODE_KP_C = 190,
    PAL_SCANCODE_KP_D = 191,
    PAL_SCANCODE_KP_E = 192,
    PAL_SCANCODE_KP_F = 193,
    PAL_SCANCODE_KP_XOR = 194,
    PAL_SCANCODE_KP_POWER = 195,
    PAL_SCANCODE_KP_PERCENT = 196,
    PAL_SCANCODE_KP_LESS = 197,
    PAL_SCANCODE_KP_GREATER = 198,
    PAL_SCANCODE_KP_AMPERSAND = 199,
    PAL_SCANCODE_KP_DBLAMPERSAND = 200,
    PAL_SCANCODE_KP_VERTICALBAR = 201,
    PAL_SCANCODE_KP_DBLVERTICALBAR = 202,
    PAL_SCANCODE_KP_COLON = 203,
    PAL_SCANCODE_KP_HASH = 204,
    PAL_SCANCODE_KP_SPACE = 205,
    PAL_SCANCODE_KP_AT = 206,
    PAL_SCANCODE_KP_EXCLAM = 207,
    PAL_SCANCODE_KP_MEMSTORE = 208,
    PAL_SCANCODE_KP_MEMRECALL = 209,
    PAL_SCANCODE_KP_MEMCLEAR = 210,
    PAL_SCANCODE_KP_MEMADD = 211,
    PAL_SCANCODE_KP_MEMSUBTRACT = 212,
    PAL_SCANCODE_KP_MEMMULTIPLY = 213,
    PAL_SCANCODE_KP_MEMDIVIDE = 214,
    PAL_SCANCODE_KP_PLUSMINUS = 215,
    PAL_SCANCODE_KP_CLEAR = 216,
    PAL_SCANCODE_KP_CLEARENTRY = 217,
    PAL_SCANCODE_KP_BINARY = 218,
    PAL_SCANCODE_KP_OCTAL = 219,
    PAL_SCANCODE_KP_DECIMAL = 220,
    PAL_SCANCODE_KP_HEXADECIMAL = 221,

    PAL_SCANCODE_LCTRL = 224,
    PAL_SCANCODE_LSHIFT = 225,
    PAL_SCANCODE_LALT = 226, /**< alt, option */
    PAL_SCANCODE_LGUI = 227, /**< windows, command (apple), meta */
    PAL_SCANCODE_RCTRL = 228,
    PAL_SCANCODE_RSHIFT = 229,
    PAL_SCANCODE_RALT = 230, /**< alt gr, option */
    PAL_SCANCODE_RGUI = 231, /**< windows, command (apple), meta */

    PAL_SCANCODE_MODE = 257,    /**< I'm not sure if this is really not covered
                                 *   by any of the above, but since there's a
                                 *   special PAL_KMOD_MODE for it I'm adding it here
                                 */

    /* @} *//* Usage page 0x07 */

    /**
     *  \name Usage page 0x0C
     *
     *  These values are mapped from usage page 0x0C (USB consumer page).
     *
     *  There are way more keys in the spec than we can represent in the
     *  current scancode range, so pick the ones that commonly come up in
     *  real world usage.
     */
    /* @{ */

    PAL_SCANCODE_SLEEP = 258,                   /**< Sleep */
    PAL_SCANCODE_WAKE = 259,                    /**< Wake */

    PAL_SCANCODE_CHANNEL_INCREMENT = 260,       /**< Channel Increment */
    PAL_SCANCODE_CHANNEL_DECREMENT = 261,       /**< Channel Decrement */

    PAL_SCANCODE_MEDIA_PLAY = 262,          /**< Play */
    PAL_SCANCODE_MEDIA_PAUSE = 263,         /**< Pause */
    PAL_SCANCODE_MEDIA_RECORD = 264,        /**< Record */
    PAL_SCANCODE_MEDIA_FAST_FORWARD = 265,  /**< Fast Forward */
    PAL_SCANCODE_MEDIA_REWIND = 266,        /**< Rewind */
    PAL_SCANCODE_MEDIA_NEXT_TRACK = 267,    /**< Next Track */
    PAL_SCANCODE_MEDIA_PREVIOUS_TRACK = 268, /**< Previous Track */
    PAL_SCANCODE_MEDIA_STOP = 269,          /**< Stop */
    PAL_SCANCODE_MEDIA_EJECT = 270,         /**< Eject */
    PAL_SCANCODE_MEDIA_PLAY_PAUSE = 271,    /**< Play / Pause */
    PAL_SCANCODE_MEDIA_SELECT = 272,        /* Media Select */

    PAL_SCANCODE_AC_NEW = 273,              /**< AC New */
    PAL_SCANCODE_AC_OPEN = 274,             /**< AC Open */
    PAL_SCANCODE_AC_CLOSE = 275,            /**< AC Close */
    PAL_SCANCODE_AC_EXIT = 276,             /**< AC Exit */
    PAL_SCANCODE_AC_SAVE = 277,             /**< AC Save */
    PAL_SCANCODE_AC_PRINT = 278,            /**< AC Print */
    PAL_SCANCODE_AC_PROPERTIES = 279,       /**< AC Properties */

    PAL_SCANCODE_AC_SEARCH = 280,           /**< AC Search */
    PAL_SCANCODE_AC_HOME = 281,             /**< AC Home */
    PAL_SCANCODE_AC_BACK = 282,             /**< AC Back */
    PAL_SCANCODE_AC_FORWARD = 283,          /**< AC Forward */
    PAL_SCANCODE_AC_STOP = 284,             /**< AC Stop */
    PAL_SCANCODE_AC_REFRESH = 285,          /**< AC Refresh */
    PAL_SCANCODE_AC_BOOKMARKS = 286,        /**< AC Bookmarks */

    /* @} *//* Usage page 0x0C */


    /**
     *  \name Mobile keys
     *
     *  These are values that are often used on mobile phones.
     */
    /* @{ */

    PAL_SCANCODE_SOFTLEFT = 287, /**< Usually situated below the display on phones and
                                      used as a multi-function feature key for selecting
                                      a software defined function shown on the bottom left
                                      of the display. */
    PAL_SCANCODE_SOFTRIGHT = 288, /**< Usually situated below the display on phones and
                                       used as a multi-function feature key for selecting
                                       a software defined function shown on the bottom right
                                       of the display. */
    PAL_SCANCODE_CALL = 289, /**< Used for accepting phone calls. */
    PAL_SCANCODE_ENDCALL = 290, /**< Used for rejecting phone calls. */

    /* @} *//* Mobile keys */

    /* Add any other keys here. */

    PAL_SCANCODE_RESERVED = 400,    /**< 400-500 reserved for dynamic keycodes */

    PAL_SCANCODE_COUNT = 512 /**< not a key, just marks the number of scancodes for array bounds */

} PAL_Scancode;

typedef uint16_t pal_keymod;

#define PAL_KEYMOD_NONE  0x0000u 
#define PAL_KMOD_LSHIFT 0x0001u 
#define PAL_KMOD_RSHIFT 0x0002u 
#define PAL_KMOD_LEVEL5 0x0004u // level 5 shift key, not common on most keyboards.
#define PAL_KMOD_LCTRL  0x0040u 
#define PAL_KMOD_RCTRL  0x0080u 
#define PAL_KMOD_LALT   0x0100u 
#define PAL_KMOD_RALT   0x0200u 
#define PAL_KMOD_LWIN   0x0400u 
#define PAL_KMOD_RWIN   0x0800u 
#define PAL_KMOD_LSUPER PAL_KMOD_LWIN 
#define PAL_KMOD_RSUPER PAL_KMOD_RWIN 
#define PAL_KMOD_NUM    0x1000u 
#define PAL_KMOD_CAPS   0x2000u 
#define PAL_KMOD_MODE   0x4000u 
#define PAL_KMOD_SCROLL 0x8000u 
#define PAL_KMOD_CTRL   (PAL_KMOD_LCTRL | PAL_KMOD_RCTRL)   
#define PAL_KMOD_SHIFT  (PAL_KMOD_LSHIFT | PAL_KMOD_RSHIFT) 
#define PAL_KMOD_ALT    (PAL_KMOD_LALT | PAL_KMOD_RALT)     
#define PAL_KMOD_WIN    (PAL_KMOD_LWIN | PAL_KMOD_RWIN)     
#define PAL_KMOD_SUPER  (PAL_KMOD_LSUPER | PAL_KMOD_RSUPER)     

//----------------------------------------------------------------------------------
// Controller Buttons
//----------------------------------------------------------------------------------
#ifdef _WIN32 // these only work for xinput.
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
#endif

#if defined(__cplusplus)
#define CLITERAL(type)      type
#else
#define CLITERAL(type)      (type)
#endif

#ifdef __cplusplus
extern "C" {
#endif

PALAPI void pal_init();
PALAPI pal_window* pal_create_window(int width, int height, const char* windowTitle);
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
PALAPI void pal_window_hint(int type, int value);
PALAPI VideoMode* pal_get_video_mode(pal_monitor* monitor);
PALAPI pal_monitor* pal_get_primary_monitor(void);
PALAPI void* gl_get_proc_address(const unsigned char* proc);
PALAPI uint8_t pal_poll_events(pal_event* event, pal_window* window);
PALAPI int make_context_current(pal_window* window);
PALAPI int register_input_devices(pal_window* window);
PALAPI uint8_t is_key_pressed(int key);
PALAPI uint8_t is_key_down(int key);
PALAPI uint8_t is_key_processed(int key);
PALAPI void set_key_processed(int key);

// Mouse input
PALAPI uint8_t is_mouse_pressed(int button);
PALAPI uint8_t is_mouse_down(int button);
PALAPI uint8_t is_mouse_processed(int button);
PALAPI void set_mouse_processed(int button); // TODO: @CLEANUP There is probably no reason to give the user access to this, might remove.
PALAPI v2 get_mouse_position(pal_window* window);

// Gamepad Input
PALAPI int is_button_down(int controller_id, unsigned short button);
PALAPI int is_button_pressed(int controller_id, unsigned short button);
PALAPI int is_button_released(int controller_id, unsigned short button);
PALAPI v2 get_right_stick(int controller_id);
PALAPI v2 get_left_stick(int controller_id);
PALAPI float get_right_trigger(int controller_id);
PALAPI float get_left_trigger(int controller_id);
PALAPI void set_controller_vibration(int controller_id, float left_motor, float right_motor);
PALAPI void stop_controller_vibration(int controller_id);

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

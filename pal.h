#ifndef PAL_INCLUDE_H
#define PAL_INCLUDE_H

#include <stdint.h>   // For Clearly Defined Types.
#include <stddef.h>
#include <sys/stat.h> // For time_t and stat.

typedef uint8_t pal_bool;
typedef void pal_file;
typedef void pal_signal;
typedef void pal_thread;
typedef void *(*pal_thread_func)(void *arg);
typedef struct pal_window pal_window;
typedef struct pal_monitor pal_monitor;
typedef struct pal_sound pal_sound;
typedef struct pal_music pal_music;
typedef struct pal_mutex pal_mutex;

typedef struct pal_video_mode {
    int width;
    int height;
    int refresh_rate;
    int bits_per_pixel;
} pal_video_mode;

typedef struct {
    uint32_t year;
    uint32_t month;
    uint32_t weeks;
    uint32_t day;
    uint32_t hours;
    uint32_t minutes;
    uint32_t seconds;
} pal_time;
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
    float battery_level; // 0.0-1.0
    pal_bool is_charging;

    // Motion sensors
    float accel_x, accel_y, accel_z; // In G's
    float gyro_x, gyro_y, gyro_z;    // In degrees/second

    // Touchpad
    struct {
        int touch_count;
        struct {
            int id;                 // Touch ID
            float x, y;             // Normalized coordinates (0-1)
            pal_bool down;          // Is touch active
        } touches[PAL_MAX_TOUCHES]; // Two fingers on the touch pad at the same time. Kinky. commonly used for scrolling, zooming and rotating.
    } touchpad;
} pal_gamepad_state;

// events.
typedef enum pal_event_type {
    PAL_EVENT_NONE = 0x0,
    PAL_EVENT_QUIT = 0x100,

    PAL_EVENT_WINDOW = 0x200,
    PAL_EVENT_WINDOW_SHOWN,
    PAL_EVENT_WINDOW_HIDDEN,
    PAL_EVENT_WINDOW_EXPOSED,
    PAL_EVENT_WINDOW_CLOSE_REQUESTED,
    PAL_EVENT_WINDOW_CLOSED,
    PAL_EVENT_WINDOW_MINIMIZED,
    PAL_EVENT_WINDOW_MAXIMIZED,
    PAL_EVENT_WINDOW_MOVED,
    PAL_EVENT_WINDOW_RESIZED,
    PAL_EVENT_WINDOW_ENTERED_FULLSCREEN,
    PAL_EVENT_WINDOW_LEFT_FULLSCREEN,
    PAL_EVENT_WINDOW_GAINED_FOCUS,
    PAL_EVENT_WINDOW_LOST_FOCUS,

    PAL_EVENT_KEY_DOWN = 0x300,
    PAL_EVENT_KEY_UP,
    PAL_EVENT_TEXT_EDITING,
    PAL_EVENT_TEXT_INPUT,
    PAL_EVENT_KEYMAP_CHANGED,
    PAL_EVENT_TEXT_EDITING_EXT,

    PAL_EVENT_MOUSE_MOTION = 0x400,
    PAL_EVENT_MOUSE_BUTTON_DOWN,
    PAL_EVENT_MOUSE_BUTTON_UP,
    PAL_EVENT_MOUSE_WHEEL,

    PAL_EVENT_JOY_AXIS_MOTION = 0x600,
    PAL_EVENT_JOY_BALL_MOTION,
    PAL_EVENT_JOY_HAT_MOTION,
    PAL_EVENT_JOY_BUTTON_DOWN,
    PAL_EVENT_JOY_BUTTON_UP,
    PAL_EVENT_JOY_DEVICE_ADDED,
    PAL_EVENT_JOY_DEVICE_REMOVED,
    PAL_EVENT_JOY_BATTERY_UPDATED,

    PAL_EVENT_GAMEPAD_AXIS_MOTION = 0x650,
    PAL_EVENT_GAMEPAD_BUTTON_DOWN,
    PAL_EVENT_GAMEPAD_BUTTON_UP,
    PAL_EVENT_GAMEPAD_DEVICE_ADDED,
    PAL_EVENT_GAMEPAD_DEVICE_REMOVED,
    PAL_EVENT_GAMEPAD_REMAPPED,
    PAL_EVENT_GAMEPAD_TOUCHPAD_DOWN,
    PAL_EVENT_GAMEPAD_TOUCHPAD_MOTION,
    PAL_EVENT_GAMEPAD_TOUCHPAD_UP,
    PAL_EVENT_GAMEPAD_SENSOR_UPDATE,

    PAL_EVENT_FINGER_DOWN = 0x700,
    PAL_EVENT_FINGER_UP,
    PAL_EVENT_FINGER_MOTION,

    PAL_EVENT_DOLLAR_GESTURE = 0x800,
    PAL_EVENT_DOLLAR_RECORD,
    PAL_EVENT_MULTI_GESTURE,

    PAL_EVENT_CLIPBOARD_EVENT = 0x900,

    PAL_EVENT_DROP_FILE = 0x1000,
    PAL_EVENT_DROP_TEXT,
    PAL_EVENT_DROP_BEGIN,
    PAL_EVENT_DROP_COMPLETE,

    PAL_EVENT_AUDIO_DEVICE_ADDED = 0x1100,
    PAL_EVENT_AUDIO_DEVICE_REMOVED,

    PAL_EVENT_SENSOR_UPDATE = 0x1200,

    PAL_EVENT_RENDER_TARGETS_RESET = 0x2000,
    PAL_EVENT_RENDER_DEVICE_RESET,

    PAL_EVENT_PEN_PROXIMITY = 0x1400,
    PAL_EVENT_PEN_TOUCH,
    PAL_EVENT_PEN_MOTION,
    PAL_EVENT_PEN_BUTTON,
    PAL_EVENT_PEN_AXIS,

    PAL_EVENT_CAMERA_DEVICE_ADDED = 0x1500,
    PAL_EVENT_CAMERA_DEVICE_REMOVED,

    PAL_EVENT_TEXT_EDITING_CANDIDATES = 0x1600,

    PAL_EVENT_USER = 0x8000,

    PAL_EVENT_LAST = 0xFFFF
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
    int32_t keyboard_id;
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
    int32_t mouse_id;
} pal_mouse_motion_event;

typedef struct pal_mouse_button_event {
    int32_t x;
    int32_t y;
    uint8_t pressed;
    uint32_t button;
    uint8_t clicks;
    uint32_t modifiers;
    int32_t mouse_id;
} pal_mouse_button_event;

typedef struct pal_mouse_wheel_event {
    int32_t mouse_x;
    int32_t mouse_y;
    float x;
    float y;
    uint32_t modifiers;
    int wheel_direction; // some weird ahh mice have horizontal scroll wheels
    int32_t mouse_id;
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
    uint8_t level; // 0-100
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
    void *data1;
    void *data2;
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
    const char **paths;
    int32_t count;
} pal_drop_event;

typedef struct pal_clipboard_event {
    const char *text;
} pal_clipboard_event;

typedef struct pal_event {
    uint32_t type; /**< Event type, shared with all events, Uint32 to cover user events which are not in the pal_event_type enumeration */
                   // Event data:
    union {
        pal_common_event common;                           /**< Common event data */
        pal_display_event display;                         /**< Display event data */
        pal_window_event window;                           /**< Window event data */
        pal_keyboard_device_event kdevice;                 /**< Keyboard device change event data */
        pal_keyboard_event key;                            /**< Keyboard event data */
        pal_text_editing_event edit;                       /**< Text editing event data */
        pal_text_editing_candidates_event edit_candidates; /**< Text editing candidates event data */
        pal_text_input_event text;                         /**< Text input event data */
        pal_mouse_device_event mdevice;                    /**< Mouse device change event data */
        pal_mouse_motion_event motion;                     /**< Mouse motion event data */
        pal_mouse_button_event button;                     /**< Mouse button event data */
        pal_mouse_wheel_event wheel;                       /**< Mouse wheel event data */
        pal_joy_device_event jdevice;                      /**< Joystick device change event data */
        pal_joy_axis_event jaxis;                          /**< Joystick axis event data */
        pal_joy_ball_event jball;                          /**< Joystick ball event data */
        pal_joy_hat_event jhat;                            /**< Joystick hat event data */
        pal_joy_button_event jbutton;                      /**< Joystick button event data */
        pal_joy_battery_event jbattery;                    /**< Joystick battery event data */
        pal_gamepad_device_event gdevice;                  /**< Gamepad device event data */
        pal_gamepad_axis_event gaxis;                      /**< Gamepad axis event data */
        pal_gamepad_button_event gbutton;                  /**< Gamepad button event data */
        pal_gamepad_touchpad_event gtouchpad;              /**< Gamepad touchpad event data */
        pal_gamepad_sensor_event gsensor;                  /**< Gamepad sensor event data */
        pal_audio_device_event adevice;                    /**< Audio device event data */
        pal_camera_device_event cdevice;                   /**< Camera device event data */
        pal_sensor_event sensor;                           /**< Sensor event data */
        pal_quit_event quit;                               /**< Quit request event data */
        pal_user_event user;                               /**< Custom event data */
        pal_touch_finger_event tfinger;                    /**< Touch finger event data */
        pal_pen_proximity_event pproximity;                /**< Pen proximity event data */
        pal_pen_touch_event ptouch;                        /**< Pen tip touching event data */
        pal_pen_motion_event pmotion;                      /**< Pen motion event data */
        pal_pen_button_event pbutton;                      /**< Pen button event data */
        pal_pen_axis_event paxis;                          /**< Pen axis event data */
        pal_render_event render;                           /**< Render event data */
        pal_drop_event drop;                               /**< Drag and drop event data */
        pal_clipboard_event clipboard;                     /**< Clipboard event data */
    };

    uint8_t padding[128];
} pal_event;

typedef struct pal_event_queue {
    size_t size;
    size_t capacity;
    int front;
    int back;
    pal_event *events;
} pal_event_queue;

#if defined(_WIN32)
#if defined(__TINYC__)
#define __declspec(x) __attribute__((x))
#endif

#if defined(PAL_BUILD_SHARED)
#define PALAPI __declspec(dllexport) // We are building the library as a Win32 shared library (.dll)
#elif defined(PAL_USE_SHARED)
#define PALAPI __declspec(dllimport) // We are using the library as a Win32 shared library (.dll)
#endif

#else
#if defined(PAL_BUILD_SHARED)
#define PALAPI __attribute__((visibility("default"))) // We are building as a Unix shared library (.so/.dylib)
#endif
#endif

#ifndef PALAPI
#define PALAPI extern // extern is default, but it doesn't hurt to be explicit.
#endif

//----------------------------------------------------------------------------------
// Math Defines
//----------------------------------------------------------------------------------
#define PAL_PI 3.14159265358979323846f
#define PAL_DEG2RAD (PAL_PI / 180.0f)
#define PAL_RAD2DEG (180.0f / PAL_PI)

typedef struct {
    int width, height, x, y;
}pal_rect;

typedef struct pal_vec2 {
    union {
        float x, r, u, s;
    };
    union {
        float y, g, v, t;
    };
} pal_vec2;

typedef struct pal_vec3 {
    union {
        float x, r, u, s;
    };
    union {
        float y, g, v, t;
    };
    union {
        float z, b;
    };
} pal_vec3;

typedef struct pal_vec4 {
    union {
        float x, r, u, s;
    };
    union {
        float y, g, v, t;
    };
    union {
        float z, b;
    };
    union {
        float w, a;
    };
} pal_vec4;

typedef struct pal_ivec2 {
    union {
        int32_t x, r, u, s;
    };
    union {
        int32_t y, g, v, t;
    };
} pal_ivec2;

typedef struct pal_ivec3 {
    union {
        int32_t x, r, u, s;
    };
    union {
        int32_t y, g, v, t;
    };
    union {
        int32_t z, b;
    };
} pal_ivec3;

typedef struct pal_ivec4 {
    union {
        int32_t x, r, u, s;
    };
    union {
        int32_t y, g, v, t;
    };
    union {
        int32_t z, b;
    };
    union {
        int32_t w, a;
    };
} pal_ivec4;

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
#define PAL_WINDOW_OCCLUDED 0x4 // unimplemented
#define PAL_WINDOW_HIDDEN 0x8
#define PAL_WINDOW_TRANSPARENT 0x400000 // unimplemented
#define PAL_WINDOW_ALWAYS_ON_TOP 0x8000

// graphics APIs
#define PAL_WINDOW_OPENGL 0x2
#define PAL_WINDOW_VULKAN 0x100000 // unimplemented
#define PAL_WINDOW_METAL 0x200000  // unimplemented

// window type
#define PAL_WINDOW_MODAL 0x1000 // unimplemented
#define PAL_WINDOW_UTILITY 0x10000
#define PAL_WINDOW_TOOLTIP 0x20000
#define PAL_WINDOW_POPUP_MENU 0x40000

#define PAL_WINDOW_INPUT_FOCUS 0x200 // unimplemented
#define PAL_WINDOW_MOUSE_FOCUS 0x400 // unimplemented
#define PAL_WINDOW_MOUSE_CONFINED 0x100
#define PAL_WINDOW_KEYBOARD_GRABBED 0x80000 // unimplemented
#define PAL_WINDOW_NOT_FOCUSABLE 0x800000   // unimplemented
#define PAL_WINDOW_MOUSE_CAPTURE 0x4000     // unimplemented
// unreleated stuff.
#define PAL_WINDOW_EXTERNAL 0x800
#define PAL_WINDOW_HIGH_PIXEL_DENSITY 0x2000 // not implemented

//----------------------------------------------------------------------------------
// Mouse Buttons
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

//----------------------------------------------------------------------------------
// Mouse Wheel
//----------------------------------------------------------------------------------
#define PAL_MOUSEWHEEL_VERTICAL 0x0
#define PAL_MOUSEWHEEL_HORIZONTAL 0x1

//----------------------------------------------------------------------------------
// Keys
//----------------------------------------------------------------------------------
#define PAL_KEY_NONE 0
#define PAL_KEY_BACKSPACE 0x08
#define PAL_KEY_TAB 0x09
#define PAL_KEY_ENTER 0x0D
#define PAL_KEY_SHIFT 0x10
#define PAL_KEY_CONTROL 0x11
#define PAL_KEY_ALT 0x12
#define PAL_KEY_META PAL_KEY_ALT
#define PAL_KEY_OPTION PAL_KEY_ALT
#define PAL_KEY_PAUSE 0x13
#define PAL_KEY_CAPSLOCK 0x14
#define PAL_KEY_ESCAPE 0x1B
#define PAL_KEY_SPACE 0x20
#define PAL_KEY_PAGEUP 0x21
#define PAL_KEY_PAGEDOWN 0x22
#define PAL_KEY_END 0x23
#define PAL_KEY_HOME 0x24
#define PAL_KEY_LEFT 0x25
#define PAL_KEY_UP 0x26
#define PAL_KEY_RIGHT 0x27
#define PAL_KEY_DOWN 0x28
#define PAL_KEY_PRINTSCREEN 0x2C
#define PAL_KEY_INSERT 0x2D
#define PAL_KEY_DELETE 0x2E

#define PAL_KEY_0 0x30
#define PAL_KEY_1 0x31
#define PAL_KEY_2 0x32
#define PAL_KEY_3 0x33
#define PAL_KEY_4 0x34
#define PAL_KEY_5 0x35
#define PAL_KEY_6 0x36
#define PAL_KEY_7 0x37
#define PAL_KEY_8 0x38
#define PAL_KEY_9 0x39

#define PAL_KEY_LWIN 0x3a
#define PAL_KEY_RWIN 0x 3b
#define PAL_KEY_LSUPER PAL_KEY_LWIN
#define PAL_KEY_RSUPER PAL_KEY_RWIN
#define PAL_KEY_LCOMMAND PAL_KEY_LWIN
#define PAL_KEY_RCOMMAND PAL_KEY_RWIN
#define PAL_KEY_APPS 0x3c

#define PAL_KEY_NUMPAD_0 0x3d
#define PAL_KEY_NUMPAD_1 0x3e
#define PAL_KEY_NUMPAD_2 0x3f
#define PAL_KEY_NUMPAD_3 0x40
#define PAL_KEY_NUMPAD_4 0x41
#define PAL_KEY_NUMPAD_5 0x42
#define PAL_KEY_NUMPAD_6 0x43
#define PAL_KEY_NUMPAD_7 0x44
#define PAL_KEY_NUMPAD_8 0x45
#define PAL_KEY_NUMPAD_9 0x46
#define PAL_KEY_MULTIPLY 0x47
#define PAL_KEY_ADD 0x48
#define PAL_KEY_SEPARATOR 0x49 // Most modern Keyboards don't have this.
#define PAL_KEY_SUBTRACT 0x4a
#define PAL_KEY_DECIMAL 0x4b
#define PAL_KEY_DIVIDE 0x4c

#define PAL_KEY_A 0x4d
#define PAL_KEY_B 0x4e
#define PAL_KEY_C 0x4f
#define PAL_KEY_D 0x50
#define PAL_KEY_E 0x51
#define PAL_KEY_F 0x52
#define PAL_KEY_G 0x53
#define PAL_KEY_H 0x54
#define PAL_KEY_I 0x55
#define PAL_KEY_J 0x56
#define PAL_KEY_K 0x57
#define PAL_KEY_L 0x58
#define PAL_KEY_M 0x59
#define PAL_KEY_N 0x5A
#define PAL_KEY_O 0x5B
#define PAL_KEY_P 0x5c
#define PAL_KEY_Q 0x5d
#define PAL_KEY_R 0x5e
#define PAL_KEY_S 0x5f
#define PAL_KEY_T 0x60
#define PAL_KEY_U 0x61
#define PAL_KEY_V 0x62
#define PAL_KEY_W 0x63
#define PAL_KEY_X 0x64
#define PAL_KEY_Y 0x65
#define PAL_KEY_Z 0x66

#define PAL_KEY_F1 0x70
#define PAL_KEY_F2 0x71
#define PAL_KEY_F3 0x72
#define PAL_KEY_F4 0x73
#define PAL_KEY_F5 0x74
#define PAL_KEY_F6 0x75
#define PAL_KEY_F7 0x76
#define PAL_KEY_F8 0x77
#define PAL_KEY_F9 0x78
#define PAL_KEY_F10 0x79
#define PAL_KEY_F11 0x7A
#define PAL_KEY_F12 0x7B

#define PAL_KEY_F13 0x7c
#define PAL_KEY_F14 0x7d
#define PAL_KEY_F15 0x7e
#define PAL_KEY_F16 0x7f
#define PAL_KEY_F17 0x80
#define PAL_KEY_F18 0x81
#define PAL_KEY_F19 0x82
#define PAL_KEY_F20 0x83
#define PAL_KEY_F21 0x84
#define PAL_KEY_F22 0x85
#define PAL_KEY_F23 0x86
#define PAL_KEY_F24 0x87

#define PAL_KEY_NUMLOCK 0x90
#define PAL_KEY_SCROLLLOCK 0x91

#define PAL_KEY_BACKSLASH 0x92
#define PAL_KEY_FORWARD_SLASH 0x93
#define PAL_KEY_COMMA 0x94
#define PAL_KEY_PERIOD 0x95
#define PAL_KEY_DOT PAL_KEY_PERIOD

#define PAL_KEY_BACKTICK 0x96
#define PAL_KEY_GRAVE PAL_KEY_BACKTICK

#define PAL_KEY_RIGHTBRACE 0x97
#define PAL_KEY_LEFTBRACE 0x98
#define PAL_KEY_SEMICOLON 0x99
#define PAL_KEY_APOSTROPHE 0x9a
#define PAL_KEY_MINUS 0x9b
#define PAL_KEY_EQUAL 0x9c

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
// Scancodes
//----------------------------------------------------------------------------------
#define PAL_SCAN_A 4
#define PAL_SCAN_B 5
#define PAL_SCAN_C 6
#define PAL_SCAN_D 7
#define PAL_SCAN_E 8
#define PAL_SCAN_F 9
#define PAL_SCAN_G 10
#define PAL_SCAN_H 11
#define PAL_SCAN_I 12
#define PAL_SCAN_J 13
#define PAL_SCAN_K 14
#define PAL_SCAN_L 15
#define PAL_SCAN_M 16
#define PAL_SCAN_N 17
#define PAL_SCAN_O 18
#define PAL_SCAN_P 19
#define PAL_SCAN_Q 20
#define PAL_SCAN_R 21
#define PAL_SCAN_S 22
#define PAL_SCAN_T 23
#define PAL_SCAN_U 24
#define PAL_SCAN_V 25
#define PAL_SCAN_W 26
#define PAL_SCAN_X 27
#define PAL_SCAN_Y 28
#define PAL_SCAN_Z 29
#define PAL_SCAN_1 30
#define PAL_SCAN_2 31
#define PAL_SCAN_3 32
#define PAL_SCAN_4 33
#define PAL_SCAN_5 34
#define PAL_SCAN_6 35
#define PAL_SCAN_7 36
#define PAL_SCAN_8 37
#define PAL_SCAN_9 38
#define PAL_SCAN_0 39
#define PAL_SCAN_RETURN 40
#define PAL_SCAN_ESCAPE 41
#define PAL_SCAN_BACKSPACE 42
#define PAL_SCAN_TAB 43
#define PAL_SCAN_SPACE 44
#define PAL_SCAN_MINUS 45
#define PAL_SCAN_EQUALS 46
#define PAL_SCAN_LEFTBRACKET 47
#define PAL_SCAN_RIGHTBRACKET 48
#define PAL_SCAN_BACKSLASH 49 /**< Located at the lower left of the return   \
                               *   key on ISO keyboards and at the right end \
                               *   of the QWERTY row on ANSI keyboards.      \
                               *   Produces REVERSE SOLIDUS (backslash) and  \
                               *   VERTICAL LINE in a US layout REVERSE      \
                               *   SOLIDUS and VERTICAL LINE in a UK Mac     \
                               *   layout NUMBER SIGN and TILDE in a UK      \
                               *   Windows layout DOLLAR SIGN and POUND SIGN \
                               *   in a Swiss German layout NUMBER SIGN and  \
                               *   APOSTROPHE in a German layout GRAVE       \
                               *   ACCENT and POUND SIGN in a French Mac     \
                               *   layout and ASTERISK and MICRO SIGN in a   \
                               *   French Windows layout.                    \
                               */
#define PAL_SCAN_NONUSHASH 50 /**< ISO USB keyboards actually use this code  \
                               *   instead of 49 for the same key but all    \
                               *   OSes I've seen treat the two codes        \
                               *   identically. So as an implementor unless  \
                               *   your keyboard generates both of those     \
                               *   codes and your OS treats them differently \
                               *   you should generate PAL_SCAN_BACKSLASH    \
                               *   instead of this code. As a user you       \
                               *   should not rely on this code because SDL  \
                               *   will never generate it with most (all?)   \
                               *   keyboards.                                \
                               */
#define PAL_SCAN_SEMICOLON 51
#define PAL_SCAN_APOSTROPHE 52
#define PAL_SCAN_GRAVE 53 /**< Located in the top left corner (on both ANSI  \
                           *   and ISO keyboards). Produces GRAVE ACCENT and \
                           *   TILDE in a US Windows layout and in US and UK \
                           *   Mac layouts on ANSI keyboards GRAVE ACCENT    \
                           *   and NOT SIGN in a UK Windows layout SECTION   \
                           *   SIGN and PLUS-MINUS SIGN in US and UK Mac     \
                           *   layouts on ISO keyboards SECTION SIGN and     \
                           *   DEGREE SIGN in a Swiss German layout (Mac:    \
                           *   only on ISO keyboards) CIRCUMFLEX ACCENT and  \
                           *   DEGREE SIGN in a German layout (Mac: only on  \
                           *   ISO keyboards) SUPERSCRIPT TWO and TILDE in a \
                           *   French Windows layout COMMERCIAL AT and       \
                           *   NUMBER SIGN in a French Mac layout on ISO     \
                           *   keyboards and LESS-THAN SIGN and GREATER-THAN \
                           *   SIGN in a Swiss German German or French Mac   \
                           *   layout on ANSI keyboards.                     \
                           */
#define PAL_SCAN_COMMA 54
#define PAL_SCAN_PERIOD 55
#define PAL_SCAN_SLASH 56
#define PAL_SCAN_CAPSLOCK 57
#define PAL_SCAN_F1 58
#define PAL_SCAN_F2 59
#define PAL_SCAN_F3 60
#define PAL_SCAN_F4 61
#define PAL_SCAN_F5 62
#define PAL_SCAN_F6 63
#define PAL_SCAN_F7 64
#define PAL_SCAN_F8 65
#define PAL_SCAN_F9 66
#define PAL_SCAN_F10 67
#define PAL_SCAN_F11 68
#define PAL_SCAN_F12 69
#define PAL_SCAN_PRINTSCREEN 70
#define PAL_SCAN_SCROLLLOCK 71
#define PAL_SCAN_PAUSE 72
#define PAL_SCAN_INSERT 73 /**< insert on PC help on some Mac keyboards (but does send code 73 not 117) */
#define PAL_SCAN_HOME 74
#define PAL_SCAN_PAGEUP 75
#define PAL_SCAN_DELETE 76
#define PAL_SCAN_END 77
#define PAL_SCAN_PAGEDOWN 78
#define PAL_SCAN_RIGHT 79
#define PAL_SCAN_LEFT 80
#define PAL_SCAN_DOWN 81
#define PAL_SCAN_UP 82
#define PAL_SCAN_NUMCLEAR 83 /**< num lock on PC clear on Mac keyboards */
#define PAL_SCAN_KP_DIVIDE 84
#define PAL_SCAN_KP_MULTIPLY 85
#define PAL_SCAN_KP_MINUS 86
#define PAL_SCAN_KP_PLUS 87
#define PAL_SCAN_KP_ENTER 88
#define PAL_SCAN_KP_1 89
#define PAL_SCAN_KP_2 90
#define PAL_SCAN_KP_3 91
#define PAL_SCAN_KP_4 92
#define PAL_SCAN_KP_5 93
#define PAL_SCAN_KP_6 94
#define PAL_SCAN_KP_7 95
#define PAL_SCAN_KP_8 96
#define PAL_SCAN_KP_9 97
#define PAL_SCAN_KP_0 98
#define PAL_SCAN_KP_PERIOD 99
#define PAL_SCAN_NONUSBACKSLASH 100 /**< This is the additional key that ISO     \
                                #define *   keyboards have over ANSI ones        \
                                #define *   located between left shift and Y.    \
                                #define *   Produces GRAVE ACCENT and TILDE in a \
                                #define *   US or UK Mac layout REVERSE SOLIDUS  \
                                #define *   (backslash) and VERTICAL LINE in a   \
                                #define *   US or UK Windows layout and          \
                                #define *   LESS-THAN SIGN and GREATER-THAN SIGN \
                                #define *   in a Swiss German German or French   \
                                #define *   layout. */
#define PAL_SCAN_APPLICATION 101    /**< windows contextual menu compose */
#define PAL_SCAN_POWER 102          /**< The USB document says this is a status flag    \
                                #define *   not a physical key - but some Mac keyboards \
                                #define *   do have a power key. */
#define PAL_SCAN_KP_EQUALS 103
#define PAL_SCAN_F13 104
#define PAL_SCAN_F14 105
#define PAL_SCAN_F15 106
#define PAL_SCAN_F16 107
#define PAL_SCAN_F17 108
#define PAL_SCAN_F18 109
#define PAL_SCAN_F19 110
#define PAL_SCAN_F20 111
#define PAL_SCAN_F21 112
#define PAL_SCAN_F22 113
#define PAL_SCAN_F23 114
#define PAL_SCAN_F24 115
#define PAL_SCAN_EXECUTE 116
#define PAL_SCAN_HELP 117 /**< AL Integrated Help Center */
#define PAL_SCAN_MENU 118 /**< Menu (show menu) */
#define PAL_SCAN_SELECT 119
#define PAL_SCAN_STOP 120  /**< AC Stop */
#define PAL_SCAN_AGAIN 121 /**< AC Redo/Repeat */
#define PAL_SCAN_UNDO 122  /**< AC Undo */
#define PAL_SCAN_CUT 123   /**< AC Cut */
#define PAL_SCAN_COPY 124  /**< AC Copy */
#define PAL_SCAN_PASTE 125 /**< AC Paste */
#define PAL_SCAN_FIND 126  /**< AC Find */
#define PAL_SCAN_MUTE 127
#define PAL_SCAN_VOLUMEUP 128
#define PAL_SCAN_VOLUMEDOWN 129
#define PAL_SCAN_LOCKINGCAPSLOCK 130
#define PAL_SCAN_LOCKINGNUMLOCK 131
#define PAL_SCAN_LOCKINGSCROLLLOCK 132
#define PAL_SCAN_KP_COMMA 133
#define PAL_SCAN_KP_EQUALSAS400 134
#define PAL_SCAN_INTERNATIONAL1 135 /**< used on Asian keyboards see */
#define PAL_SCAN_INTERNATIONAL2 136
#define PAL_SCAN_INTERNATIONAL3 137 /**< Yen */
#define PAL_SCAN_INTERNATIONAL4 138
#define PAL_SCAN_INTERNATIONAL5 139
#define PAL_SCAN_INTERNATIONAL6 140
#define PAL_SCAN_INTERNATIONAL7 141
#define PAL_SCAN_INTERNATIONAL8 142
#define PAL_SCAN_INTERNATIONAL9 143
#define PAL_SCAN_LANG1 144    /**< Hangul/English toggle */
#define PAL_SCAN_LANG2 145    /**< Hanja conversion */
#define PAL_SCAN_LANG3 146    /**< Katakana */
#define PAL_SCAN_LANG4 147    /**< Hiragana */
#define PAL_SCAN_LANG5 148    /**< Zenkaku/Hankaku */
#define PAL_SCAN_LANG6 149    /**< reserved */
#define PAL_SCAN_LANG7 150    /**< reserved */
#define PAL_SCAN_LANG8 151    /**< reserved */
#define PAL_SCAN_LANG9 152    /**< reserved */
#define PAL_SCAN_ALTERASE 153 /**< Erase-Eaze */
#define PAL_SCAN_SYSREQ 154
#define PAL_SCAN_CANCEL 155 /**< AC Cancel */
#define PAL_SCAN_CLEAR 156
#define PAL_SCAN_PRIOR 157
#define PAL_SCAN_RETURN2 158
#define PAL_SCAN_SEPARATOR 159
#define PAL_SCAN_OUT 160
#define PAL_SCAN_OPER 161
#define PAL_SCAN_CLEARAGAIN 162
#define PAL_SCAN_CRSEL 163
#define PAL_SCAN_EXSEL 164
#define PAL_SCAN_KP_00 176
#define PAL_SCAN_KP_000 177
#define PAL_SCAN_THOUSANDSSEPARATOR 178
#define PAL_SCAN_DECIMALSEPARATOR 179
#define PAL_SCAN_CURRENCYUNIT 180
#define PAL_SCAN_CURRENCYSUBUNIT 181
#define PAL_SCAN_KP_LEFTPAREN 182
#define PAL_SCAN_KP_RIGHTPAREN 183
#define PAL_SCAN_KP_LEFTBRACE 184
#define PAL_SCAN_KP_RIGHTBRACE 185
#define PAL_SCAN_KP_TAB 186
#define PAL_SCAN_KP_BACKSPACE 187
#define PAL_SCAN_KP_A 188
#define PAL_SCAN_KP_B 189
#define PAL_SCAN_KP_C 190
#define PAL_SCAN_KP_D 191
#define PAL_SCAN_KP_E 192
#define PAL_SCAN_KP_F 193
#define PAL_SCAN_KP_XOR 194
#define PAL_SCAN_KP_POWER 195
#define PAL_SCAN_KP_PERCENT 196
#define PAL_SCAN_KP_LESS 197
#define PAL_SCAN_KP_GREATER 198
#define PAL_SCAN_KP_AMPERSAND 199
#define PAL_SCAN_KP_DBLAMPERSAND 200
#define PAL_SCAN_KP_VERTICALBAR 201
#define PAL_SCAN_KP_DBLVERTICALBAR 202
#define PAL_SCAN_KP_COLON 203
#define PAL_SCAN_KP_HASH 204
#define PAL_SCAN_KP_SPACE 205
#define PAL_SCAN_KP_AT 206
#define PAL_SCAN_KP_EXCLAM 207
#define PAL_SCAN_KP_MEMSTORE 208
#define PAL_SCAN_KP_MEMRECALL 209
#define PAL_SCAN_KP_MEMCLEAR 210
#define PAL_SCAN_KP_MEMADD 211
#define PAL_SCAN_KP_MEMSUBTRACT 212
#define PAL_SCAN_KP_MEMMULTIPLY 213
#define PAL_SCAN_KP_MEMDIVIDE 214
#define PAL_SCAN_KP_PLUSMINUS 215
#define PAL_SCAN_KP_CLEAR 216
#define PAL_SCAN_KP_CLEARENTRY 217
#define PAL_SCAN_KP_BINARY 218
#define PAL_SCAN_KP_OCTAL 219
#define PAL_SCAN_KP_DECIMAL 220
#define PAL_SCAN_KP_HEXADECIMAL 221
// 222 - 223 reserved
#define PAL_SCAN_LCTRL 224
#define PAL_SCAN_LSHIFT 225
#define PAL_SCAN_LALT 226 /**< alt option */
#define PAL_SCAN_LGUI 227 /**< windows command (apple) meta */
#define PAL_SCAN_RCTRL 228
#define PAL_SCAN_RSHIFT 229
#define PAL_SCAN_RALT 230  /**< alt gr option */
#define PAL_SCAN_RGUI 231  /**< windows command (apple) meta */
#define PAL_SCAN_ALTGR 257 /**< I'm not sure if this is really not covered      \
                            *   by any of the above but since there's a         \
                            *   special SDL_KMOD_MODE for it I'm adding it here \
                            */
/* @} */                   /* Usage page 0x07 */
/**
 *  \name Usage page 0x0C
 *
 *  These values are mapped from usage page 0x0C (USB consumer page).
 *
 *  There are way more keys in the spec than we can represent in the
 *  current scancode range so pick the ones that commonly come up in
 *  real world usage.
 */
/* @{ */
#define PAL_SCAN_SLEEP 258                /**< Sleep */
#define PAL_SCAN_WAKE 259                 /**< Wake */
#define PAL_SCAN_CHANNEL_INCREMENT 260    /**< Channel Increment */
#define PAL_SCAN_CHANNEL_DECREMENT 261    /**< Channel Decrement */
#define PAL_SCAN_MEDIA_PLAY 262           /**< Play */
#define PAL_SCAN_MEDIA_PAUSE 263          /**< Pause */
#define PAL_SCAN_MEDIA_RECORD 264         /**< Record */
#define PAL_SCAN_MEDIA_FAST_FORWARD 265   /**< Fast Forward */
#define PAL_SCAN_MEDIA_REWIND 266         /**< Rewind */
#define PAL_SCAN_MEDIA_NEXT_TRACK 267     /**< Next Track */
#define PAL_SCAN_MEDIA_PREVIOUS_TRACK 268 /**< Previous Track */
#define PAL_SCAN_MEDIA_STOP 269           /**< Stop */
#define PAL_SCAN_MEDIA_EJECT 270          /**< Eject */
#define PAL_SCAN_MEDIA_PLAY_PAUSE 271     /**< Play / Pause */
#define PAL_SCAN_MEDIA_SELECT 272         /* Media Select */
#define PAL_SCAN_AC_NEW 273               /**< AC New */
#define PAL_SCAN_AC_OPEN 274              /**< AC Open */
#define PAL_SCAN_AC_CLOSE 275             /**< AC Close */
#define PAL_SCAN_AC_EXIT 276              /**< AC Exit */
#define PAL_SCAN_AC_SAVE 277              /**< AC Save */
#define PAL_SCAN_AC_PRINT 278             /**< AC Print */
#define PAL_SCAN_AC_PROPERTIES 279        /**< AC Properties */
#define PAL_SCAN_AC_SEARCH 280            /**< AC Search */
#define PAL_SCAN_AC_HOME 281              /**< AC Home */
#define PAL_SCAN_AC_BACK 282              /**< AC Back */
#define PAL_SCAN_AC_FORWARD 283           /**< AC Forward */
#define PAL_SCAN_AC_STOP 284              /**< AC Stop */
#define PAL_SCAN_AC_REFRESH 285           /**< AC Refresh */
#define PAL_SCAN_AC_BOOKMARKS 286         /**< AC Bookmarks */

/* @} */ /* Usage page 0x0C */

/**
 *  \name Mobile keys
 *
 *  These are values that are often used on mobile phones.
 */
/* @{ */

#define PAL_SCAN_SOFTLEFT 287  /**< Usually situated below the display on phones and \
                               used as a multi-function feature key for selecting    \
                               a software defined function shown on the bottom left  \
                               of the display. */
#define PAL_SCAN_SOFTRIGHT 288 /**< Usually situated below the display on phones and \
                               used as a multi-function feature key for selecting    \
                               a software defined function shown on the bottom right \
                               of the display. */
#define PAL_SCAN_CALL 289      /**< Used for accepting phone calls. */
#define PAL_SCAN_ENDCALL 290   /**< Used for rejecting phone calls. */

/* @} */ /* Mobile keys */

/* Add any other keys here. */

#define PAL_SCAN_RESERVED 400 /**< 400-500 reserved for dynamic keycodes */

#define PAL_SCAN_COUNT 512 /**< not a key just marks the number of scancodes for array bounds */

//----------------------------------------------------------------------------------
// True and False defines
//----------------------------------------------------------------------------------
#define pal_true 1
#define pal_false 0

//----------------------------------------------------------------------------------
// File Permissions.
//----------------------------------------------------------------------------------
#define PAL_READ 0x01
#define PAL_WRITE 0x02
#define PAL_EXECUTE 0x04

#if defined(__cplusplus)
#define CLITERAL(type) type
#else
#define CLITERAL(type) (type)
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Init and Shutdown
PALAPI void pal_init(void);
PALAPI void pal_shutdown(void);

// Video and Windowing subsystem.
PALAPI pal_window *pal_create_window(int width, int height, const char *windowTitle, uint64_t window_flags);
PALAPI void pal_close_window(pal_window *window);
PALAPI pal_ivec2 pal_get_window_border_size(pal_window *window);
PALAPI void *pal_get_window_handle(pal_window *window);
PALAPI int pal_show_cursor(pal_window *window);
PALAPI int pal_hide_cursor(pal_window *window);
PALAPI pal_bool pal_set_window_title(pal_window *window, const char *string);
PALAPI pal_bool pal_make_window_fullscreen(pal_window *window);
PALAPI pal_bool pal_make_window_fullscreen_ex(pal_window *window, int width, int height, int refresh_rate);
PALAPI pal_bool pal_make_window_fullscreen_windowed(pal_window *window);
PALAPI pal_bool pal_make_window_windowed(pal_window *window);
PALAPI pal_bool pal_maximize_window(pal_window *window);
PALAPI pal_bool pal_minimize_window(pal_window *window);
PALAPI void pal_set_window_icon(pal_window *window, const char *image_path);
// I am thinking that a better way of doing legacy stuff is with macros.
// Just have a macro like WINDOWS_XP_OR_LATER or WINDOWS_VISTA_OR_LATER.
PALAPI void pal_set_window_icon_legacy(pal_window *window, const char *image_path);
PALAPI void pal_set_taskbar_icon(pal_window *taskbar, const char *image_path);
PALAPI void pal_set_taskbar_icon_legacy(pal_window *taskbar, const char *image_path);
PALAPI void pal_set_cursor(pal_window *window, const char *image_path, int size, int hotspot_x, int hotspot_y);
PALAPI pal_video_mode *pal_get_video_mode(pal_monitor *monitor);
PALAPI pal_bool pal_set_video_mode(pal_video_mode *mode);
PALAPI pal_monitor *pal_get_primary_monitor(void);
PALAPI void *pal_gl_get_proc_address(const char *proc);
PALAPI pal_bool pal_poll_events(pal_event *event);
PALAPI int pal_make_context_current(pal_window *window);

// Rendering functions (implemented using GDI on windows and X11 on linux)
PALAPI void pal_draw_rect(pal_window *window, int x, int y, int width, int height, pal_vec4 color);

// Image Loading
PALAPI unsigned char *pal_load_image(char const *filename, int *x, int *y, int *comp, int req_comp);

// Keyboard input
PALAPI int pal_get_keyboard_count(void);
PALAPI const char *pal_get_keyboard_name(int keyboard_id);
PALAPI int pal_get_keyboard_indices(int key, int *keyboard_indices);
PALAPI pal_bool pal_is_key_pressed(int keyboard_id, int key);
PALAPI pal_bool pal_is_key_down(int keyboard_id, int key);

// Mouse input
PALAPI int pal_get_mouse_count(void);
PALAPI const char *pal_get_mouse_name(int mouse_id);
PALAPI int pal_get_mouse_indices(int *mouse_indices);
PALAPI pal_bool pal_is_mouse_down(int mouse_id, int button);
PALAPI pal_bool pal_is_mouse_pressed(int mouse_id, int button);
PALAPI pal_vec2 pal_get_mouse_delta(int mouse_id);
PALAPI pal_vec2 pal_get_mouse_position(pal_window *window);

// Gamepad Input
PALAPI int pal_get_gamepad_count(void);
PALAPI pal_bool pal_get_gamepad_state(int index, pal_gamepad_state *out_state);
PALAPI void pal_set_gamepad_vibration(int controller_id, float left_motor, float right_motor, float left_trigger, float right_trigger);
PALAPI void pal_stop_gamepad_vibration(int controller_id);

// Rendering stuff.
PALAPI void pal_swap_buffers(pal_window *window);
PALAPI void pal_swap_interval(int interval);

// Sound
PALAPI pal_sound *pal_load_sound(const char *filename);
PALAPI int pal_play_sound(pal_sound *sound, float volume);
PALAPI int pal_stop_sound(pal_sound *sound);
PALAPI void pal_free_sound(pal_sound *sound);

PALAPI pal_sound *pal_load_music(const char *filename);
PALAPI int pal_play_music(pal_sound *sound, float volume);
PALAPI int pal_stop_music(pal_sound *sound); // unimplemented
PALAPI void pal_free_music(pal_sound *sound);

// File I/O
PALAPI pal_bool pal_does_file_exist(const char *file_path);
PALAPI size_t pal_get_last_write_time(const char *file);
PALAPI size_t pal_get_last_read_time(const char *file);
PALAPI size_t pal_get_file_size(const char *file_path);
PALAPI uint32_t pal_get_file_permissions(const char *file_path);
PALAPI pal_bool pal_change_file_permissions(const char *file_path, uint32_t permission_flags);
PALAPI unsigned char *pal_read_entire_file(const char *file_path, size_t *bytes_read);
PALAPI pal_bool pal_write_file(const char *file_path, size_t file_size, char *buffer);
PALAPI pal_bool pal_copy_file(const char *original_path, const char *copy_path);

// Directory Listing
PALAPI pal_bool pal_path_is_dir(const char *path);

// Open File I/O
PALAPI pal_file *pal_open_file(const char *file_path);
PALAPI pal_bool pal_read_from_open_file(pal_file *file, size_t offset, size_t bytes_to_read, char *buffer);
PALAPI pal_bool pal_close_file(pal_file *file);

// Random Number Generation
PALAPI void pal_srand(uint64_t *state, uint64_t seed);
PALAPI uint32_t pal_rand(uint64_t *state);

// Clip board
PALAPI void pal_clipboard_set(const char *text);
PALAPI char *pal_clipboard_get(void);

// File dialog / requester
PALAPI void pal_create_save_dialog(char **types, uint32_t type_count, void *id);
PALAPI void pal_create_load_dialog(char **types, uint32_t type_count, void *id);
PALAPI char *pal_show_save_dialog(void *id);
PALAPI char *pal_show_load_dialog(void *id);

// URL launch
PALAPI void pal_url_launch(char *url);

// Mouse Warp
PALAPI void pal_mouse_warp(int x, int y);
PALAPI void pal_mouse_warp_relative(int dx, int dy);

// File Parsing
PALAPI pal_bool pal_is_uppercase(char ch);
PALAPI pal_bool pal_is_lowercase(char ch);
PALAPI pal_bool pal_is_letter(char ch);
PALAPI pal_bool pal_is_end_of_line(char ch);
PALAPI pal_bool pal_is_whitespace(char ch);
PALAPI pal_bool pal_is_number(char ch);
PALAPI pal_bool pal_is_alphanumeric(char ch);
PALAPI pal_bool pal_is_underscore(char ch);
PALAPI pal_bool pal_is_hyphen(char ch);
PALAPI pal_bool pal_is_dot(char ch);
PALAPI pal_bool pal_are_chars_equal(char ch1, char ch2);
PALAPI pal_bool pal_are_strings_equal(const char *s1, const char *s2);
PALAPI int pal_strcmp(const char *s1, const char *s2);
PALAPI int pal_strncmp(const char *s1, const char *s2, size_t n);

// Time functions
PALAPI pal_time pal_get_date_and_time_utc(void);
PALAPI pal_time pal_get_date_and_time_local(void);
PALAPI pal_time pal_get_time_since_boot(void);
PALAPI double pal_get_time_since_pal_started(void);
PALAPI uint64_t pal_get_timer(void);
PALAPI uint64_t pal_get_timer_frequency(void);

// Multi-threadding functions
PALAPI pal_mutex *pal_create_mutex();
PALAPI void pal_lock_mutex(pal_mutex *mutex);
PALAPI pal_bool pal_lock_mutex_try(pal_mutex *mutex);
PALAPI void pal_unlock_mutex(pal_mutex *mutex);
PALAPI void pal_destroy_mutex(pal_mutex *mutex);

PALAPI pal_signal *pal_create_signal(void);
PALAPI pal_bool pal_activate_signal(pal_signal *signal);
PALAPI pal_bool pal_deactivate_signal(pal_signal *signal);
PALAPI pal_bool pal_wait_for_signal(pal_signal *signal, pal_mutex *mutex);
PALAPI void pal_destroy_signal(pal_signal *signal);

PALAPI pal_thread *pal_create_thread(pal_thread_func func, void *arg);
PALAPI pal_bool pal_start_thread(pal_thread *thread);
PALAPI pal_bool pal_join_thread(pal_thread *thread);
PALAPI void pal_destroy_thread(pal_thread *thread);

// .dll/.so/.dylib loading
PALAPI void *pal_load_dynamic_library(const char *dll);
PALAPI void *pal_load_dynamic_function(void *dll, char *func_name);
PALAPI pal_bool pal_free_dynamic_library(void *dll);

#ifdef __cplusplus
}
#endif

#endif // PAL_INCLUDE_H

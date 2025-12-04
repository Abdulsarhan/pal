#ifndef PAL_H
#define PAL_H

#include <stdint.h>   /* For Clearly Defined Types. */
#include <stddef.h>

/* defines for building or using this library as a shared lib */
#if defined(_WIN32)
    #if defined(__TINYC__)
        #define __declspec(x) __attribute__((x))
    #endif

    #if defined(PAL_BUILD_SHARED)
        #define PALAPI __declspec(dllexport) /* We are building the library as a Win32 shared library (.dll) */
    #elif defined(PAL_USE_SHARED)
        #define PALAPI __declspec(dllimport) /* We are using the library as a Win32 shared library (.dll) */
    #endif

#else
    #if defined(PAL_BUILD_SHARED)
        #define PALAPI __attribute__((visibility("default"))) /* We are building as a Unix shared library (.so/.dylib) */
    #endif
#endif

#ifndef PALAPI
    #define PALAPI extern /* extern is default, but it doesn't hurt to be explicit. */
#endif

/*---------------------------------------------------------------------------------- */
/* Obligatory CLITERAL macro */
/*---------------------------------------------------------------------------------- */
#if defined(__cplusplus)
    #define CLITERAL(type) type
#else
    #define CLITERAL(type) (type)
#endif

/*---------------------------------------------------------------------------------- */
/* Math Defines */
/*---------------------------------------------------------------------------------- */
#define PAL_PI 3.14159265358979323846f
#define PAL_DEG2RAD (PAL_PI / 180.0f)
#define PAL_RAD2DEG (180.0f / PAL_PI)

/*
----------------------------------------------------------------------------------
File Permissions.
----------------------------------------------------------------------------------
*/
#define PAL_READ 0x01
#define PAL_WRITE 0x02
#define PAL_EXECUTE 0x04

/*---------------------------------------------------------------------------------- */
/* Window Flags */
/*---------------------------------------------------------------------------------- */

/* window visibility */
#define PAL_WINDOW_FULLSCREEN 0x1
#define PAL_WINDOW_BORDERLESS 0x10
#define PAL_WINDOW_MINIMIZED 0x40
#define PAL_WINDOW_MAXIMIZED 0x80
#define PAL_WINDOW_RESIZABLE 0x20
#define PAL_WINDOW_OCCLUDED 0x4 /* unimplemented */
#define PAL_WINDOW_HIDDEN 0x8
#define PAL_WINDOW_TRANSPARENT 0x400000 /* unimplemented */
#define PAL_WINDOW_ALWAYS_ON_TOP 0x8000

/* graphics APIs */
#define PAL_WINDOW_OPENGL 0x2
#define PAL_WINDOW_VULKAN 0x100000 /* unimplemented */
#define PAL_WINDOW_METAL 0x200000  /* unimplemented */

/* window type */
#define PAL_WINDOW_MODAL 0x1000 /* unimplemented */
#define PAL_WINDOW_UTILITY 0x10000
#define PAL_WINDOW_TOOLTIP 0x20000
#define PAL_WINDOW_POPUP_MENU 0x40000

#define PAL_WINDOW_INPUT_FOCUS 0x200 /* unimplemented */
#define PAL_WINDOW_MOUSE_FOCUS 0x400 /* unimplemented */
#define PAL_WINDOW_MOUSE_CONFINED 0x100
#define PAL_WINDOW_KEYBOARD_GRABBED 0x80000 /* unimplemented */
#define PAL_WINDOW_NOT_FOCUSABLE 0x800000   /* unimplemented */
#define PAL_WINDOW_MOUSE_CAPTURE 0x4000     /* unimplemented */

/* unreleated stuff. */
#define PAL_WINDOW_EXTERNAL 0x800
#define PAL_WINDOW_HIGH_PIXEL_DENSITY 0x2000 /* not implemented */

/*---------------------------------------------------------------------------------- */
/* Mouse Buttons */
/*---------------------------------------------------------------------------------- */
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

/*
----------------------------------------------------------------------------------
Mouse Wheel
----------------------------------------------------------------------------------
*/
#define PAL_MOUSEWHEEL_VERTICAL 0x0
#define PAL_MOUSEWHEEL_HORIZONTAL 0x1

/*
----------------------------------------------------------------------------------
 Keys
----------------------------------------------------------------------------------
*/

#define PAL_KEY_EXTENDED_MASK              (1u << 29)
#define PAL_KEY_SCANCODE_MASK              (1u << 30)
#define PAL_SCANCODE_TO_KEYCODE(X)         (X | PAL_KEY_SCANCODE_MASK)
#define PAL_KEY_NONE                       0x00000000u /**< 0 */
#define PAL_KEY_RETURN                     0x0000000du /**< '\r' */
#define PAL_KEY_ENTER PAL_KEY_RETURN
#define PAL_KEY_ESCAPE                     0x0000001bu /**< '\x1B' */
#define PAL_KEY_BACKSPACE                  0x00000008u /**< '\b' */
#define PAL_KEY_TAB                        0x00000009u /**< '\t' */
#define PAL_KEY_SPACE                      0x00000020u /**< ' ' */
#define PAL_KEY_EXCLAIM                    0x00000021u /**< '!' */
#define PAL_KEY_DBLAPOSTROPHE              0x00000022u /**< '"' */
#define PAL_KEY_HASH                       0x00000023u /**< '#' */
#define PAL_KEY_DOLLAR                     0x00000024u /**< '$' */
#define PAL_KEY_PERCENT                    0x00000025u /**< '%' */
#define PAL_KEY_AMPERSAND                  0x00000026u /**< '&' */
#define PAL_KEY_APOSTROPHE                 0x00000027u /**< '\'' */
#define PAL_KEY_LEFTPAREN                  0x00000028u /**< '(' */
#define PAL_KEY_RIGHTPAREN                 0x00000029u /**< ')' */
#define PAL_KEY_ASTERISK                   0x0000002au /**< '*' */
#define PAL_KEY_STAR PAL_KEY_ASTERISK
#define PAL_KEY_MULTIPLY PAL_KEY_ASTERISK
#define PAL_KEY_PLUS                       0x0000002bu /**< '+' */
#define PAL_KEY_ADD PAL_KEY_PLUS
#define PAL_KEY_COMMA                      0x0000002cu /**< ',' */
#define PAL_KEY_MINUS                      0x0000002du /**< '-' */
#define PAL_KEY_SUBTRACT PAL_KEY_MINUS
#define PAL_KEY_PERIOD                     0x0000002eu /**< '.' */
#define PAL_KEY_DOT PAL_KEY_PERIOD
#define PAL_KEY_SLASH                      0x0000002fu /**< '/' */
#define PAL_KEY_DIVIDE PAL_KEY_SLASH
#define PAL_KEY_FORWARD_SLASH PAL_KEY_SLASH
#define PAL_KEY_0                          0x00000030u /**< '0' */
#define PAL_KEY_1                          0x00000031u /**< '1' */
#define PAL_KEY_2                          0x00000032u /**< '2' */
#define PAL_KEY_3                          0x00000033u /**< '3' */
#define PAL_KEY_4                          0x00000034u /**< '4' */
#define PAL_KEY_5                          0x00000035u /**< '5' */
#define PAL_KEY_6                          0x00000036u /**< '6' */
#define PAL_KEY_7                          0x00000037u /**< '7' */
#define PAL_KEY_8                          0x00000038u /**< '8' */
#define PAL_KEY_9                          0x00000039u /**< '9' */
#define PAL_KEY_COLON                      0x0000003au /**< ':' */
#define PAL_KEY_SEMICOLON                  0x0000003bu /**< ';' */
#define PAL_KEY_LESS                       0x0000003cu /**< '<' */
#define PAL_KEY_EQUALS                     0x0000003du /**< '=' */
#define PAL_KEY_EQUAL PAL_KEY_EQUALS
#define PAL_KEY_GREATER                    0x0000003eu /**< '>' */
#define PAL_KEY_QUESTION                   0x0000003fu /**< '?' */
#define PAL_KEY_AT                         0x00000040u /**< '@' */
#define PAL_KEY_LEFTBRACKET                0x0000005bu /**< '[' */
#define PAL_KEY_BACKSLASH                  0x0000005cu /**< '\\' */
#define PAL_KEY_RIGHTBRACKET               0x0000005du /**< ']' */
#define PAL_KEY_CARET                      0x0000005eu /**< '^' */
#define PAL_KEY_UNDERSCORE                 0x0000005fu /**< '_' */
#define PAL_KEY_GRAVE                      0x00000060u /**< '`' */
#define PAL_KEY_BACKTICK PAL_KEY_GRAVE
#define PAL_KEY_A                          0x00000061u /**< 'a' */
#define PAL_KEY_B                          0x00000062u /**< 'b' */
#define PAL_KEY_C                          0x00000063u /**< 'c' */
#define PAL_KEY_D                          0x00000064u /**< 'd' */
#define PAL_KEY_E                          0x00000065u /**< 'e' */
#define PAL_KEY_F                          0x00000066u /**< 'f' */
#define PAL_KEY_G                          0x00000067u /**< 'g' */
#define PAL_KEY_H                          0x00000068u /**< 'h' */
#define PAL_KEY_I                          0x00000069u /**< 'i' */
#define PAL_KEY_J                          0x0000006au /**< 'j' */
#define PAL_KEY_K                          0x0000006bu /**< 'k' */
#define PAL_KEY_L                          0x0000006cu /**< 'l' */
#define PAL_KEY_M                          0x0000006du /**< 'm' */
#define PAL_KEY_N                          0x0000006eu /**< 'n' */
#define PAL_KEY_O                          0x0000006fu /**< 'o' */
#define PAL_KEY_P                          0x00000070u /**< 'p' */
#define PAL_KEY_Q                          0x00000071u /**< 'q' */
#define PAL_KEY_R                          0x00000072u /**< 'r' */
#define PAL_KEY_S                          0x00000073u /**< 's' */
#define PAL_KEY_T                          0x00000074u /**< 't' */
#define PAL_KEY_U                          0x00000075u /**< 'u' */
#define PAL_KEY_V                          0x00000076u /**< 'v' */
#define PAL_KEY_W                          0x00000077u /**< 'w' */
#define PAL_KEY_X                          0x00000078u /**< 'x' */
#define PAL_KEY_Y                          0x00000079u /**< 'y' */
#define PAL_KEY_Z                          0x0000007au /**< 'z' */
#define PAL_KEY_LEFTBRACE                  0x0000007bu /**< '{' */
#define PAL_KEY_PIPE                       0x0000007cu /**< '|' */
#define PAL_KEY_BAR PAL_KEY_PIPE
#define PAL_KEY_VERTICAL_BAR PAL_KEY_PIPE
#define PAL_KEY_RIGHTBRACE                 0x0000007du /**< '}' */
#define PAL_KEY_TILDE                      0x0000007eu /**< '~' */
#define PAL_KEY_DELETE                     0x0000007fu /**< '\x7F' */

#define PAL_KEY_PLUSMINUS                  0x000000b1u /**< '\xB1' */
#define PAL_KEY_CAPSLOCK                   0x40000039u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CAPSLOCK) */
#define PAL_KEY_F1                         0x4000003au /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F1) */
#define PAL_KEY_F2                         0x4000003bu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F2) */
#define PAL_KEY_F3                         0x4000003cu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F3) */
#define PAL_KEY_F4                         0x4000003du /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F4) */
#define PAL_KEY_F5                         0x4000003eu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F5) */
#define PAL_KEY_F6                         0x4000003fu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F6) */
#define PAL_KEY_F7                         0x40000040u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F7) */
#define PAL_KEY_F8                         0x40000041u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F8) */
#define PAL_KEY_F9                         0x40000042u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F9) */
#define PAL_KEY_F10                        0x40000043u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F10) */
#define PAL_KEY_F11                        0x40000044u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F11) */
#define PAL_KEY_F12                        0x40000045u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F12) */
#define PAL_KEY_PRINTSCREEN                0x40000046u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PRINTSCREEN) */
#define PAL_KEY_SCROLLLOCK                 0x40000047u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SCROLLLOCK) */
#define PAL_KEY_PAUSE                      0x40000048u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAUSE) */
#define PAL_KEY_INSERT                     0x40000049u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_INSERT) */
#define PAL_KEY_HOME                       0x4000004au /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_HOME) */
#define PAL_KEY_PAGEUP                     0x4000004bu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAGEUP) */
#define PAL_KEY_END                        0x4000004du /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_END) */
#define PAL_KEY_PAGEDOWN                   0x4000004eu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAGEDOWN) */
#define PAL_KEY_RIGHT                      0x4000004fu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RIGHT) */
#define PAL_KEY_LEFT                       0x40000050u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LEFT) */
#define PAL_KEY_DOWN                       0x40000051u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DOWN) */
#define PAL_KEY_UP                         0x40000052u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_UP) */
#define PAL_KEY_NUMLOCKCLEAR               0x40000053u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMLOCKCLEAR) */
#define PAL_KEY_NUMPAD_DIVIDE              0x40000054u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_DIVIDE) */
#define PAL_KEY_NUMPAD_MULTIPLY            0x40000055u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_MULTIPLY) */
#define PAL_KEY_NUMPAD_MINUS               0x40000056u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_MINUS) */
#define PAL_KEY_NUMPAD_PLUS                0x40000057u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_PLUS) */
#define PAL_KEY_NUMPAD_ENTER               0x40000058u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_ENTER) */
#define PAL_KEY_NUMPAD_1                   0x40000059u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_1) */
#define PAL_KEY_NUMPAD_2                   0x4000005au /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_2) */
#define PAL_KEY_NUMPAD_3                   0x4000005bu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_3) */
#define PAL_KEY_NUMPAD_4                   0x4000005cu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_4) */
#define PAL_KEY_NUMPAD_5                   0x4000005du /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_5) */
#define PAL_KEY_NUMPAD_6                   0x4000005eu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_6) */
#define PAL_KEY_NUMPAD_7                   0x4000005fu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_7) */
#define PAL_KEY_NUMPAD_8                   0x40000060u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_8) */
#define PAL_KEY_NUMPAD_9                   0x40000061u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_9) */
#define PAL_KEY_NUMPAD_0                   0x40000062u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_0) */
#define PAL_KEY_NUMPAD_PERIOD              0x40000063u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_PERIOD) */
#define PAL_KEY_APPLICATION                0x40000065u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_APPLICATION) */
#define PAL_KEY_POWER                      0x40000066u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_POWER) */
#define PAL_KEY_NUMPAD_EQUALS              0x40000067u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_EQUALS) */
#define PAL_KEY_F13                        0x40000068u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F13) */
#define PAL_KEY_F14                        0x40000069u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F14) */
#define PAL_KEY_F15                        0x4000006au /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F15) */
#define PAL_KEY_F16                        0x4000006bu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F16) */
#define PAL_KEY_F17                        0x4000006cu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F17) */
#define PAL_KEY_F18                        0x4000006du /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F18) */
#define PAL_KEY_F19                        0x4000006eu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F19) */
#define PAL_KEY_F20                        0x4000006fu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F20) */
#define PAL_KEY_F21                        0x40000070u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F21) */
#define PAL_KEY_F22                        0x40000071u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F22) */
#define PAL_KEY_F23                        0x40000072u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F23) */
#define PAL_KEY_F24                        0x40000073u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F24) */
#define PAL_KEY_EXECUTE                    0x40000074u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EXECUTE) */
#define PAL_KEY_HELP                       0x40000075u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_HELP) */
#define PAL_KEY_MENU                       0x40000076u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MENU) */
#define PAL_KEY_SELECT                     0x40000077u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SELECT) */
#define PAL_KEY_STOP                       0x40000078u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_STOP) */
#define PAL_KEY_AGAIN                      0x40000079u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AGAIN) */
#define PAL_KEY_UNDO                       0x4000007au /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_UNDO) */
#define PAL_KEY_CUT                        0x4000007bu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CUT) */
#define PAL_KEY_COPY                       0x4000007cu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_COPY) */
#define PAL_KEY_PASTE                      0x4000007du /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PASTE) */
#define PAL_KEY_FIND                       0x4000007eu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_FIND) */
#define PAL_KEY_MUTE                       0x4000007fu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MUTE) */
#define PAL_KEY_VOLUMEUP                   0x40000080u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_VOLUMEUP) */
#define PAL_KEY_VOLUMEDOWN                 0x40000081u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_VOLUMEDOWN) */
#define PAL_KEY_NUMPAD_COMMA               0x40000085u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_COMMA) */
#define PAL_KEY_NUMPAD_EQUALSAS400         0x40000086u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_EQUALSAS400) */
#define PAL_KEY_ALTERASE                   0x40000099u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_ALTERASE) */
#define PAL_KEY_SYSREQ                     0x4000009au /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SYSREQ) */
#define PAL_KEY_CANCEL                     0x4000009bu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CANCEL) */
#define PAL_KEY_CLEAR                      0x4000009cu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CLEAR) */
#define PAL_KEY_PRIOR                      0x4000009du /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PRIOR) */
#define PAL_KEY_RETURN2                    0x4000009eu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RETURN2) */
#define PAL_KEY_SEPARATOR                  0x4000009fu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SEPARATOR) */
#define PAL_KEY_OUT                        0x400000a0u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_OUT) */
#define PAL_KEY_OPER                       0x400000a1u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_OPER) */
#define PAL_KEY_CLEARAGAIN                 0x400000a2u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CLEARAGAIN) */
#define PAL_KEY_CRSEL                      0x400000a3u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CRSEL) */
#define PAL_KEY_EXSEL                      0x400000a4u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EXSEL) */
#define PAL_KEY_NUMPAD_00                  0x400000b0u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_00) */
#define PAL_KEY_NUMPAD_000                 0x400000b1u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_000) */
#define PAL_KEY_THOUSANDSSEPARATOR         0x400000b2u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_THOUSANDSSEPARATOR) */
#define PAL_KEY_DECIMALSEPARATOR           0x400000b3u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DECIMALSEPARATOR) */
#define PAL_KEY_CURRENCYUNIT               0x400000b4u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CURRENCYUNIT) */
#define PAL_KEY_CURRENCYSUBUNIT            0x400000b5u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CURRENCYSUBUNIT) */
#define PAL_KEY_NUMPAD_LEFTPAREN           0x400000b6u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_LEFTPAREN) */
#define PAL_KEY_NUMPAD_RIGHTPAREN          0x400000b7u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_RIGHTPAREN) */
#define PAL_KEY_NUMPAD_LEFTBRACE           0x400000b8u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_LEFTBRACE) */
#define PAL_KEY_NUMPAD_RIGHTBRACE          0x400000b9u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_RIGHTBRACE) */
#define PAL_KEY_NUMPAD_TAB                 0x400000bau /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_TAB) */
#define PAL_KEY_NUMPAD_BACKSPACE           0x400000bbu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_BACKSPACE) */
#define PAL_KEY_NUMPAD_A                   0x400000bcu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_A) */
#define PAL_KEY_NUMPAD_B                   0x400000bdu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_B) */
#define PAL_KEY_NUMPAD_C                   0x400000beu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_C) */
#define PAL_KEY_NUMPAD_D                   0x400000bfu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_D) */
#define PAL_KEY_NUMPAD_E                   0x400000c0u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_E) */
#define PAL_KEY_NUMPAD_F                   0x400000c1u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_F) */
#define PAL_KEY_NUMPAD_XOR                 0x400000c2u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_XOR) */
#define PAL_KEY_NUMPAD_POWER               0x400000c3u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_POWER) */
#define PAL_KEY_NUMPAD_PERCENT             0x400000c4u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_PERCENT) */
#define PAL_KEY_NUMPAD_LESS                0x400000c5u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_LESS) */
#define PAL_KEY_NUMPAD_GREATER             0x400000c6u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_GREATER) */
#define PAL_KEY_NUMPAD_AMPERSAND           0x400000c7u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_AMPERSAND) */
#define PAL_KEY_NUMPAD_DBLAMPERSAND        0x400000c8u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_DBLAMPERSAND) */
#define PAL_KEY_NUMPAD_VERTICALBAR         0x400000c9u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_VERTICALBAR) */
#define PAL_KEY_NUMPAD_DBLVERTICALBAR      0x400000cau /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_DBLVERTICALBAR) */
#define PAL_KEY_NUMPAD_COLON               0x400000cbu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_COLON) */
#define PAL_KEY_NUMPAD_HASH                0x400000ccu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_HASH) */
#define PAL_KEY_NUMPAD_SPACE               0x400000cdu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_SPACE) */
#define PAL_KEY_NUMPAD_AT                  0x400000ceu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_AT) */
#define PAL_KEY_NUMPAD_EXCLAM              0x400000cfu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_EXCLAM) */
#define PAL_KEY_NUMPAD_MEMSTORE            0x400000d0u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_MEMSTORE) */
#define PAL_KEY_NUMPAD_MEMRECALL           0x400000d1u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_MEMRECALL) */
#define PAL_KEY_NUMPAD_MEMCLEAR            0x400000d2u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_MEMCLEAR) */
#define PAL_KEY_NUMPAD_MEMADD              0x400000d3u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_MEMADD) */
#define PAL_KEY_NUMPAD_MEMSUBTRACT         0x400000d4u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_MEMSUBTRACT) */
#define PAL_KEY_NUMPAD_MEMMULTIPLY         0x400000d5u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_MEMMULTIPLY) */
#define PAL_KEY_NUMPAD_MEMDIVIDE           0x400000d6u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_MEMDIVIDE) */
#define PAL_KEY_NUMPAD_PLUSMINUS           0x400000d7u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_PLUSMINUS) */
#define PAL_KEY_NUMPAD_CLEAR               0x400000d8u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_CLEAR) */
#define PAL_KEY_NUMPAD_CLEARENTRY          0x400000d9u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_CLEARENTRY) */
#define PAL_KEY_NUMPAD_BINARY              0x400000dau /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_BINARY) */
#define PAL_KEY_NUMPAD_OCTAL               0x400000dbu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_OCTAL) */
#define PAL_KEY_NUMPAD_DECIMAL             0x400000dcu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_DECIMAL) */
#define PAL_KEY_NUMPAD_HEXADECIMAL         0x400000ddu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMPAD_HEXADECIMAL) */
#define PAL_KEY_LCTRL                      0x400000e0u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LCTRL) */
#define PAL_KEY_LSHIFT                     0x400000e1u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LSHIFT) */
#define PAL_KEY_LALT                       0x400000e2u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LALT) */
#define PAL_KEY_LOPTION PAL_KEY_LALT
#define PAL_KEY_LGUI                       0x400000e3u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LGUI) */
#define PAL_KEY_LWIN PAL_KEY_LGUI
#define PAL_KEY_LSUPER PAL_KEY_LGUI
#define PAL_KEY_LCOMMAND PAL_KEY_LGUI
#define PAL_KEY_RCTRL                      0x400000e4u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RCTRL) */
#define PAL_KEY_RSHIFT                     0x400000e5u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RSHIFT) */
#define PAL_KEY_RALT                       0x400000e6u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RALT) */
#define PAL_KEY_ROPTION PAL_KEY_RALT
#define PAL_KEY_RGUI                       0x400000e7u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RGUI) */
#define PAL_KEY_RWIN PAL_KEY_RGUI
#define PAL_KEY_RSUPER PAL_KEY_RGUI
#define PAL_KEY_RCOMMAND PAL_KEY_RGUI
#define PAL_KEY_MODE                       0x40000101u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MODE) */
#define PAL_KEY_SLEEP                      0x40000102u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SLEEP) */
#define PAL_KEY_WAKE                       0x40000103u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_WAKE) */
#define PAL_KEY_CHANNEL_INCREMENT          0x40000104u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CHANNEL_INCREMENT) */
#define PAL_KEY_CHANNEL_DECREMENT          0x40000105u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CHANNEL_DECREMENT) */
#define PAL_KEY_MEDIA_PLAY                 0x40000106u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_PLAY) */
#define PAL_KEY_MEDIA_PAUSE                0x40000107u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_PAUSE) */
#define PAL_KEY_MEDIA_RECORD               0x40000108u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_RECORD) */
#define PAL_KEY_MEDIA_FAST_FORWARD         0x40000109u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_FAST_FORWARD) */
#define PAL_KEY_MEDIA_REWIND               0x4000010au /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_REWIND) */
#define PAL_KEY_MEDIA_NEXT_TRACK           0x4000010bu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_NEXT_TRACK) */
#define PAL_KEY_MEDIA_PREVIOUS_TRACK       0x4000010cu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_PREVIOUS_TRACK) */
#define PAL_KEY_MEDIA_STOP                 0x4000010du /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_STOP) */
#define PAL_KEY_MEDIA_EJECT                0x4000010eu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_EJECT) */
#define PAL_KEY_MEDIA_PLAY_PAUSE           0x4000010fu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_PLAY_PAUSE) */
#define PAL_KEY_MEDIA_SELECT               0x40000110u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_SELECT) */
#define PAL_KEY_AC_NEW                     0x40000111u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_NEW) */
#define PAL_KEY_AC_OPEN                    0x40000112u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_OPEN) */
#define PAL_KEY_AC_CLOSE                   0x40000113u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_CLOSE) */
#define PAL_KEY_AC_EXIT                    0x40000114u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_EXIT) */
#define PAL_KEY_AC_SAVE                    0x40000115u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_SAVE) */
#define PAL_KEY_AC_PRINT                   0x40000116u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_PRINT) */
#define PAL_KEY_AC_PROPERTIES              0x40000117u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_PROPERTIES) */
#define PAL_KEY_AC_SEARCH                  0x40000118u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_SEARCH) */
#define PAL_KEY_AC_HOME                    0x40000119u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_HOME) */
#define PAL_KEY_AC_BACK                    0x4000011au /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_BACK) */
#define PAL_KEY_AC_FORWARD                 0x4000011bu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_FORWARD) */
#define PAL_KEY_AC_STOP                    0x4000011cu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_STOP) */
#define PAL_KEY_AC_REFRESH                 0x4000011du /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_REFRESH) */
#define PAL_KEY_AC_BOOKMARKS               0x4000011eu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_BOOKMARKS) */
#define PAL_KEY_SOFTLEFT                   0x4000011fu /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SOFTLEFT) */
#define PAL_KEY_SOFTRIGHT                  0x40000120u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SOFTRIGHT) */
#define PAL_KEY_CALL                       0x40000121u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CALL) */
#define PAL_KEY_ENDCALL                    0x40000122u /**< SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_ENDCALL) */
#define PAL_KEY_LEFT_TAB                   0x20000001u /**< Extended key Left Tab */
#define PAL_KEY_LEVEL5_SHIFT               0x20000002u /**< Extended key Level 5 Shift */
#define PAL_KEY_MULTI_KEY_COMPOSE          0x20000003u /**< Extended key Multi-key Compose */
#define PAL_KEY_LMETA                      0x20000004u /**< Extended key Left Meta */
#define PAL_KEY_RMETA                      0x20000005u /**< Extended key Right Meta */
#define PAL_KEY_LHYPER                     0x20000006u /**< Extended key Left Hyper */
#define PAL_KEY_RHYPER                     0x20000007u /**< Extended key Right Hyper */

/*
---------------------------------------------------------------------------------
Modifier Keys
---------------------------------------------------------------------------------
*/

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

/*
----------------------------------------------------------------------------------
Scancodes
----------------------------------------------------------------------------------
*/

#define PAL_SCAN_NONE 0
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
/* 222 - 223 reserved */
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
/*
----------------------------------------------------------------------------------
True and False defines
----------------------------------------------------------------------------------
*/

#define pal_true 1
#define pal_false 0

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
    /* Standard gamepad controls */
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

    /* Identification */
    char name[128];
    uint16_t vendor_id;
    uint16_t product_id;
    pal_bool connected;
    pal_bool is_xinput;

    /* Battery information */
    float battery_level; /* 0.0-1.0 */
    pal_bool is_charging;

    /* Motion sensors */
    float accel_x, accel_y, accel_z; /* In G's */
    float gyro_x, gyro_y, gyro_z;    /* In degrees/second */

    /* Touchpad */
    struct {
        int touch_count;
        struct {
            int id;                 /* Touch ID */
            float x, y;             /* Normalized coordinates (0-1) */
            pal_bool down;          /* Is touch active */
        } touches[PAL_MAX_TOUCHES]; /* Two fingers on the touch pad at the same time. Kinky. commonly used for scrolling, zooming and rotating. */
    } touchpad;
} pal_gamepad_state;

/* events. */
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
    char text[8];
    int keyboard_id;
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
    int wheel_direction; /* some weird ahh mice have horizontal scroll wheels */
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
    uint8_t level; /* 0-100 */
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
                   /* Event data: */
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

#ifdef __cplusplus
extern "C" {
#endif

/* Init and Shutdown */
PALAPI void pal_init(void);
PALAPI void pal_shutdown(void);

/* Video and Windowing subsystem. */
PALAPI pal_window *pal_create_window(int width, int height, const char *window_title, uint64_t window_flags);
PALAPI void pal_close_window(pal_window *window);
PALAPI pal_ivec2 pal_get_window_border_size(pal_window *window);
PALAPI void *pal_get_window_handle(pal_window *window);
PALAPI int pal_show_cursor(pal_window *window);
PALAPI int pal_hide_cursor(pal_window *window);
PALAPI pal_bool pal_set_window_title(pal_window *window, const char *string);

/* might change all of these functions into one function, that takes in a different full screen mode. */

PALAPI pal_bool pal_make_window_fullscreen(pal_window *window);
PALAPI pal_bool pal_make_window_fullscreen_ex(pal_window *window, int width, int height, int refresh_rate);
PALAPI pal_bool pal_make_window_fullscreen_windowed(pal_window *window);
PALAPI pal_bool pal_make_window_windowed(pal_window *window);

PALAPI pal_bool pal_maximize_window(pal_window *window);
PALAPI pal_bool pal_minimize_window(pal_window *window);

PALAPI void pal_set_window_icon(pal_window *window, const char *image_path);
/* I am thinking that a better way of doing legacy stuff is with macros.
 Just have a macro like WINDOWS_XP_OR_LATER or WINDOWS_VISTA_OR_LATER. */
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

/* Rendering functions (implemented using GDI on windows and X11 on linux) */
PALAPI void pal_draw_rect(pal_window *window, int x, int y, int width, int height, pal_vec4 color);

/* Image Loading */
PALAPI unsigned char *pal_load_image(char const *filename, int *x, int *y, int *comp, int req_comp);

/* Keyboard input */
PALAPI int pal_get_keyboard_count(void);
PALAPI const char *pal_get_keyboard_name(int keyboard_id);
PALAPI int pal_get_keyboard_indices(int key, int *keyboard_indices);
PALAPI pal_bool pal_is_key_pressed(int keyboard_id, int key);
PALAPI pal_bool pal_is_key_down(int keyboard_id, int key);

/* Mouse input */
PALAPI int pal_get_mouse_count(void);
PALAPI const char *pal_get_mouse_name(int mouse_id);
PALAPI int pal_get_mouse_indices(int *mouse_indices);
PALAPI pal_bool pal_is_mouse_down(int mouse_id, int button);
PALAPI pal_bool pal_is_mouse_pressed(int mouse_id, int button);
PALAPI pal_vec2 pal_get_mouse_delta(int mouse_id);
PALAPI pal_vec2 pal_get_mouse_position(pal_window *window);

/* Gamepad Input */
PALAPI int pal_get_gamepad_count(void);
PALAPI pal_bool pal_get_gamepad_state(int index, pal_gamepad_state *out_state);
PALAPI void pal_set_gamepad_vibration(int controller_id, float left_motor, float right_motor, float left_trigger, float right_trigger);
PALAPI void pal_stop_gamepad_vibration(int controller_id);

/* Rendering stuff. */
PALAPI void pal_swap_buffers(pal_window *window);
PALAPI void pal_swap_interval(int interval);

/* Sound */
PALAPI pal_sound *pal_load_sound(const char *filename);
PALAPI int pal_play_sound(pal_sound *sound, float volume);
PALAPI int pal_stop_sound(pal_sound *sound);
PALAPI void pal_free_sound(pal_sound *sound);

PALAPI pal_sound *pal_load_music(const char *filename);
PALAPI int pal_play_music(pal_sound *sound, float volume);
PALAPI int pal_stop_music(pal_sound *sound); /* unimplemented */
PALAPI void pal_free_music(pal_sound *sound);

/* File I/O */
PALAPI pal_bool pal_does_file_exist(const char *file_path);
PALAPI size_t pal_get_last_write_time(const char *file);
PALAPI size_t pal_get_last_read_time(const char *file);
PALAPI size_t pal_get_file_size(const char *file_path);
PALAPI uint32_t pal_get_file_permissions(const char *file_path);
PALAPI pal_bool pal_change_file_permissions(const char *file_path, uint32_t permission_flags);
PALAPI unsigned char *pal_read_entire_file(const char *file_path, size_t *bytes_read); // This heap-allocates memory, and give you back a pointer to it. call pal_close_file() to free the memory.
PALAPI pal_bool pal_write_file(const char *file_path, size_t file_size, char *buffer);
PALAPI pal_bool pal_copy_file(const char *original_path, const char *copy_path);
PALAPI pal_bool pal_close_file(const unsigned char *file);

/* Directory Listing */
PALAPI pal_bool pal_path_is_dir(const char *path);

/* Open File I/O */
PALAPI pal_file *pal_open_file(const char *file_path);
PALAPI pal_bool pal_read_from_open_file(pal_file *file, size_t offset, size_t bytes_to_read, char *buffer);
PALAPI pal_bool pal_close_open_file(pal_file *file);

/* Random Number Generation */
PALAPI void pal_srand(uint64_t *state, uint64_t seed);
PALAPI uint32_t pal_rand(uint64_t *state);

/* Clip board */
PALAPI void pal_clipboard_set(const char *text);
PALAPI char *pal_clipboard_get(void);

/* File dialog / requester */
PALAPI void pal_create_save_dialog(char **types, uint32_t type_count, void *id);
PALAPI void pal_create_load_dialog(char **types, uint32_t type_count, void *id);
PALAPI char *pal_show_save_dialog(void *id);
PALAPI char *pal_show_load_dialog(void *id);

/* URL launch */
PALAPI void pal_url_launch(char *url);

/* Mouse Warp */
PALAPI void pal_mouse_warp(int x, int y);
PALAPI void pal_mouse_warp_relative(int dx, int dy);

/* File Parsing */
PALAPI pal_bool pal_is_uppercase(char ch);
PALAPI pal_bool pal_is_lowercase(char ch);
PALAPI pal_bool pal_is_letter(char ch);
PALAPI pal_bool pal_is_number(char ch);
PALAPI pal_bool pal_is_alphanumeric(char ch);

PALAPI pal_bool pal_is_whitespace(char ch);
PALAPI pal_bool pal_is_end_of_line(char ch);
PALAPI pal_bool pal_is_underscore(char ch);
PALAPI pal_bool pal_is_hyphen(char ch);
PALAPI pal_bool pal_is_dot(char ch);
PALAPI pal_bool pal_are_chars_equal(char ch1, char ch2);
PALAPI pal_bool pal_are_strings_equal(const char *s1, const char *s2);

PALAPI int pal_memcmp(const void *a, const void *b, size_t n);
PALAPI void *pal_memcpy(void *dest, const void *src, size_t n);
PALAPI void *pal_memset(void *buf, int value, size_t count);

PALAPI size_t pal_strlen(const char *str);
PALAPI char *pal_strcpy(char *dest, const char *src);
PALAPI char *pal_strncpy(char *dest, const char *src, size_t n);
PALAPI int pal_strcmp(const char *s1, const char *s2);
PALAPI int pal_strncmp(const char *s1, const char *s2, size_t n);

/* Time functions */
PALAPI pal_time pal_get_date_and_time_utc(void);
PALAPI pal_time pal_get_date_and_time_local(void);
PALAPI pal_time pal_get_time_since_boot(void);
PALAPI double   pal_get_time_since_pal_init(void);
PALAPI uint64_t pal_get_ticks(void);
PALAPI uint64_t pal_get_timer_frequency(void);

/* Multi-threadding functions */
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

/* .dll/.so/.dylib loading */
PALAPI void *pal_load_dynamic_library(const char *dll);
PALAPI void *pal_load_dynamic_function(void *dll, char *func_name);
PALAPI pal_bool pal_free_dynamic_library(void *dll);

#ifdef __cplusplus
}
#endif

#endif /* PAL_H */

#ifdef PAL_IMPLEMENTATION
/*-------------------------------------*/
/* Cross-platform code ----------------*/
/*-------------------------------------*/
#define STB_VORBIS_IMPLEMENTATION
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

// push / enqueue
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

PALAPI unsigned char *pal_load_image(char const *filename, int *x, int *y, int *comp, int req_comp) {
    return stbi_load(filename, x, y, comp, req_comp);
}

/*----------------------------------------*/
/* Keyboard and mouse input system -------*/
/*----------------------------------------*/
#define MAX_MICE 16
#define MAX_KEYBOARDS 16
#define MAX_KEYS 256
#define MAX_SCANCODES 512
#define MAX_MOUSE_BUTTONS 8

typedef struct {
#ifdef _WIN32
    HANDLE handles[MAX_KEYBOARDS];
#else
    int handles[MAX_KEYBOARDS];
#endif
    char names[MAX_KEYBOARDS][256];
    int count;
    unsigned char keys[MAX_KEYBOARDS][MAX_SCANCODES];
    unsigned char keys_toggled[MAX_KEYBOARDS][MAX_SCANCODES];
    int cached_modifiers[MAX_KEYBOARDS];
} pal_keyboard_state;

typedef struct {
#ifdef _WIN32
    HANDLE handles[MAX_MICE];
#else
    int handles[MAX_MICE];
#endif
    char names[MAX_MICE][256];
    int count;
    int buttons[MAX_MICE][MAX_MOUSE_BUTTONS];
    int buttons_toggled[MAX_MICE][MAX_MOUSE_BUTTONS];
    int dx[MAX_MICE];
    int dy[MAX_MICE];
    int wheel[MAX_MICE];
} pal_mouse_state;

static pal_keyboard_state g_keyboards = {0};
static pal_mouse_state g_mice = {0};

static uint32_t g_cached_mouse_buttons = 0;

PALAPI int pal_get_keyboard_count(void) {
    return g_keyboards.count;
}

PALAPI const char *pal_get_keyboard_name(int keyboard_id) {
    if (keyboard_id < 0 || keyboard_id >= g_keyboards.count) {
        return NULL;
    }
    return g_keyboards.names[keyboard_id];
}

PALAPI int pal_get_keyboard_indices(int scan_code, int *keyboard_indices) {
    if (!keyboard_indices || scan_code < 0 || scan_code >= MAX_SCANCODES) {
        return 0;
    }

    int count = 0;
    for (int i = 0; i < g_keyboards.count; i++) {
        if (g_keyboards.keys[i][scan_code]) {
            keyboard_indices[count++] = i;
        }
    }
    return count;
}

PALAPI pal_bool pal_is_key_pressed(int keyboard_id, int scan_code) {
    if (scan_code < 0 || scan_code >= MAX_SCANCODES) {
        return pal_false;
    }

    // -1 means check ANY keyboard
    if (keyboard_id == -1) {
        for (int i = 0; i < g_keyboards.count; i++) {
            if (g_keyboards.keys_toggled[i][scan_code] && g_keyboards.keys[i][scan_code]) {
                return pal_true;
            }
        }
        return pal_false;
    }

    if (keyboard_id < 0 || keyboard_id >= g_keyboards.count) {
        return pal_false;
    }

    // Return true only if the key was toggled this frame AND is currently down
    return (g_keyboards.keys_toggled[keyboard_id][scan_code] && g_keyboards.keys[keyboard_id][scan_code]) ? pal_true : pal_false;
}

PALAPI pal_bool pal_is_key_down(int keyboard_id, int scan_code) {
    if (scan_code < 0 || scan_code >= MAX_SCANCODES) {
        return pal_false;
    }

    // -1 means check ANY keyboard
    if (keyboard_id == -1) {
        for (int i = 0; i < g_keyboards.count; i++) {
            if (g_keyboards.keys[i][scan_code]) {
                return pal_true;
            }
        }
        return pal_false;
    }

    if (keyboard_id < 0 || keyboard_id >= g_keyboards.count) {
        return pal_false;
    }

    // Return true as long as the key is held down
    return g_keyboards.keys[keyboard_id][scan_code] ? pal_true : pal_false;
}

// ========== Mouse Functions ==========

PALAPI int pal_get_mouse_count(void) {
    return g_mice.count;
}

PALAPI const char *pal_get_mouse_name(int mouse_id) {
    if (mouse_id < 0 || mouse_id >= g_mice.count) {
        return NULL;
    }
    return g_mice.names[mouse_id];
}

PALAPI int pal_get_mouse_indices(int *mouse_indices) {
    if (!mouse_indices) {
        return 0;
    }

    for (int i = 0; i < g_mice.count; i++) {
        mouse_indices[i] = i;
    }
    return g_mice.count;
}

PALAPI pal_bool pal_is_mouse_down(int mouse_id, int button) {
    if (button < 0 || button >= MAX_MOUSE_BUTTONS) {
        return pal_false;
    }

    // -1 means check ANY mouse
    if (mouse_id == -1) {
        for (int i = 0; i < g_mice.count; i++) {
            if (g_mice.buttons[i][button]) {
                return pal_true;
            }
        }
        return pal_false;
    }

    if (mouse_id < 0 || mouse_id >= g_mice.count) {
        return pal_false;
    }

    // Return true as long as the button is held down
    return g_mice.buttons[mouse_id][button] ? pal_true : pal_false;
}

PALAPI pal_bool pal_is_mouse_pressed(int mouse_id, int button) {
    if (button < 0 || button >= MAX_MOUSE_BUTTONS) {
        return pal_false;
    }

    // -1 means check ANY mouse
    if (mouse_id == -1) {
        for (int i = 0; i < g_mice.count; i++) {
            if (g_mice.buttons_toggled[i][button] && g_mice.buttons[i][button]) {
                return pal_true;
            }
        }
        return pal_false;
    }

    if (mouse_id < 0 || mouse_id >= g_mice.count) {
        return pal_false;
    }

    // Return true only if the button was toggled this frame AND is currently down
    return (g_mice.buttons_toggled[mouse_id][button] && g_mice.buttons[mouse_id][button]) ? pal_true : pal_false;
}

PALAPI pal_vec2 pal_get_mouse_delta(int mouse_id) {
    pal_vec2 delta;
    delta.x = 0.0f;
    delta.y = 0.0f;

    // -1 means get combined delta from ALL mice
    if (mouse_id == -1) {
        for (int i = 0; i < g_mice.count; i++) {
            delta.x += (float)g_mice.dx[i];
            delta.y += (float)g_mice.dy[i];
        }
        return delta;
    }

    if (mouse_id < 0 || mouse_id >= g_mice.count) {
        return delta;
    }

    // Return delta for specific mouse
    delta.x = (float)g_mice.dx[mouse_id];
    delta.y = (float)g_mice.dy[mouse_id];
    return delta;
}

void pal__reset_mouse_deltas(void) {
    for (int i = 0; i < g_mice.count; i++) {
        g_mice.dx[i] = 0;
        g_mice.dy[i] = 0;
        g_mice.wheel[i] = 0;
    }
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
    ['\n'] = PAL_WHITESPACE_BIT | PAL_EOL_BIT, // new line
    ['\v'] = PAL_WHITESPACE_BIT, // vertical tab (not used anymore)
    ['\f'] = PAL_WHITESPACE_BIT, // form feed (not used anymore)
    ['\r'] = PAL_WHITESPACE_BIT | PAL_EOL_BIT, // carriage return

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

PALAPI void *pal_memset(void *buf, int value, size_t count) {
    unsigned char *p = buf;
    unsigned char v = (unsigned char)value;

    for (size_t i = 0; i < count; i++) {
        p[i] = v;
    }

    return buf;
}

PALAPI int pal_memcmp(const void *a, const void *b, size_t n) {
    const unsigned char *p1 = a;
    const unsigned char *p2 = b;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i])
            return (p1[i] < p2[i]) ? -1 : 1;
    }

    return 0;
}

PALAPI void *pal_memcpy(void *dest, const void *src, size_t n) {
    unsigned char *d = dest;
    const unsigned char *s = src;

    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }

    return dest;
}

PALAPI size_t pal_strlen(const char *str) {
    if (!str) return 0;

    const char *count = str;
    while (*count != '\0')
        count++;

    return count - str;
}

PALAPI char *pal_strcpy(char *dest, const char *src) {
    char *orig = dest;
    while ((*dest++ = *src++));
    return orig;
}

PALAPI char *pal_strncpy(char *dest, const char *src, size_t n) {
    size_t i = 0;

    // Copy characters from src until either end-of-source or reaching n.
    for (; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }

    // Pad the rest with '\0' if needed.
    for (; i < n; i++) {
        dest[i] = '\0';
    }

    return dest;
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

/*-------------------------------------*/
/* platform-specific code--------------*/
/*-------------------------------------*/
#if defined(_WIN32)

#ifndef PLATFORM_WIN32_H
#define PLATFORM_WIN32_H
#define _CRT_SECURE_NO_WARNINGS

// Windows system headers
#include <Windows.h>
#include <windowsx.h> // Useful macros (e.g., GET_X_LPARAM)
#include <xaudio2.h>  // For Sound.
#include <Xinput.h> // For gamepad input.
#include <hidsdi.h> // Link with hid.lib

// for file permissions.
#include <aclapi.h>

// OpenGL
#include <gl/gl.h>
#include <GL/wglext.h>

// C Standard Library
#include <assert.h>

// Global function pointers
static DWORD(WINAPI* XinputGetstate_fn)(DWORD dwUserIndex, XINPUT_STATE* pState) = NULL;
static DWORD(WINAPI* XInputSetState_fn)(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration) = NULL;
static DWORD(WINAPI* XInputGetCapabilities_fn)(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES* pCapabilities) = NULL;
static void(WINAPI* XInputEnable_fn)(BOOL enable) = NULL;

typedef unsigned __int64 QWORD;

// Global state
static HMODULE g_xinput_dll = NULL;
static pal_bool g_has_trigger_motors = pal_false;

static HDC s_fakeDC = {0};
pal_window* g_current_window;

IXAudio2* g_xaudio2 = NULL;
IXAudio2MasteringVoice* g_mastering_voice = NULL;

/* On windows, a message pump is specific to the gui thread (a thread that creates windows). */
pal_bool g_message_pump_drained = pal_false; /* initiallly false because windows sends messages as soon as you call Create_WindowExA() */
struct pal_window {
    HWND hwnd;
    HDC hdc;
    HGLRC hglrc;
    LONG windowedStyle;
    pal_bool confine_mouse;
    float width;
    float height;
};

struct pal_monitor {
    HMONITOR handle;
};

struct pal_sound {
    // Core audio data
    unsigned char* data; // Raw PCM audio data (initial buffer)
    size_t data_size;    // Size in bytes of initial buffer
    int sample_rate;     // Samples per second (e.g., 44100)
    int channels;        // Number of audio channels (e.g., 2 for stereo)
    int bits_per_sample; // Usually 16 or 32
    int is_float;        // 0 = PCM, 1 = IEEE float

    // XAudio2
    IXAudio2SourceVoice* source_voice;
    IXAudio2VoiceCallback* voice_callback;

    // Streaming - OGG
    void* decoder;  // stb_vorbis* (using void* to avoid header dependency)
    char* filename; // Filename for reopening OGG decoder

    // Streaming - WAV
    FILE* source_file;
    size_t total_data_size; // Total size of audio data in file
    size_t bytes_streamed;  // How many bytes we've read so far
    size_t data_offset;     // Offset in file where audio data starts

    // Streaming control
    float preload_seconds; // How many seconds were preloaded
    int is_streaming;      // 1 if this is a streaming sound
    int stream_finished;   // 1 when streaming is complete
};

struct pal_mutex {
    CRITICAL_SECTION cs;
};

#define MAX_XINPUT_CONTROLLERS 4
#define PAL_MAX_GAMEPADS 16
#define PAL_MAX_BUTTONS 32
#define PAL_MAX_AXES 16
#define PAL_MAX_MAPPINGS 256

static const uint8_t win32_button_to_pal_button[] = {
    [0] = PAL_MOUSE_LEFT,
    [1] = PAL_MOUSE_RIGHT,
    [2] = PAL_MOUSE_MIDDLE,
    [3] = PAL_MOUSE_4,
    [4] = PAL_MOUSE_5,
};

// clang-format off
static const uint16_t win32_key_to_pal_key[] = {
    // unassigned.
    [0x00] = 0x00, [0x01] = 0x00, [0x02] = 0x00, [0x03] = 0x00, [0x04] = 0x00,
    [0x05] = 0x00, [0x06] = 0x00, [0x07] = 0x00,

    [0x08] = PAL_KEY_BACKSPACE, // Backspace
    [0x09] = PAL_KEY_TAB, // Tab
    [0x0A] = 0x00, [0x0B] = 0x00, // unassigned.
    [0x0C] = 0x0C, // Clear
    [0x0D] = PAL_KEY_ENTER, // Return
    [0x0E] = 0x00, [0x0F] = 0x00, // unassigned.
    [0x10] = PAL_KEY_SHIFT, // Shift
    [0x11] = PAL_KEY_CONTROL, // Control
    [0x12] = PAL_KEY_ALT, // Menu (Alt)
    [0x13] = 0x13, // Pause
    [0x14] = PAL_KEY_CAPSLOCK, // Caps Lock
    [0x15] = 0x15, [0x16] = 0x16, [0x17] = 0x17,
    [0x18] = 0x18, [0x19] = 0x19, [0x1A] = 0x1A, // Japanese keys (?)
    [0x1B] = 0x1B, [0x1C] = 0x1C, [0x1D] = 0x1D,
    [0x1E] = 0x1E, [0x1F] = 0x1F,
    [0x20] = 0x20, [0x21] = 0x21, [0x22] = 0x22,
    [0x23] = 0x23, [0x24] = 0x24, [0x25] = 0x25,
    [0x26] = 0x26, [0x27] = 0x27, [0x28] = 0x28,
    [0x29] = 0x29, [0x2A] = 0x2A, [0x2B] = 0x2B,
    [0x2C] = 0x2C, [0x2D] = 0x2D, [0x2E] = 0x2E,
    [0x2F] = 0x2F,

    // 0 - 9
    [0x30] = PAL_KEY_0, [0x31] = PAL_KEY_1, [0x32] = PAL_KEY_2, [0x33] = PAL_KEY_3,
    [0x34] = PAL_KEY_4, [0x35] = PAL_KEY_5, [0x36] = PAL_KEY_6, [0x37] = PAL_KEY_7,
    [0x38] = PAL_KEY_8, [0x39] = PAL_KEY_9,
    // Unassigned.
    [0x3A] = 0x3A, [0x3B] = 0x3B, [0x3C] = 0x3C, [0x3D] = 0x3D,
    [0x3E] = 0x3E, [0x3F] = 0x3F, [0x40] = 0x40,

    // A - Z
    [0x41] = PAL_KEY_A, [0x42] = PAL_KEY_B, [0x43] = PAL_KEY_C, [0x44] = PAL_KEY_D,
    [0x45] = PAL_KEY_E, [0x46] = PAL_KEY_F, [0x47] = PAL_KEY_G, [0x48] = PAL_KEY_H,
    [0x49] = PAL_KEY_I, [0x4A] = PAL_KEY_J, [0x4B] = PAL_KEY_K, [0x4C] = PAL_KEY_L,
    [0x4D] = PAL_KEY_M, [0x4E] = PAL_KEY_N, [0x4F] = PAL_KEY_O, [0x50] = PAL_KEY_P,
    [0x51] = PAL_KEY_Q, [0x52] = PAL_KEY_R, [0x53] = PAL_KEY_S, [0x54] = PAL_KEY_T,
    [0x55] = PAL_KEY_U, [0x56] = PAL_KEY_V, [0x57] = PAL_KEY_W, [0x58] = PAL_KEY_X,
    [0x59] = PAL_KEY_Y, [0x5A] = PAL_KEY_Z,

    // LWin, RWin, Apps (The Apps key is supposed to open up a context menu. Most keyboards don't have this.)
    [0x5B] = PAL_KEY_LWIN, [0x5C] = PAL_KEY_RWIN, [0x5D] = PAL_KEY_APPS,

    [0x5E] = 0x5E, // Unassigned

    [0x5F] = 0x5F, // Sleep

    // Numpad 0 - 9
    [0x60] = PAL_KEY_NUMPAD_0, [0x61] = PAL_KEY_NUMPAD_1, [0x62] = PAL_KEY_NUMPAD_2,
    [0x63] = PAL_KEY_NUMPAD_3, [0x64] = PAL_KEY_NUMPAD_4, [0x65] = PAL_KEY_NUMPAD_5,
    [0x66] = PAL_KEY_NUMPAD_6, [0x67] = PAL_KEY_NUMPAD_7, [0x68] = PAL_KEY_NUMPAD_8,
    [0x69] = PAL_KEY_NUMPAD_9, 

    // numpad operands
    [0x6A] = PAL_KEY_MULTIPLY, [0x6B] = PAL_KEY_ADD, [0x6C] = PAL_KEY_SEPARATOR,
    [0x6D] = PAL_KEY_SUBTRACT, [0x6E] = PAL_KEY_DECIMAL, [0x6F] = PAL_KEY_DIVIDE, 

    // F1 - F24.
    [0x70] = PAL_KEY_F1, [0x71] = PAL_KEY_F2, [0x72] = PAL_KEY_F3,
    [0x73] = PAL_KEY_F4, [0x74] = PAL_KEY_F5, [0x75] = PAL_KEY_F6,
    [0x76] = PAL_KEY_F7, [0x77] = PAL_KEY_F8, [0x78] = PAL_KEY_F9,
    [0x79] = PAL_KEY_F10, [0x7A] = PAL_KEY_F11, [0x7B] = PAL_KEY_F12,
    [0x7C] = PAL_KEY_F13, [0x7D] = PAL_KEY_F14, [0x7E] = PAL_KEY_F15,
    [0x7F] = PAL_KEY_F16, [0x80] = PAL_KEY_F17, [0x81] = PAL_KEY_F18,
    [0x82] = PAL_KEY_F19, [0x83] = PAL_KEY_F20, [0x84] = PAL_KEY_F21,
    [0x85] = PAL_KEY_F22, [0x86] = PAL_KEY_F23, [0x87] = PAL_KEY_F24,

    // Reserved
    [0x88] = 0x88, [0x89] = 0x89, [0x8A] = 0x8A, [0x8B] = 0x8B,
    [0x8C] = 0x8C, [0x8D] = 0x8D, [0x8E] = 0x8E, [0x8F] = 0x8F,

    [0x90] = PAL_KEY_NUMLOCK, [0x91] = PAL_KEY_SCROLLOCK, // NumLock, ScrollLock
    [0xBA] = PAL_KEY_SEMICOLON,  [0xBB] = PAL_KEY_EQUAL,
    [0xBC] = PAL_KEY_COMMA,      [0xBD] = PAL_KEY_MINUS,
    [0xBE] = PAL_KEY_PERIOD,     [0xBF] = PAL_KEY_FORWARD_SLASH,
    [0xC0] = PAL_KEY_BACKTICK,   [0xDB] = PAL_KEY_LEFTBRACE,
    [0xDC] = PAL_KEY_BACKSLASH,  [0xDD] = PAL_KEY_RIGHTBRACE,
    [0xDE] = PAL_KEY_APOSTROPHE, [0xE2] = PAL_KEY_BACKSLASH,
};

// clang-format on
static int win32_makecode_to_pal_scancode[256] = {
    [0x00] = 0,                       // Invalid
    [0x01] = PAL_SCAN_ESCAPE,         // Escape
    [0x02] = PAL_SCAN_1,              // 1
    [0x03] = PAL_SCAN_2,              // 2
    [0x04] = PAL_SCAN_3,              // 3
    [0x05] = PAL_SCAN_4,              // 4
    [0x06] = PAL_SCAN_5,              // 5
    [0x07] = PAL_SCAN_6,              // 6
    [0x08] = PAL_SCAN_7,              // 7
    [0x09] = PAL_SCAN_8,              // 8
    [0x0A] = PAL_SCAN_9,              // 9
    [0x0B] = PAL_SCAN_0,              // 0
    [0x0C] = PAL_SCAN_MINUS,          // -
    [0x0D] = PAL_SCAN_EQUALS,         // =
    [0x0E] = PAL_SCAN_BACKSPACE,      // Backspace
    [0x0F] = PAL_SCAN_TAB,            // Tab
    [0x10] = PAL_SCAN_Q,              // Q
    [0x11] = PAL_SCAN_W,              // W
    [0x12] = PAL_SCAN_E,              // E
    [0x13] = PAL_SCAN_R,              // R
    [0x14] = PAL_SCAN_T,              // T
    [0x15] = PAL_SCAN_Y,              // Y
    [0x16] = PAL_SCAN_U,              // U
    [0x17] = PAL_SCAN_I,              // I
    [0x18] = PAL_SCAN_O,              // O
    [0x19] = PAL_SCAN_P,              // P
    [0x1A] = PAL_SCAN_LEFTBRACKET,    // [
    [0x1B] = PAL_SCAN_RIGHTBRACKET,   // ]
    [0x1C] = PAL_SCAN_RETURN,         // Enter
    [0x1D] = PAL_SCAN_LCTRL,          // Left Ctrl
    [0x1E] = PAL_SCAN_A,              // A
    [0x1F] = PAL_SCAN_S,              // S
    [0x20] = PAL_SCAN_D,              // D
    [0x21] = PAL_SCAN_F,              // F
    [0x22] = PAL_SCAN_G,              // G
    [0x23] = PAL_SCAN_H,              // H
    [0x24] = PAL_SCAN_J,              // J
    [0x25] = PAL_SCAN_K,              // K
    [0x26] = PAL_SCAN_L,              // L
    [0x27] = PAL_SCAN_SEMICOLON,      // ;
    [0x28] = PAL_SCAN_APOSTROPHE,     // '
    [0x29] = PAL_SCAN_GRAVE,          // `
    [0x2A] = PAL_SCAN_LSHIFT,         // Left Shift
    [0x2B] = PAL_SCAN_BACKSLASH,      // \ (backslash)
    [0x2C] = PAL_SCAN_Z,              // Z
    [0x2D] = PAL_SCAN_X,              // X
    [0x2E] = PAL_SCAN_C,              // C
    [0x2F] = PAL_SCAN_V,              // V
    [0x30] = PAL_SCAN_B,              // B
    [0x31] = PAL_SCAN_N,              // N
    [0x32] = PAL_SCAN_M,              // M
    [0x33] = PAL_SCAN_COMMA,          // ,
    [0x34] = PAL_SCAN_PERIOD,         // .
    [0x35] = PAL_SCAN_SLASH,          // /
    [0x36] = PAL_SCAN_RSHIFT,         // Right Shift
    [0x37] = PAL_SCAN_KP_MULTIPLY,    // Keypad *
    [0x38] = PAL_SCAN_LALT,           // Left Alt
    [0x39] = PAL_SCAN_SPACE,          // Space
    [0x3A] = PAL_SCAN_CAPSLOCK,       // Caps Lock
    [0x3B] = PAL_SCAN_F1,             // F1
    [0x3C] = PAL_SCAN_F2,             // F2
    [0x3D] = PAL_SCAN_F3,             // F3
    [0x3E] = PAL_SCAN_F4,             // F4
    [0x3F] = PAL_SCAN_F5,             // F5
    [0x40] = PAL_SCAN_F6,             // F6
    [0x41] = PAL_SCAN_F7,             // F7
    [0x42] = PAL_SCAN_F8,             // F8
    [0x43] = PAL_SCAN_F9,             // F9
    [0x44] = PAL_SCAN_F10,            // F10
    [0x45] = PAL_SCAN_NUMCLEAR,       // Num Lock
    [0x46] = PAL_SCAN_SCROLLLOCK,     // Scroll Lock
    [0x47] = PAL_SCAN_KP_7,           // Keypad 7 / Home
    [0x48] = PAL_SCAN_KP_8,           // Keypad 8 / Up
    [0x49] = PAL_SCAN_KP_9,           // Keypad 9 / Page Up
    [0x4A] = PAL_SCAN_KP_MINUS,       // Keypad -
    [0x4B] = PAL_SCAN_KP_4,           // Keypad 4 / Left
    [0x4C] = PAL_SCAN_KP_5,           // Keypad 5
    [0x4D] = PAL_SCAN_KP_6,           // Keypad 6 / Right
    [0x4E] = PAL_SCAN_KP_PLUS,        // Keypad +
    [0x4F] = PAL_SCAN_KP_1,           // Keypad 1 / End
    [0x50] = PAL_SCAN_KP_2,           // Keypad 2 / Down
    [0x51] = PAL_SCAN_KP_3,           // Keypad 3 / Page Down
    [0x52] = PAL_SCAN_KP_0,           // Keypad 0 / Insert
    [0x53] = PAL_SCAN_KP_PERIOD,      // Keypad . / Delete
    [0x56] = PAL_SCAN_NONUSBACKSLASH, // Non-US backslash (ISO layout)
    [0x57] = PAL_SCAN_F11,            // F11
    [0x58] = PAL_SCAN_F12,            // F12
    [0x64] = PAL_SCAN_F13,            // F13
    [0x65] = PAL_SCAN_F14,            // F14
    [0x66] = PAL_SCAN_F15,            // F15
    [0x67] = PAL_SCAN_F16,            // F16
    [0x68] = PAL_SCAN_F17,            // F17
    [0x69] = PAL_SCAN_F18,            // F18
    [0x6A] = PAL_SCAN_F19,            // F19
    [0x6B] = PAL_SCAN_F20,            // F20
    [0x6C] = PAL_SCAN_F21,            // F21
    [0x6D] = PAL_SCAN_F22,            // F22
    [0x6E] = PAL_SCAN_F23,            // F23
    [0x6F] = PAL_SCAN_F24,            // F24
    [0x70] = PAL_SCAN_INTERNATIONAL2, // Katakana/Hiragana
    [0x73] = PAL_SCAN_INTERNATIONAL1, // Ro
    [0x79] = PAL_SCAN_INTERNATIONAL4, // Henkan
    [0x7B] = PAL_SCAN_INTERNATIONAL5, // Muhenkan
    [0x7D] = PAL_SCAN_INTERNATIONAL3, // Yen

};

// Translation table for extended makecodes (E0 prefix keys)
static int win32_extended_makecode_to_pal_scancode[256] = {
    [0x1C] = PAL_SCAN_KP_ENTER,     // Keypad Enter
    [0x1D] = PAL_SCAN_RCTRL,        // Right Ctrl
    [0x35] = PAL_SCAN_KP_DIVIDE,    // Keypad /
    [0x37] = PAL_SCAN_PRINTSCREEN,  // Print Screen
    [0x38] = PAL_SCAN_RALT,         // Right Alt / AltGr
    [0x46] = PAL_SCAN_PAUSE,        // Pause/Break
    [0x47] = PAL_SCAN_HOME,         // Home
    [0x48] = PAL_SCAN_UP,           // Up Arrow
    [0x49] = PAL_SCAN_PAGEUP,       // Page Up
    [0x4B] = PAL_SCAN_LEFT,         // Left Arrow
    [0x4D] = PAL_SCAN_RIGHT,        // Right Arrow
    [0x4F] = PAL_SCAN_END,          // End
    [0x50] = PAL_SCAN_DOWN,         // Down Arrow
    [0x51] = PAL_SCAN_PAGEDOWN,     // Page Down
    [0x52] = PAL_SCAN_INSERT,       // Insert
    [0x53] = PAL_SCAN_DELETE,       // Delete
    [0x5B] = PAL_SCAN_LGUI,         // Left Windows/Super
    [0x5C] = PAL_SCAN_RGUI,         // Right Windows/Super
    [0x5D] = PAL_SCAN_APPLICATION,  // Menu/Application
    [0x5F] = PAL_SCAN_SLEEP,        // Sleep
    [0x63] = PAL_SCAN_WAKE,         // Wake
    [0x65] = PAL_SCAN_AC_SEARCH,    // Search
    [0x66] = PAL_SCAN_AC_BOOKMARKS, // Favorites
    [0x67] = PAL_SCAN_AC_REFRESH,   // Refresh
    [0x68] = PAL_SCAN_AC_STOP,      // Stop
    [0x69] = PAL_SCAN_AC_FORWARD,   // Forward
    [0x6A] = PAL_SCAN_AC_BACK,      // Back
    [0x6B] = PAL_SCAN_AC_HOME,      // My Computer
    [0x6C] = PAL_SCAN_AC_OPEN,      // Mail
    [0x6D] = PAL_SCAN_MEDIA_SELECT, // Media Select
};

typedef struct {
    uint8_t usage;
    float value;
} win32_gamepad_button;

typedef struct {
    uint8_t usage;
    float value;
    int32_t min, max;
    pal_bool inverted;
} win32_gamepad_axis;

typedef struct {
    uint16_t vendor_id;
    uint16_t product_id;
    char name[128];
    uint8_t button_map[15];
    struct {
        uint8_t usage;
        pal_bool inverted;
    } axis_map[6];
} win32_gamepad_mapping;

typedef struct {
    uint8_t report[64];
    uint8_t report_size;
    pal_bool has_report;
    OVERLAPPED overlapped;
} win32_dualsense_state;

// Global State
typedef struct hid_device {
    HANDLE handle;
    PHIDP_PREPARSED_DATA pp_data;
    uint16_t vendor_id;
    uint16_t product_id;
    char name[128];
    win32_gamepad_button buttons[PAL_MAX_BUTTONS];
    uint8_t button_count;
    win32_gamepad_axis axes[PAL_MAX_AXES];
    uint8_t axis_count;
    pal_bool connected;
} hid_device;

typedef struct win32_gamepad_context {
    uint8_t xinput_connected[MAX_XINPUT_CONTROLLERS];
    XINPUT_STATE xinput_state[MAX_XINPUT_CONTROLLERS];

    uint8_t raw_input_buffer[1024]; // <-- THIS IS THE BUFFER

    struct hid_device hid_devices[PAL_MAX_GAMEPADS];
    struct { // DualSense/DS4
        HANDLE handle;
        win32_dualsense_state state;
        uint16_t vendor_id;
        uint16_t product_id;
        char name[128];
        pal_bool connected;
    } ds_devices[PAL_MAX_GAMEPADS];

    win32_gamepad_mapping mappings[PAL_MAX_MAPPINGS];
    int mapping_count;
    pal_bool initialized;
    HWND hwnd;
} win32_gamepad_context;
win32_gamepad_context win32_gamepad_ctx = {0};

#pragma pack(push, 1)
typedef struct {
    uint16_t idReserved; // Must be 0
    uint16_t idType;     // Must be 1 for icons
    uint16_t idCount;    // Number of images
} ICONDIR;

typedef struct {
    uint8_t bWidth;         // Width in pixels
    uint8_t bHeight;        // Height in pixels
    uint8_t bColorCount;    // 0 if >= 8bpp
    uint8_t bReserved;      // Must be 0
    uint16_t wPlanes;       // Should be 1
    uint16_t wBitCount;     // Usually 32
    uint32_t dwBytesInRes;  // Size of PNG data
    uint32_t dwImageOffset; // Offset to PNG data (after header)
} ICONDIRENTRY;
#pragma pack(pop)

PALAPI pal_bool pal_make_window_fullscreen_ex(pal_window* window, int width, int height, int refresh_rate) {
    DEVMODEA dm = {0};

    window->windowedStyle = GetWindowLongA(window->hwnd, GWL_STYLE);

    dm.dmSize = sizeof(dm);
    dm.dmPelsWidth = width;
    dm.dmPelsHeight = height;
    dm.dmDisplayFrequency = refresh_rate;
    dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;

    if (ChangeDisplaySettingsExA(NULL, &dm, NULL, CDS_FULLSCREEN, NULL) != DISP_CHANGE_SUCCESSFUL) {
        MessageBoxA(window->hwnd, "Failed to switch display mode", "Error", MB_OK);
        return pal_false;
    }

    SetWindowLongA(window->hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
    SetWindowPos(window->hwnd, HWND_TOP, 0, 0, width, height, SWP_FRAMECHANGED | SWP_NOOWNERZORDER);

    return pal_true;
}

PALAPI pal_bool pal_make_window_fullscreen(pal_window* window) {
    int width = (int)window->width;
    int height = (int)window->height;
    DEVMODEA dm = {0};

    window->windowedStyle = GetWindowLongA(window->hwnd, GWL_STYLE);
    EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &dm);
    dm.dmSize = sizeof(dm);
    dm.dmPelsWidth = (DWORD)window->width;
    dm.dmPelsHeight = (DWORD)window->height;
    dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;

    if (ChangeDisplaySettingsExA(NULL, &dm, NULL, CDS_FULLSCREEN, NULL) != DISP_CHANGE_SUCCESSFUL) {
        MessageBoxA(window->hwnd, "Failed to switch display mode", "Error", MB_OK);
        return pal_false;
    }

    SetWindowLongA(window->hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
    SetWindowPos(window->hwnd, HWND_TOP, 0, 0, dm.dmPelsWidth, dm.dmPelsHeight, SWP_FRAMECHANGED | SWP_NOOWNERZORDER);

    return pal_true;
}

PALAPI pal_bool pal_make_window_fullscreen_windowed(pal_window* window) {
    HMONITOR monitor = MonitorFromWindow(window->hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO mi = {.cbSize = sizeof(mi)};

    // Save the current window style and rect
    window->windowedStyle = GetWindowLongA(window->hwnd, GWL_STYLE);

    // Get the monitor bounds
    if (!GetMonitorInfo(monitor, &mi)) {
        MessageBoxA(window->hwnd, "Failed to get monitor info.", "Error", MB_OK);
        return pal_false;
    }

    // Set the window to borderless fullscreen
    SetWindowLongA(window->hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
    if (!SetWindowPos(window->hwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_FRAMECHANGED | SWP_NOOWNERZORDER)) {
        MessageBoxA(window->hwnd, "Failed to resize window.", "Error", MB_OK);
        return pal_false;
    }

    return pal_true;
}

PALAPI pal_bool pal_make_window_windowed(pal_window* window) {
    RECT rect;
    // Restore display mode (in case exclusive mode was used)
    ChangeDisplaySettings(NULL, 0);

    // Restore the window style
    if (SetWindowLongA(window->hwnd, GWL_STYLE, window->windowedStyle) == 0) {
        MessageBoxA(window->hwnd, "Failed to restore window style.", "Error", MB_OK);
        return pal_false;
    }
    GetWindowRect(window->hwnd, &rect);
    // Restore the window's size and position
    if (!SetWindowPos(window->hwnd, NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER | SWP_FRAMECHANGED)) {
        MessageBoxA(window->hwnd, "Failed to restore window position.", "Error", MB_OK);
        return pal_false;
    }

    return pal_true;
}

PALAPI void pal_set_cursor(pal_window* window, const char* filepath, int size, int hotspot_x, int hotspot_y) {
    FILE* f = fopen(filepath, "rb");
    unsigned char header[12] = {0};
    HCURSOR hCursor = NULL;
    int width, height, channels;
    unsigned char *pixels = NULL, *resized = NULL;
    HDC hdc = GetDC(NULL);
    BITMAPV5HEADER bi = {0};
    void* bitmapData = NULL;
    HBITMAP hBitmap = NULL;
    ICONINFO ii = {0};

    if (size <= 0)
        size = 32;
    if (size > 256)
        size = 256;

    if (!f) {
        MessageBoxA(window->hwnd, "Failed to open cursor file.", "SetCustomCursor Error", MB_ICONERROR);
        return;
    }

    fread(header, 1, sizeof(header), f);
    fclose(f);


    if (pal_memcmp(header, "RIFF", 4) == 0 && pal_memcmp(header + 8, "ACON", 4) == 0) {
        hCursor = (HCURSOR)LoadImageA(NULL, filepath, IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);
        if (!hCursor) {
            MessageBoxA(window->hwnd, "Failed to load .ani cursor.", "SetCustomCursor Error", MB_ICONERROR);
            return;
        }
    } else if (header[0] == 0x00 && header[1] == 0x00 && header[2] == 0x02 && header[3] == 0x00) {
        hCursor = (HCURSOR)LoadImageA(NULL, filepath, IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);
        if (!hCursor) {
            MessageBoxA(window->hwnd, "Failed to load .cur cursor.", "SetCustomCursor Error", MB_ICONERROR);
            return;
        }
    } else {
        pixels = stbi_load(filepath, &width, &height, &channels, 4);
        if (!pixels) {
            MessageBoxA(window->hwnd, "Failed to load image with stb_image.", "SetCustomCursor Error", MB_ICONERROR);
            return;
        }

        resized = malloc(size * size * 4);
        if (!resized) {
            stbi_image_free(pixels);
            MessageBoxA(window->hwnd, "Failed to allocate memory for resized image.", "SetCustomCursor Error", MB_ICONERROR);
            return;
        }

        stbir_resize_uint8_srgb(
            pixels, width, height, width * 4, resized, size, size, size * 4, STBIR_RGBA);
        stbi_image_free(pixels);

        bi.bV5Size = sizeof(BITMAPV5HEADER);
        bi.bV5Width = size;
        bi.bV5Height = -size;
        bi.bV5Planes = 1;
        bi.bV5BitCount = 32;
        bi.bV5Compression = BI_BITFIELDS;
        bi.bV5RedMask = 0x00FF0000;
        bi.bV5GreenMask = 0x0000FF00;
        bi.bV5BlueMask = 0x000000FF;
        bi.bV5AlphaMask = 0xFF000000;

        hBitmap = CreateDIBSection(hdc, (BITMAPINFO*)&bi, DIB_RGB_COLORS, &bitmapData, NULL, 0);
        ReleaseDC(NULL, hdc);

        if (!hBitmap || !bitmapData) {
            free(resized);
            MessageBoxA(window->hwnd, "Failed to create DIB section.", "SetCustomCursor Error", MB_ICONERROR);
            return;
        }

        for (int y = 0; y < size; ++y) {
            for (int x = 0; x < size; ++x) {
                unsigned char* src = &resized[(y * size + x) * 4];
                unsigned char* dst = (unsigned char*)bitmapData + (y * size + x) * 4;
                dst[0] = src[2]; // B
                dst[1] = src[1]; // G
                dst[2] = src[0]; // R
                dst[3] = src[3]; // A
            }
        }

        free(resized);

        ii.fIcon = pal_false;
        ii.xHotspot = hotspot_x;
        ii.yHotspot = hotspot_y;
        ii.hbmColor = hBitmap;
        ii.hbmMask = CreateBitmap(size, size, 1, 1, NULL);

        hCursor = CreateIconIndirect(&ii);

        DeleteObject(ii.hbmMask);
        DeleteObject(hBitmap);

        if (!hCursor) {
            MessageBoxA(window->hwnd, "Failed to create cursor from image.", "SetCustomCursor Error", MB_ICONERROR);
            return;
        }
    }

    SetClassLongPtr(window->hwnd, GCLP_HCURSOR, (LONG_PTR)hCursor);
    SetCursor(hCursor);
}
// older windows versions do not support pngs being embedded in .ico
// files directly, therefore, we have to decode the .png with stb_image
// and only then can we use it.
static HICON win32_load_icon_from_file(const char* image_path, BOOL legacy) {
    FILE* file = fopen(image_path, "rb");
    uint8_t header[8];
    const unsigned char png_sig[8] = {0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A};
    BOOL is_png;
    uint8_t* png_data = NULL;
    long size = 0;
    size_t header_size, total_size = 0;
    uint8_t* ico_data = NULL;
    ICONDIR* icon_dir;
    ICONDIRENTRY* entry;
    HICON hIcon = NULL;
    int width, height, channels;
    uint8_t* rgba = NULL;
    BITMAPV5HEADER bi = {0};
    void* dib_pixels = NULL;
    HDC hdc = GetDC(NULL);
    HBITMAP color_bitmap, mask_bitmap;
    ICONINFO ii = {0};
    HICON hIcon;

    if (!file)
        return NULL;

    if (fread(header, 1, sizeof(header), file) < sizeof(header)) {
        fclose(file);
        return NULL;
    }

    // Check for ICO header
    if (header[0] == 0x00 && header[1] == 0x00 &&
        header[2] == 0x01 && header[3] == 0x00) {
        fclose(file);
        return (HICON)LoadImageA(NULL, image_path, IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
    }

    // Check for PNG header
    is_png = (pal_memcmp(header, png_sig, 8) == 0);
    fseek(file, 0, SEEK_SET);

    if (is_png && !legacy) {
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        rewind(file);
        if (size <= 0) {
            fclose(file);
            return NULL;
        }

        png_data = (uint8_t*)malloc(size);
        if (!png_data) {
            fclose(file);
            return NULL;
        }
        fread(png_data, 1, size, file);
        fclose(file);

        header_size = sizeof(ICONDIR) + sizeof(ICONDIRENTRY);
        total_size = header_size + size;
        ico_data = (uint8_t*)malloc(total_size);
        if (!ico_data) {
            free(png_data);
            return NULL;
        }

        icon_dir = (ICONDIR*)ico_data;
        icon_dir->idReserved = 0;
        icon_dir->idType = 1;
        icon_dir->idCount = 1;

        entry = (ICONDIRENTRY*)(ico_data + sizeof(ICONDIR));
        entry->bWidth = 0;
        entry->bHeight = 0;
        entry->bColorCount = 0;
        entry->bReserved = 0;
        entry->wPlanes = 1;
        entry->wBitCount = 32;
        entry->dwBytesInRes = (uint32_t)size;
        entry->dwImageOffset = (uint32_t)header_size;

        pal_memcpy(ico_data + header_size, png_data, size);
        free(png_data);
        hIcon = CreateIconFromResourceEx(
            ico_data + entry->dwImageOffset,
            entry->dwBytesInRes,
            pal_true,
            0x00030000,
            0,
            0,
            LR_DEFAULTCOLOR);

        free(ico_data);
        return hIcon;
    }

    // Fallback: decode image with stb_image (PNG in legacy mode, JPEG, BMP)
    fclose(file);
    rgba = stbi_load(image_path, &width, &height, &channels, 4);
    if (!rgba)
        return NULL;

    // Convert RGBA to BGRA
    for (int i = 0; i < width * height; ++i) {
        uint8_t r = rgba[i * 4 + 0];
        uint8_t g = rgba[i * 4 + 1];
        uint8_t b = rgba[i * 4 + 2];
        uint8_t a = rgba[i * 4 + 3];
        rgba[i * 4 + 0] = b;
        rgba[i * 4 + 1] = g;
        rgba[i * 4 + 2] = r;
        rgba[i * 4 + 3] = a;
    }

    bi.bV5Size = sizeof(BITMAPV5HEADER);
    bi.bV5Width = width;
    bi.bV5Height = -height;
    bi.bV5Planes = 1;
    bi.bV5BitCount = 32;
    bi.bV5Compression = BI_BITFIELDS;
    bi.bV5RedMask = 0x00FF0000;
    bi.bV5GreenMask = 0x0000FF00;
    bi.bV5BlueMask = 0x000000FF;
    bi.bV5AlphaMask = 0xFF000000;

    color_bitmap = CreateDIBSection(hdc, (BITMAPINFO*)&bi, DIB_RGB_COLORS, &dib_pixels, NULL, 0);
    ReleaseDC(NULL, hdc);

    if (!color_bitmap || !dib_pixels) {
        stbi_image_free(rgba);
        return NULL;
    }

    pal_memcpy(dib_pixels, rgba, width * height * 4);
    stbi_image_free(rgba);
    mask_bitmap = CreateBitmap(width, height, 1, 1, NULL);

    ii.fIcon = pal_true;
    ii.hbmMask = mask_bitmap;
    ii.hbmColor = color_bitmap;

    hIcon = CreateIconIndirect(&ii);

    DeleteObject(color_bitmap);
    DeleteObject(mask_bitmap);

    return hIcon;
}

PALAPI void pal_set_window_icon(pal_window* window, const char* image_path) {
    HICON hIcon = win32_load_icon_from_file(image_path, pal_false);
    if (hIcon) {
        SendMessage(window->hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        SendMessage(window->hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    } else {
        MessageBoxA(window->hwnd, "Failed to load window icon", "Error", MB_OK | MB_ICONERROR);
    }
}

PALAPI void pal_set_window_icon_legacy(pal_window* window, const char* image_path) {
    HICON hIcon = win32_load_icon_from_file(image_path, pal_true);
    if (hIcon) {
        SendMessage(window->hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        SendMessage(window->hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    } else {
        MessageBoxA(window->hwnd, "Failed to load window icon", "Error", MB_OK | MB_ICONERROR);
    }
}

PALAPI void pal_set_taskbar_icon(pal_window* window, const char* image_path) {
    HICON hIcon = win32_load_icon_from_file(image_path, pal_false);
    if (hIcon) {
        SetClassLongPtr(window->hwnd, GCLP_HICONSM, (LONG_PTR)hIcon);
        SetClassLongPtr(window->hwnd, GCLP_HICON, (LONG_PTR)hIcon);
    } else {
        MessageBoxA(window->hwnd, "Failed to load taskbar icon", "Error", MB_OK | MB_ICONERROR);
    }
}

PALAPI void pal_set_taskbar_icon_legacy(pal_window* window, const char* image_path) {
    HICON hIcon = win32_load_icon_from_file(image_path, pal_true);
    if (hIcon) {
        SetClassLongPtr(window->hwnd, GCLP_HICONSM, (LONG_PTR)hIcon);
        SetClassLongPtr(window->hwnd, GCLP_HICON, (LONG_PTR)hIcon);
    } else {
        MessageBoxA(window->hwnd, "Failed to load legacy taskbar icon", "Error", MB_OK | MB_ICONERROR);
    }
}

LRESULT CALLBACK win32_fake_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

void win32_handle_raw_input(HRAWINPUT raw_input) {
    // currently unused. This is for GetRawInputData().
    // we currently use GetRawInputBuffer(), because it's better for high-polling rate mice.
}

void win32_handle_device_change(HANDLE hDevice, DWORD dwChange) {
}

// --- pal_get_gamepad_count ---

PALAPI int pal_get_gamepad_count(void) {
    // Poll remaining XInput slots
    for (DWORD i = 0; i < MAX_XINPUT_CONTROLLERS; ++i) {

        XINPUT_STATE state;
        if (XinputGetstate_fn(i, &state) == ERROR_SUCCESS) {
            win32_gamepad_ctx.xinput_connected[i] = pal_true;
            win32_gamepad_ctx.xinput_state[i] = state;
        } else {
            win32_gamepad_ctx.xinput_connected[i] = pal_false;
        }
    }

    int total_count = 0;
    for (int i = 0; i < MAX_XINPUT_CONTROLLERS; ++i) {
        if (win32_gamepad_ctx.xinput_connected[i])
            total_count++;
    }

    return total_count;
}

int win32_init_gamepads() {
    //---------------
    //     Xinput
    //---------------

    // Try XInput 1.4 first (Windows 8+, has trigger motors)
    g_xinput_dll = LoadLibraryW(L"xinput1_4.dll");
    if (g_xinput_dll) {
        g_has_trigger_motors = pal_true;
    } else {
        // Fallback to XInput 1.3 (Windows Vista/7)
        g_xinput_dll = LoadLibraryW(L"xinput1_3.dll");
        if (!g_xinput_dll) {
            // Last resort: XInput 9.1.0 (Windows 7 compatibility)
            g_xinput_dll = LoadLibraryW(L"xinput9_1_0.dll");
        }
        g_has_trigger_motors = pal_false;
    }

    if (!g_xinput_dll) {
        return pal_false;
    }
    // Load function pointers
    XinputGetstate_fn = (DWORD(WINAPI*)(DWORD, XINPUT_STATE*))GetProcAddress(g_xinput_dll, "XInputGetState");
    XInputSetState_fn = (DWORD(WINAPI*)(DWORD, XINPUT_VIBRATION*))GetProcAddress(g_xinput_dll, "XInputSetState");
    XInputGetCapabilities_fn = (DWORD(WINAPI*)(DWORD, DWORD, XINPUT_CAPABILITIES*))GetProcAddress(g_xinput_dll, "XInputGetCapabilities");
    XInputEnable_fn = (void(WINAPI*)(BOOL))GetProcAddress(g_xinput_dll, "XInputEnable");

    // Check if we got the essential functions
    if (!XinputGetstate_fn || !XInputSetState_fn) {
        FreeLibrary(g_xinput_dll);
        g_xinput_dll = NULL;
        return pal_false;
    }

    return pal_true;
}

void win32_shutdown_gamepads(void) {
    if (g_xinput_dll) {
        FreeLibrary(g_xinput_dll);
        g_xinput_dll = NULL;
    }

    XinputGetstate_fn = NULL;
    XInputSetState_fn = NULL;
    XInputGetCapabilities_fn = NULL;
    XInputEnable_fn = NULL;
    g_has_trigger_motors = pal_false;
}

PALAPI pal_bool pal_get_gamepad_state(int index, pal_gamepad_state* out_state) {
    pal_memset(out_state, 0, sizeof(pal_gamepad_state));

    // XInput controllers only
    if (index >= MAX_XINPUT_CONTROLLERS) {
        return pal_false;
    }

    // Check if this specific controller slot is connected
    if (!win32_gamepad_ctx.xinput_connected[index]) {
        return pal_false;
    }

    const XINPUT_GAMEPAD* pad = &win32_gamepad_ctx.xinput_state[index].Gamepad;

    // Process analog sticks with proper deadzone handling
    float lx = (float)pad->sThumbLX;
    float ly = (float)pad->sThumbLY;
    float rx = (float)pad->sThumbRX;
    float ry = (float)pad->sThumbRY;

    // Apply circular deadzone for left stick
    float left_magnitude = sqrtf(lx * lx + ly * ly);
    if (left_magnitude < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
        lx = ly = 0;
    } else {
        // Normalize to remove deadzone
        float normalized = (left_magnitude - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) /
                           (32767.0f - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
        lx = (lx / left_magnitude) * normalized;
        ly = (ly / left_magnitude) * normalized;
    }

    // Apply circular deadzone for right stick
    float right_magnitude = sqrtf(rx * rx + ry * ry);
    if (right_magnitude < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
        rx = ry = 0;
    } else {
        // Normalize to remove deadzone
        float normalized = (right_magnitude - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) /
                           (32767.0f - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
        rx = (rx / right_magnitude) * normalized;
        ry = (ry / right_magnitude) * normalized;
    }

    out_state->axes.left_x = fmaxf(-1.0f, fminf(1.0f, lx));
    out_state->axes.left_y = fmaxf(-1.0f, fminf(1.0f, ly));
    out_state->axes.right_x = fmaxf(-1.0f, fminf(1.0f, rx));
    out_state->axes.right_y = fmaxf(-1.0f, fminf(1.0f, ry));

    // Process triggers with deadzone
    out_state->axes.left_trigger = (pad->bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? 0.0f : (pad->bLeftTrigger / 255.0f);
    out_state->axes.right_trigger = (pad->bRightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? 0.0f : (pad->bRightTrigger / 255.0f);

    // Process buttons
    WORD b = pad->wButtons;
    out_state->buttons.a = (b & XINPUT_GAMEPAD_A) != 0;
    out_state->buttons.b = (b & XINPUT_GAMEPAD_B) != 0;
    out_state->buttons.x = (b & XINPUT_GAMEPAD_X) != 0;
    out_state->buttons.y = (b & XINPUT_GAMEPAD_Y) != 0;
    out_state->buttons.back = (b & XINPUT_GAMEPAD_BACK) != 0;
    out_state->buttons.start = (b & XINPUT_GAMEPAD_START) != 0;
    out_state->buttons.left_stick = (b & XINPUT_GAMEPAD_LEFT_THUMB) != 0;
    out_state->buttons.right_stick = (b & XINPUT_GAMEPAD_RIGHT_THUMB) != 0;
    out_state->buttons.left_shoulder = (b & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0;
    out_state->buttons.right_shoulder = (b & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
    out_state->buttons.dpad_up = (b & XINPUT_GAMEPAD_DPAD_UP) != 0;
    out_state->buttons.dpad_down = (b & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
    out_state->buttons.dpad_left = (b & XINPUT_GAMEPAD_DPAD_LEFT) != 0;
    out_state->buttons.dpad_right = (b & XINPUT_GAMEPAD_DPAD_RIGHT) != 0;

    // Set controller info
    pal_strncpy(out_state->name, "Xbox Controller", sizeof(out_state->name) - 1);
    out_state->name[sizeof(out_state->name) - 1] = '\0';
    out_state->vendor_id = 0x045E;                // Microsoft
    out_state->product_id = (uint16_t)0xDEADBEEF; // Since xinput supports xbox360 and various Xbone controllers, we don't know this.
    out_state->connected = pal_true;
    out_state->is_xinput = pal_true;

    return pal_true;
}

PALAPI void pal_set_gamepad_vibration(int controller_id, float left_motor, float right_motor, float left_trigger, float right_trigger) {
    if (!XInputSetState_fn || controller_id > 4)
        return;

    if (g_has_trigger_motors) {
        // Extended vibration structure with trigger motors (XInput 1.4+)
        struct {
            WORD wLeftMotorSpeed;
            WORD wRightMotorSpeed;
            WORD wLeftTriggerMotor;
            WORD wRightTriggerMotor;
        } vibration_ex; // we call it this to avoid potential naming conflicts.

        vibration_ex.wLeftMotorSpeed = (WORD)(left_motor * 65535.0f);
        vibration_ex.wRightMotorSpeed = (WORD)(right_motor * 65535.0f);
        vibration_ex.wLeftTriggerMotor = (WORD)(left_trigger * 65535.0f);
        vibration_ex.wRightTriggerMotor = (WORD)(right_trigger * 65535.0f);

        XInputSetState_fn(controller_id, (XINPUT_VIBRATION*)&vibration_ex);
    } else {
        // Standard vibration (XInput 9.1.0/1.3)
        XINPUT_VIBRATION vibration;
        vibration.wLeftMotorSpeed = (WORD)(left_motor * 65535.0f);
        vibration.wRightMotorSpeed = (WORD)(right_motor * 65535.0f);

        XInputSetState_fn(controller_id, &vibration);
    }
}

PALAPI void pal_stop_gamepad_vibration(int controller_id) {
    pal_set_gamepad_vibration(controller_id, 0.0f, 0.0f, 0.0f, 0.0f);
}

PALAPI pal_bool pal_close_window(pal_window* window) {
    return (pal_bool)DestroyWindow(window->hwnd);
}

void win32_enumerate_keyboards(void) {
    UINT numDevices;
    GetRawInputDeviceList(NULL, &numDevices, sizeof(RAWINPUTDEVICELIST));

    PRAWINPUTDEVICELIST deviceList = malloc(numDevices * sizeof(RAWINPUTDEVICELIST));
    GetRawInputDeviceList(deviceList, &numDevices, sizeof(RAWINPUTDEVICELIST));

    g_keyboards.count = 0;

    for (UINT i = 0; i < numDevices && g_keyboards.count < MAX_KEYBOARDS; i++) {
        if (deviceList[i].dwType == RIM_TYPEKEYBOARD) {
            int idx = g_keyboards.count;
            g_keyboards.handles[idx] = deviceList[i].hDevice;

            UINT size = 0;
            GetRawInputDeviceInfo(g_keyboards.handles[idx], RIDI_DEVICENAME, NULL, &size);
            if (size < sizeof(g_keyboards.names[idx])) {
                GetRawInputDeviceInfo(g_keyboards.handles[idx], RIDI_DEVICENAME, 
                                    g_keyboards.names[idx], &size);
            }

            // Initialize state arrays for this keyboard
            pal_memset(g_keyboards.keys[idx], 0, MAX_SCANCODES);
            pal_memset(g_keyboards.keys_toggled[idx], 0, MAX_SCANCODES);

            g_keyboards.count++;
        }
    }

    free(deviceList);
}

void win32_enumerate_mice(void) {
    UINT numDevices;
    GetRawInputDeviceList(NULL, &numDevices, sizeof(RAWINPUTDEVICELIST));

    PRAWINPUTDEVICELIST deviceList = malloc(numDevices * sizeof(RAWINPUTDEVICELIST));
    GetRawInputDeviceList(deviceList, &numDevices, sizeof(RAWINPUTDEVICELIST));

    g_mice.count = 0;

    for (UINT i = 0; i < numDevices && g_mice.count < MAX_MICE; i++) {
        if (deviceList[i].dwType == RIM_TYPEMOUSE) {
            int idx = g_mice.count;
            g_mice.handles[idx] = deviceList[i].hDevice;

            UINT size = 0;
            GetRawInputDeviceInfo(g_mice.handles[idx], RIDI_DEVICENAME, NULL, &size);
            if (size < sizeof(g_mice.names[idx])) {
                GetRawInputDeviceInfo(g_mice.handles[idx], RIDI_DEVICENAME, 
                                    g_mice.names[idx], &size);
            }

            // Initialize state arrays for this mouse
            pal_memset(g_mice.buttons[idx], 0, MAX_MOUSE_BUTTONS * sizeof(int));
            pal_memset(g_mice.buttons_toggled[idx], 0, MAX_MOUSE_BUTTONS * sizeof(int));
            g_mice.dx[idx] = 0;
            g_mice.dy[idx] = 0;
            g_mice.wheel[idx] = 0;

            g_mice.count++;
        }
    }

    free(deviceList);
}

static void update_modifier_state(USHORT vk, pal_bool is_key_released, int keyboard_index) {
    int modifier_flag = 0;

    switch (vk) {
        case VK_LSHIFT:
            modifier_flag = PAL_MOD_LSHIFT;
            break;
        case VK_RSHIFT:
            modifier_flag = PAL_MOD_RSHIFT;
            break;
        case VK_LCONTROL:
            modifier_flag = PAL_MOD_LCTRL;
            break;
        case VK_RCONTROL:
            modifier_flag = PAL_MOD_RCTRL;
            break;
        case VK_LMENU:
            modifier_flag = PAL_MOD_LALT;
            break;
        case VK_RMENU:
            modifier_flag = PAL_MOD_RALT;
            if (is_key_released) {
                g_keyboards.cached_modifiers[keyboard_index] &= ~PAL_MOD_ALTGR;
            } else {
                g_keyboards.cached_modifiers[keyboard_index] |= PAL_MOD_ALTGR;
            }
            break;
        case VK_LWIN:
            modifier_flag = PAL_MOD_LSUPER;
            break;
        case VK_RWIN:
            modifier_flag = PAL_MOD_RSUPER;
            break;
        case VK_CAPITAL:
            if (!is_key_released) {
                g_keyboards.cached_modifiers[keyboard_index] ^= PAL_MOD_CAPS;
            }
            return;
        case VK_NUMLOCK:
            if (!is_key_released) {
                g_keyboards.cached_modifiers[keyboard_index] ^= PAL_MOD_NUM;
            }
            return;
        case VK_SCROLL:
            if (!is_key_released) {
                g_keyboards.cached_modifiers[keyboard_index] ^= PAL_MOD_SCROLL;
            }
            return;
        default:
            return;
    }

    if (is_key_released) {
        g_keyboards.cached_modifiers[keyboard_index] &= ~modifier_flag;
    } else {
        g_keyboards.cached_modifiers[keyboard_index] |= modifier_flag;
    }
}

void win32_handle_keyboard(const RAWINPUT* raw) {
    // Find keyboard index
    int kb_index = 0;
    for (int i = 0; i < g_keyboards.count; i++) {
        if (g_keyboards.handles[i] == raw->header.hDevice) {
            kb_index = i;
            break;
        }
    }

    USHORT vk = raw->data.keyboard.VKey;
    USHORT makecode = raw->data.keyboard.MakeCode;
    USHORT flags = raw->data.keyboard.Flags;
    pal_event event = {0};

    pal_bool is_key_released = (flags & RI_KEY_BREAK) != 0;
    pal_bool is_extended = (flags & RI_KEY_E0) != 0;

    static uint8_t key_is_down[MAX_KEYBOARDS][256] = {0};
    pal_bool is_repeat = 0;
    if (vk < 256) {
        if (!is_key_released && key_is_down[kb_index][vk]) {
            is_repeat = 1;
        }
        key_is_down[kb_index][vk] = !is_key_released;
    }

    update_modifier_state(vk, is_key_released, keyboard_index);

    int pal_key = (vk < 256) ? win32_key_to_pal_key[vk] : 0;
    int pal_scancode = 0;

    if (is_extended) {
        if (makecode < 256) pal_scancode = win32_extended_makecode_to_pal_scancode[makecode];
    } else {
        if (makecode < 256) pal_scancode = win32_makecode_to_pal_scancode[makecode];
    }

    if (is_key_released) {
        event.type = PAL_EVENT_KEY_UP;
        event.key.virtual_key = pal_key,
        event.key.scancode = pal_scancode,
        event.key.pressed = 0,
        event.key.repeat = 0,
        event.key.modifiers = g_keyboards.cached_modifiers[kb_index],
        event.key.keyboard_id = kb_index

        g_keyboards.keys[kb_index][pal_scancode] = 0;
        g_keyboards.keys_toggled[kb_index][pal_scancode] = 0;
        pal__eventq_push(&g_event_queue, event);
    } else {
        event.type = PAL_EVENT_KEY_DOWN;
        event.key.virtual_key = pal_key,
        event.key.scancode = pal_scancode,
        event.key.pressed = 1,
        event.key.repeat = is_repeat,
        event.key.modifiers = g_keyboards.cached_modifiers[kb_index],
        event.key.keyboard_id = kb_index
        pal__eventq_push(&g_event_queue, event);


        WCHAR utf_16_char[4] = {0};
        UINT scan_code = MapVirtualKey(vk, MAPVK_VK_TO_VSC);
        int result = ToUnicode(vk, scan_code, keyboard_state, utf16_buffer, 4, 0);
        
        if (result > 0) {
            // Convert UTF-16 to UTF-8
            char utf8_buffer[8] = {0};
            int utf8_len = WideCharToMultiByte(
                CP_UTF8,
                0,
                utf16_buffer,
                result,
                utf8_buffer,
                sizeof(utf8_buffer) - 1,
                NULL,
                NULL
            );
            
            if (utf8_len > 0) {
                utf8_buffer[utf8_len] = '\0';
                event.type = PAL_EVENT_TEXT_INPUT;
                pal_strncpy(event.text.text, utf8_buffer, sizeof(event.text.text) - 1);
                event.text.text[sizeof(event.text.text) - 1] = '\0';
                event.text.keyboard_id = kb_index;
                pal__eventq_push(&g_event_queue, event);
            }

        }

        unsigned char old_state = g_keyboards.keys[kb_index][pal_scancode];
        g_keyboards.keys[kb_index][pal_scancode] = 1;

        if (old_state != 1) {
            g_keyboards.keys_toggled[kb_index][pal_scancode] = 1;
        }
    }
}

void win32_handle_mouse(const RAWINPUT* raw) {
    // Find mouse index
    int mouse_index = 0;
    for (int i = 0; i < g_mice.count; i++) {
        if (g_mice.handles[i] == raw->header.hDevice) {
            mouse_index = i;
            break;
        }
    }

    pal_event event = {0};
    int32_t dx = raw->data.mouse.lLastX;
    int32_t dy = raw->data.mouse.lLastY;

    // Update per-mouse delta
    g_mice.dx[mouse_index] += dx;
    g_mice.dy[mouse_index] += dy;

    USHORT buttons = raw->data.mouse.usButtonFlags;
    POINT point = {0};
    GetCursorPos(&point);
    ScreenToClient(g_current_window->hwnd, &point);

    // Handle motion
    if (dx || dy) {
        event.type = PAL_EVENT_MOUSE_MOTION;
        event.motion.x = point.x;
        event.motion.y = point.y;
        event.motion.delta_x = dx;
        event.motion.delta_y = dy;
        event.motion.buttons = g_cached_mouse_buttons;
        event.motion.mouse_id = mouse_index;
        pal__eventq_push(&g_event_queue, event);
    }

    // Handle mouse wheel
    if (buttons & RI_MOUSE_WHEEL) {
        SHORT wheel_delta = (SHORT)HIWORD(raw->data.mouse.usButtonData);
        g_mice.wheel[mouse_index] += wheel_delta / WHEEL_DELTA;

        event.type = PAL_EVENT_MOUSE_WHEEL;
        event.wheel.mouse_x = point.x;
        event.wheel.mouse_y = point.y;
        event.wheel.x = 0;
        event.wheel.y = (float)(wheel_delta / WHEEL_DELTA);
        event.wheel.wheel_direction = (wheel_delta > 0) ? PAL_MOUSEWHEEL_VERTICAL : PAL_MOUSEWHEEL_HORIZONTAL;
        event.wheel.mouse_id = mouse_index;
        pal__eventq_push(&g_event_queue, event);
    }

    // Handle horizontal wheel
    if (buttons & RI_MOUSE_HWHEEL) {
        SHORT hwheel_delta = (SHORT)HIWORD(raw->data.mouse.usButtonData);

        event.type = PAL_EVENT_MOUSE_WHEEL;
        event.wheel.mouse_x = point.x;
        event.wheel.mouse_y = point.y;
        event.wheel.x = (float)(hwheel_delta / WHEEL_DELTA);
        event.wheel.y = 0;
        event.wheel.wheel_direction = (hwheel_delta > 0) ? PAL_MOUSEWHEEL_VERTICAL : PAL_MOUSEWHEEL_HORIZONTAL;
        event.wheel.mouse_id = mouse_index;
        pal__eventq_push(&g_event_queue, event);
    }

    // Handle button events
    for (int i = 0; i < 5; i++) {
        uint16_t down = (buttons >> (i * 2)) & 1;
        uint16_t up = (buttons >> (i * 2 + 1)) & 1;
        int pal_button = win32_button_to_pal_button[i];

        if (down) {
            int old_state = g_mice.buttons[mouse_index][pal_button];
            g_mice.buttons[mouse_index][pal_button] = 1;
            
            // Mark as toggled if state changed
            if (old_state != 1) {
                g_mice.buttons_toggled[mouse_index][pal_button] = 1;
            }
            
            g_cached_mouse_buttons |= (1 << i);

            int cached_modifers = 0;
            for(int i =0; i < g_keyboards.count; i++) {
                cached_modifiers |= g_keyboards.cached_modifiers[i];
            }
            event.type = PAL_EVENT_MOUSE_BUTTON_DOWN;
            event.button.x = point.x;
            event.button.y = point.y;
            event.button.pressed = 1;
            event.button.clicks = 1;
            event.button.modifiers = cached_modifiers;
            event.button.button = pal_button;
            event.button.mouse_id = mouse_index;
            pal__eventq_push(&g_event_queue, event);
        } else if (up) {
            g_mice.buttons[mouse_index][pal_button] = 0;
            g_mice.buttons_toggled[mouse_index][pal_button] = 0;
            g_cached_mouse_buttons &= ~(1 << i);

            event.type = PAL_EVENT_MOUSE_BUTTON_UP;
            event.button.x = point.x;
            event.button.y = point.y;
            event.button.pressed = 0;
            event.button.clicks = 1;
            event.button.modifiers = cached_modifiers;
            event.button.button = pal_button;
            event.button.mouse_id = mouse_index;
            pal__eventq_push(&g_event_queue, event);
        }
    }
}

static LRESULT CALLBACK win32_window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    pal_window* window = g_current_window;
    pal_event event = {0};
    WINDOWPOS* pos;
    HDROP hDrop;
    UINT count;
    RECT rect;
    POINT tl;
    POINT br;
    const char** paths;
    WCHAR buffer[MAX_PATH];
    int len;
    char* utf8;

     switch (msg) {
        case WM_CLOSE:
            event.type = PAL_EVENT_WINDOW_CLOSE_REQUESTED;
            event.window.x = LOWORD(lparam);
            event.window.y = HIWORD(lparam);
            event.window.width = 0;
            event.window.height = 0;
            event.window.focused = 1;
            event.window.visible = 0;
            break;
        case WM_DESTROY:
            event.type = PAL_EVENT_WINDOW_CLOSED;
            event.window.x = LOWORD(lparam);
            event.window.y = HIWORD(lparam);
            event.window.width = 0;
            event.window.height = 0;
            event.window.focused = 0;
            event.window.visible = 0;
            break;
        case WM_QUIT: // we only get this when we call PostQuitMessage. This is fucking retarted. If we want to kill the app, we just break from the main loop. I think we should just make this event do nothing.
            event.type = PAL_EVENT_QUIT;
            event.quit.code = 0;
            break;
        case WM_MOVE:
            event.type = PAL_EVENT_WINDOW_MOVED;
            event.window.x = LOWORD(lparam);
            event.window.y = HIWORD(lparam);
            event.window.width = 0;
            event.window.height = 0;
            event.window.focused = 1;
            event.window.visible = 1;
            break;
        case WM_SIZE:
            event.type = PAL_EVENT_WINDOW_RESIZED;
            event.window.event_code = WM_SIZE;
            event.window.x = 0;
            event.window.y = 0;
            event.window.width = LOWORD(lparam);
            event.window.height = HIWORD(lparam);
            event.window.focused = 1;
            event.window.visible = 1;
            break;

        case WM_MOUSEMOVE: {
            if (window->confine_mouse) {
                TRACKMOUSEEVENT tme = {
                    .cbSize = sizeof(tme),
                    .dwFlags = TME_LEAVE,
                    .hwndTrack = window->hwnd,
                    .dwHoverTime = HOVER_DEFAULT};
                TrackMouseEvent(&tme);
                GetClientRect(window->hwnd, &rect);
                tl = {rect.left, rect.top};
                br = {rect.right, rect.bottom};
                ClientToScreen(window->hwnd, &tl);
                ClientToScreen(window->hwnd, &br);
                rect.left = tl.x;
                rect.top = tl.y;
                rect.right = br.x;
                rect.bottom = br.y;
                ClipCursor(&rect);
            }
            // Mouse just entered the window
            event.type = PAL_EVENT_MOUSE_MOTION;
            event.motion.x = GET_X_LPARAM(lparam);
            event.motion.y = GET_Y_LPARAM(lparam);
            event.motion.buttons = (uint32_t)wparam};
        } break;

        case WM_MOUSELEAVE:
            ClipCursor(NULL); // we unclip the cursor to the window in case it was clipped before.
            // Mouse just left the window
            break;
        case WM_WINDOWPOSCHANGED:
        case WM_WINDOWPOSCHANGING:
            pos = (WINDOWPOS*)lparam;
            event.type = PAL_EVENT_WINDOW_MOVED;
            event.window.event_code = msg;
            event.window.x = pos->x;
            event.window.y = pos->y;
            event.window.width = pos->cx;
            event.window.height = pos->cy;
            event.window.focused = 1; // This is wrong; fixevent.window.
            event.window.visible = 1;
            break;

        case WM_INPUT: {
            win32_handle_raw_input((HRAWINPUT)lparam);
            event.type = PAL_EVENT_SENSOR_UPDATE;
            event.sensor.device_id = 0;
            event.sensor.x = 0;
            event.sensor.y = 0;
            event.sensor.z = 0;
            event.sensor.sensor_type = 0;
        }; break;

        case WM_DROPFILES: {
            hDrop = (HDROP)wparam;
            count = DragQueryFileW(hDrop, 0xFFFFFFFF, NULL, 0);
            paths = malloc(sizeof(char*) * count);
            for (UINT i = 0; i < count; ++i) {
                DragQueryFileW(hDrop, i, buffer, MAX_PATH);
                len = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, NULL, 0, NULL, NULL);
                utf8 = malloc(len);
                WideCharToMultiByte(CP_UTF8, 0, buffer, -1, utf8, len, NULL, NULL);
                paths[i] = utf8;
            }
            event.type = PAL_EVENT_DROP_FILE;
            event.drop.paths = paths;
            event.drop.count = count;
            DragFinish(hDrop);
            break;
        }

        case WM_ACTIVATEAPP: {
            event.window.event_code = WM_MOVE;
            event.window.x = LOWORD(lparam);
            event.window.y = HIWORD(lparam);
            event.window.width = 0;
            event.window.height = 0;
            event.window.visible = 1;
            if ((BOOL)wparam == FALSE) {
                event.type = PAL_EVENT_WINDOW_LOST_FOCUS;
                event.window.focused = 0;
                printf("PAL: Lost Focus!\n");
            } else {
                event.type = PAL_EVENT_WINDOW_GAINED_FOCUS;
                event.window.focused = 1;
                printf("PAL: Gained Focus!\n");
            }
        }; break;

            // TODO: Make this return a pal_event of some kind.
        case WM_INPUT_DEVICE_CHANGE: {
            win32_enumerate_keyboards();
            win32_enumerate_mice();
            win32_handle_device_change((HANDLE)lparam, (DWORD)wparam);
            printf("Device Changed!\n");
        }; break;

        default:
            event.type = PAL_EVENT_NONE;
            return DefWindowProcA(hwnd, msg, wparam, lparam);
    }
    pal__eventq_push(&g_event_queue, event);
    return 0;
}

static PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
PALAPI pal_window* pal_create_window(int width, int height, const char *window_title, uint64_t window_flags) {
    DWORD ext_window_style = 0;
    DWORD window_style = 0;

    if (window_flags & PAL_WINDOW_NOT_FOCUSABLE) {
        ext_window_style |= WS_EX_NOACTIVATE;
    }
    if (window_flags & PAL_WINDOW_ALWAYS_ON_TOP) {
        ext_window_style |= WS_EX_TOPMOST;
    }

    if (window_flags & PAL_WINDOW_UTILITY) {

        ext_window_style |= WS_EX_TOOLWINDOW;
        window_style |= WS_SYSMENU;
    } else if (window_flags & PAL_WINDOW_POPUP_MENU) {
        window_style |= WS_POPUPWINDOW;
    } else if (window_flags & PAL_WINDOW_TOOLTIP) {
        ext_window_style |= WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
    } else if (window_flags & PAL_WINDOW_RESIZABLE) {
        window_style |= WS_OVERLAPPEDWINDOW;

    } else {
        window_style |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
    }

    if (window_flags & PAL_WINDOW_BORDERLESS) {
        window_style = WS_POPUP;
    }

    if (window_flags & PAL_WINDOW_FULLSCREEN) {

        DEVMODE dm = {0};
        dm.dmSize = sizeof(dm);
        dm.dmPelsWidth = width;
        dm.dmPelsHeight = height;
        dm.dmBitsPerPel = 32;
        dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

        LONG result = ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
        if (result != DISP_CHANGE_SUCCESSFUL) {
            MessageBoxA(NULL, "Failed to make window fullscreen!", "Error", MB_OK);
        }
        window_style = WS_POPUP;
    }

    // these variables are only
    // used when initializing opengl.
    pal_window* fakewindow = NULL;
    HGLRC fakeRC = 0;
    PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
    // we default to opengl.
    if (!(window_flags & PAL_WINDOW_OPENGL) || !(window_flags & PAL_WINDOW_VULKAN) || !(window_flags & PAL_WINDOW_METAL)) {
        window_flags |= PAL_WINDOW_OPENGL;
    }

    if (window_flags & PAL_WINDOW_OPENGL) {
        fakewindow = (pal_window*)malloc(sizeof(pal_window));
        WNDCLASSEXA fakewc = {0};
        fakewc.cbSize = sizeof(WNDCLASSEXA);
        fakewc.lpfnWndProc = win32_fake_window_proc;
        fakewc.hInstance = GetModuleHandleA(0);
        fakewc.lpszClassName = "Win32 Fake Window Class";
        fakewc.hCursor = LoadCursorW(NULL, IDC_ARROW);

        RegisterClassExA(&fakewc);

        fakewindow->hwnd = CreateWindowExA(
            ext_window_style,     // Optional window styles.
            fakewc.lpszClassName, // Window class
            window_title,   // Window text
            window_style,  // Window style

            // Size and position
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,

            NULL,             // Parent window
            NULL,             // Menu
            fakewc.hInstance, // Instance handle
            NULL              // Additional application data
        );

        if (fakewindow->hwnd == NULL) {
            return fakewindow;
        }

        s_fakeDC = GetDC(fakewindow->hwnd);

        PIXELFORMATDESCRIPTOR fakePFD;
        ZeroMemory(&fakePFD, sizeof(fakePFD));
        fakePFD.nSize = sizeof(fakePFD);
        fakePFD.nVersion = 1;
        fakePFD.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        fakePFD.iPixelType = PFD_TYPE_RGBA;
        fakePFD.cColorBits = 32;
        fakePFD.cAlphaBits = 8;
        fakePFD.cDepthBits = 24;

        int fakePFDID = ChoosePixelFormat(s_fakeDC, &fakePFD);

        if (fakePFDID == 0) {
            MessageBoxA(fakewindow->hwnd, "ChoosePixelFormat() failed.", "Try again later", MB_ICONERROR);
            return fakewindow;
        }
        if (SetPixelFormat(s_fakeDC, fakePFDID, &fakePFD) == 0) {
            MessageBoxA(fakewindow->hwnd, "SetPixelFormat() failed.", "Try again later", MB_ICONERROR);
            return fakewindow;
        }

        fakeRC = wglCreateContext(s_fakeDC);
        if (fakeRC == 0) {
            MessageBoxA(fakewindow->hwnd, "wglCreateContext() failed.", "Try again later", MB_ICONERROR);
            return fakewindow;
        }
        if (wglMakeCurrent(s_fakeDC, fakeRC) == 0) {
            MessageBoxA(fakewindow->hwnd, "wglMakeCurrent() failed.", "Try again later", MB_ICONERROR);
            return fakewindow;
        }
        wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)(wglGetProcAddress("wglChoosePixelFormatARB"));
        if (wglChoosePixelFormatARB == NULL) {
            MessageBoxA(fakewindow->hwnd, "wglGetProcAddress() failed.", "Try again later", MB_ICONERROR);
            return fakewindow;
        }
        wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)(wglGetProcAddress("wglCreateContextAttribsARB"));
        if (wglCreateContextAttribsARB == NULL) {
            MessageBoxA(fakewindow->hwnd, "wglGetProcAddress() failed.", "Try again later", MB_ICONERROR);
            return fakewindow;
        }
        wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
        if (wglSwapIntervalEXT == NULL) {
            MessageBoxA(fakewindow->hwnd, "wglGetProcAddress() failed.", "Try again later", MB_ICONERROR);
            return fakewindow;
        }
    }

    WNDCLASSEXA wc = {0};

    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.lpfnWndProc = win32_window_proc;
    wc.hInstance = GetModuleHandleA(0);
    wc.lpszClassName = "Win32 Window Class";
    wc.hCursor = LoadCursorW(NULL, IDC_ARROW);

    RegisterClassExA(&wc);

    pal_window* window = (pal_window*)malloc(sizeof(pal_window));
    window->width = (float)width;
    window->height = (float)height;
    // -- CREATING EVENT QUEUE --
    // It's very important to set g_current_window to the pal_window
    // before we call CreateWindowExA() on window->hwnd, because if we don't,
    // windows will call win32_window_proc() as soon as the window is created,
    // and in that function, we rely on g_current_window for some things like
    // getting the event queue that belongs to the window, and if it's not set,
    // we will just crash. - Abdelrahman July 29, 2025
    g_current_window = window;
    // -- CREATING WINDOW --
    window->hwnd = CreateWindowExA(
        ext_window_style, // Optional window styles.
        wc.lpszClassName, // Window class
        window_title,     // Window text
        window_style,     // Window style

        // Size and position
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width,
        height,

        NULL,         // Parent window
        NULL,         // Menu
        wc.hInstance, // Instance handle
        NULL          // Additional application data
    );

    if (window->hwnd == NULL) {
        return window;
    }
    
    if (window_flags & PAL_WINDOW_OPENGL) {
		window->hdc = GetDC(window->hwnd);

		const int pixelAttribs[] = {
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE, WGL_SUPPORT_OPENGL_ARB, GL_TRUE, WGL_DOUBLE_BUFFER_ARB, GL_TRUE, WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB, WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB, WGL_COLOR_BITS_ARB, 32, WGL_ALPHA_BITS_ARB, 8, WGL_DEPTH_BITS_ARB, 24, WGL_STENCIL_BITS_ARB, 8, WGL_SAMPLE_BUFFERS_ARB, GL_TRUE, WGL_SAMPLES_ARB, 4, // NOTE: Maybe this is used for multisampling?
			0                                                                                                                                                                                                                                                                                                                                              // null terminator for attrib list.
		};

		int pixelFormatID;
		UINT numFormats;
		uint8_t status = wglChoosePixelFormatARB(window->hdc, pixelAttribs, NULL, 1, &pixelFormatID, &numFormats);
		if (status == 0 || numFormats == 0) {
			MessageBoxA(window->hwnd, "wglChoosePixelFormatARB() failed.", "Try again later", MB_ICONERROR);
			return window;
		}

		PIXELFORMATDESCRIPTOR PFD;
		PFD.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
		DescribePixelFormat(window->hdc, pixelFormatID, sizeof(PFD), &PFD);
		SetPixelFormat(window->hdc, pixelFormatID, &PFD);

		int contextAttribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3, WGL_CONTEXT_MINOR_VERSION_ARB, 3, WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB, WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB, 0};

		window->hglrc = wglCreateContextAttribsARB(window->hdc, 0, contextAttribs);
    }

    pal_window *final_window = NULL;
    if (window_flags & PAL_WINDOW_OPENGL) {
		if (window->hglrc) {
            final_window = window;
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(fakeRC);
			ReleaseDC(fakewindow->hwnd, s_fakeDC);
			DestroyWindow(fakewindow->hwnd);
			free(fakewindow);
		} else {
			// This is supposed to be a fallback in case we can't create the context that we want.
			// Ideally, this should never happen. - Abdelrahman june 13, 2024
            final_window = fakewindow;
            final_window->hglrc = fakeRC;
            final_window->hdc = s_fakeDC;
		}
    } else {
        final_window = window;
    }

    RAWINPUTDEVICE rid[3];

    // 1. Keyboard
    rid[0].usUsagePage = 0x01;                          // Generic desktop controls
    rid[0].usUsage = 0x06;                              // Keyboard
    rid[0].dwFlags = RIDEV_INPUTSINK | RIDEV_DEVNOTIFY; // Receive input even when not focused
    rid[0].hwndTarget = final_window->hwnd;

    // 2. Mouse
    rid[1].usUsagePage = 0x01; // Generic desktop controls
    rid[1].usUsage = 0x02;     // Mouse
    rid[1].dwFlags = RIDEV_INPUTSINK | RIDEV_DEVNOTIFY;
    rid[1].hwndTarget = final_window->hwnd;

    // 3. Joystick/Gamepad (Note: Not all controllers appear as HIDs)
    rid[2].usUsagePage = 0x01; // Generic desktop controls
    rid[2].usUsage = 0x04;     // Joystick
    rid[2].dwFlags = RIDEV_INPUTSINK | RIDEV_DEVNOTIFY;
    rid[2].hwndTarget = final_window->hwnd;

    if (!RegisterRawInputDevices(rid, 3, sizeof(RAWINPUTDEVICE))) {
        DWORD error = GetLastError();
        printf("RegisterRawInputDevices failed. Error code: %lu\n", error);
    }

	if (!(window_flags & PAL_WINDOW_HIDDEN)) {
		if (window_flags & PAL_WINDOW_FULLSCREEN) {
			ShowWindow(final_window->hwnd, SW_SHOW);
		} else if (window_flags & PAL_WINDOW_MAXIMIZED) {
			ShowWindow(final_window->hwnd, SW_SHOWMAXIMIZED);
		} else if (window_flags & PAL_WINDOW_MINIMIZED) {
			ShowWindow(final_window->hwnd, SW_SHOWMINIMIZED);
		} else {
			ShowWindow(final_window->hwnd, SW_SHOWNORMAL);
		}
	} else {
		ShowWindow(final_window->hwnd, SW_HIDE);
	}
	if (window_flags & PAL_WINDOW_MOUSE_CONFINED) {
		RECT rect;
		GetClientRect(final_window->hwnd, &rect);
		POINT tl = {rect.left, rect.top};
		POINT br = {rect.right, rect.bottom};
		ClientToScreen(final_window->hwnd, &tl);
		ClientToScreen(final_window->hwnd, &br);
		rect.left = tl.x;
		rect.top = tl.y;
		rect.right = br.x;
		rect.bottom = br.y;
		ClipCursor(&rect);
		final_window->confine_mouse = pal_true;
	} else {
		final_window->confine_mouse = pal_false;
	}
	SetForegroundWindow(final_window->hwnd);
	SetFocus(final_window->hwnd);
	// save the final_window style and the final_window rect in case the user sets the final_window to windowed before setting it to fullscreen.
	// The fullscreen function is supposed to save this state whenever the user calls it,
	// but if the user doesn't, the pal_make_window_windowed() function uses a state that's all zeroes,
	// so we have to save it here. - Abdelrahman june 13, 2024
	final_window->windowedStyle = GetWindowLongA(final_window->hwnd, GWL_STYLE); // style of the final_window.
	return final_window;
}

PALAPI void pal_close_window(pal_window *window)
{
    pal_event event = {0};
    if (!window || !window->hwnd)
        return;

    DestroyWindow(window->hwnd);
    window->hwnd = NULL;
    event.type = PAL_EVENT_WINDOW_CLOSED;
    pal__eventq_push(&g_event_queue, event);
}

PALAPI pal_ivec2 pal_get_window_border_size(pal_window* window) {
    RECT rect;
    HDC hdc;
    int dpi_x, dpi_y;
    float scale_x, scale_y;
    GetClientRect(window->hwnd, &rect);

    hdc = GetDC(window->hwnd);
    dpi_x = GetDeviceCaps(hdc, LOGPIXELSX);
    dpi_y = GetDeviceCaps(hdc, LOGPIXELSY);
    ReleaseDC(window->hwnd, hdc);

    // Convert logical pixels to physical pixels
    float scale_x = dpi_x / 96.0f;
    float scale_y = dpi_y / 96.0f;

    pal_ivec2 border_size;
    border_size.x = (int)((rect.right - rect.left) * scale_x);
    border_size.y = (int)((rect.bottom - rect.top) * scale_y);
    return border_size;
}

PALAPI void *pal_get_window_handle(pal_window *window) {
    return (void*)window->hwnd;
}

PALAPI int pal_make_context_current(pal_window* window) {
    if (!wglMakeCurrent(window->hdc, window->hglrc)) {
        MessageBoxA(window->hwnd, "wglMakeCurrent() failed.", "Try again later", MB_ICONERROR);
        return 1;
    }
    return 0;
}

PALAPI int pal_show_cursor(void) {
    int result = -1;
    while (result < 0) {
        result = ShowCursor(pal_true);
    }
    return result;
}

PALAPI int pal_hide_cursor(void) {
    int result = 1;
    while (result >= 0) {
        result = ShowCursor(FALSE);
    }
    return result;
}

PALAPI pal_bool pal_maximize_window(pal_window* window) {
    return ShowWindow(window->hwnd, SW_MAXIMIZE);
}

PALAPI pal_bool pal_minimize_window(pal_window* window) {
    return ShowWindow(window->hwnd, SW_MINIMIZE);
}

static int win32_get_raw_input_buffer(void);

PALAPI pal_bool pal_poll_events(pal_event* event) {
    MSG msg = {0};
    pal_event_queue* queue = &g_event_queue;

    if (!g_message_pump_drained) {
        pal__reset_mouse_deltas();
        win32_get_raw_input_buffer();
        while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE) != 0) {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
        g_message_pump_drained = pal_true;
    }

    if (queue->size) { // if queue is not empty,

        // peek
        *event = queue->events[queue->front];

        // dequeue
        queue->front = (queue->front + 1) % queue->capacity;
        queue->size--;
        return 1;
    } else {
        g_message_pump_drained = pal_false;
        return 0;
    }
}

PALAPI pal_bool pal_set_window_title(pal_window* window, const char* string) {
    return SetWindowTextA(window->hwnd, string);
}

PALAPI pal_monitor* pal_get_primary_monitor(void) {
    // The point (0, 0) is guaranteed to be on the primary monitor
    pal_monitor* monitor = malloc(sizeof(pal_monitor));
    POINT pt = {0, 0};
    monitor->handle = MonitorFromPoint(pt, MONITOR_DEFAULTTOPRIMARY);
    return monitor;
}

PALAPI pal_video_mode* pal_get_video_mode(pal_monitor* monitor) {
    MONITORINFOEX mi = {.cbSize = sizeof(MONITORINFOEX)};
    DEVMODE dm = {.dmSize = sizeof(DEVMODE)};
    pal_video_mode* mode;

    if (!GetMonitorInfo(monitor->handle, (MONITORINFO*)&mi))
        return 0;

    if (!EnumDisplaySettings(mi.szDevice, ENUM_CURRENT_SETTINGS, &dm))
        return 0;

    mode = (pal_video_mode*)malloc(sizeof(pal_video_mode));
    mode->width = dm.dmPelsWidth;
    mode->height = dm.dmPelsHeight;
    mode->refresh_rate = dm.dmDisplayFrequency;
    mode->bits_per_pixel = dm.dmBitsPerPel;

    return mode;
}

PALAPI pal_bool pal_set_video_mode(pal_video_mode* mode) {
    DEVMODEA dm = {.dmSize = sizeof(DEVMODEA)};
    if (mode == NULL) {
        if (ChangeDisplaySettingsA(NULL, 0)) {
            return 1;
        } else {
            return 0;
        }
    } else {
        dm.dmPelsWidth = mode->width;
        dm.dmPelsHeight = mode->height;
        dm.dmDisplayFrequency = mode->refresh_rate;
        dm.dmBitsPerPel = mode->bits_per_pixel;
        if (ChangeDisplaySettingsA(&dm, 0)) {
            return 1;
        } else {
            return 0;
        }
    }
}

PALAPI void* pal_gl_get_proc_address(const char* proc) {
    static HMODULE opengl_module = NULL; // Cached across all calls
    void* p = (void*)wglGetProcAddress(proc);

    if (p == NULL || p == (void*)0x1 || p == (void*)0x2 || p == (void*)0x3 || p == (void*)-1) {
        // Load opengl32.dll once on first call, reuse handle afterwards
        if (opengl_module == NULL) {
            opengl_module = LoadLibraryA("opengl32.dll");
        }

        if (opengl_module != NULL) {
            p = (void*)GetProcAddress(opengl_module, proc);
        } else {
            p = NULL;
        }
    }
    return p;
}

PALAPI void pal_swap_buffers(pal_window* window) {
    SwapBuffers(window->hdc);
}

PALAPI void pal_swap_interval(int interval) {
    wglSwapIntervalEXT(interval);
}

// Handler function signatures
typedef void (*RawInputHandler)(const RAWINPUT*);

PALAPI pal_vec2 pal_get_mouse_position(pal_window* window) {
    POINT cursor_pos = {0};
    pal_vec2 returned_pos = {0};

    GetCursorPos(&cursor_pos);

    ScreenToClient(window->hwnd, &cursor_pos); // Convert to client-area coordinates
    returned_pos.x = cursor_pos.x;
    returned_pos.x = cursor_pos.y;
    return returned_pos;
}

// Handles Gamepads, Joysticks, Steering wheels, etc...
void win32_handle_hid(const RAWINPUT* raw) {
    printf("%d", raw->data.hid.dwCount);
}

RawInputHandler Win32InputHandlers[3] = {
    win32_handle_mouse,
    win32_handle_keyboard,
    win32_handle_hid
};

#define RAW_INPUT_BUFFER_CAPACITY (64 * 1024) // 64 KB

static BYTE g_raw_input_buffer[RAW_INPUT_BUFFER_CAPACITY];

static int win32_get_raw_input_buffer(void) {
    UINT bufferSize = RAW_INPUT_BUFFER_CAPACITY;
    UINT inputEventCount = GetRawInputBuffer((PRAWINPUT)g_raw_input_buffer, &bufferSize, sizeof(RAWINPUTHEADER));
    PRAWINPUT raw = (PRAWINPUT)g_raw_input_buffer;
    UINT type = 0;

    for (UINT i = 0; i < inputEventCount; ++i) {
        type = raw->header.dwType;
        if (type == RIM_TYPEMOUSE) {
            win32_handle_mouse(raw);
        } else if (type == RIM_TYPEKEYBOARD) {
            win32_handle_keyboard(raw);
        } else {
            win32_handle_hid(raw);
        }
        raw = NEXTRAWINPUTBLOCK(raw);
    }
    return 0;
}

//----------------------------------------------------------------------------------
// File Functions.
//----------------------------------------------------------------------------------

// Helper function to convert UTF-8 to UTF-16
static wchar_t* win32_utf8_to_utf16(const char* utf8_str) {
    if (!utf8_str) return NULL;
    
    int len = MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1, NULL, 0);
    if (len == 0) return NULL;
    
    wchar_t* utf16_str = (wchar_t*)malloc(len * sizeof(wchar_t));
    if (!utf16_str) return NULL;
    
    if (MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1, utf16_str, len) == 0) {
        free(utf16_str);
        return NULL;
    }
    
    return utf16_str;
}

PALAPI pal_bool pal_does_file_exist(const char* file_path) {
    wchar_t* wide_path = win32_utf8_to_utf16(file_path);
    DWORD attrs;

    if (!wide_path) return 0;
    
    attrs = GetFileAttributesW(wide_path);
    free(wide_path);
    return (attrs != INVALID_FILE_ATTRIBUTES) && !(attrs & FILE_ATTRIBUTE_DIRECTORY);
}

PALAPI size_t pal_get_file_size(const char* file_path) {
    wchar_t* wide_path = win32_utf8_to_utf16(file_path);
    HANDLE file = NULL;
    LARGE_INTEGER file_size;

    if (!wide_path) return 0;
    
    file = CreateFileW(
        wide_path,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    
    free(wide_path);

    if (file == INVALID_HANDLE_VALUE) {
        return 0;
    }

    if (GetFileSizeEx(file, &file_size)) {
        CloseHandle(file);
        return file_size.QuadPart;
    }
    CloseHandle(file);
    return 0;
}

PALAPI size_t pal_get_last_write_time(const char* file) {
    wchar_t* wide_path = win32_utf8_to_utf16(file);
    WIN32_FILE_ATTRIBUTE_DATA fileInfo;

    if (!wide_path) return 0;
    
    if (!GetFileAttributesExW(wide_path, GetFileExInfoStandard, &fileInfo)) {
        free(wide_path);
        return 0;
    }
    free(wide_path);
    return ((uint64_t)fileInfo.ftLastWriteTime.dwHighDateTime << 32) |
           fileInfo.ftLastWriteTime.dwLowDateTime;
}

PALAPI size_t pal_get_last_read_time(const char* file) {
    wchar_t* wide_path = win32_utf8_to_utf16(file);
    WIN32_FILE_ATTRIBUTE_DATA fileInfo;

    if (!wide_path) return 0;
    
    if (!GetFileAttributesExW(wide_path, GetFileExInfoStandard, &fileInfo)) {
        free(wide_path);
        return 0;
    }
    free(wide_path);
    return ((uint64_t)fileInfo.ftLastAccessTime.dwHighDateTime << 32) |
           fileInfo.ftLastAccessTime.dwLowDateTime;
}

PALAPI uint32_t pal_get_file_permissions(const char* file_path) {
    wchar_t* wide_path = NULL;
    uint32_t permissions = 0;
    PACL pDacl = NULL;
    PSECURITY_DESCRIPTOR pSD = NULL;
    DWORD dwRes = 0;
    HANDLE hToken = NULL;
    GENERIC_MAPPING mapping = {0};

    PRIVILEGE_SET privileges = {0};
    DWORD privSize = sizeof(privileges);
    BOOL accessStatus = FALSE;
    ACCESS_MASK accessRights = 0;

    if (!file_path) {
        return 0;
    }

    wide_path = win32_utf8_to_utf16(file_path);
    if (!wide_path) return 0;


    dwRes = GetNamedSecurityInfoW(
        wide_path,
        SE_FILE_OBJECT,
        DACL_SECURITY_INFORMATION,
        NULL,
        NULL,
        &pDacl,
        NULL,
        &pSD);

    free(wide_path);

    if (dwRes != ERROR_SUCCESS) {
        if (pSD)
            LocalFree(pSD);
        return 0;
    }

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        LocalFree(pSD);
        return 0;
    }

    mapping.GenericRead = FILE_GENERIC_READ;
    mapping.GenericWrite = FILE_GENERIC_WRITE;
    mapping.GenericExecute = FILE_GENERIC_EXECUTE;
    mapping.GenericAll = FILE_ALL_ACCESS;

    dwRes = GetEffectiveRightsFromAclA(pDacl, NULL, &accessRights);

    if (dwRes == ERROR_SUCCESS) {
        if (accessRights & FILE_GENERIC_READ)
            permissions |= PAL_READ;
        if (accessRights & FILE_GENERIC_WRITE)
            permissions |= PAL_WRITE;
        if (accessRights & FILE_GENERIC_EXECUTE)
            permissions |= PAL_EXECUTE;
    }

    CloseHandle(hToken);
    LocalFree(pSD);

    return permissions;
}

PALAPI pal_bool pal_change_file_permissions(const char* file_path, uint32_t permission_flags) {
    wchar_t* wide_path = NULL;
    DWORD dwAccessRights = 0;
    HANDLE hToken;
    DWORD dwSize = 0;
    PTOKEN_USER pTokenUser = NULL;
    EXPLICIT_ACCESS_W ea = {0};
    PACL pOldDACL = NULL, pNewDACL = NULL;
    PSECURITY_DESCRIPTOR pSD = NULL;
    DWORD dwRes;

    if (!file_path) {
        return 0;
    }

    wide_path = win32_utf8_to_utf16(file_path);
    if (!wide_path) return 0;

    if (permission_flags & PAL_READ)
        dwAccessRights |= GENERIC_READ;
    if (permission_flags & PAL_WRITE)
        dwAccessRights |= GENERIC_WRITE;
    if (permission_flags & PAL_EXECUTE)
        dwAccessRights |= GENERIC_EXECUTE;

    if (dwAccessRights == 0) {
        free(wide_path);
        return 0;
    }

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        free(wide_path);
        return 0;
    }

    GetTokenInformation(hToken, TokenUser, NULL, 0, &dwSize);
    if (dwSize == 0) {
        CloseHandle(hToken);
        free(wide_path);
        return 0;
    }

    pTokenUser = (PTOKEN_USER)malloc(dwSize);
    if (!pTokenUser) {
        CloseHandle(hToken);
        free(wide_path);
        return 0;
    }

    if (!GetTokenInformation(hToken, TokenUser, pTokenUser, dwSize, &dwSize)) {
        free(pTokenUser);
        CloseHandle(hToken);
        free(wide_path);
        return 0;
    }

    CloseHandle(hToken);

    ea.grfAccessPermissions = dwAccessRights;
    ea.grfAccessMode = SET_ACCESS;
    ea.grfInheritance = NO_INHERITANCE;
    ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea.Trustee.TrusteeType = TRUSTEE_IS_USER;
    ea.Trustee.ptstrName = (LPWSTR)pTokenUser->User.Sid;


    dwRes = GetNamedSecurityInfoW(
        wide_path,
        SE_FILE_OBJECT,
        DACL_SECURITY_INFORMATION,
        NULL,
        NULL,
        &pOldDACL,
        NULL,
        &pSD);

    if (dwRes != ERROR_SUCCESS) {
        free(pTokenUser);
        free(wide_path);
        return 0;
    }

    dwRes = SetEntriesInAclW(1, &ea, pOldDACL, &pNewDACL);
    if (dwRes != ERROR_SUCCESS) {
        LocalFree(pSD);
        free(pTokenUser);
        free(wide_path);
        return 0;
    }

    dwRes = SetNamedSecurityInfoW(
        wide_path,
        SE_FILE_OBJECT,
        DACL_SECURITY_INFORMATION,
        NULL,
        NULL,
        pNewDACL,
        NULL);

    LocalFree(pNewDACL);
    LocalFree(pSD);
    free(pTokenUser);
    free(wide_path);

    return (dwRes == ERROR_SUCCESS) ? 1 : 0;
}

PALAPI unsigned char *pal_read_entire_file(const char *file_path, size_t *bytes_read) {
    wchar_t* wide_path = win32_utf8_to_utf16(file_path);
    HANDLE file = NULL;
    LARGE_INTEGER file_size;
    size_t total_size = 0;
    char *buffer = NULL;
    size_t total_read = 0;

    if (!wide_path) return NULL;
    
    file = CreateFileW(
        wide_path,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    free(wide_path);

    if (file == INVALID_HANDLE_VALUE) {
        return NULL;
    }

    if (!GetFileSizeEx(file, &file_size)) {
        CloseHandle(file);
        return NULL;
    }

    if (file_size.QuadPart > SIZE_MAX) {
        CloseHandle(file);
        return NULL;
    }

    total_size = (size_t)file_size.QuadPart;
    buffer = (char *)malloc(total_size + 1);
    if (!buffer) {
        CloseHandle(file);
        return NULL;
    }

    while (total_read < total_size) {
        DWORD chunk = (DWORD)((total_size - total_read > MAXDWORD) ? MAXDWORD : (total_size - total_read));
        DWORD read_now = 0;

        if (!ReadFile(file, buffer + total_read, chunk, &read_now, NULL)) {
            free(buffer);
            CloseHandle(file);
            return NULL;
        }

        if (read_now == 0) break;

        total_read += read_now;
    }
    buffer[total_size] = '\0';
    CloseHandle(file);

    if (bytes_read)
        *bytes_read = total_read;

    return (unsigned char*)buffer;
}

PALAPI pal_bool pal_write_file(const char* file_path, size_t file_size, char* buffer) {
    wchar_t* wide_path = win32_utf8_to_utf16(file_path);
    HANDLE file;
    size_t remaining = 0;
    char* current_pos = NULL;
    DWORD chunk = 0;
    DWORD bytes_written = 0;

    if (!wide_path) return 1;
    
    file = CreateFileW(
        wide_path,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    free(wide_path);

    if (file == INVALID_HANDLE_VALUE) {
        return 1;
    }

    remaining = file_size;
    current_pos = buffer;

    while (remaining > 0) {
        chunk = (remaining > MAXDWORD) ? MAXDWORD : (DWORD)remaining;

        if (!WriteFile(file, current_pos, chunk, &bytes_written, NULL) ||
            bytes_written != chunk) {
            CloseHandle(file);
            return 1;
        }

        remaining -= chunk;
        current_pos += chunk;
    }

    CloseHandle(file);
    return 0;
}

PALAPI pal_bool pal_copy_file(const char* original_path, const char* copy_path) {
    wchar_t* wide_original = win32_utf8_to_utf16(original_path);
    wchar_t* wide_copy = win32_utf8_to_utf16(copy_path);
    BOOL result;

    if (!wide_original || !wide_copy) {
        if (wide_original) free(wide_original);
        if (wide_copy) free(wide_copy);
        return 1;
    }
    
    result = CopyFileW(wide_original, wide_copy, FALSE);
    
    free(wide_original);
    free(wide_copy);
    
    return result ? 0 : 1;
}

PALAPI pal_file* pal_open_file(const char* file_path) {
    wchar_t* wide_path = win32_utf8_to_utf16(file_path);
    pal_file* file = NULL;

    if (!wide_path) return NULL;
    
    if (!file) {
        free(wide_path);
        return NULL;
    }
    
    file = CreateFileW(
        wide_path,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    free(wide_path);

    if (file == INVALID_HANDLE_VALUE) {
        free(file);
        return NULL;
    }

    return file;
}

PALAPI pal_bool pal_read_from_open_file(pal_file* file, size_t offset, size_t bytes_to_read, char* buffer) {
    LARGE_INTEGER file_offset = {0};
    size_t total_read = 0;
    DWORD to_read = 0;
    DWORD bytesRead = 0;
    BOOL success = 0;

    if (!file || file == INVALID_HANDLE_VALUE || !buffer) {
        return 0;
    }

    file_offset.QuadPart = (LONGLONG)offset;

    if (!SetFilePointerEx(file, file_offset, NULL, FILE_BEGIN)) {
        return 0;
    }

    while (total_read < bytes_to_read) {
        if ((DWORD)((bytes_to_read - total_read) > MAXDWORD)) {
            to_read = MAXDWORD;
        } else {
            to_read = (DWORD)(bytes_to_read - total_read);
        }
        success = ReadFile(file, buffer + total_read, to_read, &bytesRead, NULL);
        if (!success || bytesRead != to_read) {
            return 0;
        }
        total_read += bytesRead;
    }

    return 1;
}

PALAPI pal_bool pal_close_file(const unsigned char *file) {
    if(!file) return 0;
    free((void*)file);
    return 1;
}

PALAPI pal_bool pal_close_open_file(pal_file *file) {
    if (!file) return 0;
    if (!CloseHandle(file)) {
        return 0;
    }
    return 1;
}

//----------------------------------------------------------------------------------
// Directory Listing.
//----------------------------------------------------------------------------------
PALAPI pal_bool pal_path_is_dir(const char *path) {
    wchar_t *wide_path = win32_utf8_to_utf16(path);
    if (!wide_path)
        return pal_false;

    DWORD attrs = GetFileAttributesW(wide_path);
    free(wide_path);

    if (attrs == INVALID_FILE_ATTRIBUTES)
        return pal_false;

    return (attrs & FILE_ATTRIBUTE_DIRECTORY) ? pal_true : pal_false;
}

//----------------------------------------------------------------------------------
// Random Number Generator.
//----------------------------------------------------------------------------------

PALAPI void pal_srand(uint64_t* state, uint64_t seed) {
    if (seed == 0) {
        seed = 1; // Avoid zero state which would produce all zeros
    }
    *state = seed;
}

PALAPI uint32_t pal_rand(uint64_t* state) {
    // SDL's well-tested LCG constants:
    // - Multiplier: 0xff1cd035 (32-bit for better performance on 32-bit archs)
    // - Increment: 0x05 (small odd number, generates smaller ARM code)
    // - These constants passed extensive testing with PractRand and TestU01
    *state = *state * 0xff1cd035ul + 0x05;

    // Return upper 32 bits - they have better statistical properties
    // and longer period than lower bits in an LCG
    return (uint32_t)(*state >> 32);
}

//----------------------------------------------------------------------------------
// Sound Functions.
//----------------------------------------------------------------------------------
int win32_init_sound(void) {
    int hr;

    // Initialize COM (needed for XAudio2)
    hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr)) {
        return hr;
    }

    // Initialize XAudio2 engine
    hr = XAudio2Create(&g_xaudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
    if (FAILED(hr)) {
        return hr;
    }

    // Create mastering voice
    hr = g_xaudio2->lpVtbl->CreateMasteringVoice(g_xaudio2, &g_mastering_voice, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, NULL, NULL, AudioCategory_GameEffects);

    return hr;
}

// XAudio2 callback for streaming
typedef struct {
    IXAudio2VoiceCallbackVtbl* lpVtbl;
    pal_sound* sound;
} StreamingVoiceCallback;

static size_t calculate_buffer_size_for_seconds(pal_sound* sound, float seconds) {
    // bytes_per_second = sample_rate * channels * (bits_per_sample / 8)
    size_t bytes_per_second = sound->sample_rate * sound->channels * (sound->bits_per_sample / 8);
    return (size_t)(bytes_per_second * seconds);
}

static size_t load_next_chunk(pal_sound* sound, unsigned char* buffer, size_t buffer_size) {
    size_t bytes_read = 0;
    size_t remaining = 0;
    size_t to_read = 0;
    size_t seek_pos = 0;

    if (sound->source_file) {
        remaining = sound->total_data_size - sound->bytes_streamed;

        if (remaining == 0) {
            printf("WAV: No remaining data\n");
            return 0;
        }

        if (!sound->source_file) {
            printf("ERROR: WAV file handle is NULL!\n");
            return 0;
        }

        to_read = (buffer_size < remaining) ? buffer_size : remaining;
        seek_pos = sound->data_offset + sound->bytes_streamed;

        printf("WAV: Seeking to %zu (data_offset=%zu + bytes_streamed=%zu), reading %zu bytes\n",
               seek_pos,
               sound->data_offset,
               sound->bytes_streamed,
               to_read);

        if (fseek(sound->source_file, (long)seek_pos, SEEK_SET) != 0) {
            printf("WAV: Seek failed to position %zu\n", seek_pos);
            return 0;
        }

        bytes_read = fread(buffer, 1, to_read, sound->source_file);

        if (bytes_read != to_read) {
            printf("WAV: Read %zu bytes but expected %zu, feof=%d, ferror=%d\n",
                   bytes_read,
                   to_read,
                   feof(sound->source_file),
                   ferror(sound->source_file));
        }

        sound->bytes_streamed += bytes_read;
        printf("WAV: Read %zu bytes, new bytes_streamed=%zu\n", bytes_read, sound->bytes_streamed);

    } else if (sound->decoder) {
        stb_vorbis* vorbis = (stb_vorbis*)sound->decoder;

        // Calculate how many sample frames we can fit in the buffer
        size_t bytes_per_sample_frame = sound->channels * sizeof(short);
        size_t max_sample_frames = buffer_size / bytes_per_sample_frame;

        if (max_sample_frames <= 0) {
            printf("OGG: Buffer too small for samples\n");
            return 0;
        }

        // Calculate how many sample frames we should skip
        size_t sample_frames_streamed = sound->bytes_streamed / bytes_per_sample_frame;
        int current_decoder_pos = stb_vorbis_get_sample_offset(vorbis);

        printf("OGG: Loading chunk - streamed %zu sample frames, decoder at %d\n",
               sample_frames_streamed,
               current_decoder_pos);

        // If decoder is behind our streaming position, seek forward
        if (current_decoder_pos < (int)sample_frames_streamed) {
            printf("OGG: Seeking decoder from %d to %zu\n", current_decoder_pos, sample_frames_streamed);
            if (stb_vorbis_seek(vorbis, (unsigned int)sample_frames_streamed)) {
                current_decoder_pos = stb_vorbis_get_sample_offset(vorbis);
                printf("OGG: Seek successful, decoder now at %u\n", current_decoder_pos);
            } else {
                printf("OGG: Seek failed!\n");
                return 0;
            }
        }

        // Allocate temporary float buffers for non-interleaved data
        float** channel_buffers = (float**)malloc(sound->channels * sizeof(float*));
        for (int i = 0; i < sound->channels; i++) {
            channel_buffers[i] = (float*)malloc(max_sample_frames * sizeof(float));
        }

        // Read using non-interleaved API
        int total_sample_frames_read = stb_vorbis_get_samples_float(
            vorbis, sound->channels, channel_buffers, (int)max_sample_frames);

        if (total_sample_frames_read > 0) {
            // Convert float samples to interleaved 16-bit shorts
            short* output_ptr = (short*)buffer;

            for (int sample = 0; sample < total_sample_frames_read; sample++) {
                for (int ch = 0; ch < sound->channels; ch++) {
                    float f_sample = channel_buffers[ch][sample];
                    // Clamp and convert to 16-bit
                    if (f_sample > 1.0f)
                        f_sample = 1.0f;
                    if (f_sample < -1.0f)
                        f_sample = -1.0f;
                    short s_sample = (short)(f_sample * 32767.0f);
                    output_ptr[sample * sound->channels + ch] = s_sample;
                }
            }
        }

        // Clean up temporary buffers
        for (int i = 0; i < sound->channels; i++) {
            free(channel_buffers[i]);
        }
        free(channel_buffers);

        bytes_read = total_sample_frames_read * bytes_per_sample_frame;

        // Update bytes_streamed to track total bytes processed
        sound->bytes_streamed += bytes_read;

        int after_sample = stb_vorbis_get_sample_offset(vorbis);

        printf("OGG: Read %d sample frames (%zu bytes)\n", total_sample_frames_read, bytes_read);
        printf("OGG: Decoder position: %d -> %u (advanced %d samples)\n",
               current_decoder_pos,
               after_sample,
               after_sample - current_decoder_pos);
        printf("OGG: Total bytes streamed: %zu\n", sound->bytes_streamed);

        // Check if we're at the end
        if (total_sample_frames_read == 0) {
            unsigned int total_samples = stb_vorbis_stream_length_in_samples(vorbis);
            printf("OGG: End of stream - position %d of %u total samples\n", after_sample, total_samples);
        }
    }

    return bytes_read;
}

static void STDMETHODCALLTYPE OnBufferEnd(IXAudio2VoiceCallback* callback, void* pBufferContext) {
    StreamingVoiceCallback* cb = (StreamingVoiceCallback*)callback;
    pal_sound* sound = cb->sound;
    static int buffer_end_count = 0;
    XAUDIO2_VOICE_STATE state;

    printf("OnBufferEnd %d: buffer=%p\n", buffer_end_count++, pBufferContext);

    // Free the buffer that just finished playing
    // pBufferContext contains the buffer pointer we set in buffer.pContext
    if (pBufferContext) {
        free(pBufferContext);
    }

    if (!sound->is_streaming || sound->stream_finished) {
        return;
    }

    // Check how many buffers are queued
    sound->source_voice->lpVtbl->GetState(sound->source_voice, &state, 0);

    // If we have fewer than 2 buffers queued, queue another one
    if (state.BuffersQueued < 2) {
        float chunk_seconds = sound->preload_seconds;
        size_t buffer_chunk_size = calculate_buffer_size_for_seconds(sound, chunk_seconds);

        unsigned char* chunk_buffer = (unsigned char*)malloc(buffer_chunk_size);

        if (chunk_buffer) {
            size_t bytes_read = load_next_chunk(sound, chunk_buffer, buffer_chunk_size);

            if (bytes_read > 0) {
                XAUDIO2_BUFFER buffer = {0};
                buffer.AudioBytes = (UINT32)bytes_read;
                buffer.pAudioData = chunk_buffer;
                buffer.pContext = chunk_buffer; // For OnBufferEnd to free

                HRESULT hr = sound->source_voice->lpVtbl->SubmitSourceBuffer(
                    sound->source_voice, &buffer, NULL);

                if (FAILED(hr)) {
                    printf("ERROR: Failed to submit buffer in OnBufferEnd: 0x%08X\n", hr);
                    free(chunk_buffer);
                } else {
                    printf("Queued next buffer: %u bytes\n", buffer.AudioBytes);
                }
            } else {
                // End of stream reached
                free(chunk_buffer);
                sound->stream_finished = 1;
                printf("Stream finished - no more data\n");
            }
        }
    }
}

static void STDMETHODCALLTYPE OnVoiceProcessingPassEnd(IXAudio2VoiceCallback* callback) {
    // OnBufferEnd now handles buffer queuing, so this can be much simpler
    StreamingVoiceCallback* cb = (StreamingVoiceCallback*)callback;
    pal_sound* sound = cb->sound;
    XAUDIO2_VOICE_STATE state;
    static int callback_count = 0;

    if (!sound->is_streaming || sound->stream_finished) {
        return;
    }

    // Just log state for debugging
    sound->source_voice->lpVtbl->GetState(sound->source_voice, &state, 0);

    if (callback_count % 100 == 0) { // Log less frequently
        printf("ProcessingPass %d: BuffersQueued=%u, SamplesPlayed=%llu\n",
               callback_count,
               state.BuffersQueued,
               state.SamplesPlayed);
    }
    callback_count++;
}

static void STDMETHODCALLTYPE OnBufferStart(IXAudio2VoiceCallback* callback, void* pBufferContext) {
    // Called when XAudio2 starts processing a buffer
    // pBufferContext contains the buffer we passed in SubmitSourceBuffer
    // For streaming, we don't need to do anything special here
}

static void STDMETHODCALLTYPE OnLoopEnd(IXAudio2VoiceCallback* callback, void* pBufferContext) {
    // Called when a buffer with XAUDIO2_LOOP_INFINITE completes a loop
    // We don't use looping buffers in streaming, so this stays empty
}

static void STDMETHODCALLTYPE OnVoiceError(IXAudio2VoiceCallback* callback, void* pBufferContext, HRESULT error) {
    // Called when XAudio2 encounters an error
    StreamingVoiceCallback* cb = (StreamingVoiceCallback*)callback;
    pal_sound* sound = cb->sound;

    printf("XAudio2 Voice Error: 0x%08X\n", error);

    // Stop streaming on error
    sound->stream_finished = 1;
}

static void STDMETHODCALLTYPE OnVoiceProcessingPassStart(IXAudio2VoiceCallback* callback, UINT32 BytesRequired) {
    // Called when XAudio2 starts processing audio for this voice
    // BytesRequired tells us how much data XAudio2 needs
    StreamingVoiceCallback* cb = (StreamingVoiceCallback*)callback;
    pal_sound* sound = cb->sound;

    // We can use BytesRequired to be more intelligent about buffer management
    // For now, we'll handle this in OnBufferEnd instead
}

static void STDMETHODCALLTYPE OnStreamEnd(IXAudio2VoiceCallback* callback) {
    // Called when the last buffer with XAUDIO2_END_OF_STREAM finishes playing
    StreamingVoiceCallback* cb = (StreamingVoiceCallback*)callback;
    pal_sound* sound = cb->sound;

    printf("Audio stream ended\n");

    // Mark stream as finished
    sound->stream_finished = 1;

    // You could trigger a callback here to notify your game that the music finished
    // or automatically start the next track in a playlist
}

static IXAudio2VoiceCallbackVtbl StreamingCallbackVtbl = {
    OnVoiceProcessingPassStart,
    OnVoiceProcessingPassEnd,
    OnStreamEnd,
    OnBufferStart,
    OnBufferEnd,
    OnLoopEnd,
    OnVoiceError,
};

PALAPI int pal_play_music(pal_sound* sound, float volume) {
    XAUDIO2_BUFFER buffer = {0};

    if (!g_xaudio2 || !g_mastering_voice) {
        printf("ERROR: XAudio2 not initialized\n");
        return E_FAIL;
    }

    printf("Playing sound: streaming=%s, voice_callback=%p\n",
           sound->is_streaming ? "YES" : "NO",
           sound->voice_callback);

    // Set volume
    sound->source_voice->lpVtbl->SetVolume(sound->source_voice, volume, 0);

    // Submit initial buffer (the preloaded data)
    buffer.AudioBytes = (UINT32)sound->data_size;
    buffer.pAudioData = sound->data;
    buffer.pContext = NULL; // Don't free this buffer - it's owned by the sound object

    // For non-streaming sounds, set end of stream flag
    // For streaming sounds, let the callback handle subsequent buffers
    buffer.Flags = sound->is_streaming ? 0 : XAUDIO2_END_OF_STREAM;

    float initial_seconds = (float)sound->data_size /
                            (sound->sample_rate * sound->channels * (sound->bits_per_sample / 8));

    printf("Submitting initial buffer: %u bytes (%.3f seconds), flags=0x%x\n",
           buffer.AudioBytes,
           initial_seconds,
           buffer.Flags);

    HRESULT hr = sound->source_voice->lpVtbl->SubmitSourceBuffer(sound->source_voice, &buffer, NULL);
    if (FAILED(hr)) {
        printf("ERROR: Failed to submit source buffer: 0x%08X\n", hr);
        return hr;
    }

    // IMPROVEMENT: For streaming sounds, queue an additional buffer immediately
    if (sound->is_streaming && !sound->stream_finished) {
        float chunk_seconds = sound->preload_seconds;
        size_t buffer_chunk_size = calculate_buffer_size_for_seconds(sound, chunk_seconds);

        unsigned char* chunk_buffer = (unsigned char*)malloc(buffer_chunk_size);
        if (chunk_buffer) {
            size_t bytes_read = load_next_chunk(sound, chunk_buffer, buffer_chunk_size);

            if (bytes_read > 0) {
                XAUDIO2_BUFFER next_buffer = {0};
                next_buffer.AudioBytes = (UINT32)bytes_read;
                next_buffer.pAudioData = chunk_buffer;
                next_buffer.pContext = chunk_buffer; // For OnBufferEnd to free

                hr = sound->source_voice->lpVtbl->SubmitSourceBuffer(sound->source_voice, &next_buffer, NULL);
                if (FAILED(hr)) {
                    printf("ERROR: Failed to submit second buffer: 0x%08X\n", hr);
                    free(chunk_buffer);
                } else {
                    printf("Queued second buffer: %u bytes\n", next_buffer.AudioBytes);
                }
            } else {
                free(chunk_buffer);
                // If we can't load more data immediately, that's fine for short files
            }
        }
    }

    // Start playback
    hr = sound->source_voice->lpVtbl->Start(sound->source_voice, 0, 0);
    if (FAILED(hr)) {
        printf("ERROR: Failed to start voice: 0x%08X\n", hr);
        return hr;
    }

    printf("Playback started successfully\n");
    return S_OK;
}

static int pal__load_wav(const char* filename, pal_sound* out, float seconds);
static int pal__load_ogg(const char* filename, pal_sound* out, float seconds);

pal_sound* win32_load_sound(const char* filename, float seconds);
PALAPI pal_sound* pal_load_music(const char* filename) {
    // every loaded buffer will be this long.
    const float buffer_length_in_seconds = 2.0f;
    return win32_load_sound(filename, buffer_length_in_seconds);
}

PALAPI pal_sound* pal_load_sound(const char* filename) {
    return win32_load_sound(filename, 0.0f);
}

pal_sound* win32_load_sound(const char* filename, float seconds) {
    FILE* file = fopen(filename, "rb");
    char header[12];
    int result = 0;

    static const GUID KSDATAFORMAT_SUBTYPE_PCM = {
        0x00000001, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}};

    static const GUID KSDATAFORMAT_SUBTYPE_IEEE_FLOAT = {
        0x00000003, 0x0000, 0x0010, {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71}};

    WAVEFORMATEXTENSIBLE wfex = {0};

    if (!file)
        return NULL;

    if (fread(header, 1, sizeof(header), file) < 12) {
        fclose(file);
        return NULL;
    }

    pal_sound* sound = (pal_sound*)malloc(sizeof(pal_sound));
    if (!sound) {
        fclose(file);
        printf("ERROR: %s(): Failed to allocate memory for sound!\n", __func__);
        return NULL;
    }
    *sound = (pal_sound){0};

    if (pal_memcmp(header, "RIFF", 4) == 0 && pal_memcmp(header + 8, "WAVE", 4) == 0) {
        fclose(file);
        result = pal__load_wav(filename, sound, seconds);
    } else if (pal_memcmp(header, "OggS", 4) == 0) {
        fclose(file);
        result = pal__load_ogg(filename, sound, seconds);
    } else {
        fclose(file);
        free(sound);
        return NULL; // unsupported format
    }

    // we expect pal__load_wav and pal__load_ogg to return 1 for success.
    if (result != 1) {
        free(sound);
        return NULL;
    }

    wfex.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
    wfex.Format.nChannels = sound->channels;
    wfex.Format.nSamplesPerSec = sound->sample_rate;
    wfex.Format.wBitsPerSample = sound->bits_per_sample;
    wfex.Format.nBlockAlign = (wfex.Format.nChannels * wfex.Format.wBitsPerSample) / 8;
    wfex.Format.nAvgBytesPerSec = wfex.Format.nSamplesPerSec * wfex.Format.nBlockAlign;
    wfex.Format.cbSize = 22;
    wfex.Samples.wValidBitsPerSample = (uint16_t)sound->bits_per_sample;

    wfex.SubFormat = (sound->is_float)
                         ? KSDATAFORMAT_SUBTYPE_IEEE_FLOAT
                         : KSDATAFORMAT_SUBTYPE_PCM;

    switch (sound->channels) {
        case 1:
            wfex.dwChannelMask = SPEAKER_FRONT_CENTER;
            break;
        case 2:
            wfex.dwChannelMask = SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT;
            break;
        case 4:
            wfex.dwChannelMask = SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT |
                                 SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT;
            break;
        case 6:
            wfex.dwChannelMask = SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT |
                                 SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY |
                                 SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT;
            break;
        case 8:
            wfex.dwChannelMask = SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT |
                                 SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY |
                                 SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT |
                                 SPEAKER_SIDE_LEFT | SPEAKER_SIDE_RIGHT;
            break;
        default:
            wfex.dwChannelMask = 0;
            break;
    }

    sound->source_voice = NULL;

    // Create streaming callback if this is a streaming sound
    if (seconds > 0.0f) {
        StreamingVoiceCallback* callback = (StreamingVoiceCallback*)malloc(sizeof(StreamingVoiceCallback));
        if (callback) {
            callback->lpVtbl = &StreamingCallbackVtbl;
            callback->sound = sound;
            sound->voice_callback = (IXAudio2VoiceCallback*)callback;
        }
        sound->preload_seconds = seconds;
        sound->is_streaming = 1;

        if (sound->source_file) {
            // WAV streaming setup
            long current_pos = ftell(sound->source_file);
            fseek(sound->source_file, 0, SEEK_END);
            long file_end = ftell(sound->source_file);
            fseek(sound->source_file, current_pos, SEEK_SET);

            sound->total_data_size = file_end - sound->data_offset;
            sound->bytes_streamed = sound->data_size;

            printf("WAV streaming setup: total_size=%zu bytes, preloaded=%zu bytes\n",
                   sound->total_data_size,
                   sound->data_size);

        } else if (sound->decoder) {
            stb_vorbis* vorbis = (stb_vorbis*)sound->decoder;

            unsigned int total_sample_frames = stb_vorbis_stream_length_in_samples(vorbis);
            size_t bytes_per_sample_frame = sound->channels * sizeof(short);

            printf("OGG streaming setup - CONTINUOUS APPROACH:\n");
            printf("  - Total sample frames: %u\n", total_sample_frames);
            printf("  - Preloaded data size: %zu bytes\n", sound->data_size);

            sound->total_data_size = total_sample_frames * bytes_per_sample_frame;

            // Set bytes_streamed to match what we've already preloaded
            sound->bytes_streamed = sound->data_size;

            printf("  - Total estimated size: %zu bytes\n", sound->total_data_size);
            printf("  - Bytes already streamed (preloaded): %zu\n", sound->bytes_streamed);

            // Store filename for reopening decoder if needed
            size_t filename_len = pal_strlen(filename);
            sound->filename = (char*)malloc(filename_len + 1);
            pal_strcpy(sound->filename, filename);
        }
    } else {
        sound->preload_seconds = 0;
        sound->is_streaming = 0;
        sound->bytes_streamed = 0;
        sound->total_data_size = 0;
    }

    HRESULT hr = g_xaudio2->lpVtbl->CreateSourceVoice(
        g_xaudio2, &sound->source_voice, (const WAVEFORMATEX*)&wfex, 0, XAUDIO2_DEFAULT_FREQ_RATIO, sound->voice_callback, NULL, NULL);

    if (FAILED(hr)) {
        if (sound->voice_callback)
            free(sound->voice_callback);
        free(sound);
        return NULL;
    }

    return sound;
}

PALAPI void pal_free_music(pal_sound* sound) {
    if (sound->is_streaming) {
        sound->stream_finished = 1;
    }

    if (sound->source_file) {
        fclose(sound->source_file);
    }

    if (sound->decoder) {
        stb_vorbis_close((stb_vorbis*)sound->decoder);
    }

    if (sound->source_voice) {
        sound->source_voice->lpVtbl->DestroyVoice(sound->source_voice);
    }

    if (sound->voice_callback) {
        free(sound->voice_callback);
    }

    if (sound->filename) {
        free(sound->filename);
    }
    free(sound->data);
    free(sound);
}

PALAPI int pal_play_sound(pal_sound* sound, float volume) {
    if (!g_xaudio2 || !g_mastering_voice) {
        return E_FAIL;
    }
    
    if (sound == NULL) return S_FALSE;

    // Set the volume
    sound->source_voice->lpVtbl->SetVolume(sound->source_voice, volume, 0);

    XAUDIO2_BUFFER buffer = {
        .AudioBytes = (UINT32)sound->data_size,
        .pAudioData = sound->data,
        .Flags = XAUDIO2_END_OF_STREAM};

    HRESULT hr;

    hr = sound->source_voice->lpVtbl->SubmitSourceBuffer(sound->source_voice, &buffer, NULL);
    if (FAILED(hr)) {
        sound->source_voice->lpVtbl->DestroyVoice(sound->source_voice);
        return hr;
    }

    hr = sound->source_voice->lpVtbl->Start(sound->source_voice, 0, 0);
    if (FAILED(hr)) {
        sound->source_voice->lpVtbl->DestroyVoice(sound->source_voice);
        return hr;
    }

    return S_OK;
}

PALAPI void pal_free_sound(pal_sound* sound) {
    if (sound) {
        if (sound->source_voice) {
            sound->source_voice->lpVtbl->DestroyVoice(sound->source_voice);
            printf("ERROR: %s(): Source voice was not initialized. Was the sound played before? If not, you have to play it before destroying!\n", __func__);
        }
        if (sound->data) {
            free(sound->data);
            sound->data = NULL;
        } else {
            printf("ERROR: %s(): Pointer to data in pal_sound was null. Not going to free. Was the sound loaded?\n", __func__);
        }
        free(sound);
    } else {
        printf("ERROR: %s(): Pointer to pal_sound was null. Not going to free. Was it allocated to begin with?\n", __func__);
    }
}

PALAPI int pal_stop_sound(pal_sound* sound) {
    HRESULT hr = 0;
    hr = sound->source_voice->lpVtbl->Stop(sound->source_voice, 0, XAUDIO2_COMMIT_NOW);

    if (FAILED(hr)) {
        sound->source_voice->lpVtbl->DestroyVoice(sound->source_voice);
        return hr;
    }

    hr = sound->source_voice->lpVtbl->FlushSourceBuffers(sound->source_voice);

    if (FAILED(hr)) {
        sound->source_voice->lpVtbl->DestroyVoice(sound->source_voice);
        return hr;
    }
    return hr;
}

//----------------------------------------------------------------------------------
// Time Functions.
//----------------------------------------------------------------------------------
typedef struct _KSYSTEM_TIME {
    ULONG LowPart;  // Low 32 bits of the 64-bit time value
    LONG High1Time; // High 32 bits (first copy)
    LONG High2Time; // High 32 bits (second copy)
} KSYSTEM_TIME, *PKSYSTEM_TIME;

typedef struct _KUSER_SHARED_DATA {
    ULONG TickCountLowDeprecated;
    ULONG TickCountMultiplier;
    KSYSTEM_TIME InterruptTime;
    KSYSTEM_TIME SystemTime;
    KSYSTEM_TIME TimeZoneBias;
    // padding to get to right offsets.
    UCHAR Padding0[0x300 - 0x20];
    LONGLONG QpcFrequency; // Performance Counter Frequency at offset 0x300
    // padding to get to TickCount
    UCHAR Padding1[0x320 - 0x308];
    union {
        KSYSTEM_TIME TickCount;
        UINT64 TickCountQuad;
    };
} KUSER_SHARED_DATA, *PKUSER_SHARED_DATA;
#define KUSER_SHARED_DATA_ADDRESS 0x7FFE0000
static uint64_t g_app_start_time = 0;

PALAPI pal_time pal_get_date_and_time_utc(void) {
    PKUSER_SHARED_DATA kuser = (PKUSER_SHARED_DATA)KUSER_SHARED_DATA_ADDRESS;
    LARGE_INTEGER time = {0};
    do {
        time.HighPart = kuser->SystemTime.High1Time;
        time.LowPart = kuser->SystemTime.LowPart;
    } while (time.HighPart != kuser->SystemTime.High2Time);

    uint64_t total_100ns = time.QuadPart;
    uint64_t total_days = total_100ns / (10000000ULL * 60 * 60 * 24); // 100ns to days
    uint64_t remaining_100ns = total_100ns % (10000000ULL * 60 * 60 * 24);

    uint32_t year = 1601 + (uint32_t)(total_days / 365.25);

    uint64_t days_since_1601 = total_days;
    year = 1601;
    while (1) {
        uint32_t days_in_year = 365;
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            days_in_year = 366;
        }

        if (days_since_1601 < days_in_year)
            break;
        days_since_1601 -= days_in_year;
        year++;
    }

    uint32_t days_in_months[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        days_in_months[1] = 29;
    }

    uint32_t month = 1;
    while (month <= 12 && days_since_1601 >= days_in_months[month - 1]) {
        days_since_1601 -= days_in_months[month - 1];
        month++;
    }

    uint32_t day = (uint32_t)days_since_1601 + 1;

    uint64_t total_seconds = remaining_100ns / 10000000ULL;
    uint32_t hours = (uint32_t)(total_seconds / 3600);
    total_seconds %= 3600;
    uint32_t minutes = (uint32_t)(total_seconds / 60);
    uint32_t seconds = (uint32_t)(total_seconds % 60);

    pal_time result = {0};
    result.year = year;
    result.month = month;
    result.day = day;
    result.weeks = 0; // Unused for system time
    result.hours = hours;
    result.minutes = minutes;
    result.seconds = seconds;

    return result;
}

PALAPI pal_time pal_get_date_and_time_local(void) {
    PKUSER_SHARED_DATA kuser = (PKUSER_SHARED_DATA)KUSER_SHARED_DATA_ADDRESS;
    LARGE_INTEGER system_time = {0};
    LARGE_INTEGER timezone_bias = {0};
    uint64_t local_time_100ns = 0;
    uint64_t total_days = 0;
    uint64_t remaining_100ns = 0;
    uint64_t days_since_1601 = 0;
    uint32_t days_in_months[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    uint32_t days_in_year = 365;
    uint64_t total_seconds = 0;
    uint32_t year = 0, month = 1, day = 0, hours = 0, minutes = 0, seconds = 0;

    do {
        system_time.HighPart = kuser->SystemTime.High1Time;
        system_time.LowPart = kuser->SystemTime.LowPart;
    } while (system_time.HighPart != kuser->SystemTime.High2Time);

    do {
        timezone_bias.HighPart = kuser->TimeZoneBias.High1Time;
        timezone_bias.LowPart = kuser->TimeZoneBias.LowPart;
    } while (timezone_bias.HighPart != kuser->TimeZoneBias.High2Time);

    local_time_100ns = system_time.QuadPart - timezone_bias.QuadPart;

    total_days = local_time_100ns / (10000000ULL * 60 * 60 * 24); // 100ns to days
    remaining_100ns = local_time_100ns % (10000000ULL * 60 * 60 * 24);

    year = 1601 + (uint32_t)(total_days / 365.25);

    days_since_1601 = total_days;
    year = 1601;
    while (1) {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            days_in_year = 366;
        }

        if (days_since_1601 < days_in_year)
            break;
        days_since_1601 -= days_in_year;
        year++;
    }


    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        days_in_months[1] = 29;
    }

    while (month <= 12 && days_since_1601 >= days_in_months[month - 1]) {
        days_since_1601 -= days_in_months[month - 1];
        month++;
    }

    day = (uint32_t)days_since_1601 + 1;

    total_seconds = remaining_100ns / 10000000ULL;
    hours = (uint32_t)(total_seconds / 3600);
    total_seconds %= 3600;
    minutes = (uint32_t)(total_seconds / 60);
    seconds = (uint32_t)(total_seconds % 60);

    pal_time result = {0};
    result.year = year;
    result.month = month;
    result.day = day;
    result.weeks = 0; // Unused for system time
    result.hours = hours;
    result.minutes = minutes;
    result.seconds = seconds;

    return result;
}

PALAPI pal_time pal_get_time_since_boot(void) {
    PKUSER_SHARED_DATA kuser = (PKUSER_SHARED_DATA)KUSER_SHARED_DATA_ADDRESS;
    LARGE_INTEGER time = {0};

    do {
        time.HighPart = kuser->TickCount.High1Time;
        time.LowPart = kuser->TickCount.LowPart;
    } while (time.HighPart != kuser->TickCount.High2Time);

    uint64_t tick_ms = ((uint64_t)time.QuadPart * kuser->TickCountMultiplier) >> 24;
    uint64_t total_seconds = tick_ms / 1000;
    uint32_t total_days = (uint32_t)(total_seconds / (24 * 60 * 60));
    uint32_t remaining_seconds = (uint32_t)(total_seconds % (24 * 60 * 60));

    uint32_t years = total_days / 365;
    uint32_t remaining_days = total_days % 365;

    uint32_t leap_days = years / 4 - years / 100 + years / 400;
    if (remaining_days >= leap_days && years > 0) {
        remaining_days -= leap_days;
    }

    uint32_t months = remaining_days / 30;
    remaining_days %= 30;

    uint32_t weeks = remaining_days / 7;
    remaining_days %= 7;

    uint32_t hours = remaining_seconds / 3600;
    remaining_seconds %= 3600;
    uint32_t minutes = remaining_seconds / 60;
    uint32_t seconds = remaining_seconds % 60;

    pal_time result = {0};
    result.year = years;
    result.month = months;
    result.weeks = weeks;
    result.day = remaining_days;
    result.hours = hours;
    result.minutes = minutes;
    result.seconds = seconds;

    return result;
}

void win32_init_timer(void) {
    volatile KUSER_SHARED_DATA *kuser = (volatile KUSER_SHARED_DATA*)KUSER_SHARED_DATA_ADDRESS;

    g_app_start_time = kuser->QpcData.QpcPerformanceCounter;
    assert(g_app_start_time != 0);
}

PALAPI double pal_get_time_since_pal_init(void) {
    volatile KUSER_SHARED_DATA *kuser = (volatile KUSER_SHARED_DATA*)KUSER_SHARED_DATA_ADDRESS;

    uint64_t current = kuser->QpcData.QpcPerformanceCounter;
    uint64_t elapsed_ticks = current - g_app_start_time;
    uint64_t frequency = kuser->QpcFrequency;

    // we divide kernel ticks by frequency in order to get that time in seconds.
    return (double)elapsed_ticks / (double)frequency;
}

PALAPI uint64_t pal_get_ticks(void) {
    volatile KUSER_SHARED_DATA *kuser = (volatile KUSER_SHARED_DATA*)KUSER_SHARED_DATA_ADDRESS;
    return kuser->QpcData.QpcPerformanceCounter;
}

// Gets the frequency of the raw timer that is used by pal, not including any time the computer
// is sleeping while pal is running.
PALAPI uint64_t pal_get_timer_frequency(void) {
    volatile KUSER_SHARED_DATA *kuser = (volatile KUSER_SHARED_DATA*)KUSER_SHARED_DATA_ADDRESS;
    return (uint64_t)kuser->QpcFrequency;
}

//----------------------------------------------------------------------------------
// Clip Board Functions.
//----------------------------------------------------------------------------------

PALAPI char* pal_clipboard_get(void) {
    HANDLE hData;
    wchar_t* wtext = NULL;
    int size_needed = 0;
    char* text = NULL;

    if (!OpenClipboard(NULL))
        return NULL;

    hData = GetClipboardData(CF_UNICODETEXT);
    if (!hData) {
        CloseClipboard();
        return NULL;
    }

    wtext = GlobalLock(hData);
    if (!wtext) {
        CloseClipboard();
        return NULL;
    }

    // Convert wide char text to UTF-8
    size_needed = WideCharToMultiByte(CP_UTF8, 0, wtext, -1, NULL, 0, NULL, NULL);
    text = (char*)malloc(size_needed);
    if (text)
        WideCharToMultiByte(CP_UTF8, 0, wtext, -1, text, size_needed, NULL, NULL);

    GlobalUnlock(hData);
    CloseClipboard();

    return text; // caller must free()
}

PALAPI void pal_clipboard_set(const char* text) {
    size_t len = 0;
    HGLOBAL hMem;

    if (text == NULL || *text == '\0')
        return;

    // Calculate the size of the text, including the null terminator
    len = pal_strlen(text) + 1;
    hMem = GlobalAlloc(GMEM_MOVEABLE, len);
    if (!hMem)
        return;

    // Copy the text into the allocated memory
    pal_memcpy(GlobalLock(hMem), text, len);
    GlobalUnlock(hMem);

    // Open the clipboard and set the data
    if (OpenClipboard(NULL)) {
        EmptyClipboard();
        SetClipboardData(CF_TEXT, hMem);
        CloseClipboard();
    } else {
        GlobalFree(hMem);
    }
}

//----------------------------------------------------------------------------------
// Mouse Warp Functions.
//----------------------------------------------------------------------------------

void pal_mouse_warp(int x, int y) {
    SetCursorPos(x, y);
}

void pal_mouse_warp_relative(int dx, int dy) {
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    input.mi.dx = dx;
    input.mi.dy = dy;
    SendInput(1, &input, sizeof(INPUT));
}

//----------------------------------------------------------------------------------
// Url Launch Function.
//----------------------------------------------------------------------------------
PALAPI void pal_url_launch(char* url) {
    HINSTANCE result;

    if (!url || !*url)
        return;

    // ShellExecuteA automatically opens the URL with the default app (e.g., browser)
    result = ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);

    // Optional: check if it failed
    if ((INT_PTR)result <= 32) {
        MessageBoxA(NULL, "Failed to open URL.", "Error", MB_ICONERROR);
    }
}

//----------------------------------------------------------------------------------
// File Requester Functions.
//----------------------------------------------------------------------------------

#include <commdlg.h>

typedef struct PalRequester {
    char path[MAX_PATH];
} PalRequester;

static PalRequester g_requesters[16]; // simple static pool, indexed by `id`

static PalRequester* win32_get_requester(void* id) {
    uintptr_t index = (uintptr_t)id;
    if (index >= 16)
        return NULL;
    return &g_requesters[index];
}

static void win32_build_filter_string(char** types, uint32_t type_count, char* out, size_t out_size) {
    const char* ext = types[i];
    int written = 0;

    // Builds Windows filter string like: "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0"
    out[0] = '\0';
    size_t pos = 0;
    for (uint32_t i = 0; i < type_count; i++) {
        written = snprintf(out + pos, out_size - pos, "%s files (*.%s)%c*.%s%c", ext, ext, '\0', ext, '\0');
        pos += written;
        if (pos >= out_size)
            break;
    }
    // Add final double null terminator
    out[pos++] = '\0';
}

void pal_create_save_dialog(char** types, uint32_t type_count, void* id) {
    PalRequester* req = win32_get_requester(id);
    OPENFILENAMEA ofn = {0};
    char filter[512];
    char path[MAX_PATH] = {0};

    if (!req)
        return;

    win32_build_filter_string(types, type_count, filter, sizeof(filter));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = filter[0] ? filter : "All Files (*.*)\0*.*\0";
    ofn.lpstrFile = path;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
    ofn.lpstrDefExt = type_count > 0 ? types[0] : "";

    if (GetSaveFileNameA(&ofn)) {
        pal_strcpy(req->path, MAX_PATH, path);
    } else {
        req->path[0] = '\0';
    }
}

void pal_create_load_dialog(char** types, uint32_t type_count, void* id) {
    PalRequester* req = win32_get_requester(id);
    OPENFILENAMEA ofn = {0};
    char filter[512];
    char path[MAX_PATH] = {0};

    if (!req)
        return;

    win32_build_filter_string(types, type_count, filter, sizeof(filter));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = filter[0] ? filter : "All Files (*.*)\0*.*\0";
    ofn.lpstrFile = path;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
    ofn.lpstrDefExt = type_count > 0 ? types[0] : "";

    if (GetOpenFileNameA(&ofn)) {
        pal_strcpy(req->path, MAX_PATH, path);
    } else {
        req->path[0] = '\0';
    }
}

char* pal_show_save_dialog(void* id) {
    PalRequester* req = win32_get_requester(id);
    return (req && req->path[0]) ? req->path : NULL;
}

char* pal_show_load_dialog(void* id) {
    PalRequester* req = win32_get_requester(id);
    return (req && req->path[0]) ? req->path : NULL;
}

//----------------------------------------------------------------------------------
// Multi-threadding functions.
//----------------------------------------------------------------------------------
PALAPI pal_mutex *pal_create_mutex() {
    pal_mutex *mutex = malloc(sizeof(*mutex));
    if (!mutex) return NULL;
    InitializeCriticalSection(&mutex->cs);
    return mutex;
}

PALAPI void pal_lock_mutex(pal_mutex *mutex) {
    EnterCriticalSection(&mutex->cs);
}

PALAPI pal_bool pal_lock_mutex_try(pal_mutex *mutex) {
    return TryEnterCriticalSection(&mutex->cs) ? 1 : 0;
}

PALAPI void pal_unlock_mutex(pal_mutex *mutex) {
    LeaveCriticalSection(&mutex->cs);
}

PALAPI void pal_destroy_mutex(pal_mutex *mutex) {
    DeleteCriticalSection(&mutex->cs);
    free(mutex);
}

PALAPI pal_signal *pal_create_signal(void) {
    // Manual-reset event, initially non-signaled
    return (pal_signal *)CreateEventW(NULL, TRUE, FALSE, NULL);
}

PALAPI pal_bool pal_wait_for_signal(pal_signal *signal, pal_mutex *mutex) {
    if (!signal)
        return pal_false;

    // Release the mutex so other threads can activate the signal
    if (mutex)
        pal_unlock_mutex(mutex);

    // Wait for the signal to be activated
    DWORD result = WaitForSingleObject((HANDLE)signal, INFINITE);

    // Reacquire the mutex before returning
    if (mutex)
        pal_lock_mutex(mutex);

    return (result == WAIT_OBJECT_0);
}

PALAPI pal_bool pal_activate_signal(pal_signal *signal) {
    if (!signal)
        return pal_false;

    return SetEvent((HANDLE)signal) ? pal_true : pal_false;
}

PALAPI pal_bool pal_deactivate_signal(pal_signal *signal) {
    if (!signal)
        return pal_false;

    return ResetEvent((HANDLE)signal) ? pal_true : pal_false;
}

PALAPI void pal_destroy_signal(pal_signal *signal) {
    if (signal)
        CloseHandle((HANDLE)signal);
}

typedef struct {
    pal_thread_func func;
    void *arg;
} thread_wrapper_arg;

// Wrapper to adapt pal_thread_func to Windows signature
DWORD WINAPI thread_wrapper(LPVOID param) {
    thread_wrapper_arg *wrapper = (thread_wrapper_arg *)param;
    wrapper->func(wrapper->arg);
    HeapFree(GetProcessHeap(), 0, wrapper);
    return 0;
}

PALAPI pal_thread *pal_create_thread(pal_thread_func func, void *arg) {
    thread_wrapper_arg *wrapper = (thread_wrapper_arg *)HeapAlloc(GetProcessHeap(), 0, sizeof(thread_wrapper_arg));
    HANDLE thread;

    if (!wrapper) return NULL;
    wrapper->func = func;
    wrapper->arg = arg;

    thread = CreateThread(NULL, 0, thread_wrapper, wrapper, CREATE_SUSPENDED, NULL);
    return (pal_thread *)thread;
}

PALAPI pal_bool pal_start_thread(pal_thread *thread) {
    if (!thread) return pal_false;
    return ResumeThread((HANDLE)thread) != (DWORD)-1;
}

PALAPI pal_bool pal_join_thread(pal_thread *thread) {
    if (!thread) return pal_false;
    return WaitForSingleObject((HANDLE)thread, INFINITE) == WAIT_OBJECT_0;
}

PALAPI void pal_destroy_thread(pal_thread *thread) {
    if (thread) CloseHandle((HANDLE)thread);
}

//----------------------------------------------------------------------------------
// Dynamic Library Functions.
//----------------------------------------------------------------------------------
PALAPI void* pal_load_dynamic_library(const char* dll) {
    HMODULE result = LoadLibraryA(dll);
    assert(result);
    return (void*)result;
}

PALAPI void* pal_load_dynamic_function(void* dll, char* func_name) {
    FARPROC proc = GetProcAddress(dll, func_name);
    assert(proc);
    return (void*)proc;
}

PALAPI pal_bool pal_free_dynamic_library(void* dll) {
    pal_bool free_result = FreeLibrary(dll);
    assert(free_result);
    return (pal_bool)free_result;
}

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
#endif // PLATFORM_WIN32_H

#elif __linux__

/* dlopen/dlsym/dlclose */
#include <dlfcn.h>

/* linux-specific headers */
#include <linux/input.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <libudev.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

//----------------------------------------------------------------------------------
// File I/O
//----------------------------------------------------------------------------------

PALAPI pal_bool pal_does_file_exist(const char *file_path) {
    return access(file_path, F_OK) == 0;
}

PALAPI size_t pal_get_last_write_time(const char *file_path) {
    struct stat fstat;
    int err = stat(file_path, &fstat);
    if(!err) {
        return fstat.st_mtime;
    } else {
        return 0;
    }
}

PALAPI size_t pal_get_last_read_time(const char *file_path) {
    struct stat fstat;
    int err = stat(file_path, &fstat);
    if(!err) {
        return fstat.st_atime;
    } else {
        return 0;
    }
}

PALAPI size_t pal_get_file_size(const char *file_path) {
    struct stat fstat;
    int err = stat(file_path, &fstat);
    if(!err) {
        return fstat.st_size;
    } else {
        return 0;
    }
}

PALAPI uint32_t pal_get_file_permissions(const char *file_path) {
    uint32_t permissions = 0;

    if (access(file_path, R_OK) == 0) {
        permissions |= PAL_READ;
    }

    if (access(file_path, W_OK) == 0) {
        permissions |= PAL_WRITE;
    }

    if (access(file_path, X_OK) == 0) {
        permissions |= PAL_EXECUTE;
    }

    return permissions;
}

PALAPI pal_bool pal_change_file_permissions(const char *file_path, uint32_t permission_flags) {
    mode_t mode = 0;

    if (permission_flags & PAL_READ) {
        mode |= S_IRUSR | S_IRGRP | S_IROTH;
    }

    if (permission_flags & PAL_WRITE) {
        mode |= S_IWUSR | S_IWGRP | S_IWOTH;
    }

    if (permission_flags & PAL_EXECUTE) {
        mode |= S_IXUSR | S_IXGRP | S_IXOTH;
    }

    if (chmod(file_path, mode) == -1) {
        return pal_false;
    }

    return pal_true;
}

PALAPI unsigned char *pal_read_entire_file(const char *file_path, size_t *bytes_read) {
    size_t file_size = pal_get_file_size(file_path);
    if (file_size == 0) {
        return NULL;
    }

    unsigned char *file = malloc(file_size + 1);
    if (!file) {
        return NULL;
    }

    int fd = open(file_path, O_RDWR);
    if (fd == -1) {
        free(file);
        return NULL;
    }

    size_t total_read = 0;

    while (total_read < file_size) {
        ssize_t result = read(fd, file + total_read, file_size - total_read);
        if (result <= 0) {  // error or unexpected EOF
            close(fd);
            free(file);
            return NULL;
        }
        total_read += result;
    }

    close(fd);

    file[file_size] = '\0'; // optional for text files
    *bytes_read = file_size;
    return file;
}

PALAPI pal_bool pal_write_file(const char *file_path, size_t file_size, char *buffer) {
    int fd = open(file_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        return pal_false;
    }

    size_t total_written = 0;

    while (total_written < file_size) {
        ssize_t result = write(fd, buffer + total_written, file_size - total_written);

        if (result <= 0) {
            close(fd);
            return pal_false;
        }

        total_written += result;
    }

    close(fd);
    return pal_true;
}

PALAPI pal_bool pal_copy_file(const char *original_path, const char *copy_path) {
    int src = open(original_path, O_RDONLY);
    if (src == -1) {
        return pal_false;
    }

    int dst = open(copy_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dst == -1) {
        close(src);
        return pal_false;
    }

    char buffer[65536];  // 64KB buffer
    size_t bytes_read = 0;

    while ((bytes_read = read(src, buffer, sizeof(buffer))) > 0) {
        size_t total_written = 0;

        while (total_written < bytes_read) {
            ssize_t result = write(dst, buffer + total_written,
                                   bytes_read - total_written);

            if (result <= 0) {
                close(src);
                close(dst);
                return pal_false;
            }

            total_written += result;
        }
    }

    if (bytes_read == 0) {
        close(src);
        close(dst);
        return pal_false;
    }

    close(src);
    close(dst);
    return pal_true;
}

PALAPI pal_file *pal_open_file(const char *file_path) {
    int *file = (int*)malloc(sizeof(int));
    if (file == NULL) {
        return NULL;
    }
    *file = open(file_path, O_RDONLY);
    if (*file == -1) {
        free(file);
        return NULL;
    }
    return (pal_file*)file;
}

PALAPI pal_bool pal_read_from_open_file(pal_file *file, size_t offset, size_t bytes_to_read, char *buffer) {
    int fd = *(int*)file;
    
    // Seek to the specified offset
    if (lseek(fd, offset, SEEK_SET) == -1) {
        return pal_false;
    }
    
    size_t total_read = 0;
    while (total_read < bytes_to_read) {
        ssize_t result = read(fd, buffer + total_read, bytes_to_read - total_read);
        if (result < 0) {
            // Error occurred
            return pal_false;
        }
        if (result == 0) {
            // EOF reached before reading all requested bytes
            return pal_false;
        }
        total_read += result;
    }
    
    return pal_true;
}

PALAPI pal_bool pal_close_file(const unsigned char *file) {
    if(file) {
        free((void*)file);
        return 0;
    } else {
        return 1;
    }
}

PALAPI pal_bool pal_close_open_file(pal_file *file) {
    int fd = *(int*)file;
    int err = close(fd);
    free(file);
    return err == 0 ? pal_true : pal_false;
}

//----------------------------------------------------------------------------------
// Dynamic Library Functions.
//----------------------------------------------------------------------------------
PALAPI void *pal_load_dynamic_library(const char *dll) {
    void *lib = dlopen(dll, RTLD_NOW | RTLD_LOCAL);
    if (!lib) {
        fprintf(stderr, "dlopen failed: %s\n", dlerror());
        assert(0 && "Failed to load shared library");
    }
    return lib;
}

PALAPI void *pal_load_dynamic_function(void *dll, char *func_name) {
    dlerror(); /* Clear existing errors */
    void *symbol = dlsym(dll, func_name);
    const char *error = dlerror();
    if (error) {
        fprintf(stderr, "dlsym failed: %s\n", error);
        assert(0 && "Failed to load function from shared library");
    }
    return symbol;
}

PALAPI pal_bool pal_free_dynamic_library(void *dll) {
    int result = dlclose(dll);
    if (result != 0) {
        fprintf(stderr, "dlclose failed: %s\n", dlerror());
        assert(0 && "Failed to unload shared library");
    }
    return (uint8_t)(result == 0);
}

/* Check if the platform is PAL_PLATFORM_LINUX_X11 */
#if !defined(PAL_PLATFORM_LINUX_WAYLAND) && !defined(PAL_PLATFORM_WINDOWS)
#ifndef PLATFORM_LINUX_X11_H
#define PLATFORM_LINUX_X11_H

/* C standard lib */
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

/* X11 window shit */
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/Xrandr.h>

/*x11 keyboards shit */
#include <X11/XKBlib.h>

/* Opengl */
#include <GL/gl.h>
#include <GL/glx.h>


struct pal_window {
    Window window;
    GLXContext gl_context;
    GC graphics_context;
    float width;
    float height;
    float x,y;
};

struct pal_sound {
    /* Core audio data */
    unsigned char* data; /* Raw PCM audio data (initial buffer) */
    size_t data_size;    /* Size in bytes of initial buffer */
    int sample_rate;     /* Samples per second (e.g., 44100) */
    int channels;        /* Number of audio channels (e.g., 2 for stereo) */
    int bits_per_sample; /* Usually 16 or 32 */
    int is_float;        /* 0 = PCM, 1 = IEEE float */

    /* pulseaudio */

    /* Streaming - OGG */
    void* decoder;  /* stb_vorbis* (using void* to avoid header dependency) */
    char* filename; /* Filename for reopening OGG decoder */

    /* Streaming - WAV */
    FILE* source_file;
    size_t total_data_size; /* Total size of audio data in file */
    size_t bytes_streamed;  /* How many bytes we've read so far */
    size_t data_offset;     /* Offset in file where audio data starts */

    /* Streaming control */
    float preload_seconds; /* How many seconds were preloaded */
    int is_streaming;      /* 1 if this is a streaming sound */
    int stream_finished;   /* 1 when streaming is complete */
};

typedef struct pal_monitor {
    Display *display;
    RROutput output;
} pal_monitor;

Display *g_display = NULL;
Atom g_wm_delete = 0;

/* XKB keyboard translation support (uses existing g_display) */
static XkbDescPtr g_xkb = NULL;
static XIM g_xim = NULL;
static XIC g_xic = NULL;

void linux_x11_init_raw_input();

PALAPI void pal_init(void) {
    pal__init_eventq();
    linux_x11_init_raw_input();
    g_display = XOpenDisplay(NULL);
    if (!g_display) {
        fprintf(stderr, "Failed to open display\n");
        return;
    }
    g_wm_delete = XInternAtom(g_display, "WM_DELETE_WINDOW", False);
}



void linux_x11_cleanup_raw_input();
PALAPI void pal_shutdown() {
    if(g_display) {
        XCloseDisplay(g_display);
        linux_x11_cleanup_raw_input();
    }
}

pal_bool g_message_pump_drained = pal_false;
pal_event_queue g_event_queue;

void linux_x11_translate_event(XEvent *xevent) {
    pal_event event = {0};

    switch(xevent->type) {
        case ClientMessage: {
            if ((Atom)xevent->xclient.data.l[0] == g_wm_delete) {
                event.type = PAL_EVENT_WINDOW_CLOSE_REQUESTED;
                event.window.windowid = (uint32_t)xevent->xclient.window;
                pal__eventq_push(&g_event_queue, event);
            }
            break;
        }

        case ConfigureNotify: {
            XConfigureEvent *ce = &xevent->xconfigure;

            /* Window moved */
            event.type = PAL_EVENT_WINDOW_MOVED;
            event.window.windowid = (uint32_t)ce->window;
            event.window.x = ce->x;
            event.window.y = ce->y;
            pal__eventq_push(&g_event_queue, event);

            /* Window resized */
            event.type = PAL_EVENT_WINDOW_RESIZED;
            event.window.windowid = (uint32_t)ce->window;
            event.window.width = ce->width;
            event.window.height = ce->height;
            pal__eventq_push(&g_event_queue, event);
            break;
        }

        case MapNotify: {
            event.type = PAL_EVENT_WINDOW_SHOWN;
            event.window.windowid = (uint32_t)xevent->xmap.window;
            event.window.visible = 1;
            pal__eventq_push(&g_event_queue, event);
            break;
        }

        case UnmapNotify: {
            event.type = PAL_EVENT_WINDOW_HIDDEN;
            event.window.windowid = (uint32_t)xevent->xunmap.window;
            event.window.visible = 0;
            pal__eventq_push(&g_event_queue, event);
            break;
        }

        case Expose: {
            if (xevent->xexpose.count == 0) { /* Only on last expose event */
                event.type = PAL_EVENT_WINDOW_EXPOSED;
                event.window.windowid = (uint32_t)xevent->xexpose.window;
                pal__eventq_push(&g_event_queue, event);
            }
            break;
        }

        case FocusIn: {
            event.type = PAL_EVENT_WINDOW_GAINED_FOCUS;
            event.window.windowid = (uint32_t)xevent->xfocus.window;
            event.window.focused = 1;
            pal__eventq_push(&g_event_queue, event);
            break;
        }

        case FocusOut: {
            event.type = PAL_EVENT_WINDOW_LOST_FOCUS;
            event.window.windowid = (uint32_t)xevent->xfocus.window;
            event.window.focused = 0;
            pal__eventq_push(&g_event_queue, event);
            break;
        }

        case DestroyNotify: {
            event.type = PAL_EVENT_WINDOW_CLOSED;
            event.window.windowid = (uint32_t)xevent->xdestroywindow.window;
            pal__eventq_push(&g_event_queue, event);
            break;
        }

        default:
            break;
    }
}

void linux_x11_poll_raw_input();

PALAPI pal_bool pal_poll_events(pal_event* event) {
    pal_event_queue* queue = &g_event_queue;
    if (!g_message_pump_drained) {
        linux_x11_poll_raw_input();

        while (XPending(g_display) > 0) {
            XEvent xevent;
            XNextEvent(g_display, &xevent);
            linux_x11_translate_event(&xevent);
        }
        g_message_pump_drained = pal_true;
    }

    if (queue->size) {
        /* peek */
        *event = queue->events[queue->front];
        /* dequeue */
        queue->front = (queue->front + 1) % queue->capacity;
        queue->size--;
        return 1;
    } else {
        g_message_pump_drained = pal_false;
        return 0;
    }
}

/* udev context and monitor */
struct udev *g_udev = NULL;
struct udev_monitor *g_monitor = NULL;

/* -------- Helpers -------- */
int is_keyboard(int fd) {
    unsigned long evbits[(EV_MAX+7)/8] = {0};
    ioctl(fd, EVIOCGBIT(0, EV_MAX), evbits);
    
    int has_key = (evbits[EV_KEY/8] & (1 << (EV_KEY % 8))) != 0;
    int has_rel = (evbits[EV_REL/8] & (1 << (EV_REL % 8))) != 0;
    int has_abs = (evbits[EV_ABS/8] & (1 << (EV_ABS % 8))) != 0;
    
    /* It's a keyboard if it has keys but NOT mouse/touchpad properties */
    return has_key && !has_rel && !has_abs;
}

int is_mouse(int fd) {
    unsigned long evbits[(EV_MAX+7)/8] = {0};
    ioctl(fd, EVIOCGBIT(0, EV_MAX), evbits);
    
    int has_rel = (evbits[EV_REL/8] & (1 << (EV_REL % 8))) != 0;
    int has_abs = (evbits[EV_ABS/8] & (1 << (EV_ABS % 8))) != 0;
    
    /* It's a mouse/touchpad if it has relative motion OR absolute positioning */
    return has_rel || has_abs;
}

/* -------- Initialization -------- */

Window g_dummy_window;

void linux_x11_create_dummy_window() {
    int screen = DefaultScreen(g_display);

    g_dummy_window = XCreateSimpleWindow(
        g_display,
        RootWindow(g_display, screen),
        0, 0, 1, 1, 0,
        BlackPixel(g_display, screen),
        BlackPixel(g_display, screen)
    );

    XMapWindow(g_display, g_dummy_window);
    XFlush(g_display);
}

void linux_x11_init_raw_input() {
    /* Initialize X11 keyboard translation support */
    if (!g_display) {
        fprintf(stderr, "Warning: Failed to open X display for keyboard translation\n");
        fprintf(stderr, "Text input will fall back to US layout\n");
    } else {
        g_xkb = XkbGetMap(g_display, XkbAllComponentsMask, XkbUseCoreKbd);
        if (!g_xkb) {
            fprintf(stderr, "Warning: Failed to get XKB map\n");
        }

        /* Set up input method for IME support (Chinese, Japanese, Korean, etc.) */
        g_xim = XOpenIM(g_display, NULL, NULL, NULL);
        if (g_xim) {
            linux_x11_create_dummy_window();
            g_xic = XCreateIC(g_xim,
                XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
                XNClientWindow, g_dummy_window,
                XNFocusWindow, g_dummy_window,
                NULL);
            if (!g_xic) {
                fprintf(stderr, "Warning: Failed to create input context\n");
            }
        }
    }

    /* Initialize udev for device hotplug detection */
    g_udev = udev_new();
    if (!g_udev) {
        fprintf(stderr, "Failed to create udev context\n");
        exit(1);
    }

    g_monitor = udev_monitor_new_from_netlink(g_udev, "udev");
    udev_monitor_filter_add_match_subsystem_devtype(g_monitor, "input", NULL);
    udev_monitor_enable_receiving(g_monitor);

    /* Enumerate existing devices */
    struct udev_enumerate *enumerate = udev_enumerate_new(g_udev);
    udev_enumerate_add_match_subsystem(enumerate, "input");
    udev_enumerate_scan_devices(enumerate);
    struct udev_list_entry *devices = udev_enumerate_get_list_entry(enumerate);
    struct udev_list_entry *entry;

    udev_list_entry_foreach(entry, devices) {
        const char *path = udev_list_entry_get_name(entry);
        struct udev_device *dev = udev_device_new_from_syspath(g_udev, path);
        const char *devnode = udev_device_get_devnode(dev);
        if (!devnode) { udev_device_unref(dev); continue; }
        
        int dev_fd = open(devnode, O_RDONLY | O_NONBLOCK);
        if (dev_fd < 0) { udev_device_unref(dev); continue; }
        
        char name[256] = {0};
        ioctl(dev_fd, EVIOCGNAME(sizeof(name)), name);
        
        if (is_keyboard(dev_fd) && g_keyboards.count < MAX_KEYBOARDS) {

            g_keyboards.handles[g_keyboards.count] = dev_fd;
            pal_strncpy(g_keyboards.names[g_keyboards.count], name, 255);
            pal_memset(g_keyboards.keys[g_keyboards.count], 0, MAX_KEYS);
            pal_memset(g_keyboards.keys_toggled[g_keyboards.count], 0, MAX_KEYS);
            g_keyboards.count++;
        } else if (is_mouse(dev_fd) && g_mice.count < MAX_MICE) {
            g_mice.handles[g_mice.count] = dev_fd;
            pal_strncpy(g_mice.names[g_mice.count], name, 255);
            pal_memset(g_mice.buttons[g_mice.count], 0, MAX_MOUSE_BUTTONS * sizeof(int));
            pal_memset(g_mice.buttons_toggled[g_mice.count], 0, MAX_MOUSE_BUTTONS * sizeof(int));
            g_mice.dx[g_mice.count] = 0;
            g_mice.dy[g_mice.count] = 0;
            g_mice.wheel[g_mice.count] = 0;
            g_mice.count++;
        } else {
            close(dev_fd);
        }
        
        udev_device_unref(dev);
    }
    
    udev_enumerate_unref(enumerate);
}

void linux_x11_cleanup_raw_input() {
    /* Clean up X11 resources */
    if (g_xic) XDestroyIC(g_xic);
    if (g_xim) XCloseIM(g_xim);
    if (g_xkb) XkbFreeKeyboard(g_xkb, XkbAllComponentsMask, True);
    if (g_display) XCloseDisplay(g_display);
    
    /* Clean up keyboards */
    for (int i = 0; i < g_keyboards.count; i++) {
        close(g_keyboards.handles[i]);
    }
    
    /* Clean up mice */
    for (int i = 0; i < g_mice.count; i++) {
        close(g_mice.handles[i]);
    }
    
    /* Clean up udev */
    if (g_monitor) udev_monitor_unref(g_monitor);
    if (g_udev) udev_unref(g_udev);
}

PALAPI pal_vec2 pal_get_mouse_position(pal_window *window) {
    pal_vec2 mouse_pos;

    if (!g_display || !window || !window->window) {
        mouse_pos.x = 0;
        mouse_pos.y = 0;
        return mouse_pos;
    }

    Window root_return, child_return;
    int root_x, root_y, win_x, win_y;
    unsigned int mask_return;

    /* Query pointer position relative to the window */
    XQueryPointer(g_display,
                  window->window,
                  &root_return, &child_return,
                  &root_x, &root_y,
                  &win_x, &win_y,
                  &mask_return);

    mouse_pos.x = win_x;
    mouse_pos.y = win_y;
    return mouse_pos;
}

static int linux_button_to_pal_button(int linux_code) {
    switch(linux_code) {
        case BTN_LEFT:    return PAL_MOUSE_LEFT;    /* 0x0 */
        case BTN_RIGHT:   return PAL_MOUSE_RIGHT;   /* 0x1 */
        case BTN_MIDDLE:  return PAL_MOUSE_MIDDLE;  /* 0x2 */
        case BTN_SIDE:    return PAL_MOUSE_4;       /* 0x3 (also known as "back") */
        case BTN_EXTRA:   return PAL_MOUSE_5;       /* 0x4 (also known as "forward") */
        case BTN_FORWARD: return PAL_MOUSE_5;       /* 0x4 (alternative code for forward) */
        case BTN_BACK:    return PAL_MOUSE_4;       /* 0x3 (alternative code for back) */
        default:          return -1;
    }
}

/* Mapping table from Linux input key codes to PAL virtual key codes */
static const int linux_keycode_to_pal_vk[KEY_MAX] = {
    [KEY_ESC]           = PAL_KEY_ESCAPE,
    [KEY_1]             = PAL_KEY_1,
    [KEY_2]             = PAL_KEY_2,
    [KEY_3]             = PAL_KEY_3,
    [KEY_4]             = PAL_KEY_4,
    [KEY_5]             = PAL_KEY_5,
    [KEY_6]             = PAL_KEY_6,
    [KEY_7]             = PAL_KEY_7,
    [KEY_8]             = PAL_KEY_8,
    [KEY_9]             = PAL_KEY_9,
    [KEY_0]             = PAL_KEY_0,
    [KEY_MINUS]         = PAL_KEY_MINUS,
    [KEY_EQUAL]         = PAL_KEY_EQUAL,
    [KEY_BACKSPACE]     = PAL_KEY_BACKSPACE,
    [KEY_TAB]           = PAL_KEY_TAB,
    [KEY_Q]             = PAL_KEY_Q,
    [KEY_W]             = PAL_KEY_W,
    [KEY_E]             = PAL_KEY_E,
    [KEY_R]             = PAL_KEY_R,
    [KEY_T]             = PAL_KEY_T,
    [KEY_Y]             = PAL_KEY_Y,
    [KEY_U]             = PAL_KEY_U,
    [KEY_I]             = PAL_KEY_I,
    [KEY_O]             = PAL_KEY_O,
    [KEY_P]             = PAL_KEY_P,
    [KEY_LEFTBRACE]     = PAL_KEY_LEFTBRACE,  /* VK_OEM_4 '[' */
    [KEY_RIGHTBRACE]    = PAL_KEY_RIGHTBRACE,  /* VK_OEM_6 ']' */
    [KEY_ENTER]         = PAL_KEY_ENTER,
    [KEY_LEFTCTRL]      = PAL_KEY_LCTRL,
    [KEY_A]             = PAL_KEY_A,
    [KEY_S]             = PAL_KEY_S,
    [KEY_D]             = PAL_KEY_D,
    [KEY_F]             = PAL_KEY_F,
    [KEY_G]             = PAL_KEY_G,
    [KEY_H]             = PAL_KEY_H,
    [KEY_J]             = PAL_KEY_J,
    [KEY_K]             = PAL_KEY_K,
    [KEY_L]             = PAL_KEY_L,
    [KEY_SEMICOLON]     = PAL_KEY_SEMICOLON,  /* VK_OEM_1 ';' */
    [KEY_APOSTROPHE]    = PAL_KEY_APOSTROPHE,  /* VK_OEM_7 ''' */
    [KEY_GRAVE]         = PAL_KEY_BACKTICK,  /* VK_OEM_3 '`' */
    [KEY_LEFTSHIFT]     = PAL_KEY_LSHIFT,
    [KEY_BACKSLASH]     = PAL_KEY_BACKSLASH,  /* VK_OEM_5 '\' */
    [KEY_Z]             = PAL_KEY_Z,
    [KEY_X]             = PAL_KEY_X,
    [KEY_C]             = PAL_KEY_C,
    [KEY_V]             = PAL_KEY_V,
    [KEY_B]             = PAL_KEY_B,
    [KEY_N]             = PAL_KEY_N,
    [KEY_M]             = PAL_KEY_M,
    [KEY_COMMA]         = PAL_KEY_COMMA,  /* VK_OEM_COMMA */
    [KEY_DOT]           = PAL_KEY_DOT,  /* VK_OEM_PERIOD */
    [KEY_SLASH]         = PAL_KEY_FORWARD_SLASH,  /* VK_OEM_2 '/' */
    [KEY_RIGHTSHIFT]    = PAL_KEY_RSHIFT,
    [KEY_KPASTERISK]    = PAL_KEY_MULTIPLY,
    [KEY_LEFTALT]       = PAL_KEY_LALT,
    [KEY_SPACE]         = PAL_KEY_SPACE,
    [KEY_CAPSLOCK]      = PAL_KEY_CAPSLOCK,
    [KEY_F1]            = PAL_KEY_F1,
    [KEY_F2]            = PAL_KEY_F2,
    [KEY_F3]            = PAL_KEY_F3,
    [KEY_F4]            = PAL_KEY_F4,
    [KEY_F5]            = PAL_KEY_F5,
    [KEY_F6]            = PAL_KEY_F6,
    [KEY_F7]            = PAL_KEY_F7,
    [KEY_F8]            = PAL_KEY_F8,
    [KEY_F9]            = PAL_KEY_F9,
    [KEY_F10]           = PAL_KEY_F10,
    [KEY_NUMLOCK]       = PAL_KEY_NUMLOCKCLEAR,
    [KEY_SCROLLLOCK]    = PAL_KEY_SCROLLLOCK,
    [KEY_KP7]           = PAL_KEY_NUMPAD_7,
    [KEY_KP8]           = PAL_KEY_NUMPAD_8,
    [KEY_KP9]           = PAL_KEY_NUMPAD_9,
    [KEY_KPMINUS]       = PAL_KEY_SUBTRACT,
    [KEY_KP4]           = PAL_KEY_NUMPAD_4,
    [KEY_KP5]           = PAL_KEY_NUMPAD_5,
    [KEY_KP6]           = PAL_KEY_NUMPAD_6,
    [KEY_KPPLUS]        = PAL_KEY_ADD,
    [KEY_KP1]           = PAL_KEY_NUMPAD_1,
    [KEY_KP2]           = PAL_KEY_NUMPAD_2,
    [KEY_KP3]           = PAL_KEY_NUMPAD_3,
    [KEY_KP0]           = PAL_KEY_NUMPAD_0,
    [KEY_KPDOT]         = PAL_KEY_NUMPAD_PERIOD,
    [KEY_F11]           = PAL_KEY_F11,
    [KEY_F12]           = PAL_KEY_F12,
    [KEY_KPENTER]       = PAL_KEY_ENTER,
    [KEY_RIGHTCTRL]     = PAL_KEY_RCTRL,
    [KEY_KPSLASH]       = PAL_KEY_DIVIDE,
    [KEY_SYSRQ]         = PAL_KEY_PRINTSCREEN,
    [KEY_RIGHTALT]      = PAL_KEY_RALT,
    [KEY_HOME]          = PAL_KEY_HOME,
    [KEY_UP]            = PAL_KEY_UP,
    [KEY_PAGEUP]        = PAL_KEY_PAGEUP,
    [KEY_LEFT]          = PAL_KEY_LEFT,
    [KEY_RIGHT]         = PAL_KEY_RIGHT,
    [KEY_END]           = PAL_KEY_END,
    [KEY_DOWN]          = PAL_KEY_DOWN,
    [KEY_PAGEDOWN]      = PAL_KEY_PAGEDOWN,
    [KEY_INSERT]        = PAL_KEY_INSERT,
    [KEY_DELETE]        = PAL_KEY_DELETE,
    [KEY_PAUSE]         = PAL_KEY_PAUSE,
    [KEY_LEFTMETA]      = PAL_KEY_LWIN,
    [KEY_RIGHTMETA]     = PAL_KEY_RWIN,
    [KEY_MENU]          = PAL_KEY_MENU,
};

static const int linux_scancode_to_pal_scancode[256] = {
    /*   0 */ PAL_SCAN_NONE,
    /*   1 */ PAL_SCAN_ESCAPE,
    /*   2 */ PAL_SCAN_1,
    /*   3 */ PAL_SCAN_2,
    /*   4 */ PAL_SCAN_3,
    /*   5 */ PAL_SCAN_4,
    /*   6 */ PAL_SCAN_5,
    /*   7 */ PAL_SCAN_6,
    /*   8 */ PAL_SCAN_7,
    /*   9 */ PAL_SCAN_8,
    /*  10 */ PAL_SCAN_9,
    /*  11 */ PAL_SCAN_0,
    /*  12 */ PAL_SCAN_MINUS,
    /*  13 */ PAL_SCAN_EQUALS,
    /*  14 */ PAL_SCAN_BACKSPACE,
    /*  15 */ PAL_SCAN_TAB,
    /*  16 */ PAL_SCAN_Q,
    /*  17 */ PAL_SCAN_W,
    /*  18 */ PAL_SCAN_E,
    /*  19 */ PAL_SCAN_R,
    /*  20 */ PAL_SCAN_T,
    /*  21 */ PAL_SCAN_Y,
    /*  22 */ PAL_SCAN_U,
    /*  23 */ PAL_SCAN_I,
    /*  24 */ PAL_SCAN_O,
    /*  25 */ PAL_SCAN_P,
    /*  26 */ PAL_SCAN_LEFTBRACKET,
    /*  27 */ PAL_SCAN_RIGHTBRACKET,
    /*  28 */ PAL_SCAN_RETURN,
    /*  29 */ PAL_SCAN_LCTRL,
    /*  30 */ PAL_SCAN_A,
    /*  31 */ PAL_SCAN_S,
    /*  32 */ PAL_SCAN_D,
    /*  33 */ PAL_SCAN_F,
    /*  34 */ PAL_SCAN_G,
    /*  35 */ PAL_SCAN_H,
    /*  36 */ PAL_SCAN_J,
    /*  37 */ PAL_SCAN_K,
    /*  38 */ PAL_SCAN_L,
    /*  39 */ PAL_SCAN_SEMICOLON,
    /*  40 */ PAL_SCAN_APOSTROPHE,
    /*  41 */ PAL_SCAN_GRAVE,
    /*  42 */ PAL_SCAN_LSHIFT,
    /*  43 */ PAL_SCAN_BACKSLASH,
    /*  44 */ PAL_SCAN_Z,
    /*  45 */ PAL_SCAN_X,
    /*  46 */ PAL_SCAN_C,
    /*  47 */ PAL_SCAN_V,
    /*  48 */ PAL_SCAN_B,
    /*  49 */ PAL_SCAN_N,
    /*  50 */ PAL_SCAN_M,
    /*  51 */ PAL_SCAN_COMMA,
    /*  52 */ PAL_SCAN_PERIOD,
    /*  53 */ PAL_SCAN_SLASH,
    /*  54 */ PAL_SCAN_RSHIFT,
    /*  55 */ PAL_SCAN_KP_MULTIPLY,
    /*  56 */ PAL_SCAN_LALT,
    /*  57 */ PAL_SCAN_SPACE,
    /*  58 */ PAL_SCAN_CAPSLOCK,
    /*  59 */ PAL_SCAN_F1,
    /*  60 */ PAL_SCAN_F2,
    /*  61 */ PAL_SCAN_F3,
    /*  62 */ PAL_SCAN_F4,
    /*  63 */ PAL_SCAN_F5,
    /*  64 */ PAL_SCAN_F6,
    /*  65 */ PAL_SCAN_F7,
    /*  66 */ PAL_SCAN_F8,
    /*  67 */ PAL_SCAN_F9,
    /*  68 */ PAL_SCAN_F10,
    /*  69 */ PAL_SCAN_NUMCLEAR,      /* NumLock */
    /*  70 */ PAL_SCAN_SCROLLLOCK,
    /*  71 */ PAL_SCAN_KP_7,
    /*  72 */ PAL_SCAN_KP_8,
    /*  73 */ PAL_SCAN_KP_9,
    /*  74 */ PAL_SCAN_KP_MINUS,
    /*  75 */ PAL_SCAN_KP_4,
    /*  76 */ PAL_SCAN_KP_5,
    /*  77 */ PAL_SCAN_KP_6,
    /*  78 */ PAL_SCAN_KP_PLUS,
    /*  79 */ PAL_SCAN_KP_1,
    /*  80 */ PAL_SCAN_KP_2,
    /*  81 */ PAL_SCAN_KP_3,
    /*  82 */ PAL_SCAN_KP_0,
    /*  83 */ PAL_SCAN_KP_PERIOD,
    /*  84 */ PAL_SCAN_NONUSBACKSLASH,  /* 102nd key (ISO) */
    /*  85 */ PAL_SCAN_F11,
    /*  86 */ PAL_SCAN_F12,
    /*  87 */ PAL_SCAN_KP_ENTER,
    /*  88 */ PAL_SCAN_RCTRL,
    /*  89 */ PAL_SCAN_KP_DIVIDE,
    /*  90 */ PAL_SCAN_SYSREQ,
    /*  91 */ PAL_SCAN_RALT,       /* AltGr */
    /*  92 */ PAL_SCAN_HOME,
    /*  93 */ PAL_SCAN_UP,
    /*  94 */ PAL_SCAN_PAGEUP,
    /*  95 */ PAL_SCAN_LEFT,
    /*  96 */ PAL_SCAN_RIGHT,
    /*  97 */ PAL_SCAN_END,
    /*  98 */ PAL_SCAN_DOWN,
    /*  99 */ PAL_SCAN_PAGEDOWN,
    /* 100 */ PAL_SCAN_INSERT,
    /* 101 */ PAL_SCAN_DELETE,
    /* 102 */ PAL_SCAN_MUTE,
    /* 103 */ PAL_SCAN_VOLUMEDOWN,
    /* 104 */ PAL_SCAN_VOLUMEUP,
    /* 105 */ PAL_SCAN_POWER,
    /* 106 */ PAL_SCAN_KP_EQUALS,
    /* 107 */ PAL_SCAN_F13,
    /* 108 */ PAL_SCAN_F14,
    /* 109 */ PAL_SCAN_F15,
    /* 110 */ PAL_SCAN_HELP,
    /* 111 */ PAL_SCAN_MENU,
    /* 112 */ PAL_SCAN_STOP,
    /* 113 */ PAL_SCAN_AGAIN,
    /* 114 */ PAL_SCAN_AC_BACK,
    /* 115 */ PAL_SCAN_AC_FORWARD,
    /* 116 */ PAL_SCAN_AC_REFRESH,
    /* 117 */ PAL_SCAN_AC_STOP,
    /* 118 */ PAL_SCAN_AC_SEARCH,
    /* 119 */ PAL_SCAN_AC_BOOKMARKS,
    /* 120 */ PAL_SCAN_MUTE,
    /* 121 */ PAL_SCAN_VOLUMEDOWN,
    /* 122 */ PAL_SCAN_VOLUMEUP,
    /* 123 */ PAL_SCAN_POWER,   /* some keyboards */
    /* 124 */ PAL_SCAN_NONE,
    /* 125 */ PAL_SCAN_LGUI,
    /* 126 */ PAL_SCAN_RGUI,
    /* 127 */ PAL_SCAN_APPLICATION,

    /* 128–255 = mostly OEM / media keys */
    [128] = PAL_SCAN_MEDIA_PLAY_PAUSE,
    [129] = PAL_SCAN_MEDIA_STOP,
    [130] = PAL_SCAN_MEDIA_PREVIOUS_TRACK,
    [131] = PAL_SCAN_MEDIA_NEXT_TRACK,
    [132] = PAL_SCAN_MEDIA_EJECT,
    [133] = PAL_SCAN_SLEEP,
    [134] = PAL_SCAN_WAKE,
    [135] = PAL_SCAN_AC_HOME,
    [136] = PAL_SCAN_AC_SEARCH,
    [137] = PAL_SCAN_AC_BACK,
    [138] = PAL_SCAN_AC_FORWARD,
    [139] = PAL_SCAN_AC_REFRESH,
    [140] = PAL_SCAN_AC_BOOKMARKS,

};

int linux_keycode_to_pal_virtual_key(int linux_code) {
    if (linux_code < 0 || linux_code >= KEY_MAX) {
        return PAL_KEY_NONE;
    }
    return linux_keycode_to_pal_vk[linux_code];
}


int linux_keycode_to_utf8(int keycode, unsigned char *key_state, char *out, size_t out_size);
/* Add this helper function to convert key states to modifier flags */
static int compute_modifiers_from_key_state(unsigned char *keys) {
    int modifiers = PAL_MOD_NONE;
    
    if (keys[PAL_SCAN_LSHIFT]) modifiers |= PAL_MOD_LSHIFT;
    if (keys[PAL_SCAN_RSHIFT]) modifiers |= PAL_MOD_RSHIFT;
    if (keys[PAL_SCAN_LCTRL]) modifiers |= PAL_MOD_LCTRL;
    if (keys[PAL_SCAN_RCTRL]) modifiers |= PAL_MOD_RCTRL;
    if (keys[PAL_SCAN_LALT]) modifiers |= PAL_MOD_LALT;
    if (keys[PAL_SCAN_RALT]) modifiers |= PAL_MOD_RALT;
    if (keys[PAL_SCAN_LGUI]) modifiers |= PAL_MOD_LSUPER;
    if (keys[PAL_SCAN_RGUI]) modifiers |= PAL_MOD_RSUPER;
    if (keys[PAL_SCAN_CAPSLOCK]) modifiers |= PAL_MOD_CAPS;
    if (keys[PAL_SCAN_NUMCLEAR]) modifiers |= PAL_MOD_NUM;
    if (keys[PAL_SCAN_SCROLLLOCK]) modifiers |= PAL_MOD_SCROLL;
    if (keys[PAL_SCAN_ALTGR]) modifiers |= PAL_MOD_ALTGR;
    
    return modifiers;
}

/* Update this in linux_x11_poll_raw_input() after processing keyboard events */
void linux_x11_poll_raw_input() {
    struct pollfd fds[MAX_KEYBOARDS + MAX_MICE + 1];
    int nfds = 0;

    /* Clear toggle flags */
    for (int i = 0; i < g_keyboards.count; i++)
        pal_memset(g_keyboards.keys_toggled[i], 0, sizeof(g_keyboards.keys_toggled[i]));
    for (int i = 0; i < g_mice.count; i++) {
        pal_memset(g_mice.buttons_toggled[i], 0, sizeof(g_mice.buttons_toggled[i]));
        g_mice.dx[i] = 0;
        g_mice.dy[i] = 0;
        g_mice.wheel[i] = 0;
    }

    /* Add keyboard fds */
    for (int i = 0; i < g_keyboards.count; i++) {
        fds[nfds].fd = g_keyboards.handles[i];
        fds[nfds].events = POLLIN;
        nfds++;
    }

    /* Add mouse fds */
    for (int i = 0; i < g_mice.count; i++) {
        fds[nfds].fd = g_mice.handles[i];
        fds[nfds].events = POLLIN;
        nfds++;
    }

    /* Add udev monitor */
    int monitor_fd = udev_monitor_get_fd(g_monitor);
    fds[nfds].fd = monitor_fd;
    fds[nfds].events = POLLIN;
    nfds++;

    int ret = poll(fds, nfds, 0);
    if (ret <= 0) return;

    struct input_event ev;
    pal_event event = {0};

    /* ---- Process keyboard events ---- */
    for (int i = 0; i < g_keyboards.count; i++) {
        uint32_t tmp_scancode = 0;

        while (read(fds[i].fd, &ev, sizeof(ev)) > 0) {
            if (ev.type == EV_MSC && ev.code == MSC_SCAN) {
                tmp_scancode = linux_scancode_to_pal_scancode[ev.value];
            }
            else if (ev.type == EV_KEY) {
                int pal_vk = linux_keycode_to_pal_virtual_key(ev.code);
                if (pal_vk == PAL_KEY_NONE) continue;

                uint32_t sc = tmp_scancode;
                unsigned char old_state = g_keyboards.keys[i][sc];
                unsigned char new_state = (ev.value > 0) ? 1 : 0; /* 0=released, 1=pressed, 2=repeat */
                int is_repeat = (ev.value == 2);

                g_keyboards.keys[i][sc] = new_state;

                /* Send key down/up event */
                if (old_state != new_state || is_repeat) {
                    if (!is_repeat) {
                        g_keyboards.keys_toggled[i][sc] = 1;
                    }

                    event.type = new_state ? PAL_EVENT_KEY_DOWN : PAL_EVENT_KEY_UP;
                    event.key.pressed = new_state;
                    event.key.scancode = sc;
                    event.key.virtual_key = pal_vk;
                    event.key.repeat = is_repeat;
                    event.key.keyboard_id = i;
                    g_keyboards.cached_modifiers[i] = compute_modifiers_from_key_state(g_keyboards.keys[i]);
                    event.key.modifiers = g_keyboards.cached_modifiers[i];
                    pal__eventq_push(&g_event_queue, event);

                    /* Generate text input event for printable characters */
                    if (new_state || is_repeat) {
                        char text[8] = {0};
                        if (linux_keycode_to_utf8(ev.code, g_keyboards.keys[i], text, sizeof(text))) {
                            event.type = PAL_EVENT_TEXT_INPUT;
                            pal_strncpy(event.text.text, text, sizeof(event.text.text) - 1);
                            event.text.text[sizeof(event.text.text) - 1] = '\0';
                            event.text.keyboard_id = i;
                            pal__eventq_push(&g_event_queue, event);
                        }
                    }
                }
            }
            else if (ev.type == EV_SYN && ev.code == SYN_REPORT) {
                tmp_scancode = 0; /* reset per frame */
            }
        }
    }

    /* ---- Process mouse events ---- */
    for (int i = 0; i < g_mice.count; i++) {
        int motion_accumulated = 0;
        int wheel_accumulated = 0;

        while (read(fds[g_keyboards.count + i].fd, &ev, sizeof(ev)) > 0) {
            if (ev.type == EV_KEY) {
                int pal_button = linux_button_to_pal_button(ev.code);
                if (pal_button < 0 || pal_button >= MAX_MOUSE_BUTTONS) continue;

                int old_state = g_mice.buttons[i][pal_button];
                int new_state = ev.value;
                g_mice.buttons[i][pal_button] = new_state;

                if (old_state != new_state) {
                    g_mice.buttons_toggled[i][pal_button] = 1;

                    /* Combine modifiers from all keyboards */
                    int combined_modifiers = PAL_MOD_NONE;
                    for(int k = 0; k < g_keyboards.count; k++) {
                        combined_modifiers |= g_keyboards.cached_modifiers[k];
                    }

                    event.type = new_state ? PAL_EVENT_MOUSE_BUTTON_DOWN : PAL_EVENT_MOUSE_BUTTON_UP;
                    event.button.pressed = new_state;
                    event.button.button = pal_button;
                    event.button.clicks = 1;
                    event.button.mouse_id = i;
                    event.button.x = 0;
                    event.button.y = 0;
                    event.button.modifiers = combined_modifiers;
                    pal__eventq_push(&g_event_queue, event);
                }
            }
            else if (ev.type == EV_REL) {
                if (ev.code == REL_X) { g_mice.dx[i] += ev.value; motion_accumulated = 1; }
                if (ev.code == REL_Y) { g_mice.dy[i] += ev.value; motion_accumulated = 1; }
                if (ev.code == REL_WHEEL) { g_mice.wheel[i] += ev.value; wheel_accumulated = 1; }
            }
        }

        if (motion_accumulated) {
            event.type = PAL_EVENT_MOUSE_MOTION;
            event.motion.delta_x = g_mice.dx[i];
            event.motion.delta_y = g_mice.dy[i];
            event.motion.mouse_id = i;
            event.motion.x = 0;
            event.motion.y = 0;
            event.motion.buttons = 0;
            pal__eventq_push(&g_event_queue, event);
        }

        if (wheel_accumulated) {
            /* Combine modifiers from all keyboards */
            int combined_modifiers = PAL_MOD_NONE;
            for(int k = 0; k < g_keyboards.count; k++) {
                combined_modifiers |= g_keyboards.cached_modifiers[k];
            }

            event.type = PAL_EVENT_MOUSE_WHEEL;
            event.wheel.y = (float)g_mice.wheel[i];
            event.wheel.x = 0.0f;
            event.wheel.mouse_id = i;
            event.wheel.mouse_x = 0;
            event.wheel.mouse_y = 0;
            event.wheel.wheel_direction = PAL_MOUSEWHEEL_VERTICAL;
            event.wheel.modifiers = combined_modifiers;
            pal__eventq_push(&g_event_queue, event);
        }
    }

    /* ---- Check udev hotplug ---- */
    if (fds[nfds-1].revents & POLLIN) {
        struct udev_device *dev = udev_monitor_receive_device(g_monitor);
        if (!dev) return;

        const char *action = udev_device_get_action(dev);
        const char *devnode = udev_device_get_devnode(dev);
        if (devnode && action && pal_strcmp(action, "add") == 0) {
            int dev_fd = open(devnode, O_RDONLY | O_NONBLOCK);
            if (dev_fd >= 0) {
                char name[256] = {0};
                ioctl(dev_fd, EVIOCGNAME(sizeof(name)), name);

                if (is_keyboard(dev_fd) && g_keyboards.count < MAX_KEYBOARDS) {
                    g_keyboards.handles[g_keyboards.count] = dev_fd;
                    pal_strncpy(g_keyboards.names[g_keyboards.count], name, 255);
                    pal_memset(g_keyboards.keys[g_keyboards.count], 0, MAX_KEYS);
                    pal_memset(g_keyboards.keys_toggled[g_keyboards.count], 0, MAX_KEYS);
                    g_keyboards.cached_modifiers[g_keyboards.count] = PAL_MOD_NONE;
                    g_keyboards.count++;
                    printf("[Keyboard Added] %s\n", name);
                }
                else if (is_mouse(dev_fd) && g_mice.count < MAX_MICE) {
                    g_mice.handles[g_mice.count] = dev_fd;
                    pal_strncpy(g_mice.names[g_mice.count], name, 255);
                    pal_memset(g_mice.buttons[g_mice.count], 0, MAX_MOUSE_BUTTONS * sizeof(int));
                    pal_memset(g_mice.buttons_toggled[g_mice.count], 0, MAX_MOUSE_BUTTONS * sizeof(int));
                    g_mice.dx[g_mice.count] = 0;
                    g_mice.dy[g_mice.count] = 0;
                    g_mice.wheel[g_mice.count] = 0;
                    g_mice.count++;
                    printf("[Mouse Added] %s\n", name);
                }
                else close(dev_fd);
            }
        }
        udev_device_unref(dev);
    }
}

/* Fallback function for when XKB is not available (US layout only) */
int linux_keycode_to_utf8_fallback(int keycode, unsigned char *key_state, char *out, size_t out_size) {
    if (out_size < 8) return 0;
    
    int pal_vk = linux_keycode_to_pal_virtual_key(keycode);
    if (pal_vk == PAL_KEY_NONE) return 0;
    
    /* Check modifier states */
    int shift = key_state[PAL_SCAN_LSHIFT] || key_state[PAL_SCAN_RSHIFT];
    int caps = key_state[PAL_SCAN_CAPSLOCK];
    int numlock = key_state[PAL_SCAN_NUMCLEAR];
    
    /* Handle letters (a-z) */
    if (pal_vk >= 'a' && pal_vk <= 'z') {
        char c = pal_vk;
        if (shift ^ caps) c -= 0x20; /* Convert to uppercase */
        out[0] = c;
        out[1] = '\0';
        return 1;
    }
    
    /* Handle digits with shift modifiers */
    if (pal_vk >= '0' && pal_vk <= '9') {
        if (shift) {
            const char symbols[] = ")!@#$%^&*(";
            out[0] = symbols[pal_vk - '0'];
        } else {
            out[0] = pal_vk;
        }
        out[1] = '\0';
        return 1;
    }
    
    /* Handle punctuation and symbols that match ASCII directly */
    switch (pal_vk) {
        /* Direct ASCII matches (no shift needed) */
        case PAL_KEY_SPACE:
        case PAL_KEY_TAB:
        case PAL_KEY_ENTER:
            out[0] = pal_vk;
            out[1] = '\0';
            return 1;
        
        /* Keys that change with shift */
        case PAL_KEY_MINUS:
            out[0] = shift ? '_' : '-';
            out[1] = '\0';
            return 1;
        case PAL_KEY_EQUAL:
            out[0] = shift ? '+' : '=';
            out[1] = '\0';
            return 1;
        case PAL_KEY_LEFTBRACE:
            out[0] = shift ? '{' : '[';
            out[1] = '\0';
            return 1;
        case PAL_KEY_RIGHTBRACE:
            out[0] = shift ? '}' : ']';
            out[1] = '\0';
            return 1;
        case PAL_KEY_BACKSLASH:
            out[0] = shift ? '|' : '\\';
            out[1] = '\0';
            return 1;
        case PAL_KEY_SEMICOLON:
            out[0] = shift ? ':' : ';';
            out[1] = '\0';
            return 1;
        case PAL_KEY_APOSTROPHE:
            out[0] = shift ? '"' : '\'';
            out[1] = '\0';
            return 1;
        case PAL_KEY_BACKTICK:
            out[0] = shift ? '~' : '`';
            out[1] = '\0';
            return 1;
        case PAL_KEY_COMMA:
            out[0] = shift ? '<' : ',';
            out[1] = '\0';
            return 1;
        case PAL_KEY_DOT:
            out[0] = shift ? '>' : '.';
            out[1] = '\0';
            return 1;
        case PAL_KEY_FORWARD_SLASH:
            out[0] = shift ? '?' : '/';
            out[1] = '\0';
            return 1;
        
        /* Numpad keys (always produce characters regardless of numlock) */
        case PAL_KEY_NUMPAD_DIVIDE:
            out[0] = '/';
            out[1] = '\0';
            return 1;
        case PAL_KEY_NUMPAD_MULTIPLY:
            out[0] = '*';
            out[1] = '\0';
            return 1;
        case PAL_KEY_NUMPAD_MINUS:
            out[0] = '-';
            out[1] = '\0';
            return 1;
        case PAL_KEY_NUMPAD_PLUS:
            out[0] = '+';
            out[1] = '\0';
            return 1;
        
        /* Numpad digits and period (only produce characters when numlock is on) */
        case PAL_KEY_NUMPAD_0:
            if (!numlock) return 0;
            out[0] = '0';
            out[1] = '\0';
            return 1;
        case PAL_KEY_NUMPAD_1:
            if (!numlock) return 0;
            out[0] = '1';
            out[1] = '\0';
            return 1;
        case PAL_KEY_NUMPAD_2:
            if (!numlock) return 0;
            out[0] = '2';
            out[1] = '\0';
            return 1;
        case PAL_KEY_NUMPAD_3:
            if (!numlock) return 0;
            out[0] = '3';
            out[1] = '\0';
            return 1;
        case PAL_KEY_NUMPAD_4:
            if (!numlock) return 0;
            out[0] = '4';
            out[1] = '\0';
            return 1;
        case PAL_KEY_NUMPAD_5:
            if (!numlock) return 0;
            out[0] = '5';
            out[1] = '\0';
            return 1;
        case PAL_KEY_NUMPAD_6:
            if (!numlock) return 0;
            out[0] = '6';
            out[1] = '\0';
            return 1;
        case PAL_KEY_NUMPAD_7:
            if (!numlock) return 0;
            out[0] = '7';
            out[1] = '\0';
            return 1;
        case PAL_KEY_NUMPAD_8:
            if (!numlock) return 0;
            out[0] = '8';
            out[1] = '\0';
            return 1;
        case PAL_KEY_NUMPAD_9:
            if (!numlock) return 0;
            out[0] = '9';
            out[1] = '\0';
            return 1;
        case PAL_KEY_NUMPAD_PERIOD:
            if (!numlock) return 0;
            out[0] = '.';
            out[1] = '\0';
            return 1;
    }
    
    /* Non-printable keys return 0 */
    return 0;
}
/* Helper function to convert Linux keycode to UTF-8 text */
/* Returns 1 if a printable character was generated, 0 otherwise */
int linux_keycode_to_utf8(int linux_keycode, unsigned char *key_state,
                          char *out, size_t out_size) {
    /* Fallback to your original function if X11 not available */
    if (!g_display || !g_xkb) {
        return linux_keycode_to_utf8_fallback(linux_keycode, key_state, out, out_size);
    }
    
    if (out_size < 8) return 0;
    
    /* Linux keycode to X11 keycode (offset by 8) */
    KeyCode x_keycode = linux_keycode + 8;
    
    /* Build modifier state */
    unsigned int modifiers = 0;
    if (key_state[PAL_SCAN_LSHIFT] || key_state[PAL_SCAN_RSHIFT])
        modifiers |= ShiftMask;
    if (key_state[PAL_SCAN_LCTRL] || key_state[PAL_SCAN_RCTRL])
        modifiers |= ControlMask;
    if (key_state[PAL_SCAN_LALT] || key_state[PAL_SCAN_RALT])
        modifiers |= Mod1Mask;
    if (key_state[PAL_SCAN_CAPSLOCK])
        modifiers |= LockMask;
    
    /* Translate using XKB */
    KeySym keysym;
    int shift_level = (modifiers & ShiftMask) ? 1 : 0;
    keysym = XkbKeycodeToKeysym(g_display, x_keycode, 0, shift_level);
    
    if (keysym == NoSymbol) return 0;
    
    /* Try IME-aware lookup first */
    if (g_xic) {
        XKeyEvent event = {
            .type = KeyPress,
            .display = g_display,
            .keycode = x_keycode,
            .state = modifiers
        };
        
        Status status;
        int len = Xutf8LookupString(g_xic, &event, out, out_size - 1, 
                                     &keysym, &status);
        
        if (status == XLookupChars || status == XLookupBoth) {
            out[len] = '\0';
            return len > 0 ? 1 : 0;
        }
    }
    
    /* Fallback: direct keysym to UTF-8 */
    int len = XkbTranslateKeySym(g_display, &keysym, modifiers, 
                                  out, out_size - 1, NULL);
    
    if (len > 0) {
        out[len] = '\0';
        return 1;
    }
    
    return 0;
}

PALAPI pal_window *pal_create_window(int width, int height, const char *window_title, uint64_t flags) {
    pal_window *window = (pal_window*)malloc(sizeof(pal_window));
    if (!window) return NULL;

    int screen = DefaultScreen(g_display);

    /* --- Default graphics API to OpenGL if none specified --- */
    if (!(flags & (PAL_WINDOW_OPENGL | PAL_WINDOW_VULKAN | PAL_WINDOW_METAL))) {
        flags |= PAL_WINDOW_OPENGL;
    }

    GLXFBConfig fb = 0;
    XVisualInfo visual_info;
    Colormap colormap = 0;

    /* --- OpenGL path --- */
    if (flags & PAL_WINDOW_OPENGL) {
        int fb_attribs[] = {
            GLX_X_RENDERABLE, True,
            GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
            GLX_RENDER_TYPE,   GLX_RGBA_BIT,
            GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
            GLX_RED_SIZE,      8,
            GLX_GREEN_SIZE,    8,
            GLX_BLUE_SIZE,     8,
            GLX_ALPHA_SIZE,    8,
            GLX_DEPTH_SIZE,    24,
            GLX_STENCIL_SIZE,  8,
            GLX_DOUBLEBUFFER,  True,
            None
        };

        int fb_count = 0;
        GLXFBConfig *fb_configs = glXChooseFBConfig(g_display, screen, fb_attribs, &fb_count);
        if (!fb_configs || fb_count == 0) {
            fprintf(stderr, "GLX ERROR: No framebuffer configs found!\n");
            if (fb_configs) XFree(fb_configs);
            free(window);
            return NULL;
        }

        fb = fb_configs[0];
        XVisualInfo *vi = glXGetVisualFromFBConfig(g_display, fb);
        if (!vi) {
            fprintf(stderr, "GLX ERROR: Failed to get XVisualInfo from FBConfig!\n");
            XFree(fb_configs);
            free(window);
            return NULL;
        }

        visual_info = *vi;
        XFree(vi);
        XFree(fb_configs);

        colormap = XCreateColormap(g_display, RootWindow(g_display, screen),
                                   visual_info.visual, AllocNone);
    } else {
        /* Non-OpenGL graphics APIs (unimplemented) */
        visual_info.visual = DefaultVisual(g_display, screen);
        visual_info.depth  = DefaultDepth(g_display, screen);
        colormap = XCreateColormap(g_display, RootWindow(g_display, screen),
                                   visual_info.visual, AllocNone);
    }

    /* --- Create the X11 window --- */
    XSetWindowAttributes swa;
    swa.colormap = colormap;
    swa.override_redirect = False;
    swa.background_pixel = WhitePixel(g_display, screen);
    swa.border_pixel = BlackPixel(g_display, screen);
    swa.event_mask = ExposureMask | KeyPressMask | StructureNotifyMask;

    unsigned long valuemask = CWColormap | CWBackPixel | CWBorderPixel | CWEventMask | CWOverrideRedirect;

    window->window = XCreateWindow(g_display, RootWindow(g_display, screen),
                                   0, 0, width, height, 0,
                                   visual_info.depth,
                                   InputOutput,
                                   visual_info.visual,
                                   valuemask, &swa);
    if (!window->window) {
        fprintf(stderr, "XCreateWindow failed!\n");
        XFreeColormap(g_display, colormap);
        free(window);
        return NULL;
    }

    XStoreName(g_display, window->window, window_title);

    /* --- Optional: set class hints, WM protocols, etc. --- */
    XClassHint *class_hint = XAllocClassHint();
    if (class_hint) {
        class_hint->res_name = "my_app";
        class_hint->res_class = "MyApp";
        XSetClassHint(g_display, window->window, class_hint);
        XFree(class_hint);
    }
    XSetWMProtocols(g_display, window->window, &g_wm_delete, 1);

    /* --- Create OpenGL context if requested --- */
    window->gl_context = NULL;
    if (flags & PAL_WINDOW_OPENGL) {
        PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB =
            (void*)glXGetProcAddressARB((const GLubyte*)"glXCreateContextAttribsARB");

        int context_attribs[] = {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
            GLX_CONTEXT_MINOR_VERSION_ARB, 3,
            GLX_CONTEXT_PROFILE_MASK_ARB,  GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
            None
        };

        if (glXCreateContextAttribsARB) {
            window->gl_context = glXCreateContextAttribsARB(g_display, fb, NULL, True, context_attribs);
        }

        if (!window->gl_context) {
            window->gl_context = glXCreateNewContext(g_display, fb, GLX_RGBA_TYPE, NULL, True);
        }

        if (!window->gl_context) {
            fprintf(stderr, "Failed to create OpenGL context!\n");
            XDestroyWindow(g_display, window->window);
            XFreeColormap(g_display, colormap);
            free(window);
            return NULL;
        }
    }

    /* --- Map window --- */
    XMapWindow(g_display, window->window);

    /* --- Optional GC --- */
    window->graphics_context = XCreateGC(g_display, window->window, 0, NULL);

    return window;
}

PALAPI void pal_close_window(pal_window *window) {
    if(window->graphics_context) {
        XFreeGC(g_display, window->graphics_context);
    }
    if(window->window) {
        XDestroyWindow(g_display, window->window);
    }

    pal_event event = {0};
    event.type = PAL_EVENT_WINDOW_CLOSED;
    pal__eventq_push(&g_event_queue, event);
}

static void linux_x11_send_wm_state_message(Window win, long action, Atom property) {
    XEvent e;
    pal_memset(&e, 0, sizeof(e));
    e.xclient.type = ClientMessage;
    e.xclient.serial = 0;
    e.xclient.send_event = True;
    e.xclient.message_type = XInternAtom(g_display, "_NET_WM_STATE", False);
    e.xclient.window = win;
    e.xclient.format = 32;
    e.xclient.data.l[0] = action;      /* 1 = add, 0 = remove, 2 = toggle */
    e.xclient.data.l[1] = property;    /* _NET_WM_STATE_MAXIMIZED_VERT or HORZ */
    e.xclient.data.l[2] = 0;           /* second property (unused) */
    e.xclient.data.l[3] = 1;           /* source: application */
    e.xclient.data.l[4] = 0;

    XSendEvent(
        g_display,
        DefaultRootWindow(g_display),
        False,
        SubstructureNotifyMask | SubstructureRedirectMask,
        &e
    );
}

PALAPI pal_bool pal_set_window_title(pal_window *window, const char *string) {
    if (!window || !string) return pal_false;

    XStoreName(g_display, window->window, string);
    XSetIconName(g_display, window->window, string);
    XFlush(g_display);

    return pal_true;
}

PALAPI pal_bool pal_make_window_fullscreen(pal_window *window) {
    if (!window) return pal_false;

    Atom fullscreen = XInternAtom(g_display, "_NET_WM_STATE_FULLSCREEN", False);
    linux_x11_send_wm_state_message(window->window, 1, fullscreen);

    XFlush(g_display);
    return pal_true;
}

PALAPI pal_bool pal_make_window_fullscreen_ex(pal_window *window,
                                              int width,
                                              int height,
                                              int refresh_rate) {
    if (!window) return pal_false;

    Window root = DefaultRootWindow(g_display);

    XRRScreenResources *res = XRRGetScreenResources(g_display, root);
    if (!res) return pal_false;

    RROutput primary = XRRGetOutputPrimary(g_display, root);
    if (!primary)
        primary = res->outputs[0];

    XRROutputInfo *out = XRRGetOutputInfo(g_display, res, primary);
    if (!out) {
        XRRFreeScreenResources(res);
        return pal_false;
    }

    XRRCrtcInfo *crtc = XRRGetCrtcInfo(g_display, res, out->crtc);
    if (!crtc) {
        XRRFreeOutputInfo(out);
        XRRFreeScreenResources(res);
        return pal_false;
    }

    /* Find matching mode */
    RRMode selected_mode = 0;
    for (int i = 0; i < res->nmode; i++) {
        XRRModeInfo *m = &res->modes[i];

        int mode_refresh =
            (int)(m->dotClock / (m->hTotal * m->vTotal));

        if (m->width == (unsigned int)width &&
            m->height == (unsigned int)height &&
            mode_refresh == refresh_rate)
        {
            selected_mode = m->id;
            break;
        }
    }

    if (!selected_mode) {
        XRRFreeCrtcInfo(crtc);
        XRRFreeOutputInfo(out);
        XRRFreeScreenResources(res);
        return pal_false;
    }

    /* Apply the mode */
    XRRSetCrtcConfig(
        g_display,
        res,
        out->crtc,
        CurrentTime,
        crtc->x, crtc->y,
        selected_mode,
        crtc->rotation,
        &primary,
        1
    );

    XRRFreeCrtcInfo(crtc);
    XRRFreeOutputInfo(out);
    XRRFreeScreenResources(res);

    /* Then set fullscreen */
    pal_make_window_fullscreen(window);

    return pal_true;
}

PALAPI pal_bool pal_make_window_fullscreen_windowed(pal_window *window)
{
    if (!window) return pal_false;

    Atom wmHints = XInternAtom(g_display, "_MOTIF_WM_HINTS", False);

    /* Remove decorations using Motif WM hints */
    struct {
        unsigned long flags;
        unsigned long functions;
        unsigned long decorations;
        long inputMode;
        unsigned long status;
    } hints;

    hints.flags = (1L << 1); /* MWM_HINTS_DECORATIONS */
    hints.decorations = 0;   /* No decorations */

    XChangeProperty(
        g_display,
        window->window,
        wmHints,
        wmHints,
        32,
        PropModeReplace,
        (unsigned char*)&hints,
        5
    );

    /* Resize window to match screen size */
    int sw = DisplayWidth(g_display, DefaultScreen(g_display));
    int sh = DisplayHeight(g_display, DefaultScreen(g_display));

    XMoveResizeWindow(g_display, window->window, 0, 0, sw, sh);

    XFlush(g_display);
    return pal_true;
}

PALAPI pal_bool pal_make_window_windowed(pal_window *window) {
    if (!window) return pal_false;

    Atom wmHints = XInternAtom(g_display, "_MOTIF_WM_HINTS", False);

    struct {
        unsigned long flags;
        unsigned long functions;
        unsigned long decorations;
        long inputMode;
        unsigned long status;
    } hints;

    /* Restore decorations */
    hints.flags = (1L << 1);
    hints.decorations = 1;  /* enable */

    XChangeProperty(
        g_display,
        window->window,
        wmHints,
        wmHints,
        32,
        PropModeReplace,
        (unsigned char *)&hints,
        5
    );

    /* Remove fullscreen state (if any) */
    Atom fullscreen = XInternAtom(g_display, "_NET_WM_STATE_FULLSCREEN", False);
    linux_x11_send_wm_state_message(window->window, 0, fullscreen);

    XFlush(g_display);
    return pal_true;
}
PALAPI pal_bool pal_maximize_window(pal_window *window)
{
    if (!window || !g_display)
        return pal_false;

    Atom max_vert = XInternAtom(g_display, "_NET_WM_STATE_MAXIMIZED_VERT", False);
    Atom max_horz = XInternAtom(g_display, "_NET_WM_STATE_MAXIMIZED_HORZ", False);

    /* action = 1 → add */
    linux_x11_send_wm_state_message(window->window, 1, max_vert);
    linux_x11_send_wm_state_message(window->window, 1, max_horz);

    XFlush(g_display);
    return pal_true;
}

PALAPI pal_bool pal_minimize_window(pal_window *window)
{
    if (!window || !g_display)
        return pal_false;

    /* Minimize is via _NET_ACTIVE_WINDOW with a special request */
    XIconifyWindow(
        g_display,
        window->window,
        DefaultScreen(g_display)
    );

    XFlush(g_display);
    return pal_true;
}

PALAPI void pal_draw_rect(pal_window *window, int x, int y, int width, int height, pal_vec4 color) {
    if (!window) return;

    /* Convert float color (0.0 - 1.0) to 24-bit RGB */
    unsigned long pixel = ((unsigned long)(color.r * 255) << 16) |
                          ((unsigned long)(color.g * 255) << 8)  |
                          ((unsigned long)(color.b * 255) << 0);

    XSetForeground(g_display, window->graphics_context, pixel);
    XFillRectangle(g_display, window->window, window->graphics_context, x, y, width, height);
}

/* Helper to intern an atom */
PALAPI int pal_make_context_current(pal_window *window) {
    if (window && window->gl_context) {
        if (!glXMakeCurrent(g_display, window->window, window->gl_context)) {
            fprintf(stderr, "GLX ERROR: glXMakeCurrent failed!\n");
            return -1;
        }
    } else {
        glXMakeCurrent(g_display, None, NULL); /* unbind context */
    }
    return 0;
}

PALAPI void pal_swap_buffers(pal_window *window) {
    glXSwapBuffers(g_display, window->window);
}

PALAPI int pal_show_cursor(pal_window *window) {
    int event_base, error_base;
    if (!XFixesQueryExtension(g_display, &event_base, &error_base)) {
        fprintf(stderr, "XFixes extension not available.\n");
        return -1;
    }

    XFixesShowCursor(g_display, window->window);
    XFlush(g_display);
    return 0;
}

PALAPI int pal_hide_cursor(pal_window *window) {
    int event_base, error_base;
    if (!XFixesQueryExtension(g_display, &event_base, &error_base)) {
        fprintf(stderr, "XFixes extension not available.\n");
        return -1;
    }

    XFixesHideCursor(g_display, window->window);
    XFlush(g_display);
    return 0;
}

PALAPI pal_monitor *pal_get_primary_monitor(void) {
    return NULL;
}

PALAPI pal_bool pal_set_video_mode(pal_video_mode *mode) {
    if (!mode) return pal_false;

    if (!g_display) {
        fprintf(stderr, "Cannot open X display\n");
        return pal_false;
    }

    Window root = DefaultRootWindow(g_display);
    XRRScreenResources *res = XRRGetScreenResourcesCurrent(g_display, root);
    if (!res) {
        XCloseDisplay(g_display);
        return pal_false;
    }

    /* Pick the first connected output */
    RROutput output = 0;
    for (int i = 0; i < res->noutput; i++) {
        XRROutputInfo *outInfo = XRRGetOutputInfo(g_display, res, res->outputs[i]);
        if (outInfo->connection == RR_Connected) {
            output = res->outputs[i];
            XRRFreeOutputInfo(outInfo);
            break;
        }
        XRRFreeOutputInfo(outInfo);
    }

    if (output == 0) {
        XRRFreeScreenResources(res);
        XCloseDisplay(g_display);
        return pal_false;
    }

    XRROutputInfo *outInfo = XRRGetOutputInfo(g_display, res, output);
    if (!outInfo || outInfo->crtc == 0) {
        if (outInfo) XRRFreeOutputInfo(outInfo);
        XRRFreeScreenResources(res);
        XCloseDisplay(g_display);
        return pal_false;
    }

    XRRCrtcInfo *crtcInfo = XRRGetCrtcInfo(g_display, res, outInfo->crtc);
    if (!crtcInfo) {
        XRRFreeOutputInfo(outInfo);
        XRRFreeScreenResources(res);
        XCloseDisplay(g_display);
        return pal_false;
    }

    /* Find a mode that matches width, height, and refresh rate */
    RRMode newMode = 0;
    for (int i = 0; i < res->nmode; i++) {
        XRRModeInfo *m = &res->modes[i];
        int hRate = (m->hTotal && m->vTotal) ? (int)((double)m->dotClock / (m->hTotal * m->vTotal) + 0.5) : 0;
        if (m->width == (unsigned int)mode->width && m->height == (unsigned int)mode->height &&
            (mode->refresh_rate == 0 || hRate == mode->refresh_rate)) {
            newMode = m->id;
            break;
        }
    }

    if (newMode == 0) {
        fprintf(stderr, "No matching mode found\n");
        XRRFreeCrtcInfo(crtcInfo);
        XRRFreeOutputInfo(outInfo);
        XRRFreeScreenResources(res);
        XCloseDisplay(g_display);
        return pal_false;
    }

    /* Apply the new mode */
    Status status = XRRSetCrtcConfig(g_display, res, outInfo->crtc, CurrentTime,
                                     crtcInfo->x, crtcInfo->y,
                                     newMode, crtcInfo->rotation,
                                     &output, 1);

    XRRFreeCrtcInfo(crtcInfo);
    XRRFreeOutputInfo(outInfo);
    XRRFreeScreenResources(res);
    XCloseDisplay(g_display);

    return (status == RRSetConfigSuccess) ? pal_true : pal_false;
}

PALAPI pal_video_mode *pal_get_video_mode(pal_monitor *monitor) {
    if (!monitor || !monitor->display) return NULL;

    Window root = DefaultRootWindow(g_display);
    XRRScreenResources *screenRes = XRRGetScreenResourcesCurrent(g_display, root);
    if (!screenRes) return NULL;

    XRROutputInfo *outputInfo = XRRGetOutputInfo(g_display, screenRes, monitor->output);
    if (!outputInfo || outputInfo->connection != RR_Connected || outputInfo->crtc == 0) {
        if (outputInfo) XRRFreeOutputInfo(outputInfo);
        XRRFreeScreenResources(screenRes);
        return NULL;
    }

    XRRCrtcInfo *crtcInfo = XRRGetCrtcInfo(g_display, screenRes, outputInfo->crtc);
    XRRModeInfo *modeInfo = NULL;
    for (int i = 0; i < screenRes->nmode; i++) {
        if (screenRes->modes[i].id == crtcInfo->mode) {
            modeInfo = &screenRes->modes[i];
            break;
        }
    }

    pal_video_mode *mode = malloc(sizeof(pal_video_mode));
    if (!mode) {
        XRRFreeCrtcInfo(crtcInfo);
        XRRFreeOutputInfo(outputInfo);
        XRRFreeScreenResources(screenRes);
        return NULL;
    }

    mode->width  = crtcInfo->width;
    mode->height = crtcInfo->height;
    mode->refresh_rate = (modeInfo && modeInfo->hTotal && modeInfo->vTotal) ?
        (int)((double)modeInfo->dotClock / (modeInfo->hTotal * modeInfo->vTotal) + 0.5) : 0;

    XWindowAttributes attrs;
    if (XGetWindowAttributes(g_display, root, &attrs)) {
        mode->bits_per_pixel = attrs.depth;
    } else {
        mode->bits_per_pixel = 0;
    }

    XRRFreeCrtcInfo(crtcInfo);
    XRRFreeOutputInfo(outputInfo);
    XRRFreeScreenResources(screenRes);

    return mode;
}

PALAPI void *pal_gl_get_proc_address(const char *procname) {
    return (void *)glXGetProcAddress((const GLubyte *)procname);
}

#endif /* PLATFORM_LINUX_X11_H */
#elif defined(PAL_PLATFORM_LINUX_WAYLAND) && !defined (PAL_PLATFORM_LINUX_X11) && !defined (PAL_PLATFORM_WINDOWS)
#ifndef PLATFORM_LINUX_WAYLAND_H
#define PLATFORM_LINUX_WAYLAND_H
#define _POSIX_C_SOURCE 200112L
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/un.h>
#include <unistd.h>

#define cstring_len(s) (sizeof(s) - 1)
#define roundup_4(n) (((n) + 3) & -4)

// Wayland protocol constants
static const uint32_t wayland_display_object_id = 1;
static const uint16_t wayland_wl_registry_event_global = 0;
static const uint16_t wayland_shm_pool_event_format = 0;
static const uint16_t wayland_wl_buffer_event_release = 0;
static const uint16_t wayland_xdg_wm_base_event_ping = 0;
static const uint16_t wayland_xdg_toplevel_event_configure = 0;
static const uint16_t wayland_xdg_toplevel_event_close = 1;
static const uint16_t wayland_xdg_surface_event_configure = 0;
static const uint16_t wayland_wl_display_get_registry_opcode = 1;
static const uint16_t wayland_wl_registry_bind_opcode = 0;
static const uint16_t wayland_wl_compositor_create_surface_opcode = 0;
static const uint16_t wayland_xdg_wm_base_pong_opcode = 3;
static const uint16_t wayland_xdg_surface_ack_configure_opcode = 4;
static const uint16_t wayland_wl_shm_create_pool_opcode = 0;
static const uint16_t wayland_xdg_wm_base_get_xdg_surface_opcode = 2;
static const uint16_t wayland_wl_shm_pool_create_buffer_opcode = 0;
static const uint16_t wayland_wl_surface_attach_opcode = 1;
static const uint16_t wayland_xdg_surface_get_toplevel_opcode = 1;
static const uint16_t wayland_wl_surface_commit_opcode = 6;
static const uint16_t wayland_wl_display_error_event = 0;
static const uint16_t wayland_xdg_toplevel_set_title_opcode = 2;
static const uint32_t wayland_format_xrgb8888 = 1;
static const uint32_t wayland_header_size = 8;
static const uint32_t color_channels = 4;

typedef enum {
    WINDOW_STATE_NONE,
    WINDOW_STATE_CREATED,
    WINDOW_STATE_CONFIGURED,
    WINDOW_STATE_READY,
    WINDOW_STATE_CLOSED
} window_state_t;

typedef struct pal_window {
    uint32_t id;
    uint32_t wl_registry;
    uint32_t wl_shm;
    uint32_t wl_shm_pool;
    uint32_t wl_buffer;
    uint32_t xdg_wm_base;
    uint32_t xdg_surface;
    uint32_t wl_compositor;
    uint32_t wl_surface;
    uint32_t xdg_toplevel;
    uint32_t stride;
    uint32_t width;
    uint32_t height;
    uint32_t shm_pool_size;
    int shm_fd;
    uint8_t *shm_pool_data;
    window_state_t state;
    int should_close;
} pal_window;

// Global state
static int g_wayland_fd = -1;
static uint32_t g_wayland_current_id = 1;
static pal_window **g_windows = NULL;
static size_t g_window_count = 0;
static size_t g_window_capacity = 0;

// Buffer utility functions
static void buf_write_u32(char *buf, uint64_t *buf_size, uint64_t buf_cap, uint32_t x) {
    assert(*buf_size + sizeof(x) <= buf_cap);
    assert(((size_t)buf + *buf_size) % sizeof(x) == 0);
    *(uint32_t *)(buf + *buf_size) = x;
    *buf_size += sizeof(x);
}

static void buf_write_u16(char *buf, uint64_t *buf_size, uint64_t buf_cap, uint16_t x) {
    assert(*buf_size + sizeof(x) <= buf_cap);
    assert(((size_t)buf + *buf_size) % sizeof(x) == 0);
    *(uint16_t *)(buf + *buf_size) = x;
    *buf_size += sizeof(x);
}

static void buf_write_string(char *buf, uint64_t *buf_size, uint64_t buf_cap, const char *src, uint32_t src_len) {
    assert(*buf_size + src_len <= buf_cap);
    buf_write_u32(buf, buf_size, buf_cap, src_len);
    memcpy(buf + *buf_size, src, src_len);
    uint32_t padded = roundup_4(src_len);
    if (padded > src_len) {
        memset(buf + *buf_size + src_len, 0, padded - src_len);
    }
    *buf_size += padded;
}

static uint32_t buf_read_u32(char **buf, uint64_t *buf_size) {
    assert(*buf_size >= sizeof(uint32_t));
    assert((size_t)*buf % sizeof(uint32_t) == 0);
    uint32_t res = *(uint32_t *)(*buf);
    *buf += sizeof(res);
    *buf_size -= sizeof(res);
    return res;
}

static uint16_t buf_read_u16(char **buf, uint64_t *buf_size) {
    assert(*buf_size >= sizeof(uint16_t));
    assert((size_t)*buf % sizeof(uint16_t) == 0);
    uint16_t res = *(uint16_t *)(*buf);
    *buf += sizeof(res);
    *buf_size -= sizeof(res);
    return res;
}

static void buf_read_n(char **buf, uint64_t *buf_size, char *dst, uint64_t n) {
    assert(*buf_size >= n);
    memcpy(dst, *buf, n);
    *buf += n;
    *buf_size -= n;
}

// Window registry functions
static void register_window(pal_window *window) {
    if (g_window_count >= g_window_capacity) {
        size_t new_capacity = g_window_capacity == 0 ? 4 : g_window_capacity * 2;
        pal_window **new_windows = (pal_window**)realloc(g_windows, new_capacity * sizeof(pal_window *));
        if (new_windows) {
            g_windows = new_windows;
            g_window_capacity = new_capacity;
        }
    }
    
    if (g_window_count < g_window_capacity) {
        g_windows[g_window_count++] = window;
    }
}

static void unregister_window(pal_window *window) {
    for (size_t i = 0; i < g_window_count; i++) {
        if (g_windows[i] == window) {
            g_windows[i] = g_windows[g_window_count - 1];
            g_window_count--;
            break;
        }
    }
}

static pal_window *find_window_by_object_id(uint32_t object_id) {
    for (size_t i = 0; i < g_window_count; i++) {
        pal_window *w = g_windows[i];
        if (w->wl_surface == object_id || w->xdg_surface == object_id ||
            w->xdg_toplevel == object_id || w->wl_buffer == object_id ||
            w->xdg_wm_base == object_id || w->wl_registry == object_id ||
            w->wl_shm == object_id) {
            return w;
        }
    }
    return NULL;
}

// Wayland protocol functions
static int wayland_display_connect() {
    char *xdg_runtime_dir = getenv("XDG_RUNTIME_DIR");
    if (xdg_runtime_dir == NULL) {
        return -1;
    }

    uint64_t xdg_runtime_dir_len = pal_strlen(xdg_runtime_dir);
    struct sockaddr_un addr = {.sun_family = AF_UNIX};
    
    if (xdg_runtime_dir_len > sizeof(addr.sun_path) - 20) {
        return -1;
    }

    uint64_t socket_path_len = 0;
    memcpy(addr.sun_path, xdg_runtime_dir, xdg_runtime_dir_len);
    socket_path_len += xdg_runtime_dir_len;
    addr.sun_path[socket_path_len++] = '/';

    char *wayland_display = getenv("WAYLAND_DISPLAY");
    if (wayland_display == NULL) {
        char wayland_display_default[] = "wayland-0";
        uint64_t wayland_display_default_len = cstring_len(wayland_display_default);
        memcpy(addr.sun_path + socket_path_len, wayland_display_default, wayland_display_default_len);
        socket_path_len += wayland_display_default_len;
    } else {
        uint64_t wayland_display_len = pal_strlen(wayland_display);
        memcpy(addr.sun_path + socket_path_len, wayland_display, wayland_display_len);
        socket_path_len += wayland_display_len;
    }

    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1) {
        return -1;
    }

    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        close(fd);
        return -1;
    }

    return fd;
}

static uint32_t wayland_wl_display_get_registry(int fd) {
    uint64_t msg_size = 0;
    char msg[128] = "";
    buf_write_u32(msg, &msg_size, sizeof(msg), wayland_display_object_id);
    buf_write_u16(msg, &msg_size, sizeof(msg), wayland_wl_display_get_registry_opcode);
    
    uint16_t msg_announced_size = wayland_header_size + sizeof(g_wayland_current_id);
    buf_write_u16(msg, &msg_size, sizeof(msg), msg_announced_size);
    
    g_wayland_current_id++;
    buf_write_u32(msg, &msg_size, sizeof(msg), g_wayland_current_id);

    if ((int64_t)msg_size != send(fd, msg, msg_size, 0)) {
        return 0;
    }

    return g_wayland_current_id;
}

static uint32_t wayland_wl_registry_bind(int fd, uint32_t registry, uint32_t name, const char *interface, uint32_t interface_len, uint32_t version) {
    uint64_t msg_size = 0;
    char msg[512] = "";
    buf_write_u32(msg, &msg_size, sizeof(msg), registry);
    buf_write_u16(msg, &msg_size, sizeof(msg), wayland_wl_registry_bind_opcode);

    uint16_t msg_announced_size = wayland_header_size + sizeof(name) + sizeof(interface_len) +
                                  roundup_4(interface_len) + sizeof(version) + sizeof(g_wayland_current_id);
    buf_write_u16(msg, &msg_size, sizeof(msg), msg_announced_size);

    buf_write_u32(msg, &msg_size, sizeof(msg), name);
    buf_write_string(msg, &msg_size, sizeof(msg), interface, interface_len);
    buf_write_u32(msg, &msg_size, sizeof(msg), version);

    g_wayland_current_id++;
    buf_write_u32(msg, &msg_size, sizeof(msg), g_wayland_current_id);

    if ((int64_t)msg_size != send(fd, msg, msg_size, 0)) {
        return 0;
    }

    return g_wayland_current_id;
}

static uint32_t wayland_wl_compositor_create_surface(int fd, uint32_t compositor) {
    uint64_t msg_size = 0;
    char msg[128] = "";
    buf_write_u32(msg, &msg_size, sizeof(msg), compositor);
    buf_write_u16(msg, &msg_size, sizeof(msg), wayland_wl_compositor_create_surface_opcode);

    uint16_t msg_announced_size = wayland_header_size + sizeof(g_wayland_current_id);
    buf_write_u16(msg, &msg_size, sizeof(msg), msg_announced_size);

    g_wayland_current_id++;
    buf_write_u32(msg, &msg_size, sizeof(msg), g_wayland_current_id);

    if ((int64_t)msg_size != send(fd, msg, msg_size, 0)) {
        return 0;
    }

    return g_wayland_current_id;
}

static void create_shared_memory_file(uint64_t size, pal_window *window) {
    char name[256] = "/pal-shm-";
    for (uint64_t i = 9; i < 255; i++) {
        name[i] = ((double)rand()) / (double)RAND_MAX * 26 + 'a';
    }
    name[255] = '\0';

    int fd = shm_open(name, O_RDWR | O_EXCL | O_CREAT, 0600);
    if (fd == -1) {
        return;
    }

    shm_unlink(name);

    if (ftruncate(fd, size) == -1) {
        close(fd);
        return;
    }

    window->shm_pool_data = (uint8_t*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (window->shm_pool_data == MAP_FAILED) {
        close(fd);
        window->shm_pool_data = NULL;
        return;
    }
    
    window->shm_fd = fd;
}

static void wayland_xdg_wm_base_pong(int fd, uint32_t xdg_wm_base, uint32_t ping) {
    uint64_t msg_size = 0;
    char msg[128] = "";
    buf_write_u32(msg, &msg_size, sizeof(msg), xdg_wm_base);
    buf_write_u16(msg, &msg_size, sizeof(msg), wayland_xdg_wm_base_pong_opcode);

    uint16_t msg_announced_size = wayland_header_size + sizeof(ping);
    buf_write_u16(msg, &msg_size, sizeof(msg), msg_announced_size);
    buf_write_u32(msg, &msg_size, sizeof(msg), ping);

    send(fd, msg, msg_size, 0);
}

static void wayland_xdg_surface_ack_configure(int fd, uint32_t xdg_surface, uint32_t configure) {
    uint64_t msg_size = 0;
    char msg[128] = "";
    buf_write_u32(msg, &msg_size, sizeof(msg), xdg_surface);
    buf_write_u16(msg, &msg_size, sizeof(msg), wayland_xdg_surface_ack_configure_opcode);

    uint16_t msg_announced_size = wayland_header_size + sizeof(configure);
    buf_write_u16(msg, &msg_size, sizeof(msg), msg_announced_size);
    buf_write_u32(msg, &msg_size, sizeof(msg), configure);

    send(fd, msg, msg_size, 0);
}

static uint32_t wayland_wl_shm_create_pool(int fd, pal_window *window) {
    uint64_t msg_size = 0;
    char msg[128] = "";
    buf_write_u32(msg, &msg_size, sizeof(msg), window->wl_shm);
    buf_write_u16(msg, &msg_size, sizeof(msg), wayland_wl_shm_create_pool_opcode);

    uint16_t msg_announced_size = wayland_header_size + sizeof(g_wayland_current_id) + sizeof(window->shm_pool_size);
    buf_write_u16(msg, &msg_size, sizeof(msg), msg_announced_size);

    g_wayland_current_id++;
    buf_write_u32(msg, &msg_size, sizeof(msg), g_wayland_current_id);
    buf_write_u32(msg, &msg_size, sizeof(msg), window->shm_pool_size);

    char buf[CMSG_SPACE(sizeof(window->shm_fd))];
    memset(buf, 0, sizeof(buf));

    struct iovec io = {.iov_base = msg, .iov_len = msg_size};
    struct msghdr socket_msg = {
        .msg_iov = &io,
        .msg_iovlen = 1,
        .msg_control = buf,
        .msg_controllen = sizeof(buf),
    };

    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&socket_msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(window->shm_fd));
    *((int *)CMSG_DATA(cmsg)) = window->shm_fd;

    if (sendmsg(fd, &socket_msg, 0) == -1) {
        return 0;
    }

    return g_wayland_current_id;
}

static uint32_t wayland_xdg_wm_base_get_xdg_surface(int fd, uint32_t xdg_wm_base, uint32_t wl_surface) {
    uint64_t msg_size = 0;
    char msg[128] = "";
    buf_write_u32(msg, &msg_size, sizeof(msg), xdg_wm_base);
    buf_write_u16(msg, &msg_size, sizeof(msg), wayland_xdg_wm_base_get_xdg_surface_opcode);

    uint16_t msg_announced_size = wayland_header_size + sizeof(g_wayland_current_id) + sizeof(wl_surface);
    buf_write_u16(msg, &msg_size, sizeof(msg), msg_announced_size);

    g_wayland_current_id++;
    buf_write_u32(msg, &msg_size, sizeof(msg), g_wayland_current_id);
    buf_write_u32(msg, &msg_size, sizeof(msg), wl_surface);

    if ((int64_t)msg_size != send(fd, msg, msg_size, 0)) {
        return 0;
    }

    return g_wayland_current_id;
}

static uint32_t wayland_wl_shm_pool_create_buffer(int fd, pal_window *window) {
    uint64_t msg_size = 0;
    char msg[128] = "";
    buf_write_u32(msg, &msg_size, sizeof(msg), window->wl_shm_pool);
    buf_write_u16(msg, &msg_size, sizeof(msg), wayland_wl_shm_pool_create_buffer_opcode);

    uint16_t msg_announced_size = wayland_header_size + sizeof(g_wayland_current_id) + sizeof(uint32_t) * 5;
    buf_write_u16(msg, &msg_size, sizeof(msg), msg_announced_size);

    g_wayland_current_id++;
    buf_write_u32(msg, &msg_size, sizeof(msg), g_wayland_current_id);

    uint32_t offset = 0;
    buf_write_u32(msg, &msg_size, sizeof(msg), offset);
    buf_write_u32(msg, &msg_size, sizeof(msg), window->width);
    buf_write_u32(msg, &msg_size, sizeof(msg), window->height);
    buf_write_u32(msg, &msg_size, sizeof(msg), window->stride);
    buf_write_u32(msg, &msg_size, sizeof(msg), wayland_format_xrgb8888);

    if ((int64_t)msg_size != send(fd, msg, msg_size, 0)) {
        return 0;
    }

    return g_wayland_current_id;
}

static void wayland_wl_surface_attach(int fd, uint32_t wl_surface, uint32_t wl_buffer) {
    uint64_t msg_size = 0;
    char msg[128] = "";
    buf_write_u32(msg, &msg_size, sizeof(msg), wl_surface);
    buf_write_u16(msg, &msg_size, sizeof(msg), wayland_wl_surface_attach_opcode);

    uint16_t msg_announced_size = wayland_header_size + sizeof(wl_buffer) + sizeof(uint32_t) * 2;
    buf_write_u16(msg, &msg_size, sizeof(msg), msg_announced_size);
    buf_write_u32(msg, &msg_size, sizeof(msg), wl_buffer);

    uint32_t x = 0, y = 0;
    buf_write_u32(msg, &msg_size, sizeof(msg), x);
    buf_write_u32(msg, &msg_size, sizeof(msg), y);

    send(fd, msg, msg_size, 0);
}

static uint32_t wayland_xdg_surface_get_toplevel(int fd, uint32_t xdg_surface) {
    uint64_t msg_size = 0;
    char msg[128] = "";
    buf_write_u32(msg, &msg_size, sizeof(msg), xdg_surface);
    buf_write_u16(msg, &msg_size, sizeof(msg), wayland_xdg_surface_get_toplevel_opcode);

    uint16_t msg_announced_size = wayland_header_size + sizeof(g_wayland_current_id);
    buf_write_u16(msg, &msg_size, sizeof(msg), msg_announced_size);

    g_wayland_current_id++;
    buf_write_u32(msg, &msg_size, sizeof(msg), g_wayland_current_id);

    if ((int64_t)msg_size != send(fd, msg, msg_size, 0)) {
        return 0;
    }

    return g_wayland_current_id;
}

static void wayland_xdg_toplevel_set_title(int fd, uint32_t xdg_toplevel, const char *title) {
    uint32_t title_len = pal_strlen(title) + 1; // Include null terminator
    
    uint64_t msg_size = 0;
    char msg[512] = "";
    buf_write_u32(msg, &msg_size, sizeof(msg), xdg_toplevel);
    buf_write_u16(msg, &msg_size, sizeof(msg), wayland_xdg_toplevel_set_title_opcode);

    uint16_t msg_announced_size = wayland_header_size + sizeof(title_len) + roundup_4(title_len);
    buf_write_u16(msg, &msg_size, sizeof(msg), msg_announced_size);
    buf_write_string(msg, &msg_size, sizeof(msg), title, title_len);

    send(fd, msg, msg_size, 0);
}

static void wayland_wl_surface_commit(int fd, uint32_t wl_surface) {
    uint64_t msg_size = 0;
    char msg[128] = "";
    buf_write_u32(msg, &msg_size, sizeof(msg), wl_surface);
    buf_write_u16(msg, &msg_size, sizeof(msg), wayland_wl_surface_commit_opcode);

    uint16_t msg_announced_size = wayland_header_size;
    buf_write_u16(msg, &msg_size, sizeof(msg), msg_announced_size);

    send(fd, msg, msg_size, 0);
}

static void wayland_handle_message(int fd, pal_window *window, char **msg, uint64_t *msg_len) {
    if (*msg_len < 8) {
        return;
    }

    uint32_t object_id = buf_read_u32(msg, msg_len);
    uint16_t opcode = buf_read_u16(msg, msg_len);
    uint16_t announced_size = buf_read_u16(msg, msg_len);

    if (object_id == window->wl_registry && opcode == wayland_wl_registry_event_global) {
        uint32_t name = buf_read_u32(msg, msg_len);
        uint32_t interface_len = buf_read_u32(msg, msg_len);
        uint32_t padded_interface_len = roundup_4(interface_len);

        char interface[512] = "";
        buf_read_n(msg, msg_len, interface, padded_interface_len);
        uint32_t version = buf_read_u32(msg, msg_len);

        if (strcmp("wl_shm", interface) == 0) {
            window->wl_shm = wayland_wl_registry_bind(fd, window->wl_registry, name, interface, interface_len, version);
        } else if (strcmp("xdg_wm_base", interface) == 0) {
            window->xdg_wm_base = wayland_wl_registry_bind(fd, window->wl_registry, name, interface, interface_len, version);
        } else if (strcmp("wl_compositor", interface) == 0) {
            window->wl_compositor = wayland_wl_registry_bind(fd, window->wl_registry, name, interface, interface_len, version);
        }
    } else if (object_id == wayland_display_object_id && opcode == wayland_wl_display_error_event) {
        uint32_t target_object_id = buf_read_u32(msg, msg_len);
        uint32_t code = buf_read_u32(msg, msg_len);
        uint32_t error_len = buf_read_u32(msg, msg_len);
        char error[512] = "";
        buf_read_n(msg, msg_len, error, roundup_4(error_len));
        fprintf(stderr, "Wayland error: object=%u code=%u error=%s\n", target_object_id, code, error);
    } else if (object_id == window->wl_shm && opcode == wayland_shm_pool_event_format) {
        buf_read_u32(msg, msg_len); // format
    } else if (object_id == window->wl_buffer && opcode == wayland_wl_buffer_event_release) {
        // Buffer released
    } else if (object_id == window->xdg_wm_base && opcode == wayland_xdg_wm_base_event_ping) {
        uint32_t ping = buf_read_u32(msg, msg_len);
        wayland_xdg_wm_base_pong(fd, window->xdg_wm_base, ping);
    } else if (object_id == window->xdg_toplevel && opcode == wayland_xdg_toplevel_event_configure) {
        uint32_t w = buf_read_u32(msg, msg_len);
        uint32_t h = buf_read_u32(msg, msg_len);
        uint32_t len = buf_read_u32(msg, msg_len);
        char buf[256] = "";
        if (len <= sizeof(buf)) {
            buf_read_n(msg, msg_len, buf, len);
        }
        
        // Queue resize event if dimensions changed
        if ((w > 0 && w != window->width) || (h > 0 && h != window->height)) {
            pal_event event = {0};
            event.type = PAL_EVENT_WINDOW_RESIZED;
            event.window.windowid = window->id;
            event.window.width = w > 0 ? w : window->width;
            event.window.height = h > 0 ? h : window->height;
            pal__eventq_push(&g_event_queue, event);
        }
    } else if (object_id == window->xdg_surface && opcode == wayland_xdg_surface_event_configure) {
        uint32_t configure = buf_read_u32(msg, msg_len);
        wayland_xdg_surface_ack_configure(fd, window->xdg_surface, configure);
        window->state = WINDOW_STATE_CONFIGURED;
    } else if (object_id == window->xdg_toplevel && opcode == wayland_xdg_toplevel_event_close) {
        window->should_close = 1;
        
        // Queue close event
        pal_event event = {0};
        event.type = PAL_EVENT_WINDOW_CLOSE_REQUESTED;
        event.window.windowid = window->id;
        pal__eventq_push(&g_event_queue, event);
    }
}

static void process_events(pal_window *window) {
    char read_buf[4096] = "";
    int64_t read_bytes = recv(g_wayland_fd, read_buf, sizeof(read_buf), MSG_DONTWAIT);
    
    if (read_bytes <= 0) {
        return;
    }

    char *msg = read_buf;
    uint64_t msg_len = (uint64_t)read_bytes;

    while (msg_len > 0) {
        wayland_handle_message(g_wayland_fd, window, &msg, &msg_len);
    }
}

static void process_all_events() {
    char read_buf[4096] = "";
    int64_t read_bytes = recv(g_wayland_fd, read_buf, sizeof(read_buf), MSG_DONTWAIT);
    
    if (read_bytes <= 0) {
        return;
    }

    char *msg = read_buf;
    uint64_t msg_len = (uint64_t)read_bytes;

    while (msg_len >= 8) {
        // Peek at object_id to find the right window
        uint32_t object_id = *(uint32_t *)msg;
        pal_window *window = find_window_by_object_id(object_id);
        
        if (!window && g_window_count > 0) {
            window = g_windows[0]; // Fallback to first window
        }
        
        if (window) {
            wayland_handle_message(g_wayland_fd, window, &msg, &msg_len);
        } else {
            break; // Can't process without a window
        }
    }
}

PALAPI void pal_init() {
    struct timeval tv = {0};
    gettimeofday(&tv, NULL);
    srand(tv.tv_sec * 1000000 + tv.tv_usec);

    g_wayland_fd = wayland_display_connect();
    if (g_wayland_fd == -1) {
        fprintf(stderr, "Failed to connect to Wayland display\n");
        exit(1);
    }
    
    if (!pal__init_eventq()) {
        fprintf(stderr, "Failed to initialize event queue\n");
        exit(1);
    }
}

PALAPI void pal_shutdown() {
    pal__eventq_free(g_event_queue);
    
    if (g_windows) {
        free(g_windows);
        g_windows = NULL;
    }
    g_window_count = 0;
    g_window_capacity = 0;
    
    if (g_wayland_fd >= 0) {
        close(g_wayland_fd);
        g_wayland_fd = -1;
    }
}

PALAPI pal_window *pal_create_window(int width, int height, const char *window_title, uint64_t window_flags) {
    (void)window_flags; // Unused for now

    if (g_wayland_fd == -1) {
        fprintf(stderr, "pal_init() must be called before creating windows\n");
        return NULL;
    }

    pal_window *window = calloc(1, sizeof(pal_window));
    if (!window) {
        return NULL;
    }

    static uint32_t next_window_id = 1;
    window->id = next_window_id++;
    window->width = width;
    window->height = height;
    window->stride = width * color_channels;
    window->shm_pool_size = window->height * window->stride;
    window->state = WINDOW_STATE_NONE;
    window->should_close = 0;

    // Register window early so it can receive events
    register_window(window);

    // Get registry
    window->wl_registry = wayland_wl_display_get_registry(g_wayland_fd);
    if (window->wl_registry == 0) {
        unregister_window(window);
        free(window);
        return NULL;
    }

    // Wait for registry events to bind interfaces
    int attempts = 0;
    while ((window->wl_compositor == 0 || window->wl_shm == 0 || window->xdg_wm_base == 0) && attempts < 100) {
        process_events(window);
        attempts++;
        sleep(1000); // 1ms
    }

    if (window->wl_compositor == 0 || window->wl_shm == 0 || window->xdg_wm_base == 0) {
        fprintf(stderr, "Failed to bind required Wayland interfaces\n");
        unregister_window(window);
        free(window);
        return NULL;
    }

    // Create shared memory
    create_shared_memory_file(window->shm_pool_size, window);
    if (window->shm_pool_data == NULL) {
        fprintf(stderr, "Failed to create shared memory\n");
        unregister_window(window);
        free(window);
        return NULL;
    }

    // Create surface and configure window
    window->wl_surface = wayland_wl_compositor_create_surface(g_wayland_fd, window->wl_compositor);
    if (window->wl_surface == 0) {
        munmap(window->shm_pool_data, window->shm_pool_size);
        close(window->shm_fd);
        unregister_window(window);
        free(window);
        return NULL;
    }

    window->xdg_surface = wayland_xdg_wm_base_get_xdg_surface(g_wayland_fd, window->xdg_wm_base, window->wl_surface);
    window->xdg_toplevel = wayland_xdg_surface_get_toplevel(g_wayland_fd, window->xdg_surface);
    
    if (window_title) {
        wayland_xdg_toplevel_set_title(g_wayland_fd, window->xdg_toplevel, window_title);
    }
    
    wayland_wl_surface_commit(g_wayland_fd, window->wl_surface);
    
    window->state = WINDOW_STATE_CREATED;

    // Wait for configure event
    attempts = 0;
    while (window->state != WINDOW_STATE_CONFIGURED && attempts < 100) {
        process_events(window);
        attempts++;
        usleep(1000);
    }

    if (window->state == WINDOW_STATE_CONFIGURED) {
        // Create buffer pool
        window->wl_shm_pool = wayland_wl_shm_create_pool(g_wayland_fd, window);
        window->wl_buffer = wayland_wl_shm_pool_create_buffer(g_wayland_fd, window);
        
        // Clear to black initially
        memset(window->shm_pool_data, 0, window->shm_pool_size);
        
        wayland_wl_surface_attach(g_wayland_fd, window->wl_surface, window->wl_buffer);
        wayland_wl_surface_commit(g_wayland_fd, window->wl_surface);
        
        window->state = WINDOW_STATE_READY;
        
        // Queue window shown event
        pal_event event = {0};
        event.type = PAL_EVENT_WINDOW_SHOWN;
        event.window.windowid = window->id;
        event.window.width = window->width;
        event.window.height = window->height;
        pal__eventq_push(&g_event_queue, event);
    }

    return window;
}

PALAPI void pal_close_window(pal_window *window) {
    if (!window) {
        return;
    }

    // Queue window closed event
    pal_event event = {0};
    event.type = PAL_EVENT_WINDOW_CLOSED;
    event.window.windowid = window->id;
    pal__eventq_push(&g_event_queue, event);

    unregister_window(window);

    if (window->shm_pool_data) {
        munmap(window->shm_pool_data, window->shm_pool_size);
    }
    
    if (window->shm_fd >= 0) {
        close(window->shm_fd);
    }

    window->state = WINDOW_STATE_CLOSED;
    free(window);
}

PALAPI pal_bool pal_poll_events(pal_event *event) {
    if (!event) {
        return pal_false;
    }

    // Process any pending Wayland events first
    process_all_events();

    // Check if queue has events
    if (g_event_queue.size == 0) {
        return pal_false;
    }

    // Peek
    *event = g_event_queue.events[g_event_queue.front];
    // Dequeue
    g_event_queue.front = (g_event_queue.front + 1) % g_event_queue.capacity;
    g_event_queue.size--;

    return pal_true;
}
#endif /* PLATFORM_LINUX_WAYLAND_H */
#endif // linux_* platforms

#endif // Platform selection defines

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
    if (pal_memcmp(riffHeader, "RIFF", 4) != 0 || pal_memcmp(waveID, "WAVE", 4) != 0) {
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
        if (pal_memcmp(chunkID, "fmt ", 4) == 0) {
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
                if (pal_memcmp(subFormat, SUBFORMAT_PCM, 16) == 0) {
                    audioFormat = WAV_FMT_PCM;
                    is_float = 0;
                } else if (pal_memcmp(subFormat, SUBFORMAT_IEEE_FLOAT, 16) == 0) {
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
        } else if (pal_memcmp(chunkID, "data", 4) == 0) {
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

    printf("OGG Load: Requesting %zu sample frames (%.3f seconds) from %zu total\n",
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
    }

    // Clean up temporary buffers
    for (int i = 0; i < channels; i++) {
        free(channel_buffers[i]);
    }

    free(channel_buffers);

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
#endif /* PAL_IMPLEMENTATION */

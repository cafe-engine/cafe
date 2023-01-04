#ifndef _CAFE_H
#define _CAFE_H

#define CAFE_API extern
#define CAFE_VERSION "0.1"

#define CA_TRUE 1
#define CA_FALSE 0
#define CA_OK 0
#define CA_ERROR -1

#define CAFE_RGB(r, g, b) ((ca_Color){(r), (g), (b), 255})
#define CAFE_RGBA(r, g, b, a) ((ca_Color){(r), (g), (b), (a)})

#define CAFE_PI 3.14159

#define CAFE_MAX(a, b) ((a) > (b) ? (a) : (b))
#define CAFE_MIN(a, b) ((a) < (b) ? (a) : (b))
#define CAFE_CLAMP(x, min, max) (CAFE_MAX(min, CAFE_MIN(max, x)))

typedef char ca_bool;
typedef char ca_i8;
typedef unsigned char ca_u8;
typedef short ca_i16;
typedef unsigned short ca_u16;
typedef int ca_i32;
typedef unsigned int ca_u32;
typedef long long ca_i64;
typedef unsigned long long ca_u64;
typedef float ca_f32;
typedef double ca_f64;

// SDL_SCANCODE wrap
enum {
    CAFEK_UNKNOWN = 0,

    /**
     *  \name Usage page 0x07
     *
     *  These values are from usage page 0x07 (USB keyboard page).
     */
    /* @{ */

    CAFEK_A = 4,
    CAFEK_B = 5,
    CAFEK_C = 6,
    CAFEK_D = 7,
    CAFEK_E = 8,
    CAFEK_F = 9,
    CAFEK_G = 10,
    CAFEK_H = 11,
    CAFEK_I = 12,
    CAFEK_J = 13,
    CAFEK_K = 14,
    CAFEK_L = 15,
    CAFEK_M = 16,
    CAFEK_N = 17,
    CAFEK_O = 18,
    CAFEK_P = 19,
    CAFEK_Q = 20,
    CAFEK_R = 21,
    CAFEK_S = 22,
    CAFEK_T = 23,
    CAFEK_U = 24,
    CAFEK_V = 25,
    CAFEK_W = 26,
    CAFEK_X = 27,
    CAFEK_Y = 28,
    CAFEK_Z = 29,

    CAFEK_1 = 30,
    CAFEK_2 = 31,
    CAFEK_3 = 32,
    CAFEK_4 = 33,
    CAFEK_5 = 34,
    CAFEK_6 = 35,
    CAFEK_7 = 36,
    CAFEK_8 = 37,
    CAFEK_9 = 38,
    CAFEK_0 = 39,

    CAFEK_RETURN = 40,
    CAFEK_ESCAPE = 41,
    CAFEK_BACKSPACE = 42,
    CAFEK_TAB = 43,
    CAFEK_SPACE = 44,

    CAFEK_MINUS = 45,
    CAFEK_EQUALS = 46,
    CAFEK_LEFTBRACKET = 47,
    CAFEK_RIGHTBRACKET = 48,
    CAFEK_BACKSLASH = 49, /**< Located at the lower left of the return
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
    CAFEK_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
                                  *   instead of 49 for the same key, but all
                                  *   OSes I've seen treat the two codes
                                  *   identically. So, as an implementor, unless
                                  *   your keyboard generates both of those
                                  *   codes and your OS treats them differently,
                                  *   you should generate CAFEK_BACKSLASH
                                  *   instead of this code. As a user, you
                                  *   should not rely on this code because SDL
                                  *   will never generate it with most (all?)
                                  *   keyboards.
                                  */
    CAFEK_SEMICOLON = 51,
    CAFEK_APOSTROPHE = 52,
    CAFEK_GRAVE = 53, /**< Located in the top left corner (on both ANSI
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
    CAFEK_COMMA = 54,
    CAFEK_PERIOD = 55,
    CAFEK_SLASH = 56,

    CAFEK_CAPSLOCK = 57,

    CAFEK_F1 = 58,
    CAFEK_F2 = 59,
    CAFEK_F3 = 60,
    CAFEK_F4 = 61,
    CAFEK_F5 = 62,
    CAFEK_F6 = 63,
    CAFEK_F7 = 64,
    CAFEK_F8 = 65,
    CAFEK_F9 = 66,
    CAFEK_F10 = 67,
    CAFEK_F11 = 68,
    CAFEK_F12 = 69,

    CAFEK_PRINTSCREEN = 70,
    CAFEK_SCROLLLOCK = 71,
    CAFEK_PAUSE = 72,
    CAFEK_INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
                                   does send code 73, not 117) */
    CAFEK_HOME = 74,
    CAFEK_PAGEUP = 75,
    CAFEK_DELETE = 76,
    CAFEK_END = 77,
    CAFEK_PAGEDOWN = 78,
    CAFEK_RIGHT = 79,
    CAFEK_LEFT = 80,
    CAFEK_DOWN = 81,
    CAFEK_UP = 82,

    CAFEK_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                                     */
    CAFEK_KP_DIVIDE = 84,
    CAFEK_KP_MULTIPLY = 85,
    CAFEK_KP_MINUS = 86,
    CAFEK_KP_PLUS = 87,
    CAFEK_KP_ENTER = 88,
    CAFEK_KP_1 = 89,
    CAFEK_KP_2 = 90,
    CAFEK_KP_3 = 91,
    CAFEK_KP_4 = 92,
    CAFEK_KP_5 = 93,
    CAFEK_KP_6 = 94,
    CAFEK_KP_7 = 95,
    CAFEK_KP_8 = 96,
    CAFEK_KP_9 = 97,
    CAFEK_KP_0 = 98,
    CAFEK_KP_PERIOD = 99,

    CAFEK_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                                        *   keyboards have over ANSI ones,
                                        *   located between left shift and Y.
                                        *   Produces GRAVE ACCENT and TILDE in a
                                        *   US or UK Mac layout, REVERSE SOLIDUS
                                        *   (backslash) and VERTICAL LINE in a
                                        *   US or UK Windows layout, and
                                        *   LESS-THAN SIGN and GREATER-THAN SIGN
                                        *   in a Swiss German, German, or French
                                        *   layout. */
    CAFEK_APPLICATION = 101, /**< windows contextual menu, compose */
    CAFEK_POWER = 102, /**< The USB document says this is a status flag,
                               *   not a physical key - but some Mac keyboards
                               *   do have a power key. */
    CAFEK_KP_EQUALS = 103,
    CAFEK_F13 = 104,
    CAFEK_F14 = 105,
    CAFEK_F15 = 106,
    CAFEK_F16 = 107,
    CAFEK_F17 = 108,
    CAFEK_F18 = 109,
    CAFEK_F19 = 110,
    CAFEK_F20 = 111,
    CAFEK_F21 = 112,
    CAFEK_F22 = 113,
    CAFEK_F23 = 114,
    CAFEK_F24 = 115,
    CAFEK_EXECUTE = 116,
    CAFEK_HELP = 117,
    CAFEK_MENU = 118,
    CAFEK_SELECT = 119,
    CAFEK_STOP = 120,
    CAFEK_AGAIN = 121,   /**< redo */
    CAFEK_UNDO = 122,
    CAFEK_CUT = 123,
    CAFEK_COPY = 124,
    CAFEK_PASTE = 125,
    CAFEK_FIND = 126,
    CAFEK_MUTE = 127,
    CAFEK_VOLUMEUP = 128,
    CAFEK_VOLUMEDOWN = 129,
/* not sure whether there's a reason to enable these */
/*     CAFEK_LOCKINGCAPSLOCK = 130,  */
/*     CAFEK_LOCKINGNUMLOCK = 131, */
/*     CAFEK_LOCKINGSCROLLLOCK = 132, */
    CAFEK_KP_COMMA = 133,
    CAFEK_KP_EQUALSAS400 = 134,

    CAFEK_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                            footnotes in USB doc */
    CAFEK_INTERNATIONAL2 = 136,
    CAFEK_INTERNATIONAL3 = 137, /**< Yen */
    CAFEK_INTERNATIONAL4 = 138,
    CAFEK_INTERNATIONAL5 = 139,
    CAFEK_INTERNATIONAL6 = 140,
    CAFEK_INTERNATIONAL7 = 141,
    CAFEK_INTERNATIONAL8 = 142,
    CAFEK_INTERNATIONAL9 = 143,
    CAFEK_LANG1 = 144, /**< Hangul/English toggle */
    CAFEK_LANG2 = 145, /**< Hanja conversion */
    CAFEK_LANG3 = 146, /**< Katakana */
    CAFEK_LANG4 = 147, /**< Hiragana */
    CAFEK_LANG5 = 148, /**< Zenkaku/Hankaku */
    CAFEK_LANG6 = 149, /**< reserved */
    CAFEK_LANG7 = 150, /**< reserved */
    CAFEK_LANG8 = 151, /**< reserved */
    CAFEK_LANG9 = 152, /**< reserved */

    CAFEK_ALTERASE = 153, /**< Erase-Eaze */
    CAFEK_SYSREQ = 154,
    CAFEK_CANCEL = 155,
    CAFEK_CLEAR = 156,
    CAFEK_PRIOR = 157,
    CAFEK_RETURN2 = 158,
    CAFEK_SEPARATOR = 159,
    CAFEK_OUT = 160,
    CAFEK_OPER = 161,
    CAFEK_CLEARAGAIN = 162,
    CAFEK_CRSEL = 163,
    CAFEK_EXSEL = 164,

    CAFEK_KP_00 = 176,
    CAFEK_KP_000 = 177,
    CAFEK_THOUSANDSSEPARATOR = 178,
    CAFEK_DECIMALSEPARATOR = 179,
    CAFEK_CURRENCYUNIT = 180,
    CAFEK_CURRENCYSUBUNIT = 181,
    CAFEK_KP_LEFTPAREN = 182,
    CAFEK_KP_RIGHTPAREN = 183,
    CAFEK_KP_LEFTBRACE = 184,
    CAFEK_KP_RIGHTBRACE = 185,
    CAFEK_KP_TAB = 186,
    CAFEK_KP_BACKSPACE = 187,
    CAFEK_KP_A = 188,
    CAFEK_KP_B = 189,
    CAFEK_KP_C = 190,
    CAFEK_KP_D = 191,
    CAFEK_KP_E = 192,
    CAFEK_KP_F = 193,
    CAFEK_KP_XOR = 194,
    CAFEK_KP_POWER = 195,
    CAFEK_KP_PERCENT = 196,
    CAFEK_KP_LESS = 197,
    CAFEK_KP_GREATER = 198,
    CAFEK_KP_AMPERSAND = 199,
    CAFEK_KP_DBLAMPERSAND = 200,
    CAFEK_KP_VERTICALBAR = 201,
    CAFEK_KP_DBLVERTICALBAR = 202,
    CAFEK_KP_COLON = 203,
    CAFEK_KP_HASH = 204,
    CAFEK_KP_SPACE = 205,
    CAFEK_KP_AT = 206,
    CAFEK_KP_EXCLAM = 207,
    CAFEK_KP_MEMSTORE = 208,
    CAFEK_KP_MEMRECALL = 209,
    CAFEK_KP_MEMCLEAR = 210,
    CAFEK_KP_MEMADD = 211,
    CAFEK_KP_MEMSUBTRACT = 212,
    CAFEK_KP_MEMMULTIPLY = 213,
    CAFEK_KP_MEMDIVIDE = 214,
    CAFEK_KP_PLUSMINUS = 215,
    CAFEK_KP_CLEAR = 216,
    CAFEK_KP_CLEARENTRY = 217,
    CAFEK_KP_BINARY = 218,
    CAFEK_KP_OCTAL = 219,
    CAFEK_KP_DECIMAL = 220,
    CAFEK_KP_HEXADECIMAL = 221,

    CAFEK_LCTRL = 224,
    CAFEK_LSHIFT = 225,
    CAFEK_LALT = 226, /**< alt, option */
    CAFEK_LGUI = 227, /**< windows, command (apple), meta */
    CAFEK_RCTRL = 228,
    CAFEK_RSHIFT = 229,
    CAFEK_RALT = 230, /**< alt gr, option */
    CAFEK_RGUI = 231, /**< windows, command (apple), meta */

    CAFEK_MODE = 257,    /**< I'm not sure if this is really not covered
                                 *   by any of the above, but since there's a
                                 *   special KMOD_MODE for it I'm adding it here
                                 */

    /* @} *//* Usage page 0x07 */

    /**
     *  \name Usage page 0x0C
     *
     *  These values are mapped from usage page 0x0C (USB consumer page).
     */
    /* @{ */

    CAFEK_AUDIONEXT = 258,
    CAFEK_AUDIOPREV = 259,
    CAFEK_AUDIOSTOP = 260,
    CAFEK_AUDIOPLAY = 261,
    CAFEK_AUDIOMUTE = 262,
    CAFEK_MEDIASELECT = 263,
    CAFEK_WWW = 264,
    CAFEK_MAIL = 265,
    CAFEK_CALCULATOR = 266,
    CAFEK_COMPUTER = 267,
    CAFEK_AC_SEARCH = 268,
    CAFEK_AC_HOME = 269,
    CAFEK_AC_BACK = 270,
    CAFEK_AC_FORWARD = 271,
    CAFEK_AC_STOP = 272,
    CAFEK_AC_REFRESH = 273,
    CAFEK_AC_BOOKMARKS = 274,

    /* @} *//* Usage page 0x0C */

    /**
     *  \name Walther keys
     *
     *  These are values that Christian Walther added (for mac keyboard?).
     */
    /* @{ */

    CAFEK_BRIGHTNESSDOWN = 275,
    CAFEK_BRIGHTNESSUP = 276,
    CAFEK_DISPLAYSWITCH = 277, /**< display mirroring/dual display
                                           switch, video mode switch */
    CAFEK_KBDILLUMTOGGLE = 278,
    CAFEK_KBDILLUMDOWN = 279,
    CAFEK_KBDILLUMUP = 280,
    CAFEK_EJECT = 281,
    CAFEK_SLEEP = 282,

    CAFEK_APP1 = 283,
    CAFEK_APP2 = 284,

    /* @} *//* Walther keys */

    /**
     *  \name Usage page 0x0C (additional media keys)
     *
     *  These values are mapped from usage page 0x0C (USB consumer page).
     */
    /* @{ */

    CAFEK_AUDIOREWIND = 285,
    CAFEK_AUDIOFASTFORWARD = 286,

    /* @} *//* Usage page 0x0C (additional media keys) */

    /* Add any other keys here. */

    CAFE_NUM_KEYS = 512 /**< not a key, just marks the number of scancodes
                                 for array bounds */
};

enum {
    CAFE_LINE = 0,
    CAFE_FILL
};

enum {
    CAFE_NEAREST = 0,
    CAFE_LINEAR
};

enum {
    CAFE_CLAMP = 0,
    CAFE_REPEAT
};

enum {
    CAFE_TEXTURE_STATIC = 0,
    CAFE_TEXTURE_STREAM,
    CAFE_TEXTURE_TARGET
};

typedef struct Cafe Cafe;
typedef struct {
    struct {
        char title[128];
        ca_i32 x, y;
        ca_i32 width, height;
        ca_i32 flags;
    } window;
} ca_Config;

typedef void(*ca_StepFunction)(ca_f32 dt);

typedef struct { ca_u8 r, g, b, a; } ca_Color;
typedef struct { ca_f32 x, y, w, h; } ca_Rect;
typedef struct { ca_f32 x, y; } ca_Point;
typedef ca_f32 ca_Vec2[2];
typedef ca_f32 ca_Vec3[3];
typedef ca_f32 ca_Vec4[4];
typedef ca_Vec4 ca_Mat4[4];

typedef struct mo_audio_s ca_Audio;
typedef ca_Audio ca_Sound;
typedef ca_Audio ca_Music;
typedef ca_u32 ca_SoundID;

typedef struct ca_File ca_File;

typedef struct ca_Image ca_Image;
typedef struct ca_Canvas ca_Canvas;
typedef struct ca_Font ca_Font;
typedef struct ca_Shader ca_Shader;

typedef struct ca_Joystick ca_Joystick;
typedef struct ca_Gamepad ca_Gamepad;


#if defined(__cplusplus)
extern "C" {
#endif

/*=====================*
 * CORE FUNCTIONS      *
 *=====================*/

CAFE_API ca_Config cafe_config(const char* title, ca_i32 width, ca_i32 height);
CAFE_API ca_Config cafe_config_load(const char* path);
CAFE_API ca_Config cafe_config_from_args(int argc, char** argv);

CAFE_API ca_bool cafe_open(ca_Config *conf);
CAFE_API void cafe_close(void);

CAFE_API ca_bool cafe_is_running(void);

CAFE_API void cafe_begin(void);
CAFE_API void cafe_end(void);

CAFE_API void cafe_run(ca_StepFunction step);

/*=====================*
 * AUDIO FUNCTIONS     *
 *=====================*/

CAFE_API int cafe_audio_play(ca_Audio* audio);
CAFE_API void cafe_audio_stop_all(ca_Audio* audio);
CAFE_API void cafe_audio_pause_all(ca_Audio* audio);

CAFE_API int cafe_audio_is_any_playing(ca_Audio* audio);

/******************
 * Audio Instance *
 ******************/

CAFE_API void cafe_audio_play_instance(int id);
CAFE_API void cafe_audio_stop_instance(int id);
CAFE_API void cafe_audio_pause_instance(int id);
CAFE_API void cafe_audio_set_instance_volume(int id, float volume);
CAFE_API void cafe_audio_set_instance_loop(int id, int loop);

/*****************
 * Sound         *
 *****************/
CAFE_API ca_Sound* cafe_audio_load_sound(const char* filepath);
CAFE_API ca_Sound* cafe_audio_new_sound(void* data, int size);
CAFE_API void cafe_sound_destroy(ca_Sound* sound);

/*****************
 * Music         *
 *****************/
CAFE_API ca_Music* cafe_audio_load_music(const char* filepath);
CAFE_API ca_Music* cafe_audio_new_music(void* data, int size);
CAFE_API void cafe_music_destroy(ca_Music* music);

/*=====================*
 * INPUT FUNCTIONS     *
 *=====================*/

/*****************
 * Keyboard      *
 *****************/

CAFE_API ca_i32 cafe_keyboard_from_name(const ca_i8* name);
CAFE_API const ca_i8* cafe_keyboard_get_name(ca_i32 key);

CAFE_API ca_bool cafe_keyboard_is_down(ca_i32 key);
CAFE_API ca_bool cafe_keyboard_is_up(ca_i32 key);

#if 0
CAFE_API ca_i32 cafe_keyboard_fromName(const char* name);
CAFE_API const char* cafe_keyboard_getName(ca_i32 key);

CAFE_API ca_bool cafe_keyboard_isDown(ca_i32 key);
CAFE_API ca_bool cafe_keyboard_isUp(ca_i32 key);
CAFE_API ca_bool cafe_keyboard_wasPressed(ca_i32 key);
CAFE_API ca_bool cafe_keyboard_wasReleased(ca_i32 key);
#endif
/*****************
 * Mouse         *
 ****************/

CAFE_API void cafe_mouse_get_pos(ca_f32* x, ca_f32* y);
CAFE_API void cafe_mouse_set_pos(ca_f32 x, ca_f32 y);
CAFE_API void cafe_mouse_get_delta_pos(ca_f32* x, ca_f32* y);

CAFE_API ca_bool cafe_mouse_is_down(ca_i32 button);
CAFE_API ca_bool cafe_mouse_is_up(ca_i32 button);
#if 0
CAFE_API void cafe_mouse_getPos(ca_f32* x, ca_f32* y);
CAFE_API void cafe_mouse_setPos(ca_f32 x, ca_f32 y);
CAFE_API void cafe_mouse_getDeltaPos(ca_f32* x, ca_f32* y);

CAFE_API ca_bool cafe_mouse_isDown(ca_i32 button);
CAFE_API ca_bool cafe_mouse_isUp(ca_i32 button);
CAFE_API ca_bool cafe_mouse_wasPressed(ca_i32 button);
CAFE_API ca_bool cafe_mouse_wasReleased(ca_i32 button);
#endif

/*****************
 * Joystick      *
 ****************/

CAFE_API ca_Joystick* cafe_joystick(ca_i32 index);
CAFE_API void cafe_close_joystick(ca_Joystick *joystick);

/*=====================*
 * RENDER FUNCTIONS    *
 *=====================*/

// CAFE_API void cafe_render_clear(ca_Color color);

// CAFE_API void cafe_render_setMode(ca_i32 mode);
// CAFE_API void cafe_render_setShader(ca_Shader* shader);
// CAFE_API void cafe_render_setTarget(ca_Texture* tex);
// CAFE_API void cafe_render_setColor(ca_Color color);

CAFE_API void cafe_graphics_clear(ca_Color color);

CAFE_API void cafe_graphics_draw_mode(ca_i32 mode);
CAFE_API void cafe_graphics_set_shader(ca_Shader* shader);
CAFE_API void cafe_graphics_set_canvas(ca_Canvas* canvas);
CAFE_API void cafe_graphics_set_color(ca_Color color);

CAFE_API void cafe_graphics_draw_point(ca_f32 x, ca_f32 y);
CAFE_API void cafe_graphics_draw_line(ca_f32 x0, ca_f32 y0, ca_f32 x1, ca_f32 y1);
CAFE_API void cafe_graphics_draw_rectangle(ca_f32 x, ca_f32 y, ca_f32 w, ca_f32 h);
CAFE_API void cafe_graphics_draw_circle(ca_f32 x, ca_f32 y, ca_f32 radius);
CAFE_API void cafe_graphics_draw_circle_ex(ca_f32 x, ca_f32 y, ca_f32 radius, ca_u32 segments);
CAFE_API void cafe_graphics_draw_triangle(ca_f32 x0, ca_f32 y0, ca_f32 x1, ca_f32 y1, ca_f32 x2, ca_f32 y2);

CAFE_API void cafe_graphics_draw_image(ca_Image *image, ca_Rect* dest, ca_Rect* src);
CAFE_API void cafe_graphics_draw_image_ex(ca_Image *image, ca_Rect* dest, ca_Rect* src, ca_f32 angle, ca_i32 flip);

CAFE_API void cafe_graphics_draw_canvas(ca_Canvas* canvas, ca_Rect* dest, ca_Rect* src);
CAFE_API void cafe_graphics_draw_canvas_ex(ca_Canvas* canvas, ca_Rect* dest, ca_Rect* src, ca_f32 angle, ca_i32 flip);

CAFE_API void cafe_graphics_print(ca_Font* font, const ca_i8* text, ca_f32 x, ca_f32 y);
CAFE_API void cafe_graphics_print_ex(ca_Font* font, const ca_i8* text, ca_f32 x, ca_f32 y, ca_f32 angle);

// CAFE_API void cafe_render_point(ca_f32 x, ca_f32 y);
// CAFE_API void cafe_render_line(ca_f32 x1, ca_f32 y1, ca_f32 x2, ca_f32 y2);
// CAFE_API void cafe_render_rect(ca_f32 x, ca_f32 y, ca_f32 w, ca_f32 h);
// CAFE_API void cafe_render_circle(ca_f32 x, ca_f32 y, ca_f32 radius);
// CAFE_API void cafe_render_triangle(ca_f32 x1, ca_f32 y1, ca_f32 x2, ca_f32 y2, ca_f32 x3, ca_f32 y3);

// CAFE_API void cafe_render_texture(ca_Texture* tex, ca_Rect *dest, ca_Rect *src);
// CAFE_API void cafe_render_textureEx(ca_Texture* tex, ca_Rect *dest, ca_Rect *src, ca_f32 angle, ca_Vec2 center, ca_i32 flip);

// CAFE_API void cafe_render_text(ca_Font* font, const char* text, ca_Rect *dest);
// CAFE_API void cafe_render_textEx(ca_Font* font, const char* text, ca_Rect *dest, ca_f32 angle, ca_Vec2 center, ca_i32 flip);

// CAFE_API void cafe_render_batch(ca_Batch* batch);

// CAFE_API void cafe_render_drawCall(ca_DrawCall *draw_call);

/*****************
 * Image         *
 ****************/

CAFE_API ca_Image* cafe_graphics_new_image(ca_i32 width, ca_i32 height, ca_u8 *data);
CAFE_API ca_Image* cafe_graphics_load_image(const ca_i8* path);
CAFE_API void cafe_image_destroy(ca_Image* image);

CAFE_API void cafe_image_set_filter(ca_Image* image, ca_i32 filter_min, ca_i32 filter_mag);
CAFE_API void cafe_image_set_wrap(ca_Image* image, ca_i32 wrap_s, ca_i32 wrap_t);

CAFE_API ca_i32 cafe_image_get_width(ca_Image* image);
CAFE_API ca_i32 cafe_image_get_height(ca_Image* image);
CAFE_API void cafe_image_get_size(ca_Image* image, ca_i32* width, ca_i32* height);

/*****************
 * Canvas        *
 ****************/

CAFE_API ca_Canvas* cafe_graphics_new_canvas(ca_i32 width, ca_i32 height);
CAFE_API void cafe_canvas_destroy(ca_Canvas* canvas);

CAFE_API void cafe_canvas_set_filter(ca_Canvas* canvas, ca_i32 filter_min, ca_i32 filter_mag);
CAFE_API void cafe_canvas_set_wrap(ca_Canvas* canvas, ca_i32 wrap_s, ca_i32 wrap_t);

CAFE_API ca_i32 cafe_canvas_get_width(ca_Canvas* canvas);
CAFE_API ca_i32 cafe_canvas_get_height(ca_Canvas* canvas);
CAFE_API void cafe_canvas_get_size(ca_Canvas* canvas, ca_i32* width, ca_i32* height);

/*****************
 * Font          *
 ****************/

CAFE_API ca_Font* cafe_graphics_load_font(const ca_i8* path, ca_i32 size);
CAFE_API void cafe_font_destroy(ca_Font* font);

CAFE_API ca_i32 cafe_font_get_width(ca_Font* font, const ca_i8* text);
CAFE_API ca_i32 cafe_font_get_height(ca_Font* font, const ca_i8* text);

// CAFE_API ca_Font* cafe_render_createFont(const char* path, ca_i32 size);
// CAFE_API void cafe_font_destroy(ca_Font* font);

// CAFE_API ca_i32 cafe_font_getWidth(ca_Font* font, const char* text);
// CAFE_API ca_i32 cafe_font_getHeight(ca_Font* font);

/*****************
 * Shader        *
 ****************/

CAFE_API ca_Shader* cafe_graphics_new_shader(const ca_i8* vert, const ca_i8* frag);
CAFE_API void cafe_shader_destroy(ca_Shader* shader);

CAFE_API void cafe_shader_set_uniform1i(ca_Shader* shader, const ca_i8* name, ca_i32 value);
CAFE_API void cafe_shader_set_uniform1f(ca_Shader* shader, const ca_i8* name, ca_f32 value);
CAFE_API void cafe_shader_set_uniform2f(ca_Shader* shader, const ca_i8* name, ca_f32 v0, ca_f32 v1);
CAFE_API void cafe_shader_set_uniform3f(ca_Shader* shader, const ca_i8* name, ca_f32 v0, ca_f32 v1, ca_f32 v2);
CAFE_API void cafe_shader_set_uniform4f(ca_Shader* shader, const ca_i8* name, ca_f32 v0, ca_f32 v1, ca_f32 v2, ca_f32 v3);
CAFE_API void cafe_shader_set_uniform_mat4(ca_Shader* shader, const ca_i8* name, ca_Mat4 mat);

// CAFE_API ca_Shader* cafe_render_createShader(const char* vert, const char* frag);
// CAFE_API void cafe_shader_destroy(ca_Shader* shader);

// CAFE_API void cafe_shader_setUniform1i(ca_Shader* shader, const char* name, ca_i32 value);
// CAFE_API void cafe_shader_setUniform1f(ca_Shader* shader, const char* name, ca_f32 value);
// CAFE_API void cafe_shader_setUniform2f(ca_Shader* shader, const char* name, ca_f32 x, ca_f32 y);
// CAFE_API void cafe_shader_setUniform3f(ca_Shader* shader, const char* name, ca_f32 x, ca_f32 y, ca_f32 z);
// CAFE_API void cafe_shader_setUniform4f(ca_Shader* shader, const char* name, ca_f32 x, ca_f32 y, ca_f32 z, ca_f32 w);
// CAFE_API void cafe_shader_setUniformMat4(ca_Shader* shader, const char* name, ca_Mat4* mat);

/*=====================*
 * WINDOW FUNCTIONS    *
 *=====================*/

CAFE_API const ca_i8* cafe_window_get_title(void);
CAFE_API void cafe_window_set_title(const ca_i8* title);

CAFE_API ca_i32 cafe_window_get_width(void);
CAFE_API ca_i32 cafe_window_get_height(void);
CAFE_API void cafe_window_get_size(ca_i32* width, ca_i32* height);

CAFE_API ca_bool cafe_window_is_fullscreen(void);
CAFE_API void cafe_window_set_fullscreen(ca_bool fullscreen);

// CAFE_API const char* cafe_window_getTitle(void);
// CAFE_API void cafe_window_setTitle(const char* title);

// CAFE_API ca_i32 cafe_window_getWidth(void);
// CAFE_API ca_i32 cafe_window_getHeight(void);
// CAFE_API void cafe_window_getSize(ca_i32* width, ca_i32* height);

// CAFE_API ca_bool cafe_window_isFullscreen(void);
// CAFE_API void cafe_window_setFullscreen(ca_bool fullscreen);

#if defined(__cplusplus)
}
#endif

#endif /* _CAFE_H */

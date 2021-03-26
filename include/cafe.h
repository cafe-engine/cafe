/**********************************************************************************
 *
 * MIT License
 *
 * Copyright (c) 2021 Canoi Gomes
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **********************************************************************************/
#ifndef CAFE_H
#define CAFE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAFE_API
#define CAFE_VERSION "0.1.0"

typedef struct Cafe Cafe;

#ifndef CAFE_VALUE
  #define CAFE_VALUE float
#endif

#define cafe_point(x, y) (cf_Point){(x), (y)}

#define CAFE_MAX_TEXTURES 64
#define CAFE_MAX_RTARGETS 64

typedef enum {
    CAFE_KEY_UNKNOWN = 0,

    /**
     *  \name Usage page 0x07
     *
     *  These values are from usage page 0x07 (USB keyboard page).
     */
    /* @{ */

    CAFE_KEY_A = 4,
    CAFE_KEY_B = 5,
    CAFE_KEY_C = 6,
    CAFE_KEY_D = 7,
    CAFE_KEY_E = 8,
    CAFE_KEY_F = 9,
    CAFE_KEY_G = 10,
    CAFE_KEY_H = 11,
    CAFE_KEY_I = 12,
    CAFE_KEY_J = 13,
    CAFE_KEY_K = 14,
    CAFE_KEY_L = 15,
    CAFE_KEY_M = 16,
    CAFE_KEY_N = 17,
    CAFE_KEY_O = 18,
    CAFE_KEY_P = 19,
    CAFE_KEY_Q = 20,
    CAFE_KEY_R = 21,
    CAFE_KEY_S = 22,
    CAFE_KEY_T = 23,
    CAFE_KEY_U = 24,
    CAFE_KEY_V = 25,
    CAFE_KEY_W = 26,
    CAFE_KEY_X = 27,
    CAFE_KEY_Y = 28,
    CAFE_KEY_Z = 29,

    CAFE_KEY_1 = 30,
    CAFE_KEY_2 = 31,
    CAFE_KEY_3 = 32,
    CAFE_KEY_4 = 33,
    CAFE_KEY_5 = 34,
    CAFE_KEY_6 = 35,
    CAFE_KEY_7 = 36,
    CAFE_KEY_8 = 37,
    CAFE_KEY_9 = 38,
    CAFE_KEY_0 = 39,

    CAFE_KEY_RETURN = 40,
    CAFE_KEY_ESCAPE = 41,
    CAFE_KEY_BACKSPACE = 42,
    CAFE_KEY_TAB = 43,
    CAFE_KEY_SPACE = 44,

    CAFE_KEY_MINUS = 45,
    CAFE_KEY_EQUALS = 46,
    CAFE_KEY_LEFTBRACKET = 47,
    CAFE_KEY_RIGHTBRACKET = 48,
    CAFE_KEY_BACKSLASH = 49, /**< Located at the lower left of the return
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
    CAFE_KEY_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
                                  *   instead of 49 for the same key, but all
                                  *   OSes I've seen treat the two codes
                                  *   identically. So, as an implementor, unless
                                  *   your keyboard generates both of those
                                  *   codes and your OS treats them differently,
                                  *   you should generate CAFE_KEY_BACKSLASH
                                  *   instead of this code. As a user, you
                                  *   should not rely on this code because SDL
                                  *   will never generate it with most (all?)
                                  *   keyboards.
                                  */
    CAFE_KEY_SEMICOLON = 51,
    CAFE_KEY_APOSTROPHE = 52,
    CAFE_KEY_GRAVE = 53, /**< Located in the top left corner (on both ANSI
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
    CAFE_KEY_COMMA = 54,
    CAFE_KEY_PERIOD = 55,
    CAFE_KEY_SLASH = 56,

    CAFE_KEY_CAPSLOCK = 57,

    CAFE_KEY_F1 = 58,
    CAFE_KEY_F2 = 59,
    CAFE_KEY_F3 = 60,
    CAFE_KEY_F4 = 61,
    CAFE_KEY_F5 = 62,
    CAFE_KEY_F6 = 63,
    CAFE_KEY_F7 = 64,
    CAFE_KEY_F8 = 65,
    CAFE_KEY_F9 = 66,
    CAFE_KEY_F10 = 67,
    CAFE_KEY_F11 = 68,
    CAFE_KEY_F12 = 69,

    CAFE_KEY_PRINTSCREEN = 70,
    CAFE_KEY_SCROLLLOCK = 71,
    CAFE_KEY_PAUSE = 72,
    CAFE_KEY_INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
                                   does send code 73, not 117) */
    CAFE_KEY_HOME = 74,
    CAFE_KEY_PAGEUP = 75,
    CAFE_KEY_DELETE = 76,
    CAFE_KEY_END = 77,
    CAFE_KEY_PAGEDOWN = 78,
    CAFE_KEY_RIGHT = 79,
    CAFE_KEY_LEFT = 80,
    CAFE_KEY_DOWN = 81,
    CAFE_KEY_UP = 82,

    CAFE_KEY_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                                     */
    CAFE_KEY_KP_DIVIDE = 84,
    CAFE_KEY_KP_MULTIPLY = 85,
    CAFE_KEY_KP_MINUS = 86,
    CAFE_KEY_KP_PLUS = 87,
    CAFE_KEY_KP_ENTER = 88,
    CAFE_KEY_KP_1 = 89,
    CAFE_KEY_KP_2 = 90,
    CAFE_KEY_KP_3 = 91,
    CAFE_KEY_KP_4 = 92,
    CAFE_KEY_KP_5 = 93,
    CAFE_KEY_KP_6 = 94,
    CAFE_KEY_KP_7 = 95,
    CAFE_KEY_KP_8 = 96,
    CAFE_KEY_KP_9 = 97,
    CAFE_KEY_KP_0 = 98,
    CAFE_KEY_KP_PERIOD = 99,

    CAFE_KEY_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                                        *   keyboards have over ANSI ones,
                                        *   located between left shift and Y.
                                        *   Produces GRAVE ACCENT and TILDE in a
                                        *   US or UK Mac layout, REVERSE SOLIDUS
                                        *   (backslash) and VERTICAL LINE in a
                                        *   US or UK Windows layout, and
                                        *   LESS-THAN SIGN and GREATER-THAN SIGN
                                        *   in a Swiss German, German, or French
                                        *   layout. */
    CAFE_KEY_APPLICATION = 101, /**< windows contextual menu, compose */
    CAFE_KEY_POWER = 102, /**< The USB document says this is a status flag,
                               *   not a physical key - but some Mac keyboards
                               *   do have a power key. */
    CAFE_KEY_KP_EQUALS = 103,
    CAFE_KEY_F13 = 104,
    CAFE_KEY_F14 = 105,
    CAFE_KEY_F15 = 106,
    CAFE_KEY_F16 = 107,
    CAFE_KEY_F17 = 108,
    CAFE_KEY_F18 = 109,
    CAFE_KEY_F19 = 110,
    CAFE_KEY_F20 = 111,
    CAFE_KEY_F21 = 112,
    CAFE_KEY_F22 = 113,
    CAFE_KEY_F23 = 114,
    CAFE_KEY_F24 = 115,
    CAFE_KEY_EXECUTE = 116,
    CAFE_KEY_HELP = 117,
    CAFE_KEY_MENU = 118,
    CAFE_KEY_SELECT = 119,
    CAFE_KEY_STOP = 120,
    CAFE_KEY_AGAIN = 121,   /**< redo */
    CAFE_KEY_UNDO = 122,
    CAFE_KEY_CUT = 123,
    CAFE_KEY_COPY = 124,
    CAFE_KEY_PASTE = 125,
    CAFE_KEY_FIND = 126,
    CAFE_KEY_MUTE = 127,
    CAFE_KEY_VOLUMEUP = 128,
    CAFE_KEY_VOLUMEDOWN = 129,
/* not sure whether there's a reason to enable these */
/*     CAFE_KEY_LOCKINGCAPSLOCK = 130,  */
/*     CAFE_KEY_LOCKINGNUMLOCK = 131, */
/*     CAFE_KEY_LOCKINGSCROLLLOCK = 132, */
    CAFE_KEY_KP_COMMA = 133,
    CAFE_KEY_KP_EQUALSAS400 = 134,

    CAFE_KEY_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                            footnotes in USB doc */
    CAFE_KEY_INTERNATIONAL2 = 136,
    CAFE_KEY_INTERNATIONAL3 = 137, /**< Yen */
    CAFE_KEY_INTERNATIONAL4 = 138,
    CAFE_KEY_INTERNATIONAL5 = 139,
    CAFE_KEY_INTERNATIONAL6 = 140,
    CAFE_KEY_INTERNATIONAL7 = 141,
    CAFE_KEY_INTERNATIONAL8 = 142,
    CAFE_KEY_INTERNATIONAL9 = 143,
    CAFE_KEY_LANG1 = 144, /**< Hangul/English toggle */
    CAFE_KEY_LANG2 = 145, /**< Hanja conversion */
    CAFE_KEY_LANG3 = 146, /**< Katakana */
    CAFE_KEY_LANG4 = 147, /**< Hiragana */
    CAFE_KEY_LANG5 = 148, /**< Zenkaku/Hankaku */
    CAFE_KEY_LANG6 = 149, /**< reserved */
    CAFE_KEY_LANG7 = 150, /**< reserved */
    CAFE_KEY_LANG8 = 151, /**< reserved */
    CAFE_KEY_LANG9 = 152, /**< reserved */

    CAFE_KEY_ALTERASE = 153, /**< Erase-Eaze */
    CAFE_KEY_SYSREQ = 154,
    CAFE_KEY_CANCEL = 155,
    CAFE_KEY_CLEAR = 156,
    CAFE_KEY_PRIOR = 157,
    CAFE_KEY_RETURN2 = 158,
    CAFE_KEY_SEPARATOR = 159,
    CAFE_KEY_OUT = 160,
    CAFE_KEY_OPER = 161,
    CAFE_KEY_CLEARAGAIN = 162,
    CAFE_KEY_CRSEL = 163,
    CAFE_KEY_EXSEL = 164,

    CAFE_KEY_KP_00 = 176,
    CAFE_KEY_KP_000 = 177,
    CAFE_KEY_THOUSANDSSEPARATOR = 178,
    CAFE_KEY_DECIMALSEPARATOR = 179,
    CAFE_KEY_CURRENCYUNIT = 180,
    CAFE_KEY_CURRENCYSUBUNIT = 181,
    CAFE_KEY_KP_LEFTPAREN = 182,
    CAFE_KEY_KP_RIGHTPAREN = 183,
    CAFE_KEY_KP_LEFTBRACE = 184,
    CAFE_KEY_KP_RIGHTBRACE = 185,
    CAFE_KEY_KP_TAB = 186,
    CAFE_KEY_KP_BACKSPACE = 187,
    CAFE_KEY_KP_A = 188,
    CAFE_KEY_KP_B = 189,
    CAFE_KEY_KP_C = 190,
    CAFE_KEY_KP_D = 191,
    CAFE_KEY_KP_E = 192,
    CAFE_KEY_KP_F = 193,
    CAFE_KEY_KP_XOR = 194,
    CAFE_KEY_KP_POWER = 195,
    CAFE_KEY_KP_PERCENT = 196,
    CAFE_KEY_KP_LESS = 197,
    CAFE_KEY_KP_GREATER = 198,
    CAFE_KEY_KP_AMPERSAND = 199,
    CAFE_KEY_KP_DBLAMPERSAND = 200,
    CAFE_KEY_KP_VERTICALBAR = 201,
    CAFE_KEY_KP_DBLVERTICALBAR = 202,
    CAFE_KEY_KP_COLON = 203,
    CAFE_KEY_KP_HASH = 204,
    CAFE_KEY_KP_SPACE = 205,
    CAFE_KEY_KP_AT = 206,
    CAFE_KEY_KP_EXCLAM = 207,
    CAFE_KEY_KP_MEMSTORE = 208,
    CAFE_KEY_KP_MEMRECALL = 209,
    CAFE_KEY_KP_MEMCLEAR = 210,
    CAFE_KEY_KP_MEMADD = 211,
    CAFE_KEY_KP_MEMSUBTRACT = 212,
    CAFE_KEY_KP_MEMMULTIPLY = 213,
    CAFE_KEY_KP_MEMDIVIDE = 214,
    CAFE_KEY_KP_PLUSMINUS = 215,
    CAFE_KEY_KP_CLEAR = 216,
    CAFE_KEY_KP_CLEARENTRY = 217,
    CAFE_KEY_KP_BINARY = 218,
    CAFE_KEY_KP_OCTAL = 219,
    CAFE_KEY_KP_DECIMAL = 220,
    CAFE_KEY_KP_HEXADECIMAL = 221,

    CAFE_KEY_LCTRL = 224,
    CAFE_KEY_LSHIFT = 225,
    CAFE_KEY_LALT = 226, /**< alt, option */
    CAFE_KEY_LGUI = 227, /**< windows, command(apple), meta */
    CAFE_KEY_RCTRL = 228,
    CAFE_KEY_RSHIFT = 229,
    CAFE_KEY_RALT = 230, /**< alt gr, option */
    CAFE_KEY_RGUI = 231, /**< windows, command (apple), meta */

    CAFE_KEY_MODE = 257,    /**< I'm not sure if this is really not covered
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

    CAFE_KEY_AUDIONEXT = 258,
    CAFE_KEY_AUDIOPREV = 259,
    CAFE_KEY_AUDIOSTOP = 260,
    CAFE_KEY_AUDIOPLAY = 261,
    CAFE_KEY_AUDIOMUTE = 262,
    CAFE_KEY_MEDIASELECT = 263,
    CAFE_KEY_WWW = 264,
    CAFE_KEY_MAIL = 265,
    CAFE_KEY_CALCULATOR = 266,
    CAFE_KEY_COMPUTER = 267,
    CAFE_KEY_AC_SEARCH = 268,
    CAFE_KEY_AC_HOME = 269,
    CAFE_KEY_AC_BACK = 270,
    CAFE_KEY_AC_FORWARD = 271,
    CAFE_KEY_AC_STOP = 272,
    CAFE_KEY_AC_REFRESH = 273,
    CAFE_KEY_AC_BOOKMARKS = 274,

    /* @} *//* Usage page 0x0C */

    /**
     *  \name Walther keys
     *
     *  These are values that Christian Walther added (for mac keyboard?).
     */
    /* @{ */

    CAFE_KEY_BRIGHTNESSDOWN = 275,
    CAFE_KEY_BRIGHTNESSUP = 276,
    CAFE_KEY_DISPLAYSWITCH = 277, /**< display mirroring/dual display
                                           switch, video mode switch */
    CAFE_KEY_KBDILLUMTOGGLE = 278,
    CAFE_KEY_KBDILLUMDOWN = 279,
    CAFE_KEY_KBDILLUMUP = 280,
    CAFE_KEY_EJECT = 281,
    CAFE_KEY_SLEEP = 282,

    CAFE_KEY_APP1 = 283,
    CAFE_KEY_APP2 = 284,

    /* @} *//* Walther keys */

    /**
     *  \name Usage page 0x0C (additional media keys)
     *
     *  These values are mapped from usage page 0x0C (USB consumer page).
     */
    /* @{ */

    CAFE_KEY_AUDIOREWIND = 285,
    CAFE_KEY_AUDIOFASTFORWARD = 286,
    
    
    
    /* @} *//* Usage page 0x0C (additional media keys) */

    /* Add any other keys here. */

    CAFE_KEY_COUNT = 512 /**< not a key, just marks the number of scancodes
                                 for array bounds */
} CAFE_KEY;

typedef unsigned char Byte;

typedef unsigned int cf_Canvas;
typedef unsigned int cf_Image;
typedef unsigned int cf_Shader;

typedef struct la_file_t cf_File;
typedef struct la_dir_t cf_Dir;
typedef struct la_vdrive_t cf_VDrive;

typedef struct mo_AudioBuffer cf_Audio;
typedef unsigned int cf_Sound;
typedef unsigned int cf_Music;

typedef struct { CAFE_VALUE x, y; } cf_Point;
typedef struct { Byte r, g, b, a; } cf_Color;
typedef struct { CAFE_VALUE x, y, w, h; } cf_Rect;

typedef enum { CAFE_LINE = 0, CAFE_FILL } CAFE_DRAW_MODE_;

typedef struct cf_Header {
    char name[100];
    char linkname[100];
    int type;
    int mode;
    long size;
    unsigned mtime;
    unsigned int uid, gid;
    char uname[32], gname[32];
} cf_Header;

typedef struct cf_Config {
  char title[128];
  unsigned int width, height;
  unsigned int flags;
  unsigned int window_flags;
  int argc;
  char ** argv;
} cf_Config;

CAFE_API void cafe_init_config(cf_Config *conf, const char *title, int width, int height);
CAFE_API int cafe_init(cf_Config *conf);
CAFE_API void cafe_terminate();

CAFE_API int cafe_should_close();

CAFE_API int cafe_main_loop();

CAFE_API void cafe_begin();
CAFE_API void cafe_end();

/**********************
 * Graphics           *
 **********************/
CAFE_API void cafe_graphics_clear(cf_Color color);
CAFE_API void cafe_graphics_draw_color(cf_Color color);
CAFE_API void cafe_graphics_set_canvas(cf_Canvas canvas);

CAFE_API void cafe_graphics_rectangle(CAFE_VALUE x, CAFE_VALUE y, CAFE_VALUE w, CAFE_VALUE h, CAFE_DRAW_MODE_ mode);
CAFE_API void cafe_graphics_circle(CAFE_VALUE x, CAFE_VALUE y, CAFE_VALUE radius, CAFE_DRAW_MODE_ mode);
CAFE_API void cafe_graphics_triangle(CAFE_VALUE x0, CAFE_VALUE y0, CAFE_VALUE x1, CAFE_VALUE y1, CAFE_VALUE x2, CAFE_VALUE y2, CAFE_DRAW_MODE_ mode);
// CAFE_API void cafe_graphics_

/* Canvas */

CAFE_API cf_Canvas cafe_canvas(int width,int height);
CAFE_API void cafe_graphics_draw_canvas(cf_Canvas canvas, cf_Rect *r, CAFE_VALUE x, CAFE_VALUE y);
CAFE_API void cafe_graphics_draw_canvas_ex(cf_Canvas canvas, cf_Rect *r, cf_Point p, CAFE_VALUE angle, cf_Point scale, cf_Point origin);


CAFE_API void cafe_graphics_set_canvas(cf_Canvas canvas);

/* Image */

CAFE_API cf_Image cafe_image(int width, int height, int format);
CAFE_API cf_Image cafe_image_load(const char *filename);
CAFE_API void cafe_graphics_draw_image(cf_Image img, cf_Rect *r, CAFE_VALUE x, CAFE_VALUE y);
CAFE_API void cafe_graphics_draw_image_ex(cf_Image img, cf_Rect *r, cf_Point p, CAFE_VALUE angle, cf_Point scale, cf_Point origin);

/* Shader */

CAFE_API cf_Shader cafe_shader(const char *glsl_shader, int type);
CAFE_API void cafe_shader_set(cf_Shader shd);

/**********************
 * Input              *
 **********************/

CAFE_API int cafe_key_from_name(const char *keyname);
CAFE_API int cafe_mouse_button_from_name(const char *btn_name);

CAFE_API int cafe_joy_axis_from_name(const char *axis_name);
CAFE_API int cafe_joy_button_from_name(const char *btn_name);

/* Keyboard */
CAFE_API int cafe_keyboard_isdown(int key);
CAFE_API int cafe_keyboard_isup(int key);
CAFE_API int cafe_keyboard_waspressed(int key);
CAFE_API int cafe_keyboard_wasreleased(int key);

/* Mouse */
CAFE_API int cafe_mouse_xpos(int *x);
CAFE_API int cafe_mouse_ypos(int *y);
CAFE_API int cafe_mouse_pos(cf_Point *out);

CAFE_API int cafe_mouse_isdown(int button);
CAFE_API int cafe_mouse_isup(int button);
CAFE_API int cafe_mouse_waspressed(int button);
CAFE_API int cafe_mouse_wasreleased(int button);

/* Joystick */
CAFE_API float cafe_joystick_axis(int jid, int axis); 
CAFE_API int cafe_joystick_isdown(int jid, int button);
CAFE_API int cafe_joystick_isup(int jid, int button);
CAFE_API int cafe_joystick_waspressed(int jid, int button);
CAFE_API int cafe_joystick_wasreleased(int jid, int button);

/**********************
 * Filesystem         *
 **********************/

CAFE_API int cafe_filesystem_init(const char *filepath);
CAFE_API void cafe_filesystem_deinit();

CAFE_API int cafe_filesystem_basepath(const char *path);

CAFE_API long cafe_filesystem_size(const char *filename);
CAFE_API int cafe_filesystem_read(const char *filename, char *out, int size);
CAFE_API int cafe_filesystem_write(const char *filename, const char *text, int size);

CAFE_API int cafe_filesystem_touch(const char *filename);
CAFE_API int cafe_filesystem_rm(const char *filename);

CAFE_API int cafe_filesystem_mkdir(const char *path);
CAFE_API int cafe_filesystem_rmdir(const char *path);

CAFE_API int cafe_filesystem_isfile(const char *filename);
CAFE_API int cafe_filesystem_isdir(const char *path);

/* File */
CAFE_API cf_File* cafe_file_open(const char *filename, int mode);
CAFE_API void cafe_file_close(cf_File *fp);
CAFE_API int cafe_file_close_stream(cf_File *fp);

CAFE_API int cafe_file_seek(cf_File *fp, int offset);
CAFE_API int cafe_file_tell(cf_File *fp);

CAFE_API int cafe_file_header(cf_File *fp, cf_Header *out);
CAFE_API int cafe_file_read(cf_File *fp, char *out, int bytes);

CAFE_API int cafe_file_write(cf_File *fp, const char *text, int bytes);
CAFE_API int cafe_file_append(cf_File *fp, const char *text, int bytes);
CAFE_API int cafe_file_insert(cf_File *fp, const char *text, int bytes);

/* Directory */
CAFE_API cf_Dir* cafe_dir_open(const char *filename);
CAFE_API void cafe_dir_close(cf_Dir *dir);

CAFE_API int cafe_dir_rewind(cf_Dir *dir);

CAFE_API int cafe_dir_header(cf_Dir *dir, cf_Header *out);
CAFE_API int cafe_dir_read(cf_Dir *dir, cf_Header *out);

/* Virtual */
CAFE_API cf_VDrive* cafe_vdrive_open(const char *filename, int mode);
CAFE_API void cafe_vdrive_close(cf_VDrive *drv);

/**********************
 * Audio              *
 **********************/


/* Audio Buffer */
CAFE_API void* cafe_audio_load(const char *filename, int usage);
CAFE_API void cafe_audio_destroy(void *buf);

CAFE_API int cafe_audio_play(cf_Audio *buf);
CAFE_API int cafe_audio_pause(cf_Audio *buf);
CAFE_API int cafe_audio_stop(cf_Audio *buf);

/* Sound */
CAFE_API cf_Sound cafe_sound_load(const char *filename);

CAFE_API int cafe_sound_play(cf_Sound snd);
CAFE_API int cafe_sound_pause(cf_Sound snd);
CAFE_API int cafe_sound_stop(cf_Sound snd);

/* Music */
CAFE_API cf_Music cafe_music_load(const char *filename);

CAFE_API int cafe_music_play(cf_Music mus);
CAFE_API int cafe_music_pause(cf_Music mus);
CAFE_API int cafe_music_stop(cf_Music mus);

#endif /* CAFE_H */

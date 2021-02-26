/*********************************************************************************
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

#ifndef TEA_H
#define TEA_H

#define TE_API extern
#include <SDL2/SDL.h>

#if defined(TEA_GL_RENDER)
#include <SDL2/SDL_opengl.h>
#endif

#define TEA_VERSION "0.1.0"
#define CAT(a, b) a b

#define TEA_FPS 30

#ifndef TEA_VALUE
  #define TEA_VALUE float
#endif

#define COMMAND_MAX 2048
#define STACK_MAX 255

#define MAX_FONT_CHAR 256
#define MAX_TEXTURES 64
#define MAX_RTARGETS 64
#define MAX_TRANSFORMS 64

#define STR(expr) #expr
#define TE_ASSERT(expr, msg) if (!(expr)) tea_error(msg);

#define tea_max(a, b) ((a) > (b) ? (a) : (b))
#define tea_min(a, b) ((a) > (b) ? (b) : (a))

#define tea_color(r, g, b) ((te_Color){(r), (g), (b), 255})
#define WHITE tea_color(255, 255, 255)
#define BLACK tea_color(0, 0, 0)

#define tea_vec2(x, y) (te_Vec2){(x), (y)}
#define tea_rect(x, y, w, h) ((te_Rect){(x), (y), (w), (h)})
#define tea_point(x, y) ((te_Point){(x), (y)})

typedef enum {
  TEA_BUTTON_LEFT = 0,
  TEA_BUTTON_MIDDLE,
  TEA_BUTTON_RIGHT,
  
  TEA_BUTTON_COUNT
} TEA_MOUSE_BUTTON;

// SDL Scancodes clone
typedef enum {
    TEA_KEY_UNKNOWN = 0,

    /**
     *  \name Usage page 0x07
     *
     *  These values are from usage page 0x07 (USB keyboard page).
     */
    /* @{ */

    TEA_KEY_A = 4,
    TEA_KEY_B = 5,
    TEA_KEY_C = 6,
    TEA_KEY_D = 7,
    TEA_KEY_E = 8,
    TEA_KEY_F = 9,
    TEA_KEY_G = 10,
    TEA_KEY_H = 11,
    TEA_KEY_I = 12,
    TEA_KEY_J = 13,
    TEA_KEY_K = 14,
    TEA_KEY_L = 15,
    TEA_KEY_M = 16,
    TEA_KEY_N = 17,
    TEA_KEY_O = 18,
    TEA_KEY_P = 19,
    TEA_KEY_Q = 20,
    TEA_KEY_R = 21,
    TEA_KEY_S = 22,
    TEA_KEY_T = 23,
    TEA_KEY_U = 24,
    TEA_KEY_V = 25,
    TEA_KEY_W = 26,
    TEA_KEY_X = 27,
    TEA_KEY_Y = 28,
    TEA_KEY_Z = 29,

    TEA_KEY_1 = 30,
    TEA_KEY_2 = 31,
    TEA_KEY_3 = 32,
    TEA_KEY_4 = 33,
    TEA_KEY_5 = 34,
    TEA_KEY_6 = 35,
    TEA_KEY_7 = 36,
    TEA_KEY_8 = 37,
    TEA_KEY_9 = 38,
    TEA_KEY_0 = 39,

    TEA_KEY_RETURN = 40,
    TEA_KEY_ESCAPE = 41,
    TEA_KEY_BACKSPACE = 42,
    TEA_KEY_TAB = 43,
    TEA_KEY_SPACE = 44,

    TEA_KEY_MINUS = 45,
    TEA_KEY_EQUALS = 46,
    TEA_KEY_LEFTBRACKET = 47,
    TEA_KEY_RIGHTBRACKET = 48,
    TEA_KEY_BACKSLASH = 49, /**< Located at the lower left of the return
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
    TEA_KEY_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
                                  *   instead of 49 for the same key, but all
                                  *   OSes I've seen treat the two codes
                                  *   identically. So, as an implementor, unless
                                  *   your keyboard generates both of those
                                  *   codes and your OS treats them differently,
                                  *   you should generate TEA_KEY_BACKSLASH
                                  *   instead of this code. As a user, you
                                  *   should not rely on this code because SDL
                                  *   will never generate it with most (all?)
                                  *   keyboards.
                                  */
    TEA_KEY_SEMICOLON = 51,
    TEA_KEY_APOSTROPHE = 52,
    TEA_KEY_GRAVE = 53, /**< Located in the top left corner (on both ANSI
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
    TEA_KEY_COMMA = 54,
    TEA_KEY_PERIOD = 55,
    TEA_KEY_SLASH = 56,

    TEA_KEY_CAPSLOCK = 57,

    TEA_KEY_F1 = 58,
    TEA_KEY_F2 = 59,
    TEA_KEY_F3 = 60,
    TEA_KEY_F4 = 61,
    TEA_KEY_F5 = 62,
    TEA_KEY_F6 = 63,
    TEA_KEY_F7 = 64,
    TEA_KEY_F8 = 65,
    TEA_KEY_F9 = 66,
    TEA_KEY_F10 = 67,
    TEA_KEY_F11 = 68,
    TEA_KEY_F12 = 69,

    TEA_KEY_PRINTSCREEN = 70,
    TEA_KEY_SCROLLLOCK = 71,
    TEA_KEY_PAUSE = 72,
    TEA_KEY_INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
                                   does send code 73, not 117) */
    TEA_KEY_HOME = 74,
    TEA_KEY_PAGEUP = 75,
    TEA_KEY_DELETE = 76,
    TEA_KEY_END = 77,
    TEA_KEY_PAGEDOWN = 78,
    TEA_KEY_RIGHT = 79,
    TEA_KEY_LEFT = 80,
    TEA_KEY_DOWN = 81,
    TEA_KEY_UP = 82,

    TEA_KEY_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                                     */
    TEA_KEY_KP_DIVIDE = 84,
    TEA_KEY_KP_MULTIPLY = 85,
    TEA_KEY_KP_MINUS = 86,
    TEA_KEY_KP_PLUS = 87,
    TEA_KEY_KP_ENTER = 88,
    TEA_KEY_KP_1 = 89,
    TEA_KEY_KP_2 = 90,
    TEA_KEY_KP_3 = 91,
    TEA_KEY_KP_4 = 92,
    TEA_KEY_KP_5 = 93,
    TEA_KEY_KP_6 = 94,
    TEA_KEY_KP_7 = 95,
    TEA_KEY_KP_8 = 96,
    TEA_KEY_KP_9 = 97,
    TEA_KEY_KP_0 = 98,
    TEA_KEY_KP_PERIOD = 99,

    TEA_KEY_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                                        *   keyboards have over ANSI ones,
                                        *   located between left shift and Y.
                                        *   Produces GRAVE ACCENT and TILDE in a
                                        *   US or UK Mac layout, REVERSE SOLIDUS
                                        *   (backslash) and VERTICAL LINE in a
                                        *   US or UK Windows layout, and
                                        *   LESS-THAN SIGN and GREATER-THAN SIGN
                                        *   in a Swiss German, German, or French
                                        *   layout. */
    TEA_KEY_APPLICATION = 101, /**< windows contextual menu, compose */
    TEA_KEY_POWER = 102, /**< The USB document says this is a status flag,
                               *   not a physical key - but some Mac keyboards
                               *   do have a power key. */
    TEA_KEY_KP_EQUALS = 103,
    TEA_KEY_F13 = 104,
    TEA_KEY_F14 = 105,
    TEA_KEY_F15 = 106,
    TEA_KEY_F16 = 107,
    TEA_KEY_F17 = 108,
    TEA_KEY_F18 = 109,
    TEA_KEY_F19 = 110,
    TEA_KEY_F20 = 111,
    TEA_KEY_F21 = 112,
    TEA_KEY_F22 = 113,
    TEA_KEY_F23 = 114,
    TEA_KEY_F24 = 115,
    TEA_KEY_EXECUTE = 116,
    TEA_KEY_HELP = 117,
    TEA_KEY_MENU = 118,
    TEA_KEY_SELECT = 119,
    TEA_KEY_STOP = 120,
    TEA_KEY_AGAIN = 121,   /**< redo */
    TEA_KEY_UNDO = 122,
    TEA_KEY_CUT = 123,
    TEA_KEY_COPY = 124,
    TEA_KEY_PASTE = 125,
    TEA_KEY_FIND = 126,
    TEA_KEY_MUTE = 127,
    TEA_KEY_VOLUMEUP = 128,
    TEA_KEY_VOLUMEDOWN = 129,
/* not sure whether there's a reason to enable these */
/*     TEA_KEY_LOCKINGCAPSLOCK = 130,  */
/*     TEA_KEY_LOCKINGNUMLOCK = 131, */
/*     TEA_KEY_LOCKINGSCROLLLOCK = 132, */
    TEA_KEY_KP_COMMA = 133,
    TEA_KEY_KP_EQUALSAS400 = 134,

    TEA_KEY_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                            footnotes in USB doc */
    TEA_KEY_INTERNATIONAL2 = 136,
    TEA_KEY_INTERNATIONAL3 = 137, /**< Yen */
    TEA_KEY_INTERNATIONAL4 = 138,
    TEA_KEY_INTERNATIONAL5 = 139,
    TEA_KEY_INTERNATIONAL6 = 140,
    TEA_KEY_INTERNATIONAL7 = 141,
    TEA_KEY_INTERNATIONAL8 = 142,
    TEA_KEY_INTERNATIONAL9 = 143,
    TEA_KEY_LANG1 = 144, /**< Hangul/English toggle */
    TEA_KEY_LANG2 = 145, /**< Hanja conversion */
    TEA_KEY_LANG3 = 146, /**< Katakana */
    TEA_KEY_LANG4 = 147, /**< Hiragana */
    TEA_KEY_LANG5 = 148, /**< Zenkaku/Hankaku */
    TEA_KEY_LANG6 = 149, /**< reserved */
    TEA_KEY_LANG7 = 150, /**< reserved */
    TEA_KEY_LANG8 = 151, /**< reserved */
    TEA_KEY_LANG9 = 152, /**< reserved */

    TEA_KEY_ALTERASE = 153, /**< Erase-Eaze */
    TEA_KEY_SYSREQ = 154,
    TEA_KEY_CANCEL = 155,
    TEA_KEY_CLEAR = 156,
    TEA_KEY_PRIOR = 157,
    TEA_KEY_RETURN2 = 158,
    TEA_KEY_SEPARATOR = 159,
    TEA_KEY_OUT = 160,
    TEA_KEY_OPER = 161,
    TEA_KEY_CLEARAGAIN = 162,
    TEA_KEY_CRSEL = 163,
    TEA_KEY_EXSEL = 164,

    TEA_KEY_KP_00 = 176,
    TEA_KEY_KP_000 = 177,
    TEA_KEY_THOUSANDSSEPARATOR = 178,
    TEA_KEY_DECIMALSEPARATOR = 179,
    TEA_KEY_CURRENCYUNIT = 180,
    TEA_KEY_CURRENCYSUBUNIT = 181,
    TEA_KEY_KP_LEFTPAREN = 182,
    TEA_KEY_KP_RIGHTPAREN = 183,
    TEA_KEY_KP_LEFTBRACE = 184,
    TEA_KEY_KP_RIGHTBRACE = 185,
    TEA_KEY_KP_TAB = 186,
    TEA_KEY_KP_BACKSPACE = 187,
    TEA_KEY_KP_A = 188,
    TEA_KEY_KP_B = 189,
    TEA_KEY_KP_C = 190,
    TEA_KEY_KP_D = 191,
    TEA_KEY_KP_E = 192,
    TEA_KEY_KP_F = 193,
    TEA_KEY_KP_XOR = 194,
    TEA_KEY_KP_POWER = 195,
    TEA_KEY_KP_PERCENT = 196,
    TEA_KEY_KP_LESS = 197,
    TEA_KEY_KP_GREATER = 198,
    TEA_KEY_KP_AMPERSAND = 199,
    TEA_KEY_KP_DBLAMPERSAND = 200,
    TEA_KEY_KP_VERTICALBAR = 201,
    TEA_KEY_KP_DBLVERTICALBAR = 202,
    TEA_KEY_KP_COLON = 203,
    TEA_KEY_KP_HASH = 204,
    TEA_KEY_KP_SPACE = 205,
    TEA_KEY_KP_AT = 206,
    TEA_KEY_KP_EXCLAM = 207,
    TEA_KEY_KP_MEMSTORE = 208,
    TEA_KEY_KP_MEMRECALL = 209,
    TEA_KEY_KP_MEMCLEAR = 210,
    TEA_KEY_KP_MEMADD = 211,
    TEA_KEY_KP_MEMSUBTRACT = 212,
    TEA_KEY_KP_MEMMULTIPLY = 213,
    TEA_KEY_KP_MEMDIVIDE = 214,
    TEA_KEY_KP_PLUSMINUS = 215,
    TEA_KEY_KP_CLEAR = 216,
    TEA_KEY_KP_CLEARENTRY = 217,
    TEA_KEY_KP_BINARY = 218,
    TEA_KEY_KP_OCTAL = 219,
    TEA_KEY_KP_DECIMAL = 220,
    TEA_KEY_KP_HEXADECIMAL = 221,

    TEA_KEY_LCTRL = 224,
    TEA_KEY_LSHIFT = 225,
    TEA_KEY_LALT = 226, /**< alt, option */
    TEA_KEY_LGUI = 227, /**< windows, command(apple), meta */
    TEA_KEY_RCTRL = 228,
    TEA_KEY_RSHIFT = 229,
    TEA_KEY_RALT = 230, /**< alt gr, option */
    TEA_KEY_RGUI = 231, /**< windows, command (apple), meta */

    TEA_KEY_MODE = 257,    /**< I'm not sure if this is really not covered
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

    TEA_KEY_AUDIONEXT = 258,
    TEA_KEY_AUDIOPREV = 259,
    TEA_KEY_AUDIOSTOP = 260,
    TEA_KEY_AUDIOPLAY = 261,
    TEA_KEY_AUDIOMUTE = 262,
    TEA_KEY_MEDIASELECT = 263,
    TEA_KEY_WWW = 264,
    TEA_KEY_MAIL = 265,
    TEA_KEY_CALCULATOR = 266,
    TEA_KEY_COMPUTER = 267,
    TEA_KEY_AC_SEARCH = 268,
    TEA_KEY_AC_HOME = 269,
    TEA_KEY_AC_BACK = 270,
    TEA_KEY_AC_FORWARD = 271,
    TEA_KEY_AC_STOP = 272,
    TEA_KEY_AC_REFRESH = 273,
    TEA_KEY_AC_BOOKMARKS = 274,

    /* @} *//* Usage page 0x0C */

    /**
     *  \name Walther keys
     *
     *  These are values that Christian Walther added (for mac keyboard?).
     */
    /* @{ */

    TEA_KEY_BRIGHTNESSDOWN = 275,
    TEA_KEY_BRIGHTNESSUP = 276,
    TEA_KEY_DISPLAYSWITCH = 277, /**< display mirroring/dual display
                                           switch, video mode switch */
    TEA_KEY_KBDILLUMTOGGLE = 278,
    TEA_KEY_KBDILLUMDOWN = 279,
    TEA_KEY_KBDILLUMUP = 280,
    TEA_KEY_EJECT = 281,
    TEA_KEY_SLEEP = 282,

    TEA_KEY_APP1 = 283,
    TEA_KEY_APP2 = 284,

    /* @} *//* Walther keys */

    /**
     *  \name Usage page 0x0C (additional media keys)
     *
     *  These values are mapped from usage page 0x0C (USB consumer page).
     */
    /* @{ */

    TEA_KEY_AUDIOREWIND = 285,
    TEA_KEY_AUDIOFASTFORWARD = 286,
    
    
    
    /* @} *//* Usage page 0x0C (additional media keys) */

    /* Add any other keys here. */

    TEA_KEY_COUNT = 512 /**< not a key, just marks the number of scancodes
                                 for array bounds */
} TEA_KEY;

typedef enum {
  TEA_RED,
  TEA_GREEN,
  TEA_BLUE,
  TEA_ALPHA,
  TEA_RGB,
  TEA_RGBA,

  TEA_PIXELFORMAT_COUNT
} TEA_PIXELFORMAT;

typedef enum {
  TEA_DEFAULT = 0,
  TEA_SOFTWARE_RENDER = (1 << 0),
  TEA_HARDWARE_RENDER = (1 << 1),
  TEA_BATCH_RENDER = (1 << 2)
} TEA_RENDER_FLAGS;

typedef enum {
  TEA_LINE = 0,
  TEA_FILL,

  DRAW_MODE_COUNT
} TEA_DRAW_MODE;

typedef struct Tea Tea;
typedef struct te_Config te_Config;

struct te_Config {
  unsigned char title[100];
  int width, height;

  int flags;
  int window_flags;
  int render_flags;
};

typedef SDL_Window te_Window;
typedef SDL_Renderer te_Render;
typedef SDL_Event te_Event;

typedef struct te_Font te_Font;


// #ifndef TEA_GL_RENDER

// typedef struct SDL_Texture te_Texture;
// typedef struct SDL_Texture te_RenderTarget;

// #else

// typedef unsigned int te_Texture;

// typedef struct te_RenderTarget {
//   unsigned int id;
//   te_Texture tex;
// } te_RenderTarget;

// #endif

typedef struct te_Texture te_Texture;
typedef struct te_RenderTarget te_RenderTarget;

typedef unsigned int te_Image;
typedef unsigned int te_Canvas;
typedef struct te_Shader te_Shader;

// typedef struct te_Transform te_Transform;

// typedef TEA_VALUE te_Vec2[2];
// typedef TEA_VALUE te_Vec3[3];
// typedef TEA_VALUE te_Vec4[4];
// typedef TEA_VALUE te_Matrix[4][4];
typedef struct { TEA_VALUE x, y; } te_Point;
typedef struct { TEA_VALUE x, y, w, h; } te_Rect;
typedef struct { unsigned char r, g, b, a; } te_Color;
typedef enum { TEA_FLIP_NONE = 0, TEA_FLIP_H = (1 << 0), TEA_FLIP_V = (1 << 1) } te_RenderFlip;
typedef struct { te_Point translate; TEA_VALUE angle; te_Point scale; te_Point origin; } te_Transform;

typedef void(*RenderPointFn)(te_Point);
typedef void(*RenderLineFn)(te_Point, te_Point);
typedef void(*RenderRectFn)(te_Rect);
typedef void(*RenderCircleFn)(te_Point, TEA_VALUE);
typedef void(*RenderTriangleFn)(te_Point, te_Point, te_Point);
typedef void(*RenderTextureFn)(te_Texture, te_Rect*, te_Rect*);
typedef void(*RenderTextureExFn)(te_Texture, te_Rect*, te_Rect*, TEA_VALUE, te_Point, te_RenderFlip flip);

// struct te_Texture {
//   SDL_Texture *tex;
//   int width, height;
// };

// Config
TE_API int tea_config_init(te_Config *conf, const char *title, int width, int height);

// Core
TE_API Tea* tea_context();
TE_API Tea* tea_init(struct te_Config *c);
TE_API void tea_terminate(void);

TE_API int tea_should_close();

TE_API void tea_set_target(te_RenderTarget *target);
TE_API void tea_set_canvas(te_Canvas canvas);

// Timer

TE_API float tea_get_delta(void);
TE_API int tea_get_framerate(void);

// Transforms

TE_API te_Transform tea_get_transform();
TE_API void tea_set_transform(te_Transform *t);

TE_API void tea_set_scale(te_Point scale);
TE_API void tea_set_position(te_Point position);
TE_API void tea_set_angle(TEA_VALUE angle);
TE_API void tea_set_origin(te_Point origin);

// Graphics

TE_API void tea_begin();
TE_API void tea_end();

TE_API void tea_clear();
TE_API void tea_clear_color(te_Color color);
TE_API void tea_draw_color(te_Color color);
TE_API void tea_draw_mode(TEA_DRAW_MODE mode);

TE_API void tea_draw_point(te_Point p);
TE_API void tea_draw_line(te_Point p0, te_Point p1);

TE_API void tea_draw_rect(te_Rect r);
TE_API void tea_draw_rect_(TEA_VALUE x, TEA_VALUE y, TEA_VALUE w, TEA_VALUE h);
TE_API void tea_draw_rect_point(te_Point pos, te_Point sz);
TE_API void tea_draw_circle(te_Point p, TEA_VALUE radius);
TE_API void tea_draw_triangle(te_Point p0, te_Point p1, te_Point p2);

TE_API void tea_draw_image(te_Image img, te_Rect *r, te_Point p);
TE_API void tea_draw_image_ex(te_Image img, te_Rect *r, te_Point p, TEA_VALUE angle, te_Point scale, te_Point origin);

TE_API void tea_draw_texture(te_Texture *tex, te_Rect *dest, te_Rect *src);
TE_API void tea_draw_texture_ex(te_Texture *tex, te_Rect *dest, te_Rect *src, TEA_VALUE angle, te_Point origin, te_RenderFlip flip);

TE_API void tea_draw_canvas(te_Canvas canvas, te_Rect *r, te_Point p);
TE_API void tea_draw_canvas_ex(te_Canvas canvas, te_Rect *r, te_Point p, TEA_VALUE angle, te_Point scale, te_Point origin);

TE_API void tea_draw_text(te_Font *font, const char *text, te_Point pos);

// Window

TE_API void tea_init_window(const char *title, int width, int height, int flags);

TE_API te_Window* tea_window_create(const char *title, int width, int height, int flags);
TE_API void tea_window_destroy(te_Window *window);

TE_API int tea_window_should_close(te_Window *window);

// Render

TE_API te_Render* tea_render_create(te_Window *window, TEA_RENDER_FLAGS flags);
TE_API void tea_render_destroy(te_Render *render);

TE_API void tea_render_swap();

// Texture

TE_API void tea_texture_init(te_Texture *tex, int w, int h, unsigned int format, int access);
TE_API te_Texture* tea_texture(int w, int h, unsigned int format, int access);
TE_API te_Texture* tea_texture_load(const char *filename);
// TE_API int tea_texture_init(te_Texture *t, int w, int h, unsigned int format);

TE_API int tea_texture_width(te_Texture *tex);
TE_API int tea_texture_height(te_Texture *tex);
TE_API void tea_texture_size(te_Texture *tex, te_Point *size);

TE_API void tea_texture_destroy(te_Texture *tex);

// Image

TE_API te_Image tea_image(int w, int h, int format);
TE_API te_Image tea_image_load(const char *filename);

TE_API te_Texture* tea_image_texture(te_Image img);

#define tea_image_width(img) tea_texture_width(tea_image_texture((img)));
#define tea_image_height(img) tea_texture_height(tea_image_texture((img)));
#define tea_image_size(img, sz) tea_texture_size(tea_image_texture((img)), (sz));
TE_API void tea_image_destroy(te_Image *img);

// Render Target

TE_API te_RenderTarget* tea_render_target(int w, int h, int format);
TE_API void tea_set_render_target(te_RenderTarget *target);

TE_API te_Texture* tea_render_target_texture(te_RenderTarget *target);

// Canvas

TE_API te_Canvas tea_canvas(int width, int height);
TE_API te_RenderTarget* tea_canvas_render_target(te_Canvas canvas);
TE_API te_Texture* tea_canvas_texture(te_Canvas canvas);
TE_API void tea_canvas_destroy(te_Canvas *canvas);

// Font

TE_API te_Font* tea_font(const void *data, size_t buf_size, int font_size);
TE_API te_Font* tea_font_load(const char *filename, int font_size);
TE_API int tea_font_init(te_Font *font, const void *data, size_t buf_size, int font_size);

TE_API void tea_font_destroy(te_Font *font);

TE_API void tea_font_get_rect(te_Font* font, const int c, TEA_VALUE *x, TEA_VALUE *y, te_Point *out_pos, te_Rect *r, TEA_VALUE width);
TE_API void tea_font_char_rect(te_Font* font, const unsigned int c, te_Rect *r);

TE_API int tea_font_get_text_width(te_Font *font, const char *text, int len);
TE_API int tea_font_get_text_height(te_Font *font, const char *text, int len);

// TE_API void tea_canvas_draw(te_Canvas *canvas)


/*******************
 *      Input      *
 *******************/

TE_API int tea_update_input();

TE_API int tea_keyboard_is_down(int key);
TE_API int tea_keyboard_is_up(int key);
TE_API int tea_keyboard_was_pressed(int key);
TE_API int tea_keyboard_was_released(int key);

TE_API int tea_mouse_is_down(int button);
TE_API int tea_mouse_is_up(int button);
TE_API int tea_mouse_was_pressed(int button);
TE_API int tea_mouse_was_released(int button);

TE_API int tea_joystick_axis(int jid, int axis);
TE_API int tea_joystick_is_down(int jid, int button);
TE_API int tea_joystick_is_up(int jid, int button);
TE_API int tea_joystick_was_pressed(int jid, int button);
TE_API int tea_joystick_was_released(int jid, int button);

/* Debug */

TE_API void tea_error(const char *msg);

#endif /* TEA_H */
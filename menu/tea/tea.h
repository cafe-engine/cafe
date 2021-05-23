#ifndef TEA_H
#define TEA_H

#define TEA_VER "0.1.0"
#define TEA_API

#define TEA_ASSERT(expr, msg...) \
        if (!(expr)) { \
            tea_error(msg); \
            tea_log(__LINE__, __PRETTY_FUNCTION__, "Assertion %s failed: %s", #expr, tea_geterror()); \
            exit(0); \
        }

#define TEA_FPS 30
#define MAX_JID 4

#ifndef TEA_TNUM
      #define TEA_TNUM float
#endif

#define TEA_POINT(x, y) ((te_point_t){(x),(y)})
#define TEA_RECT(x, y, w, h) ((te_rect_t){(x),(y),(w),(h)})
#define TEA_COLOR(r, g, b, a) (((a) << 24) | ((b) << 16) | ((g) << 8) | (r))

#define TEA_WHITE TEA_COLOR(255, 255, 255, 255)
#define TEA_BLACK TEA_COLOR(0, 0, 0, 255)

enum {
    TEA_MOUSE_LEFT = 0,
    TEA_MOUSE_MIDDLE,
    TEA_MOUSE_RIGHT,

    TEA_MOUSE_COUNT
};

    enum {
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
};


enum {
    /* !!! FIXME: change this to name = (1<<x). */
    TEA_WINDOW_FULLSCREEN = 0x00000001,         /**< fullscreen window */
    TEA_WINDOW_OPENGL = 0x00000002,             /**< window usable with OpenGL context */
    TEA_WINDOW_SHOWN = 0x00000004,              /**< window is visible */
    TEA_WINDOW_HIDDEN = 0x00000008,             /**< window is not visible */
    TEA_WINDOW_BORDERLESS = 0x00000010,         /**< no window decoration */
    TEA_WINDOW_RESIZABLE = 0x00000020,          /**< window can be resized */
    TEA_WINDOW_MINIMIZED = 0x00000040,          /**< window is minimized */
    TEA_WINDOW_MAXIMIZED = 0x00000080,          /**< window is maximized */
    TEA_WINDOW_INPUT_GRABBED = 0x00000100,      /**< window has grabbed input focus */
    TEA_WINDOW_INPUT_FOCUS = 0x00000200,        /**< window has input focus */
    TEA_WINDOW_MOUSE_FOCUS = 0x00000400,        /**< window has mouse focus */
    TEA_WINDOW_FULLSCREEN_DESKTOP = ( TEA_WINDOW_FULLSCREEN | 0x00001000 ),
#if 0
    TEA_WINDOW_FOREIGN = 0x00000800,            /**< window not created by SDL */
    TEA_WINDOW_ALLOW_HIGHDPI = 0x00002000,      /**< window should be created in high-DPI mode if supported.
                                                     On macOS NSHighResolutionCapable must be set true in the
                                                     application's Info.plist for this to have any effect. */
    TEA_WINDOW_MOUSE_CAPTURE = 0x00004000,      /**< window has mouse captured (unrelated to INPUT_GRABBED) */
    TEA_WINDOW_ALWAYS_ON_TOP = 0x00008000,      /**< window should always be above others */
    TEA_WINDOW_SKIP_TASKBAR  = 0x00010000,      /**< window should not be added to the taskbar */
    TEA_WINDOW_UTILITY       = 0x00020000,      /**< window should be treated as a utility window */
    TEA_WINDOW_TOOLTIP       = 0x00040000,      /**< window should be treated as a tooltip */
    TEA_WINDOW_POPUP_MENU    = 0x00080000,      /**< window should be treated as a popup menu */
    TEA_WINDOW_VULKAN        = 0x10000000       /**< window usable for Vulkan surface */
#endif
};


enum {
  TEA_TEXTURE_STATIC = 0,
  TEA_TEXTURE_STREAM,
  TEA_TEXTURE_TARGET
};

enum {
    TEA_SHADER_NONE = (     0),
    TEA_SHADER_FRAG = (1 << 0),
    TEA_SHADER_VERT = (1 << 1)
};

enum {
    TEA_UNIFORM_INT = 1,
    TEA_UNIFORM_VEC2I,
    TEA_UNIFORM_VEC3I,
    TEA_UNIFORM_VEC4I,
    TEA_UNIFORM_FLOAT,
    TEA_UNIFORM_VEC2,
    TEA_UNIFORM_VEC3,
    TEA_UNIFORM_VEC4,
    TEA_UNIFORM_MATRIX
};

enum {
    TEA_PIXELFORMAT_UNKNOWN = 0,
    TEA_GREY,
    TEA_GREY_A,
    TEA_RGB,
    TEA_RGBA,

    TEA_BGR,

    TEA_ARGB,
    TEA_BGRA,
    TEA_ABGR,

    TEA_PIXELFORMAT_COUNT
};

enum {
    TEA_WRAP_S = 1,
    TEA_WRAP_T
};

enum {
    TEA_CLAMP = 0,
    TEA_REPEAT,
    TEA_MIRROR_REPEAT,

    TEA_WRAP_COUNT
};

enum {
    TEA_FILTER_MIN = 1,
    TEA_FILTER_MAG,

    TEA_FILTER_COUNT
};

enum {
    TEA_FILTER_NEAREST = 0,
    TEA_FILTER_LINEAR,
    TEA_FILTER_BILINEAR
};

enum {
    TEA_LINE = 0,
    TEA_FILL,

    TEA_DRAWMODE_COUNT
};

enum {
  TEA_FLIP_NONE = 0,
  TEA_FLIP_HORI = 1,
  TEA_FLIP_VERT = 2
};

typedef struct Tea Tea;
typedef struct te_config_t te_config_t;

typedef struct te_texinfo_t te_texinfo_t;
typedef struct te_texture_t te_texture_t;
typedef struct te_shader_t te_shader_t;
typedef struct te_font_t te_font_t;

typedef unsigned int te_color_t;
typedef struct { TEA_TNUM x, y; } te_point_t;
typedef struct { TEA_TNUM x, y, w, h; } te_rect_t;
typedef struct {te_point_t translate; TEA_TNUM angle; te_point_t scale; te_point_t origin;} te_transform_t;

struct te_texinfo_t {
  struct { unsigned int w, h; } size;
  int format, usage;
  int filter[2];
  int wrap[2];
};

struct te_config_t {
  unsigned char title[100];
  int width, height;

  int flags;
  int render_flags;
  int window_flags;
};


TEA_API te_config_t tea_config_init(const char *title, int w, int h);
TEA_API int tea_init(te_config_t *conf);
TEA_API int tea_deinit();

TEA_API int tea_quit();

TEA_API int tea_begin();
TEA_API int tea_end();

TEA_API int tea_should_close();

TEA_API float tea_delta();
TEA_API int tea_fps();

/* Render */
TEA_API int tea_clear(te_color_t col);
TEA_API int tea_mode(int mode);
TEA_API te_color_t tea_color(te_color_t col);

TEA_API int tea_point(TEA_TNUM x, TEA_TNUM y);
TEA_API int tea_line(TEA_TNUM x0, TEA_TNUM y0, TEA_TNUM x1, TEA_TNUM y1);
TEA_API int tea_rect(TEA_TNUM x, TEA_TNUM y, TEA_TNUM w, TEA_TNUM h);
TEA_API int tea_circle(TEA_TNUM x, TEA_TNUM y, TEA_TNUM radius);
TEA_API int tea_triangle(TEA_TNUM x0, TEA_TNUM y0, TEA_TNUM x1, TEA_TNUM y1, TEA_TNUM x2, TEA_TNUM y2);

TEA_API int tea_print(const char *text, TEA_TNUM x, TEA_TNUM y);

TEA_API int tea_set_target(te_texture_t *tex);
TEA_API int tea_set_shader(te_shader_t *shader);
TEA_API int tea_set_font(te_font_t *font);
TEA_API int tea_set_transform(te_transform_t *t);

TEA_API int tea_clip(te_rect_t *clip);
TEA_API int tea_translate(TEA_TNUM x, TEA_TNUM y);
TEA_API int tea_rotate(TEA_TNUM angle);
TEA_API int tea_scale(TEA_TNUM x, TEA_TNUM y);
TEA_API int tea_origin(TEA_TNUM x, TEA_TNUM y);

/* Texture */
TEA_API int tea_texture_info(te_texture_t *tex, te_texinfo_t *info);
TEA_API te_texture_t* tea_texture(void *data, int w, int h, int format, int usage);
TEA_API te_texture_t* tea_texture_load(const char *path, int usage);

TEA_API int tea_texture_update(te_texture_t *tex, te_rect_t *rect, void *data);

TEA_API int tea_texture_width(te_texture_t *tex);
TEA_API int tea_texture_height(te_texture_t *tex);
TEA_API int tea_texture_size(te_texture_t *tex, int *w_out, int *h_out);

TEA_API int tea_texture_draw(te_texture_t *tex, te_rect_t *dest, te_rect_t *src);
TEA_API int tea_texture_draw_ex(te_texture_t *tex, te_rect_t *dest, te_rect_t *src, TEA_TNUM angle, te_point_t *origin, int flip);

/* Font */
TEA_API te_font_t* tea_default_font();

TEA_API te_font_t* tea_font(void *data, int size, int font_size);
TEA_API te_font_t* tea_font_ttf(void *data, int size, int font_size);
TEA_API te_font_t* tea_font_load(const char *path, int usage);
TEA_API te_font_t* tea_font_bitmap(te_texture_t *tex, int size, int top, int right);
TEA_API int tea_font_print(te_font_t *font, const char *text, TEA_TNUM x, TEA_TNUM y);
TEA_API int tea_font_printf(te_font_t *font, const char *text, TEA_TNUM x, TEA_TNUM y, TEA_TNUM angle, te_point_t *scale);

/* Shader */
TEA_API te_shader_t* tea_shader(const char *frag, const char *vert);
TEA_API te_shader_t* tea_shader_load(const char *glsl);
TEA_API int tea_shader_send(te_shader_t *shader, int type, const char *name, void *data);
TEA_API int tea_shader_send_count(te_shader_t *shader, int type, const char *name, void *data, int count);

TEA_API int tea_compile_shader(const char *source, int type);

/*******************************
 * Window
 *******************************/

TEA_API int tea_window_flags();

TEA_API int tea_window_title(const char *title, char *out);

TEA_API int tea_window_pos(te_point_t *out, int x, int y);

TEA_API int tea_window_width(int *out, int width);
TEA_API int tea_window_height(int *out, int height);
TEA_API int tea_window_size(te_point_t *out, int width, int height);
TEA_API int tea_window_minsize(te_point_t *out, int width, int height);
TEA_API int tea_window_maxsize(te_point_t *out, int width, int height);

TEA_API int tea_window_minimize();
TEA_API int tea_window_maximize();
TEA_API int tea_window_restore();
TEA_API int tea_window_fullscreen(int mode);
TEA_API int tea_window_bordered(int bordered);


/*******************************
 * Input
 *******************************/

TEA_API int tea_update_input();
TEA_API int tea_key_from_name(const char *name);

TEA_API int tea_key_down(int key);
TEA_API int tea_key_up(int key);
TEA_API int tea_key_pressed(int key);
TEA_API int tea_key_released(int key);

TEA_API int tea_mouse_down(int btn);
TEA_API int tea_mouse_up(int btn);
TEA_API int tea_mouse_pressed(int btn);
TEA_API int tea_mouse_released(int btn);

TEA_API float tea_jpad_axis(int jid, int axis);
TEA_API int tea_jpad_down(int jid, int btn);
TEA_API int tea_jpad_up(int jid, int btn);
TEA_API int tea_jpad_pressed(int jid, int btn);
TEA_API int tea_jpad_released(int jid, int btn);

/********************************
 * Debug
 ********************************/

TEA_API const char* tea_geterror();
TEA_API int tea_error(const char *fmt, ...);
TEA_API int tea_log(int line, const char *func, const char *fmt, ...);

#endif /* TEA_H */

#ifndef CAFE_H
#define CAFE_H

#define CAFE_API
#define CAFE_VERSION "0.1.0"

typedef struct Cafe Cafe;

#ifndef CAFE_VALUE
  #define CAFE_VALUE float
#endif

typedef unsigned char Byte;

typedef unsigned int cf_Canvas;
typedef unsigned int cf_Image;

typedef struct { CAFE_VALUE x, y; } cf_Point;
typedef struct { Byte r, g, b, a; } cf_Color;
typedef struct { CAFE_VALUE x, y, w, h; } cf_Rect;

typedef enum { CAFE_LINE = 0, CAFE_FILL } CAFE_DRAW_MODE_;

typedef struct cf_Config {
  char title[128];
  unsigned int width, height;
  unsigned int flags;
  unsigned int window_flags;
} cf_Config;

CAFE_API void cafe_init_config(cf_Config *conf, const char *title, int width, int height);
CAFE_API int cafe_init(cf_Config *conf);
CAFE_API void cafe_terminate();

CAFE_API int cafe_should_close();

CAFE_API void cafe_begin();
CAFE_API void cafe_end();

CAFE_API void cafe_graphics_clear(cf_Color color);
CAFE_API void cafe_graphics_draw_color(cf_Color color);
CAFE_API void cafe_graphics_set_canvas(cf_Canvas canvas);

CAFE_API void cafe_graphics_rectangle(CAFE_VALUE x, CAFE_VALUE y, CAFE_VALUE w, CAFE_VALUE h, CAFE_DRAW_MODE_ mode);
CAFE_API void cafe_graphics_circle(CAFE_VALUE x, CAFE_VALUE y, CAFE_VALUE radius, CAFE_DRAW_MODE_ mode);
CAFE_API void cafe_graphics_triangle(CAFE_VALUE x0, CAFE_VALUE y0, CAFE_VALUE x1, CAFE_VALUE y1, CAFE_VALUE x2, CAFE_VALUE y2, CAFE_DRAW_MODE_ mode);
// CAFE_API void cafe_graphics_

#endif /* CAFE_H */
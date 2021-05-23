#ifndef CAFE_H
#define CAFE_H

#define CAFE_VER "0.1.0"
#define CAFE_API

#ifndef CAFE_TNUM
  #define CAFE_TNUM float
#endif

typedef unsigned int cf_Image;
typedef unsigned int cf_Canvas;
typedef unsigned int cf_Audio;

typedef struct { CAFE_TNUM x, y; } cf_Point;
typedef struct { CAFE_TNUM x, y, w, h; } cf_Rect;

CAFE_API int cafe_init(int flags);
CAFE_API int cafe_deinit();

CAFE_API int cafe_loop();
CAFE_API int cafe_begin();
CAFE_API int cafe_end();

/************************
 * Window
 ************************/

CAFE_API int cafe_window_flags();

CAFE_API int cafe_window_title(const char *title, char *out);
CAFE_API int cafe_window_pos(cf_Point *out, int x, int y);
CAFE_API int cafe_window_size(cf_Point *out, int w, int h);
CAFE_API int cafe_window_minsize(cf_Point *out, int w, int h);
CAFE_API int cafe_window_maxsize(cf_Point *out, int w, int h);

CAFE_API int cafe_window_minimize();
CAFE_API int cafe_window_maximize();
CAFE_API int cafe_window_restore();
CAFE_API int cafe_window_fullscreen(int mode);
CAFE_API int cafe_window_bordered(int bordered);

/*********************
 * Graphics
 *********************/

CAFE_API int cafe_draw_point(CAFE_TNUM x, CAFE_TNUM y);
CAFE_API int cafe_draw_line(CAFE_TNUM x0, CAFE_TNUM y0, CAFE_TNUM x1, CAFE_TNUM y1);
CAFE_API int cafe_draw_rectangle(CAFE_TNUM x, CAFE_TNUM y, CAFE_TNUM w, CAFE_TNUM h);
CAFE_API int cafe_draw_circle(CAFE_TNUM x, CAFE_TNUM y, CAFE_TNUM radius);
CAFE_API int cafe_draw_triangle(CAFE_TNUM x0, CAFE_TNUM y0, CAFE_TNUM x1, CAFE_TNUM y1, CAFE_TNUM x2, CAFE_TNUM y2);

CAFE_API cf_Image cafe_image(void *data, int w, int h, int format);
CAFE_API int cafe_image_width(cf_Image id, int width);
CAFE_API int cafe_image_height(cf_Image id, int height);
CAFE_API int cafe_image_size(cf_Image id, int *width, int *height);
CAFE_API int cafe_image_draw(cf_Image id, CAFE_TNUM x, CAFE_TNUM y, CAFE_TNUM angle, cf_Point scale, cf_Point origin);

CAFE_API cf_Canvas cafe_canvas(int w, int h);
CAFE_API int cafe_canvas_width(cf_Canvas id);
CAFE_API int cafe_canvas_height(cf_Canvas id);
CAFE_API int cafe_canvas_size(cf_Canvas id, int *width, int *height);
CAFE_API int cafe_canvas_draw(cf_Canvas id, CAFE_TNUM x, CAFE_TNUM y, CAFE_TNUM angle, cf_Point scale, cf_Point origin);

#endif /* CAFE_H */

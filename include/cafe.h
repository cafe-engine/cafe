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

typedef unsigned char Byte;

typedef unsigned int cf_Canvas;
typedef unsigned int cf_Image;
typedef unsigned int cf_Shader;

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

#endif /* CAFE_H */

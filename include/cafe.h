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

typedef struct la_file_t cf_File;
typedef struct la_dir_t cf_Dir;
typedef struct la_vdrive_t cf_VDrive;

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
 * Filesystem         *
 **********************/

CAFE_API int cafe_filesystem_init(const char *filepath);
CAFE_API void cafe_filesystem_deinit();

CAFE_API int cafe_filesystem_basepath(const char *path);

CAFE_API long cafe_filesystem_size(const char *filename);
CAFE_API int cafe_filesystem_read(const char *filename, char *out, int size);
CAFE_API int cafe_filesystem_write(const char *filename, const char *text, int size);

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

CAFE_API int cafe_audio_init();
CAFE_API int cafe_audio_deinit();

CAFE_API void* cafe_audiobuf_load(const char *filename, int usage);
CAFE_API void cafe_audiobuf_destroy(void *buf);

CAFE_API int cafe_audiobuf_play(void *buf);
CAFE_API int cafe_audiobuf_pause(void *buf);
CAFE_API int cafe_audiobuf_stop(void *buf);

CAFE_API cf_Sound cafe_sound_load(const char *filename);

CAFE_API int cafe_sound_play(cf_Sound snd);
CAFE_API int cafe_sound_pause(cf_Sound snd);
CAFE_API int cafe_sound_stop(cf_Sound snd);

CAFE_API cf_Music cafe_music_load(const char *filename);

CAFE_API int cafe_music_play(cf_Music mus);
CAFE_API int cafe_music_pause(cf_Music mus);
CAFE_API int cafe_music_stop(cf_Music mus);

#endif /* CAFE_H */

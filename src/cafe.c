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

#include "cafe.h"
#include <SDL2/SDL_keyboard.h>

#define TEA_VALUE CAFE_VALUE
#include "tea.h"

#include "mocha.h"
#include "coffee.h"
#include "latte.h"

#include "lua/cafe_lua.h"

#include "cstar.h"

void cafe_init_config(cf_Config *conf, const char *title, int width, int height) {
        if (!conf) return;

        if (title) strcpy(conf->title, title);
        else strcpy(conf->title, "cafe 0.1.0");

        conf->width = width;
        conf->height = height;
}

int cafe_init(cf_Config *conf) {
        te_Config tea_cfg;
        tea_config_init(&tea_cfg, conf->title, conf->width, conf->height);

        tea_cfg.window_flags = conf->window_flags;

        tea_init(&tea_cfg);
        cafe_filesystem_init(".");

        cafe_lua_init();
        return 1;
}

void cafe_terminate() {
  tea_terminate();
}

int cafe_main_loop() {
    cafe_begin();

    cafe_lua_step();

    cafe_end();

    return !cafe_should_close();
}

int cafe_should_close() {
  return tea_should_close();
}

void cafe_begin() {
  tea_begin();
}

void cafe_end() {
  tea_end();
}

void cafe_graphics_clear(cf_Color color) {
  tea_clear_color((te_Color){color.r, color.g, color.b, color.a});
  tea_clear();
}

void cafe_graphics_draw_color(cf_Color color) {
  tea_draw_color((te_Color){color.r, color.g, color.b, color.a});
}

void cafe_graphics_rectangle(CAFE_VALUE x, CAFE_VALUE y, CAFE_VALUE w, CAFE_VALUE h, CAFE_DRAW_MODE_ mode) {
  tea_draw_mode((TEA_DRAW_MODE)mode);
  tea_draw_rect_(x, y, w, h);
}

void cafe_graphics_circle(CAFE_VALUE x, CAFE_VALUE y, CAFE_VALUE radius, CAFE_DRAW_MODE_ mode) {
  tea_draw_mode((TEA_DRAW_MODE)mode);
  tea_draw_circle(tea_point(x, y), radius);
}

void cafe_graphics_triangle(CAFE_VALUE x0, CAFE_VALUE y0, CAFE_VALUE x1, CAFE_VALUE y1, CAFE_VALUE x2, CAFE_VALUE y2, CAFE_DRAW_MODE_ mode) {
  tea_draw_mode((TEA_DRAW_MODE)mode);
  tea_draw_triangle(tea_point(x0, y0), tea_point(x1, y1), tea_point(x2, y2));
}

/* Canvas */

cf_Canvas cafe_canvas(int w, int h) {
  return tea_canvas(w, h);
}

void cafe_graphics_draw_canvas(cf_Canvas canvas, cf_Rect *r, CAFE_VALUE x, CAFE_VALUE y) {
  tea_draw_canvas(canvas, (te_Rect*)r, tea_point(x, y));
}

void cafe_graphics_set_canvas(cf_Canvas c) {
  tea_set_canvas(c);
}

/* Image */

cf_Image cafe_image(int w, int h, int format) {
  return tea_image(w, h, format);
}

cf_Image cafe_image_load(const char *filename) {
  return tea_image_load(filename);
}

void cafe_graphics_draw_image(cf_Image img, cf_Rect *r, CAFE_VALUE x, CAFE_VALUE y) {
  tea_draw_image(img, (te_Rect*)r, tea_point(x, y));
}

void cafe_graphics_draw_image_ex(cf_Image img, cf_Rect *r, cf_Point p, float angle, cf_Point scale, cf_Point origin) {
        te_Point *pp = (te_Point*)&p;
        tea_draw_image_ex(img, (te_Rect*)r, *pp, angle, tea_point(scale.x, scale.y), tea_point(origin.x, origin.y));
}

/**********************
 * Input              *
 **********************/

int cafe_key_from_name(const char *keyname) {
    int key = SDL_GetScancodeFromName(keyname);
    return key;
}

int cafe_input_init(int flags) {
    return 1;
}

int cafe_input_deinit() {
    return 1;
}

/* Keyboard */
int cafe_keyboard_isdown(int key) {
    return tea_keyboard_is_down(key);
}

int cafe_keyboard_isup(int key) {
    return tea_keyboard_is_up(key);
}

int cafe_keyboard_waspressed(int key) {
    return tea_keyboard_was_pressed(key);
}

int cafe_keyboard_wasreleased(int key) {
    return tea_keyboard_was_released(key);
}

/* Mouse */
int cafe_mouse_xpos(int *x);
int cafe_mouse_ypos(int *y);
int cafe_mouse_pos(cf_Point *out);

int cafe_mouse_isdown(int button) {
    return tea_mouse_is_down(button);
}

int cafe_mouse_isup(int button) {
    return tea_mouse_is_up(button);
}

int cafe_mouse_waspressed(int button) {
    return tea_mouse_was_pressed(button);
}

int cafe_mouse_wasreleased(int button) {
    return tea_mouse_was_released(button);
}

/* Joystick */
float cafe_joystick_axis(int jid, int axis) {
    return 0.0f;
}
int cafe_joystick_isdown(int jid, int button);
int cafe_joystick_isup(int jid, int button);
int cafe_joystick_waspressed(int jid, int button);
int cafe_joystick_wasreleased(int jid, int button);

/**********************
 * Filesystem         *
 **********************/

int cafe_filesystem_init(const char *filepath) {
    la_init(filepath);
    return 1;
}

void cafe_filesystem_deinit() {
    la_deinit();
}

int cafe_filesystem_basepath(const char *path) {
    return 1;
}

long cafe_filesystem_size(const char *filename) {
    cf_File *fp = cafe_file_open(filename, LA_READ_MODE);
    cf_Header h;
    cafe_file_header(fp, &h);

    cafe_file_close(fp);

    return h.size;
}

int cafe_filesystem_read(const char *filename, char *out, int size);
int cafe_filesystem_write(const char *filename, const char *text, int size);

int cafe_filesystem_touch(const char *filename) {
    return la_touch(filename);
}

int cafe_filesystem_rm(const char *filename) {
    return la_rm(filename);
}

int cafe_filesystem_mkdir(const char *path) {
    return la_mkdir(path);
}

int cafe_filesystem_rmdir(const char *path) {
    return la_rmdir(path);
}

int cafe_filesystem_isfile(const char *filename) {
    return la_isfile(filename);
}

int cafe_filesystem_isdir(const char *filename) {
    return la_isdir(filename);
}

/* File */
cf_File* cafe_file_open(const char *filename, int mode) {
    return (cf_File*)la_fopen(filename, mode);
}

void cafe_file_close(cf_File *fp) {
    la_fclose((la_file_t*)fp);
}

int cafe_file_close_stream(cf_File *fp) {
    return la_fclose_stream((la_file_t*)fp);
}

int cafe_file_header(cf_File *fp, cf_Header *out) {
    return la_fheader((la_file_t*)fp, (la_header_t*)out);
}
int cafe_file_read(cf_File *fp, char *out, int bytes) {
    return la_fread((la_file_t*)fp, out, bytes); 
}

int cafe_file_write(cf_File *fp, const char *text, int bytes) {
    return la_fwrite((la_file_t*)fp, text, bytes);
}

/* Directory */

cf_Dir* cafe_dir_open(const char *filename) {
    return (cf_Dir*)la_dopen(filename);
}

void cafe_dir_close(cf_Dir *dir) {
    la_dclose((la_dir_t*)dir);
}

int cafe_dir_rewind(cf_Dir *dir) {
    /* return la_drewind((la_dir_t*)dir); */
    return 1;
}

int cafe_dir_header(cf_Dir *dir, cf_Header *out) {
    return la_dheader((la_dir_t*)dir, (la_header_t*)out);
}

int cafe_dir_read(cf_Dir *dir, cf_Header *out) {
    return la_dread((la_dir_t*)dir, (la_header_t*)out);
}

/* Virtual */

cf_VDrive* cafe_vdrive_open(const char *filename, int mode) {
    return (cf_VDrive*)la_vopen(filename, mode);
}

void cafe_vdrive_close(cf_VDrive *drv) {
    la_vclose((la_vdrive_t*)drv);
}

/**********************
 * Audio              *
 **********************/


void* cafe_audio_load(const char *filename, int usage) {
    return mocha_buffer_load(filename, usage);
}

void cafe_audio_destroy(void *buf) {
    mocha_buffer_unload(buf);
}

int cafe_audio_play(cf_Audio *buf) {
    if (buf == NULL) cst_tracefatal("empty audio buffer");
    
    mocha_buffer_play((mo_AudioBuffer*)buf);
    return 1;
}

int cafe_audio_pause(cf_Audio *buf) {
   if (buf == NULL) cst_tracefatal("empty audio buffer"); 
   mocha_buffer_pause((mo_AudioBuffer*)buf);
   return 1;
}

int cafe_audio_stop(cf_Audio *buf) {
    if (buf == NULL) cst_tracefatal("empty audio buffer");
    mocha_buffer_stop((mo_AudioBuffer*)buf);
    return 1;
}

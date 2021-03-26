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
#include "SDL_render.h"
#include <SDL2/SDL_keyboard.h>

#define TEA_VALUE CAFE_VALUE
#include "tea.h"

#include "mocha.h"
#include "coffee.h"
#include "latte.h"

#include "lua/cafe_lua.h"

#include "cstar.h"

#define cafe() (&_cafe_ctx)

struct Cafe {
    te_Texture *textures[CAFE_MAX_TEXTURES];
    te_RenderTarget *targets[CAFE_MAX_RTARGETS];
};

Cafe _cafe_ctx;

void cafe_init_config(cf_Config *conf, const char *title, int width, int height) {
        if (!conf) return;

        if (title) strcpy(conf->title, title);
        else strcpy(conf->title, "cafe 0.1.0");

        conf->width = width;
        conf->height = height;

        conf->argv = NULL;
        conf->argc = 0;
}

int cafe_init(cf_Config *conf) {
        te_Config tea_cfg;
        tea_config_init(&tea_cfg, conf->title, conf->width, conf->height);

        memset(cafe()->textures, 0, sizeof(te_Texture*)*CAFE_MAX_TEXTURES);
        memset(cafe()->targets, 0, sizeof(te_RenderTarget*)*CAFE_MAX_RTARGETS);

        tea_cfg.window_flags = conf->window_flags;

        tea_init(&tea_cfg);
        int has_argv = conf->argv && conf->argv[1];
        cafe_filesystem_init(has_argv ? conf->argv[1] : ".");

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

cf_Canvas _get_rtarget_free_slot() {
    int i = 0;
    while (i < CAFE_MAX_RTARGETS) {
       if (cafe()->targets[i] == NULL) return i; 
       i++;
    }
    cst_error("Render Target slot is FULL");

    return -1; 
}

cf_Canvas cafe_canvas(int w, int h) {
    cf_Canvas canvas = _get_rtarget_free_slot();
    if (canvas < 0 || canvas > CAFE_MAX_RTARGETS) cst_fatal("failed to create cf_Canvas: %d", canvas);
    cafe()->targets[canvas] = tea_render_target(w, h, TEA_RGBA);

    return canvas+1;
}

void cafe_graphics_draw_canvas(cf_Canvas canvas, cf_Rect *r, CAFE_VALUE x, CAFE_VALUE y) {
    // tea_draw_canvas(canvas, (te_Rect*)r, tea_point(x, y));
    if (canvas <= 0 || canvas > CAFE_MAX_RTARGETS) cst_tracefatal("invalid cf_Canvas");
    te_RenderTarget *target = cafe()->targets[canvas-1];
    te_Texture *tex = (te_Texture*)target;
  
    te_Point size;
    if (r) {
        x = r->x;
        y = r->y;
        size.x = r->w;
        size.y = r->h;
    } else tea_texture_size(tex, &size);
  te_Transform t = tea_get_transform();

  TEA_VALUE px, py;
  px = x - t.origin.x + t.translate.x;
  py = y - t.origin.y + t.translate.y;

  tea_draw_texture(tex, &tea_rect(px, py, size.x*t.scale.x, size.y*t.scale.y), &tea_rect(x, y, size.x, size.y));
}

void cafe_graphics_draw_canvas_ex(cf_Canvas canvas, cf_Rect *r, cf_Point pos, CAFE_VALUE angle, cf_Point scale, cf_Point origin) {
    /*te_Point *p = (te_Point*)&pos;
    te_Point *scl = (te_Point*)&scale;
    te_Point *org = (te_Point*)&org;
    tea_draw_canvas_ex(canvas, (te_Rect*)r, *p, angle, *scl, *org);*/
    if (canvas <= 0 || canvas > CAFE_MAX_RTARGETS) cst_tracefatal("invalid cf_Canvas");
    
    te_RenderTarget *target = cafe()->targets[canvas-1];
    te_Texture *tex = (te_Texture*)target;
    te_RenderFlip flip = 0;
    if (scale.x < 0) {
        scale.x *= -1;
        flip |= TEA_FLIP_H;
    }

    if (scale.y < 0) {
        scale.y *= -1;
        flip |= TEA_FLIP_V;
    }

    int x, y;
    te_Point size;
    x = y = 0;
    if (r) {
        x = r->x;
        y = r->y;
        size.x = r->w;
        size.y = r->h;
    } else tea_texture_size(tex, &size);
    te_Rect dest = tea_rect(x, y, size.x*scale.x, size.y*scale.y);
    te_Rect src = tea_rect(x, y, size.x, size.y);
    tea_draw_texture_ex(tex, &dest, &src, angle, tea_point(origin.x*scale.x, origin.y*scale.y), flip);
}

void cafe_graphics_set_canvas(cf_Canvas c) {
    /*tea_set_canvas(c);*/
    if (c < 0 || c > CAFE_MAX_RTARGETS) cst_fatal("invalid cf_Canvas: %d", c);
    te_RenderTarget *target = NULL;
    if (c > 0) target = cafe()->targets[c-1];

    tea_set_render_target(target);
}

/* Image */

cf_Image _get_texture_free_slot() {
    int i = 0;
    while (i < CAFE_MAX_TEXTURES) {
        if (cafe()->textures[i] == NULL) return i;
        i++;
    }
    cst_error("Texture slot is FULL");

    return -1;
}

cf_Image cafe_image(int w, int h, int format) {
    cf_Image img = _get_texture_free_slot();
    if (img < 0 || img >= CAFE_MAX_TEXTURES) cst_fatal("failed to create cf_Image: %d", img);

    cafe()->textures[img] = tea_texture(w, h, format, SDL_TEXTUREACCESS_STATIC);
    return img+1;
}

cf_Image cafe_image_load(const char *filename) {
    char file[100];
    la_resolve_path(filename, file);
    cf_Image img = _get_texture_free_slot();
    if (img < 0 || img >= CAFE_MAX_TEXTURES) cst_fatal("failed to create cf_Image: %d", img);
    cafe()->textures[img] = tea_texture_load(file);
    return img+1;
}

void cafe_graphics_draw_image(cf_Image img, cf_Rect *r, CAFE_VALUE x, CAFE_VALUE y) {
  /*tea_draw_image(img, (te_Rect*)r, tea_point(x, y));*/
    if (img <= 0 || img >= CAFE_MAX_TEXTURES) cst_fatal("invalid cf_Image: %d", img);
    img--;
    te_Texture *tex = cafe()->textures[img];
    te_Point size;
    if (r) {
        size.x = r->w;
        size.y = r->h;
    } else tea_texture_size(tex, &size);
    te_Transform t = tea_get_transform();

    TEA_VALUE px, py;
    px = x - t.origin.x + t.translate.x;
    py = y - t.origin.y + t.translate.y;

    tea_draw_texture(tex, &tea_rect(px, py, size.x*t.scale.x, size.y*t.scale.y), (te_Rect*)r);
}

void cafe_graphics_draw_image_ex(cf_Image img, cf_Rect *r, cf_Point p, float angle, cf_Point scale, cf_Point origin) {
    if (img < 0 || img >= CAFE_MAX_TEXTURES) cst_fatal("invalid cf_Image: %d", img);
    int x, y;
    x = y = 0;
    te_Point size;
    te_Texture *tex = cafe()->textures[img];
    if (r) {
        x = r->x;
        y = r->y;
        size.x = r->w;
        size.y = r->h;
    } else tea_texture_size(tex, &size);
    te_Transform t = tea_get_transform();

    CAFE_VALUE px, py;
    px = p.x - t.origin.x + t.translate.x;
    py = p.y - t.origin.y + t.translate.y;

    tea_draw_texture(tex, &tea_rect(px, py, size.x*t.scale.x, size.y*t.scale.y), &tea_rect(x, y, size.x, size.y));
}

/**********************
 * Input              *
 **********************/

int cafe_key_from_name(const char *keyname) {
    int key = SDL_GetScancodeFromName(keyname);
    return key;
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
int cafe_mouse_xpos(int *x) {
    return tea_mouse_x();
}

int cafe_mouse_ypos(int *y) {
    return tea_mouse_y();
}

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
    return la_init(filepath);
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

int cafe_file_seek(cf_File *fp, int offset) {
    return la_fseek((la_file_t*)fp, offset);
}

int cafe_file_tell(cf_File *fp) {
    return la_ftell((la_file_t*)fp);
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
    char file[100];
    la_resolve_path(filename, file);
    return mocha_buffer_load(file, usage);
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

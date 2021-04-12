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
#include "tea.h"

#define TEA_VALUE CAFE_VALUE
#include "tea_api.h"

#include "mocha.h"
#include "coffee.h"
#include "latte.h"

#include "lua/cafe_lua.h"

#include "cstar.h"

#define cafe() (&_cafe_ctx)

struct Cafe {
    te_Texture *textures[CAFE_MAX_TEXTURES];
    la_vfs_t *root;
    te_Font *font;
    mo_audio_t *audio_bank[CAFE_MAX_AUDIO];
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

        tea_cfg.window_flags = conf->window_flags;

        tea_init(&tea_cfg);
        mo_init(0);
        int has_argv = conf->argv && conf->argv[1];
        cafe_filesystem_init(has_argv ? conf->argv[1] : "game.tar");

        cf_File *fp = cafe_file_open("font.ttf", LA_READ_MODE);

        cf_Header h;
        cafe_file_header(fp, &h);
        char out[h.size+1];
        cafe_file_read(fp, out, h.size);
        out[h.size] = '\0';

        cafe()->font = tea_create_font(out, h.size, 16);

        cafe_lua_init();
        return 1;
}

void cafe_terminate() {
  tea_quit();
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

int cafe_graphics_get_mode() {
    return tea_get_draw_mode();
}

int cafe_graphics_mode(int mode) {
    tea_draw_mode(mode);
    return 1;
}

void cafe_graphics_clear(cf_Color color) {
  tea_clear_color((te_Color){color.r, color.g, color.b, color.a});
  tea_clear();
}

void cafe_graphics_draw_color(cf_Color color) {
    te_Color *col = (te_Color*)&color;
    tea_draw_color(*col);
}

void cafe_graphics_rectangle(CAFE_VALUE x, CAFE_VALUE y, CAFE_VALUE w, CAFE_VALUE h) {
  tea_rect(&TEA_RECT(x, y, w, h));
}

void cafe_graphics_circle(CAFE_VALUE x, CAFE_VALUE y, CAFE_VALUE radius) {
    tea_circle(TEA_POINT(x, y), radius);
}

void cafe_graphics_triangle(cf_Point p0, cf_Point p1, cf_Point p2) {
    te_Point *tp0 = (te_Point*)&p0;
    te_Point *tp1 = (te_Point*)&p1;
    te_Point *tp2 = (te_Point*)&p2;
    tea_triangle(*tp0, *tp1, *tp2);
}

int cafe_graphics_print(cf_Font *font, const char *text, CAFE_VALUE x, CAFE_VALUE y) {
    if (!font) font = cafe()->font;
    tea_print((te_Font*)font, text, x, y); 
    return 1;
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

    cafe()->textures[img] = tea_create_texture(NULL, w, h, format, TEA_TEXTURE_STATIC);
    return img+1;
}

cf_Image cafe_image_load(const char *filename) {
    char file[100];
    cf_Image img = _get_texture_free_slot();
    if (img < 0 || img >= CAFE_MAX_TEXTURES) cst_fatal("failed to create cf_Image: %d", img);
    cf_File *fp = cafe_file_open(filename, LA_READ_MODE);
    if (!fp) la_fatal("failed to load image: %s", filename);
    cf_Header h;
    cafe_file_header(fp, &h);

    char out[h.size+1];
    cafe_file_read(fp, out, h.size);
    out[h.size] = '\0';
    cafe()->textures[img] = tea_load_texture_from_memory(out, h.size, TEA_TEXTURE_STATIC);
    cafe_file_close(fp);
    if (!cafe()->textures[img])
        cst_fatal("failed to load image: %s", filename);
    return img+1;
}

void cafe_graphics_draw_image(cf_Image img, cf_Rect *r, CAFE_VALUE x, CAFE_VALUE y) {
  /*tea_draw_image(img, (te_Rect*)r, tea_point(x, y));*/
    if (img <= 0 || img >= CAFE_MAX_TEXTURES) cst_fatal("invalid cf_Image: %d", img);
    img--;
    te_Texture *tex = cafe()->textures[img];
    te_Point size;

    te_TextureInfo info;
    tea_texture_info(tex, &info);
    size.x = info.size.w;
    size.y = info.size.h;
    if (r) {
        size.x = r->w;
        size.y = r->h;
    }
    te_Transform t;
    tea_get_transform(&t);

    if (!tea_texture(tex, &TEA_RECT(x, y, size.x, size.y), (te_Rect*)r)) {
        cst_fatal("failed to draw texture: NULL");
    }
}

void cafe_graphics_draw_image_ex(cf_Image img, cf_Rect *r, cf_Point p, float angle, cf_Point scale, cf_Point origin) {
    if (img < 0 || img >= CAFE_MAX_TEXTURES) cst_fatal("invalid cf_Image: %d", img);
    int x, y;
    int flip = TEA_FLIP_NONE;
    x = y = 0;
    img--;
    te_Point size;
    te_Texture *tex = cafe()->textures[img];
    te_TextureInfo info;
    tea_texture_info(tex, &info);
    size.x = info.size.w;
    size.y = info.size.h;
    if (r) {
        x = r->x;
        y = r->y;
        size.x = r->w;
        size.y = r->h;
    }
    te_Transform t;
    tea_get_transform(&t);
    if (scale.x < 0) {
        scale.x = fabs(scale.x);
        flip = TEA_FLIP_H;
    }
    if (scale.y < 0) {
        scale.y = fabs(scale.y);
        flip = TEA_FLIP_V;
    }

    tea_texture_ex(tex, &TEA_RECT(p.x, p.y, size.x*scale.x, size.y*scale.y), (te_Rect*)r, angle, TEA_POINT(origin.x, origin.y), flip);
}
/* Canvas */

cf_Canvas cafe_canvas(int w, int h) {
    cf_Canvas canvas = _get_texture_free_slot();
    if (canvas < 0 || canvas > CAFE_MAX_TEXTURES) cst_fatal("failed to create cf_Canvas: %d", canvas);
    cafe()->textures[canvas] = tea_create_texture(NULL, w, h, TEA_RGBA, TEA_TEXTURE_TARGET);

    return canvas+1;
}

#if 0
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
#endif

void cafe_graphics_set_canvas(cf_Canvas c) {
    /*tea_set_canvas(c);*/
    if (c < 0 || c > CAFE_MAX_TEXTURES) cst_fatal("invalid cf_Canvas: %d", c);
    te_Texture *target = NULL;
    if (c > 0) target = cafe()->textures[c-1];

    tea_set_target(target);
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

int cafe_mouse_pos(cf_Point *out) {
    if (!out) return 0;
    return tea_mouse_pos(&out->x, &out->y);
}

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

int cafe_joystick_isdown(int jid, int button) {
    return tea_joystick_is_up(jid, button);
}

int cafe_joystick_isup(int jid, int button) {
    return tea_joystick_is_down(jid, button);
}

int cafe_joystick_waspressed(int jid, int button) {
    return tea_joystick_was_pressed(jid, button);
}

int cafe_joystick_wasreleased(int jid, int button) {
    return tea_joystick_was_released(jid, button);
}

/**********************
 * Filesystem         *
 **********************/

int cafe_filesystem_init(const char *filepath) {
    char path[100];
    char file[100];
    file[0] = '\0';
    if (!filepath) strcpy(path, ".");
    else strcpy(path, filepath);

    char *str = strstr(path, ".tar");
    if (str) {
       while (*str != '/' && str != path) str--; 
       if (*str == '/') str++;
       strcpy(file, str);
       int pathlen = (char*)str - (char*)path; 
       la_log("%d", pathlen);
       if (pathlen > 0) { memcpy(path, filepath, pathlen); path[pathlen] = '\0'; }
       else strcpy(path, ".");
    }
    if (!la_init(path)) {
        cst_error("failed to init Latte");
        return 0;
    }
    cafe()->root = NULL;
    if (file[0]) {
        la_log("%s", file);
        cafe()->root = la_vopen(file, LA_READ_MODE);
    }

    return 1;
}

void cafe_filesystem_deinit() {
    la_deinit();
}

const char* cafe_filesystem_basepath() {
    return latte()->basedir;
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
    if (cafe()->root) return la_isvfile(cafe()->root, filename);
    return la_isfile(filename);
}

int cafe_filesystem_isdir(const char *filename) {
    return la_isdir(filename);
}

/* File */
cf_File* cafe_file_open(const char *filename, int mode) {
    if (cafe()->root) return (cf_File*)la_vfopen(cafe()->root, filename);
    return (cf_File*)la_fopen(filename, mode);
}

int cafe_file_close(cf_File *fp) {
    if (cafe()->root) return 1;
    return la_fclose((la_file_t*)fp);
}

int cafe_file_close_stream(cf_File *fp) {
    if (cafe()->root) return 1;
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
    la_vclose((la_vfs_t*)drv);
}

/**********************
 * Audio              *
 **********************/

cf_Audio cafe_audio_free_slot() {
    int i = 0;
    while (i < MAX_AUDIO_DATA) {
        if (cafe()->audio_bank[i] == NULL) return i;
        i++;
    }

    return -1;
}

cf_Audio cafe_audio(void *data, long size, int usage) {
    cf_Audio audio = cafe_audio_free_slot();
    cafe()->audio_bank[audio] = mo_audio(data, size, usage);
    return audio+1;
}

cf_Audio cafe_audio_load(const char *filename, int usage) {
    cf_File *fp = cafe_file_open(filename, LA_READ_MODE);
    cf_Header h;
    cafe_file_header(fp, &h);

    void *data = malloc(h.size);
    cafe_file_read(fp, data, h.size);

    return cafe_audio(data, h.size, usage);
}

int cafe_audio_destroy(cf_Audio buf) {
    //mo_audio(buf);
    return 1;
}

int cafe_audio_play(cf_Audio id) {
    if (id <= 0) cst_tracefatal("empty audio buffer");
    mo_audio_t *buf = cafe()->audio_bank[id-1];
    mo_play(buf);
    return 1;
}

int cafe_audio_pause(cf_Audio id) {
   if (id <= 0) cst_tracefatal("empty audio buffer"); 
   mo_audio_t *buf = cafe()->audio_bank[id-1];
   mo_pause(buf);
   return 1;
}

int cafe_audio_stop(cf_Audio id) {
    if (id <= 0) cst_tracefatal("empty audio buffer");
    mo_audio_t *buf = cafe()->audio_bank[id-1];
    mo_stop(buf);
    return 1;
}

cf_Sound cafe_sound(void *data, unsigned int size) {
    return cafe_audio(data, size, MO_AUDIO_STATIC);
}

cf_Sound cafe_sound_load(const char *filename) {
    return cafe_audio_load(filename, MO_AUDIO_STATIC);
}

cf_Music cafe_music(void *data, unsigned int size) {
    return cafe_audio(data, size, MO_AUDIO_STREAM);
}
cf_Music cafe_music_load(const char *filename) {
    return cafe_audio_load(filename, MO_AUDIO_STREAM);
}

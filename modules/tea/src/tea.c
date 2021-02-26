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

#include "tea.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "../external/stb_truetype.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb_image.h"

// struct te_Texture {
// #if defined(TEA_GL_RENDER)
//   unsigned int id;
// #else
//   SDL_Texture *tex;
// #endif
//   int width, height;
//   int wrap[2], filter[2];
// };

#define tea() (&_ctx)

// #if defined(TEA_GL_RENDER)
// typedef struct Texture Texture;

// struct Texture {
//   unsigned int id;
//   int width, height;
//   int wrap[2], filter[2];
// };

// #else
// typedef SDL_Texture Texture;
// typedef SDL_Texture Canvas;

// struct te_Texture {
//   SDL_Texture* tex;
//   unsigned int width, height;
//   unsigned short filter[2];
//   unsigned short wrap[2];
// };

// struct te_RenderTarget {
//   union {
//     te_Texture tex;
//     struct {
//       SDL_Texture* stex;
//       unsigned int width, height;
//       unsigned short filter[2];
//       unsigned short wrap[2];
//     };
//   };
// #if defined(TEA_GL_RENDER)
//   unsigned int fbo;
// #endif
// };
// #endif

#ifdef TE_GL_RENDER
typedef unsigned int Texture;
typedef unsigned int RenderTarget;
typedef unsigned int Shader;
#else
typedef SDL_Texture* Texture;
typedef SDL_Texture* RenderTarget;
typedef void* Shader;
#endif

struct te_Shader {
  Shader _shader;
};

struct te_Texture {
  Texture tex;
  unsigned int width, height;
  unsigned short filter[2];
  unsigned short wrap[2];
};

struct te_RenderTarget {
#ifdef TEA_GL_RENDER
  RenderTarget target;
#endif
  union {
    te_Texture tex;
    struct {
      Texture _tex;
      unsigned int width, height;
      unsigned short filter[2];
      unsigned short wrap[2];
    };
  };
};

struct te_Font {
  te_Texture* tex;
  struct {
    int ax; // advance.x
    int ay; // advance.y

    int bw; // bitmap.width;
    int bh; // bitmap.rows;

    int bl; // bitmap_left;
    int bt; // bitmap_top;

    float tx; // x offset of glyph in texture coordinates
  } c[MAX_FONT_CHAR];


  unsigned char size;

  stbtt_fontinfo info;
  float ptsize;
  float scale;
  int baseline;
  void *data;

#if defined(TEA_GL_RENDER)
  unsigned int atlas_vao;
  unsigned int atlas_vbo;
#endif

};

struct Tea {
  te_Render *render;
  te_Window *window;
  te_Event event;

  te_Color _clear_color;
  te_Color _color;
  TEA_DRAW_MODE _mode;

  te_Texture textures[MAX_TEXTURES];
  te_RenderTarget targets[MAX_RTARGETS];

  te_Transform current_transform;
  te_Transform transform[MAX_TRANSFORMS];

  struct {
    struct { Uint8 *state, old_state[TEA_KEY_COUNT]; } keyboard;
    struct { Uint8 state[TEA_BUTTON_COUNT], old_state[TEA_BUTTON_COUNT]; } mouse;
    Uint8 old_button;    
  } input;

  struct {
    unsigned char cp; // canvas pointer
    unsigned char tp; // transform pointer
    unsigned char sp; // shader pointer

    te_Canvas canvas[STACK_MAX];
    te_Transform transform[STACK_MAX];
    te_Shader shader[STACK_MAX];
  } stack;   

  struct {
    Uint32 prev_time;
    Uint32 current_time;
    float delta;
    
    Uint32 prev_fps_time;
    Uint32 frame;
    Uint32 fps;
  } timer;
};

static Tea _ctx;
static te_Config _conf;

int tea_config_init(te_Config *conf, const char *title, int width, int height) {
  if (!conf) conf = &_conf;
  title = title ? title : CAT("tea ", TEA_VERSION);

  if (title) strcpy((char*)conf->title, title);
  conf->width = width;
  conf->height = height;
  conf->flags = SDL_INIT_VIDEO;
  conf->window_flags = SDL_WINDOW_SHOWN;
  conf->render_flags = SDL_RENDERER_ACCELERATED;

  return 1;
}

Tea* tea_init(te_Config *c) {
  // if (!c) c = &_conf;
  TE_ASSERT(c != NULL, "te_Config cannot be NULL");
  // memset(tea(), 0, sizeof(*tea()));

  SDL_Init(c->flags);
  Tea *ctx = tea_context();
  memset(ctx, 0, sizeof(*ctx));

  ctx->current_transform.scale = tea_point(1, 1);

  ctx->_clear_color = BLACK;
  // ctx->key_array = SDL_GetKeyboardState(NULL);
  // ctx->input.key_array = SDL_GetKeyboardState(NULL);
  // memcpy(ctx->input.old_key, ctx->input.key_array, TEA_KEY_COUNT);
  // memset(ctx->input.keyboard.state, 0, TEA_KEY_COUNT);
  ctx->input.keyboard.state = SDL_GetKeyboardState(NULL);
  memset(ctx->input.keyboard.old_state, 0, TEA_KEY_COUNT);

  memset(ctx->textures, 0, MAX_TEXTURES * sizeof(te_Texture));
  memset(ctx->targets, 0, MAX_RTARGETS * sizeof(te_RenderTarget));

  ctx->window = tea_window_create((const char*)c->title, c->width, c->height, c->window_flags);
  ctx->render = tea_render_create(ctx->window, TEA_SOFTWARE_RENDER);

  return ctx;
}

Tea* tea_context() {
  return &_ctx;
}

void tea_terminate() {
  tea_window_destroy(tea()->window);
  tea_render_destroy(tea()->render);

  SDL_Quit();
}

int tea_should_close() {
  return tea()->event.type == SDL_QUIT;
}

void tea_begin() {
  Tea *ctx = tea();
  // SDL_PollEvent(&ctx->event);
  tea_update_input();
  tea_clear_color(BLACK);
  tea_clear();
  //tea_render_clear((te_Color){0, 0, 0, 255});


  ctx->timer.current_time = SDL_GetTicks();
  ctx->timer.delta = ctx->timer.current_time - ctx->timer.prev_time;
  ctx->timer.prev_time = ctx->timer.current_time;
  
  // float delta = (ctx->timer.current_time - ctx->timer.prev_fps_time);
  ctx->timer.frame++;
  
  SDL_Delay(TEA_FPS);
}

void tea_end() {
  tea_render_swap(tea());
}

float tea_get_delta() {
  return tea()->timer.delta / 1000.f;
}

int tea_get_framerate() {
  return tea()->timer.fps;
}

// void tea_attach_canvas(te_Canvas canvas) {
//   if (canvas <= 0) return;
//   _push_canvas_stack(tea()->textures[tea()->targets[canvas]]);
//   tea_set_canvas(canvas);
// }

// void tea_detach_canvas(void) {

// }

void tea_set_canvas(te_Canvas canvas) {
  te_RenderTarget *target = tea_canvas_render_target(canvas);
  tea_set_render_target(target);
  
}

te_Canvas tea_pop_canvas() {
  return 0;
}


void tea_clear() {
  SDL_RenderClear(tea()->render);
}

void tea_clear_color(te_Color color) {
  tea()->_clear_color = color;
  SDL_SetRenderDrawColor(tea()->render, color.r, color.g, color.b, color.a);
}
void tea_draw_color(te_Color color) {
  tea()->_color = color;
  // tea_render_draw_color(color);
  SDL_SetRenderDrawColor(tea()->render, color.r, color.g, color.b, color.a);
}
void tea_draw_mode(TEA_DRAW_MODE mode) {
  tea()->_mode = mode;
}

te_Transform tea_get_transform() {
  return tea()->current_transform;
}

void tea_set_transform(te_Transform *t) {
  if (!t) tea()->current_transform = (te_Transform){{0, 0}, 0, {1, 1}};
  else tea()->current_transform = *t;
}

void tea_set_scale(te_Point scale) {
  tea()->current_transform.scale = scale;
}

void tea_set_translate(te_Point translate) {
  tea()->current_transform.translate = translate;
}

void tea_set_angle(TEA_VALUE angle) {
  tea()->current_transform.angle = angle;
}

void tea_set_origin(te_Point origin) {
  tea()->current_transform.origin = origin;
}

void tea_draw_point(te_Point p) {
  SDL_RenderDrawPoint(tea()->render, p.x, p.y);
}

void tea_draw_line(te_Point p0, te_Point p1) {
  SDL_RenderDrawLine(tea()->render, p0.x, p0.y, p1.x, p1.y);
}

// Draw Rects

void _draw_fill_rect(te_Rect rect) {
  SDL_Rect r;
  r.x = rect.x;
  r.y = rect.y;
  r.w = rect.w;
  r.h = rect.h;
  SDL_RenderFillRect(tea()->render, &r);
}
void _draw_line_rect(te_Rect rect) {
  SDL_Rect r;
  r.x = rect.x;
  r.y = rect.y;
  r.w = rect.w;
  r.h = rect.h;
  SDL_RenderDrawRect(tea()->render, &r);
}

static RenderRectFn rect_fn[2] = {_draw_line_rect, _draw_fill_rect};

void tea_draw_rect(te_Rect r) {
  te_Transform *t = &tea()->current_transform;
  // tea()->draw.rect[tea()->_mode](tea_rect(r.x+t->translate.x, r.y+t->translate.y, r.w*t->scale.x, r.h*t->scale.y));
  rect_fn[tea()->_mode](tea_rect(r.x+t->translate.x, r.y+t->translate.y, r.w*t->scale.x, r.h*t->scale.y));
}

void tea_draw_rect_(TEA_VALUE x, TEA_VALUE y, TEA_VALUE w, TEA_VALUE h) {
  tea_draw_rect(tea_rect(x, y, w, h));
}

void tea_draw_rect_point(te_Point p, te_Point s) {
  tea_draw_rect(tea_rect(p.x, p.y, s.x, s.y));
}

// Draw Circles

void _draw_fill_circle(te_Point p, TEA_VALUE radius) {
  int x = 0;
  int y = radius;

  int P = 1 - radius;

  if (radius > 0) SDL_RenderDrawLine(tea()->render, p.x + radius, p.y, p.x - radius, p.y);

  while (x <= y) {
    if (P < 0) P += 2*x + 3;
    else {
      P += (2*(x-y))+5;
      y--;
    }
    x++;

    if (x > y) break;

    SDL_RenderDrawLine(tea()->render, p.x - x, p.y + y, p.x + x, p.y + y);
    SDL_RenderDrawLine(tea()->render, p.x + x, p.y - y, p.x - x, p.y - y);

    if (x != y) {
      SDL_RenderDrawLine(tea()->render, p.x - y, p.y + x, p.x + y, p.y + x);
      SDL_RenderDrawLine(tea()->render, p.x + y, p.y - x, p.x - y, p.y - x);
    }
  }
}

void _draw_line_circle(te_Point p, TEA_VALUE radius) {
  int x = -radius;
  int y = 0;
  int r = radius;
  int err = 2 - 2*r;

  do {
    SDL_RenderDrawPoint(tea()->render, p.x - x, p.y + y);
    SDL_RenderDrawPoint(tea()->render, p.x - y, p.y - x);
    SDL_RenderDrawPoint(tea()->render, p.x + x, p.y - y);
    SDL_RenderDrawPoint(tea()->render, p.x + y, p.y + x);
    r = err;
    if (r <= y) err += ++y*2+1;
    if (r > x || err > y) err += ++x*2+1;
  } while (x < 0);
}

static RenderCircleFn circle_fn[2] = {_draw_line_circle, _draw_fill_circle};

void tea_draw_circle(te_Point p, TEA_VALUE radius) {
  te_Transform *t = &tea()->current_transform;  
  // tea()->draw.circle[tea()->_mode](tea_point(p.x+t->translate.x, p.y+t->translate.y), radius*t->scale.x);
  circle_fn[tea()->_mode](tea_point(p.x+t->translate.x, p.y+t->translate.y), radius*t->scale.x);
}

// Draw Triangles

static void fill_bottom_flat_triangle(te_Point p0, te_Point p1, te_Point p2) {
  // float invslope0 = (p1.x - p0.x) / (p1.y - p0.y);
  // float invslope1 = (p2.x - p0.x) / (p2.y - p0.y);
  int dy = (p2.y - p0.y);
  float invslope0 = (p1.x - p0.x) / dy;
  float invslope1 = (p2.x - p0.x) / dy;

  float curx1 = p0.x;
  float curx2 = p0.x;

  int scanline_y;
  for (scanline_y = p0.y; scanline_y <= p1.y; scanline_y++) {
    tea_draw_line(tea_point(curx1, scanline_y), tea_point(curx2, scanline_y));
    curx1 += invslope0;
    curx2 += invslope1;
  }
}

static void fill_top_flat_triangle(te_Point p0, te_Point p1, te_Point p2) {
  int dy = (p2.y - p0.y);
  float invslope0 = (p2.x - p0.x) / dy;
  float invslope1 = (p2.x - p1.x) / dy;

  float curx1 = p2.x;
  float curx2 = p2.x;

  int scanline_y;
  for (scanline_y = p2.y; scanline_y > p1.y; scanline_y--) {
    tea_draw_line(tea_point(curx1, scanline_y), tea_point(curx2, scanline_y));
    curx1 -= invslope0;
    curx2 -= invslope1;
  }
}

static void points_ord_y(te_Point *points, int len) {
  for (int i = 0; i < len; i++) {
    for (int j = 0; j < len-1; j++) {
      if (points[j].y < points[j+1].y) continue;
      te_Point aux = points[j];
      points[j] = points[j+1];
      points[j+1] = aux;
    }
  }
}

void _draw_fill_triangle(te_Point p0, te_Point p1, te_Point p2) {
  te_Point points[3];
  points[0] = p0;
  points[1] = p1;
  points[2] = p2;

  points_ord_y(points, 3);

  if (points[1].y == points[2].y) fill_bottom_flat_triangle(points[0], points[1], points[2]);
  else if (points[0].y == points[1].y) fill_bottom_flat_triangle(points[0], points[1], points[2]);
  else {
    te_Point p = tea_point(
      (points[0].x + ((points[1].y - points[0].y) / (points[2].y - points[0].y)) * (points[2].x - points[0].x)),
      points[1].y
      );

    fill_bottom_flat_triangle(points[0], points[1], p);
    fill_top_flat_triangle(points[1], p, points[2]);
  }
}

void _draw_line_triangle(te_Point p0, te_Point p1, te_Point p2) {
  // printf("qqqq\n");
  tea_draw_line(p0, p1);
  tea_draw_line(p1, p2);
  tea_draw_line(p2, p0);
}

static RenderTriangleFn triangle_fn[2] = {_draw_line_triangle, _draw_fill_triangle};

static te_Point calc_vec_scale(te_Point p0, te_Point p1, te_Point scale) {
  te_Point vec = tea_point(p1.x-p0.x, p1.y-p0.y);
  vec.x *= scale.x;
  vec.y *= scale.y;
  
  return tea_point(p0.x + vec.x, p0.y + vec.y);
}

void tea_draw_triangle(te_Point p0, te_Point p1, te_Point p2) {
  te_Transform *t = &tea()->current_transform;
  te_Point tpoints[3];
  
  tpoints[0] = calc_vec_scale(p0, p1, t->scale);
  tpoints[1] = calc_vec_scale(p1, p2, t->scale);
  tpoints[2] = calc_vec_scale(p2, p0, t->scale);
  
  for (int i = 0; i < 3; i++) tpoints[i] = tea_point(tpoints[i].x+t->translate.x, tpoints[i].y+t->translate.y);
  
  // tea()->draw.triangle[tea()->_mode](tpoints[0], tpoints[1], tpoints[2]);
  triangle_fn[tea()->_mode](tpoints[0], tpoints[1], tpoints[2]);
}

// Draw textures

void tea_draw_texture(te_Texture *tex, te_Rect *dest, te_Rect *src) {
  te_Point sz;
  tea_texture_size(tex, &sz);
  SDL_Rect d;
  te_Transform *t = &tea()->current_transform;
  d.x = dest ? (dest->x - t->origin.x + t->translate.x) : 0;
  d.y = dest ? (dest->y - t->origin.y + t->translate.y) : 0;
  d.w = dest ? dest->w*t->scale.x : sz.x;
  d.h = dest ? dest->h*t->scale.y : sz.y;

  SDL_Rect s;
  s.x = src ? src->x : 0;
  s.y = src ? src->y : 0;
  s.w = src ? src->w : sz.x;
  s.h = src ? src->h : sz.y;

  SDL_RenderCopy(tea()->render, tex->tex, &s, &d);
}

void tea_draw_texture_ex(te_Texture *tex, te_Rect *dest, te_Rect *src, TEA_VALUE angle, te_Point origin, te_RenderFlip flip) {
  te_Point size;
  tea_texture_size(tex, &size);
  SDL_Rect d;
  te_Transform *t = &tea()->current_transform;
  d.x = (dest ? (dest->x - t->origin.x + t->translate.x) : 0) - origin.x;
  d.y = (dest ? (dest->y - t->origin.x + t->translate.y) : 0) - origin.y;
  d.w = dest ? (dest->w*t->scale.x) : size.x - origin.x;
  d.h = dest ? (dest->h*t->scale.y) : size.y - origin.y;

  SDL_Rect s;
  s.x = src ? src->x : 0;
  s.y = src ? src->y : 0;
  s.w = src ? src->w : size.x;
  s.h = src ? src->h : size.y;

  SDL_Point sdl_origin = {origin.x, origin.y};

  SDL_RenderCopyEx(tea()->render, tex->tex, &s, &d, angle, &sdl_origin, (SDL_RendererFlip)flip);
}

// Draw images

void tea_draw_image(te_Image img, te_Rect *r, te_Point p) {
  int x, y;
  x = y = 0;
  int w, h;
  te_Texture *tex = &tea()->textures[img];
  if (r) {
    x = r->x;
    y = r->y;
    w = r->w;
    h = r->h;
  } else SDL_QueryTexture(tex->tex, NULL, NULL, &w, &h);
  te_Transform *t = &tea()->current_transform;

  TEA_VALUE px, py;
  px = p.x - t->origin.x + t->translate.x;
  py = p.y - t->origin.y + t->translate.y;

  tea_draw_texture(tex, &tea_rect(px, py, w*t->scale.x, h*t->scale.y), &tea_rect(x, y, w, h));
}

void tea_draw_image_ex(te_Image img, te_Rect *r, te_Point p, TEA_VALUE angle, te_Point scale, te_Point origin) {
  te_Texture *tex = &tea()->textures[img];
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

  te_Rect dest = tea_rect(p.x, p.y, size.x*scale.x, size.y*scale.y);
  te_Rect src = tea_rect(x, y, size.x, size.y);
  tea_draw_texture_ex(tex, &dest, &src, angle, tea_point(origin.x*scale.x, origin.y*scale.y), flip);
}

void tea_draw_canvas(te_Canvas canvas, te_Rect *r, te_Point p) {
  te_Texture *tex = tea_canvas_texture(canvas);
  
  int x, y, w, h;
  x = y = w = h = 0;

  if (r) {
    x = r->x;
    y = r->y;
    w = r->w;
    h = r->h;
  } else {
    w = tex->width;
    h = tex->height;
  }
  te_Transform *t = &tea()->current_transform;

  TEA_VALUE px, py;
  px = p.x - t->origin.x + t->translate.x;
  py = p.y - t->origin.y + t->translate.y;

  tea_draw_texture(tex, &tea_rect(px, py, w*t->scale.x, h*t->scale.y), &tea_rect(x, y, w, h));
}

void tea_draw_text(te_Font *font, const char *text, te_Point pos) {
  if (!text) return;
  Tea *tea = tea();

  char *p = (char*)text;

  SDL_SetTextureColorMod(font->tex->tex, tea->_color.r, tea->_color.g, tea->_color.b);
  SDL_SetTextureAlphaMod(font->tex->tex, tea->_color.a);

  while(*p) {
    te_Rect r;
    tea_font_char_rect(font, *p, &r);

    int index = (int)*p;
    tea_draw_texture(font->tex, &tea_rect(pos.x, pos.y, r.w, r.h), &r);
    // tea_draw_texture(font->tex, &r, pos);
    pos.x += font->c[index].ax;

    p++;
  }
}

te_Window* tea_window_create(const char *title, int width, int height, int flags) {  
  te_Window *w = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);

  return w;
}

void tea_window_destroy(te_Window *window) {
  if (!window) return;

  SDL_DestroyWindow(window);
}

te_Render* tea_render_create(te_Window *window, TEA_RENDER_FLAGS flags) {
  TE_ASSERT(window != NULL, "te_Window cannot be NULL");
  te_Render *r = SDL_CreateRenderer(window, -1, flags);

  // te_Render *r = (te_Render*)malloc(sizeof(*r));
  // TE_ASSERT(r != NULL, "failed to create render");

  // r->handle = SDL_CreateRenderer(window->handle, -1, flags);

  // r->draw.point = tea_render_point;
  // r->draw.line = tea_render_line;

  // r->draw.rect[TEA_LINE] = tea_render_rect_line;
  // r->draw.rect[TEA_FILL] = tea_render_rect_fill;

  // r->draw.circle[TEA_LINE] = tea_render_circle_line;
  // r->draw.circle[TEA_FILL] = tea_render_circle_fill;

  // r->draw.triangle[TEA_LINE] = tea_render_triangle_line;
  // r->draw.triangle[TEA_FILL] = tea_render_triangle_line;

  return r;
}

void tea_render_destroy(te_Render *render) {
  if (!render) return;

  SDL_DestroyRenderer(render);
}

void tea_render_swap(Tea *t) {
  TE_ASSERT(t != NULL, "Tea cannot be NULL");
  SDL_RenderPresent(t->render);
}

static int cross_prod(te_Point v0, te_Point v1) {
  return v0.x*v1.y - v0.y*v1.x;
}

static int get_intersection(te_Point p0, te_Point p1, te_Point p2, te_Point p3, te_Point *out) {
  if (!out) return -1;
  te_Point d0 = tea_point(p0.x-p1.x, p0.y-p1.y);
  te_Point d1 = tea_point(p2.x-p3.x, p2.y-p3.y);
  int cross = cross_prod(d1, d0);

  if (cross == 0) return 0;

  float t = (float)cross_prod(tea_point(p0.x-p2.x, p0.y-p2.y), tea_point(p2.x-p3.x, p2.y-p3.y)) / (float)cross;
  float u = (float)cross_prod(tea_point(p0.x-p1.x, p0.y-p1.y), tea_point(p0.x-p2.x, p0.y-p2.y)) / (float)cross;
  te_Point o;

  if (t <= 0 && t <= 1) o = tea_point(p0.x+t*(p1.x-p0.x), p0.y+t*(p1.y-p0.y));
  else if (u <= 0 && u <= 1) o = tea_point(p2.x+u*(p3.x-p2.x), p2.y+u*(p3.y-p2.y));
  else return 0;

  // printf("%f %f\n", out->x, out->y);
  if (out) *out = o;

  return 1;
}

static int _set_texture(te_Texture tex) {
  int i = 1;
  while (i < MAX_TEXTURES) {
    if (tea()->textures[i].tex == NULL) {
      tea()->textures[i] = tex;
      // return i;
      return i;
    }
    i++;
  }

  tea_error("max textures reached");

  return -1;
}

static int _create_texture(int w, int h, unsigned int format, int access) {
  te_Texture tex = {0};
  /*tex.tex = SDL_CreateTexture(tea->render, format, access, w, h);
  tex.width = w;
  tex.height = h;*/
  tea_texture_init(&tex, w, h, format, access);
  return _set_texture(tex);
}

void tea_texture_init(te_Texture *tex, int w, int h, unsigned int format, int access) {
  TE_ASSERT(tex != NULL, "te_Texture cannot be null");
  
  tex->tex = SDL_CreateTexture(tea()->render, format, access, w, h);
  tex->width = w;
  tex->height = h;
}

te_Texture* tea_texture(int w, int h, unsigned int format, int access) {
  int i = _create_texture(w, h, format, access);
  return &tea()->textures[i];
}

te_Texture* tea_texture_load(const char *str) {
  Tea *t = tea();
  int req_format = STBI_rgb_alpha;
  int w, h, format;
  unsigned char *data = stbi_load(str, &w, &h, &format, req_format);
  TE_ASSERT(data != NULL, "failed to load image");

  int depth, pitch;
  Uint32 pixel_format;


  if (req_format == STBI_rgb) {
    depth = 24;
    pitch = 3*w;
    pixel_format = SDL_PIXELFORMAT_RGB24;
  } else {
    depth = 32;
    pitch = 4*w;
    // printf("teste\n");
    pixel_format = SDL_PIXELFORMAT_RGBA32;
  }

  SDL_Surface *surf = SDL_CreateRGBSurfaceWithFormatFrom(data, w, h, depth, pitch, pixel_format);

  TE_ASSERT(surf != NULL, "Failed to create surface");

  Texture *stex = SDL_CreateTextureFromSurface(t->render, surf);

  TE_ASSERT(stex != NULL, "Failed to create texture");
  SDL_FreeSurface(surf);

  te_Texture tex = {0};
  tex.tex = stex;
  tex.width = w;
  tex.height = h;

  int id = _set_texture(tex);

  return &tea()->textures[id];
}

te_Texture* tea_texture_memory(unsigned char *pixels, int w, int h, unsigned int format) {
  return NULL;
}


int tea_texture_width(te_Texture *tex) {
  // int w;
  // Texture *tex = tea()->textures[id];
  TE_ASSERT(tex != NULL, "invalid texture");

  // SDL_QueryTexture(tex, NULL, NULL, &w, NULL);
  return tex->width;
}
int tea_texture_height(te_Texture *tex) {
  // int h;
  // Texture *tex = tea()->textures[id];
  TE_ASSERT(tex != NULL, "invalid texture");

  // SDL_QueryTexture(tex, NULL, NULL, NULL, &h);
  return tex->height;
}
void tea_texture_size(te_Texture *tex, te_Point *size) {
  TE_ASSERT(tex != NULL, "invalid texture");

  // SDL_QueryTexture(tex, NULL, NULL, &w, &h);
  if (size) *size = tea_point(tex->width, tex->height);
}

// Image

te_Image tea_image(int w, int h, int format) {
  return _create_texture(w, h, format, SDL_TEXTUREACCESS_STATIC);
}

te_Image tea_image_load(const char *filename) {
  te_Texture *t = tea_texture_load(filename);
  return (int)(t - tea()->textures);
}

te_Texture* tea_image_texture(te_Image img) {
  return &tea()->textures[img];
}

void tea_image_destroy(te_Image *img) {
  TE_ASSERT(img != NULL, "te_Image must not be NULL");
  // tea_texture_destroy(tea_image_texture(*img));
}


// Render Target

static int _push_rtarget(te_RenderTarget target) {
  int i = 0;
  while (i < MAX_RTARGETS) {
    if (tea()->targets[i]._tex == NULL) {
      tea()->targets[i] = target;
      return i;
    }
    i++;
  }

  tea_error("max render target reached");

  return -1;
}

static int _create_rtarget(int w, int h, unsigned int format) {
  // te_Canvas c = _create_texture(w, h, format, SDL_TEXTUREACCESS_TARGET);
  te_RenderTarget target = {0};
  target._tex = SDL_CreateTexture(tea()->render, format, SDL_TEXTUREACCESS_TARGET, w, h);
  target.width = w;
  target.height = h;
  // _push_canvas(c);
  return _push_rtarget(target);
}

te_RenderTarget* tea_render_target(int w, int h, int format) {
  int id = _create_rtarget(w, h, format);
  return &tea()->targets[id];
}

void tea_set_render_target(te_RenderTarget *target) {
  //te_Texture *tex = target ? &target->tex : NULL;
  SDL_Texture *tex = target ? target->tex.tex : NULL;
  SDL_SetRenderTarget(tea()->render, tex);
}

// Canvas

te_Canvas tea_canvas(int width, int height) {
  TE_ASSERT(width > 0, "Canvas width must be greater than zero");
  TE_ASSERT(height > 0, "Canvas height must be greater than zero");
  te_Canvas canvas = _create_rtarget(width, height, SDL_PIXELFORMAT_RGBA32)+1;
  // printf("%d\n", canvas);

  return canvas;
}

te_RenderTarget* tea_canvas_render_target(te_Canvas canvas) {
  return canvas <= 0 ? NULL : &tea()->targets[canvas-1];
}

te_Texture* tea_canvas_texture(te_Canvas canvas) {
  return canvas <= 0 ? NULL : &tea()->targets[canvas-1].tex;
}

void tea_canvas_set(te_Canvas canvas) {
  // canvas -= 1;
 /*if (canvas > 0) {
    SDL_SetRenderTarget(tea()->render, tea()->textures[canvas]);
    tea_render_clear(BLACK);
  } else SDL_SetRenderTarget(tea()->render, NULL);*/
  // te_Texture *target = canvas <= 0 ? NULL : tea()->targets[canvas-1].stex;
  
}

/* Font */

te_Font* tea_font(const void *data, size_t buf_size, int font_size) {
  te_Font *f = malloc(sizeof(*f));
  TE_ASSERT(f != NULL, "Failed to alloc a block for font");

  tea_font_init(f, data, buf_size, font_size);

  return f;
}

te_Font* tea_font_load(const char *filename, int font_size) {
  size_t sz;
  FILE *fp;
  fp = fopen(filename, "rb");
  TE_ASSERT(fp != NULL, "Failed to load font");

  fseek(fp, 0, SEEK_END);
  sz = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  unsigned char buffer[sz];

  (void)fread(buffer, 1, sz, fp);


  te_Font *font = tea_font(buffer, sz, font_size);
  fclose(fp);

  return font;
}

int tea_font_init(te_Font *font, const void *data, size_t buf_size, int font_size) {
  font->data = (void*)data;
  if (!stbtt_InitFont(&font->info, data, 0)) tea_error("Failed to init font");

  int ascent, descent, line_gap;
  font->size = font_size;
  float fsize = font_size;

  font->scale = stbtt_ScaleForMappingEmToPixels(&font->info, fsize);
  stbtt_GetFontVMetrics(&font->info, &ascent, &descent, &line_gap);
  font->baseline = ascent * font->scale;

  int tw, th;
  tw = th = 0;

  int i;
  for (i = 0; i < MAX_FONT_CHAR; i++) {
    int ax, bl;
    int x0, y0, x1, y1;
    int w, h;

    stbtt_GetCodepointHMetrics(&font->info, i, &ax, &bl);
    stbtt_GetCodepointBitmapBox(&font->info, i, font->scale, font->scale, &x0, &y0, &x1, &y1);
    w = x1 - x0;
    h = y1 - y0;

    // printf("%c: %d %d %d %d\n", i, x0, y0, x1, y1);
    // printf("%d %d\n", w, h);

    font->c[i].ax = ax * font->scale;
    font->c[i].ay = 0;
    font->c[i].bl = bl * font->scale;
    font->c[i].bw = w;
    font->c[i].bh = h;
    font->c[i].bt = font->baseline + y0;

    tw += w;
    th = tea_max(th, h);
  }

  // printf("%d %d\n", tw, th);

  SDL_PixelFormat pixel_format;
  pixel_format.format = SDL_PIXELFORMAT_RGBA32;
  // font->tex = tea_texture(tea, tw, th, pixel_format.format);
  int id = _create_texture(tw, th, pixel_format.format, SDL_TEXTUREACCESS_STREAMING);
  font->tex = &tea()->textures[id];

  SDL_SetTextureBlendMode(font->tex->tex, SDL_BLENDMODE_BLEND);

  int x = 0;
  for (i = 0; i < MAX_FONT_CHAR; i++) {
    int ww = font->c[i].bw;
    int hh = font->c[i].bh;
    int ssize = ww * hh;
    int ox, oy;

    unsigned char *bitmap = stbtt_GetCodepointBitmap(&font->info, 0, font->scale, i, NULL, NULL, &ox, &oy);
    // printf("%p\n", bitmap);
    // Uint32 pixels[ssize];
    // unsigned char pixels[ssize];
    // for (int j = 0; j < ssize; j += 4) {
    //   int ii = j / 4;

    //   pixels[j] = 255;
    //   pixels[j+1] = 255;
    //   pixels[j+2] = 255;
    //   pixels[j+3] = bitmap[ii];
    //   // printf("%d\n", bitmap[ii]);
    // }
    Uint32 *pixels = NULL;
    int pitch;

    SDL_Rect r;
    r.x = x;
    r.y = 0;
    r.w = ww;
    r.h = hh;

    if (SDL_LockTexture(font->tex->tex, &r, (void**)&pixels, &pitch) != 0) {
      fprintf(stderr, "[tea] error: %s\n", SDL_GetError());
      exit(1);
    }
    // printf("%d\n", pitch);
    int yy = 0;
    for (int j = 0; j < ssize; j++) {
      int xx = j % ww;
      if (j != 0 && xx == 0) yy++;
      int index = xx + (yy * (pitch / 4));
      // printf("%d\n", index);
      // printf("teste %p, %d %d\n", pixels, pixels[index], bitmap[j]);
      // Uint32 pp = SDL_MapRGBA(&pixel_format, 255, 255, 255, bitmap[j]);
      Uint32 pp = bitmap[j];
      pp <<= 24;
      pp |= 0xffffff;
      pixels[index] = pp;
      // pixels[index] = SDL_MapRGBA(&format, 255, 255, 255, bitmap[j]);
    }
    SDL_UnlockTexture(font->tex->tex);

    // printf("%c: %d\n", i, ww);
    // if (SDL_UpdateTexture(font->tex, &r, pixels, tw*sizeof(*pixels)) != 0) {
    //   fprintf(stderr, "[tea] error: %s\n", SDL_GetError());
    //   exit(1);
    // }
    font->c[i].tx = x;

    x += font->c[i].bw;
  }

  return 1;
}

void tea_font_destroy(te_Font *font) {
  free(font);
}

void tea_font_get_rect(te_Font* font, const int c, TEA_VALUE *x, TEA_VALUE *y, te_Point *out_pos, te_Rect *r, TEA_VALUE width) {
  if (c == '\n') {
    *x = 0;
    int h;
    // SDL_QueryTexture(font->tex, NULL, NULL, NULL, &h);
    h = tea_texture_height(font->tex);
    *y += h;
    return;
  }


}

void tea_font_char_rect(te_Font *font, const unsigned int c, te_Rect *r) {
  if (c == '\n' || c == '\t') return;
  if (c >= MAX_FONT_CHAR) return;

  if (r) *r = tea_rect(font->c[c].tx, 0, font->c[c].bw, font->c[c].bh);
}

int tea_font_get_text_width(te_Font *font, const char *text, int len);
int tea_font_get_text_height(te_Font *font, const char *text, int len);


/* Debug */

void tea_error(const char *msg) {
  fprintf(stderr, "[tea] error: %s\n", msg);
  exit(1);
}

/*******************
 *      Input      *
 *******************/


static void _key_callback(te_Window* window, int key, int scancode, int action, int mods) {
  Uint8 *state = &tea()->input.keyboard.state[key];
  Uint8 *old = &tea()->input.keyboard.old_state[key];

  *old = *state;
  printf("%d %d %d\n", key, action, *old);
  *state = action;
}


int tea_update_input() {
  // memcpy(tea()->input.old_key, tea()->input.key_array, TEA_KEY_COUNT);
  memcpy(tea()->input.keyboard.old_state, tea()->input.keyboard.state, TEA_KEY_COUNT);
  for (int i = 0; i < TEA_BUTTON_COUNT; i++) {
    tea()->input.mouse.old_state[i] = tea()->input.mouse.state[i];
    tea()->input.mouse.state[i] = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(i+1);
  }
  SDL_PollEvent(&tea()->event);
  // SDL_Event *event = &tea()->event;
  // if (SDL_PollEvent(event)) {
  //   switch(event->type) {
  //     case SDL_KEYDOWN:
  //       _key_callback(tea()->window, event->key.keysym.scancode, event->key.keysym.scancode, 1, event->key.keysym.mod);
  //       break;
  //     case SDL_KEYUP:
  //       _key_callback(tea()->window, event->key.keysym.scancode, event->key.keysym.scancode, 0, event->key.keysym.mod);
  //       break;
  //   }
  // }
  

  return 1;
}

int tea_keyboard_is_down(int key) {
  return tea()->input.keyboard.state[key];
}

int tea_keyboard_is_up(int key) {
  return !tea_keyboard_is_down(key);
}

int tea_keyboard_was_pressed(int key) {
  return !tea()->input.keyboard.old_state[key] && tea_keyboard_is_down(key);
}
int tea_keyboard_was_released(int key) {
  return tea()->input.keyboard.old_state[key] && tea_keyboard_is_up(key);
}

int tea_mouse_is_down(int button) {
  return tea()->input.mouse.state[button];
}
int tea_mouse_is_up(int button) {
  return !tea_mouse_is_down(button);
}

int tea_mouse_was_pressed(int button) {
  return !tea()->input.mouse.old_state[button] && tea_mouse_is_down(button);
}
int tea_mouse_was_released(int button) {
  return tea()->input.mouse.old_state[button] && tea_mouse_is_up(button);
}

int tea_joystick_axis(int jid, int axis);
int tea_joystick_is_down(int jid, int button);
int tea_joystick_is_up(int jid, int button);
int tea_joystick_was_pressed(int jid, int button);
int tea_joystick_was_released(int jid, int button);

/***************** Garbage ******************/

void tea_render_triangle_fill0(te_Point p0, te_Point p1, te_Point p2) {
  Tea *tea = tea();
  // tea_render_triangle_line(t, p0, p1, p2);
  int minX = tea_min(p0.x, tea_min(p1.x, p2.x));
  int maxX = tea_max(p0.x, tea_max(p1.x, p2.x));

  int minY = tea_min(p0.y, tea_min(p1.y, p2.y));
  int maxY = tea_max(p0.y, tea_max(p1.y, p2.y));

  int y;
  for (y = minY; y <= maxY; y++) {
    // for (x = minX; x <= maxX; x++) {

      // te_Point q = tea_point(p0.x, y - p0.y);

      te_Point sp0 = tea_point(minX, y);
      te_Point sp1 = tea_point(maxX, y);
      int intersect = 0;
      te_Point inter_points[2];

      // SDL_RenderDrawLine(tea->render, sp0.x, sp0.y, pp1.x, pp1.y);
      // te_Color c = tea->_color;
      // tea_render_draw_color(tea, tea_color(255, 0, 0));
      // tea_render_line(tea, sp1, sp0);
      // tea_render_draw_color(tea, c);


      // int cross = cross_prod(tea_point(p0.x-p1.x, pp0.y-pp1.y), tea_point(p0.y-p1.y, pp0.x-pp1.x));
      // int cross = get_intersection(pp0, pp1, p0, p1, &inter_points[intersect]);
      if (get_intersection(p0, p2, sp0, sp1, &inter_points[intersect])) intersect++;
      // printf("%d\n", intersect);
      if (get_intersection(p0, p1, sp0, sp1, &inter_points[intersect])) intersect++;
      // printf("%d\n", intersect);
      if (get_intersection(p2, p1, sp0, sp1, &inter_points[intersect])) intersect++;
      printf("%d\n", intersect);

      // printf("%d\n", intersect);


      for (int i = 0; i < intersect; i++) {
        // printf("p%d: %f %f\n", i, inter_points[i].x, inter_points[i].y);
      }
      // if (intersect > 0) 
        // SDL_RenderDrawLine(tea->render, inter_points[0].x, inter_points[0].y, inter_points[1].x, inter_points[1].y);
      switch (intersect) {
        case 0: break;
        case 1: SDL_RenderDrawPoint(tea->render, inter_points[0].x, inter_points[0].y); break;
        case 2: SDL_RenderDrawLine(tea->render, inter_points[0].x, inter_points[0].y, inter_points[1].x, inter_points[1].y); break;
      }

      // float s = (float)cross_prod(q, vs1) / c;
      // float tt = (float)cross_prod(vs0, q) / c;

      // if (s >= 0 && tt >= 0 && (s + tt <= 1)) SDL_RenderDrawPoint(t->render, x, y);

      // if ((p0.x - p1.x) * (y - p0.y) - (p0.y - p1.y) * (x - p0.x) > 0 &&
      //     (p1.x - p2.x) * (y - p1.y) - (p1.y - p2.y) * (x - p1.x) > 0 &&
      //     (p2.x - p0.x) * (y - p2.y) - dy * (x - p2.x) > 0){
      //       printf("porra eh, entra aqui n?\n");
      //       SDL_RenderDrawPoint(t->render, x, y);
      // }
    // }
  }
}
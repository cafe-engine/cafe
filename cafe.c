#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cafe.h"
#include "tea.h"
#include "mocha.h"
#include "latte.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

// #include "langs/lua_cafe.h"

#include "external/stb_image.h"

#define PROJECTION_UNIFORM_NAME "u_World"
#define MODELVIEW_UNIFORM_NAME "u_ModelView" 

#define POSITION_ATTRIBUTE_NAME "a_Position"
#define COLOR_ATTRIBUTE_NAME "a_Color"
#define TEXCOORD_ATTRIBUTE_NAME "a_TexCoord"

#define MAX_DRAW_CALLS 1024
#define cafe() (&_cafe_ctx)
#define render() (&cafe()->render)
#define input() (&cafe()->input)

typedef SDL_Window ca_Window;
typedef SDL_Event ca_Event;

static const char *_130_vert_header =
"#version 130\n"
"uniform mat4 u_World;\n"
"uniform mat4 u_ModelView;\n"
"in vec2 a_Position;\n"
"in vec4 a_Color;\n"
"in vec2 a_TexCoord;\n"
"out vec4 v_Color;\n"
"out vec2 v_TexCoord;\n";

static const char *_130_frag_header =
"#version 130\n"
"in vec4 v_Color;\n"
"in vec2 v_TexCoord;\n"
"uniform sampler2D u_Texture;\n"
"out vec4 o_FragColor;\n";

static const char *_120_vert_header =
"#version 120\n"
"uniform mat4 u_World;\n"
"uniform mat4 u_ModelView;\n"
"attribute vec2 a_Position;\n"
"attribute vec4 a_Color;\n"
"attribute vec2 a_TexCoord;\n"
"varying vec4 v_Color;\n"
"varying vec2 v_TexCoord;\n";

static const char *_120_frag_header =
"#version 120\n"
"varying vec4 v_Color;\n"
"varying vec2 v_TexCoord;\n"
"uniform sampler2D u_Texture;\n"
"#define o_FragColor gl_FragColor\n"
"#define texture texture2D\n";

static const char *_default_vert_function =
"vec4 position(mat4 model_view, mat4 world, vec2 pos) {\n"
"  return world * model_view * vec4(pos.x, pos.y, 0.0, 1.0);\n"
"}\n";

static const char *_default_frag_function =
"vec4 pixel(vec4 color, vec2 tex_coord, sampler2D tex) {\n"
"  return color * texture(tex, tex_coord);\n"
"}\n";

static const char *_vert_main =
"void main() {\n"
"  v_Color = a_Color;\n"
"  v_TexCoord = a_TexCoord;\n"
"  gl_Position = position(u_ModelView, u_World, a_Position);\n"
"}\n";

static const char *_frag_main =
"void main() {\n"
"  o_FragColor = pixel(v_Color, v_TexCoord, u_Texture);\n"
"}\n";

struct ca_Sound {
    mo_audio_t* audio;
};

struct ca_Music {
    mo_audio_t* audio;
};

struct Drawable {
    te_texture_t handle;
    ca_i32 width, height;
    ca_i32 filter[2];
    ca_i32 wrap[2];
};

struct ca_Image {
    struct Drawable drawable;
    void *data;
};

struct ca_Canvas {
    struct Drawable drawable;
    te_framebuffer_t fbo;
};

struct ca_Shader {
    te_program_t program;
    ca_i32 world, modelview;
};

struct ca_Font {
    struct Drawable drawable;
    struct {
        ca_i32 _;
    } c[256];
    void* data;
};

struct ca_Input {
    const ca_u8 *keys;
    struct {
        ca_u8 down[3];
        ca_u8 pressed[3];
    } mouse;
};

typedef struct Render Render;
struct Render {
    struct {
        ca_Image* white;
        ca_Shader* shader;
        ca_Font* font;
        ca_Canvas canvas;
        te_buffer_t vbo, ibo;
    } defaults;

    ca_Image* image;
    ca_Canvas* canvas;
    ca_Shader* shader;
    ca_Font* font;
    te_vertex_format_t* format;
    te_buffer_t vbo, ibo;
    ca_i32 draw_mode;

    void(*begin)(Render*);
    void(*end)(Render*);

    void(*clear)(Render*);
    void(*flush)(Render*);
    void(*draw)(Render*);
};

struct Cafe {
    struct {
        ca_Window *handle;
        ca_i32 width, height;
    } window;
    SDL_GLContext context;
    ca_Event event;
    // struct ca_Render render;
    Render render;
    struct ca_Input input;
};

static Cafe _cafe_ctx;

static ca_bool s_cafe_render_init(Render* render);
static void s_cafe_render_deinit(Render* render);
static void s_cafe_render_draw(void);

ca_Config cafe_config(const char* title, ca_i32 width, ca_i32 height)
{
    ca_Config config;
    memset(&config, 0, sizeof(ca_Config));
    const char *t = title ? title : "cafe";
    memcpy(config.window.title, t, strlen(t));
    config.window.width = width;
    config.window.height = height;
    config.window.flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;
    return config;
}

ca_bool cafe_open(ca_Config* config) {
    memset(cafe(), 0, sizeof(Cafe));
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return CA_FALSE;
    }

    cafe()->window.handle = SDL_CreateWindow(config->window.title,
                                      SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED,
                                      config->window.width,
                                      config->window.height,
                                      config->window.flags);

    ca_Window *window = cafe()->window.handle;
    cafe()->window.width = config->window.width;
    cafe()->window.height = config->window.height;
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return CA_FALSE;
    }

    cafe()->context = SDL_GL_CreateContext(window);
    if (cafe()->context == NULL) {
        printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
        return CA_FALSE;
    }
    SDL_GL_MakeCurrent(window, cafe()->context);
    // te_config_t tea_conf = tea_config();
    tea_init(NULL);
    la_init(".");

    cafe()->input.keys = SDL_GetKeyboardState(NULL);

    if (!s_cafe_render_init(render())) {
        return CA_FALSE;
    }

    // if (!cafe_lua_setup()) {
    //     return CA_FALSE;
    // }

    return CA_TRUE;
}

void cafe_close(void) {
    s_cafe_render_deinit(render());
    tea_quit();
}

ca_bool cafe_is_running(void) {
    return !SDL_QuitRequested();
}

static int counter;
void cafe_begin() {
    while (SDL_PollEvent(&cafe()->event)) {
        if (cafe()->event.type == SDL_QUIT)
            return;

        switch (cafe()->event.type) {
            case SDL_MOUSEBUTTONDOWN:
                cafe()->input.mouse.down[cafe()->event.button.button] = CA_TRUE;
                break;
            case SDL_MOUSEBUTTONUP:
                cafe()->input.mouse.down[cafe()->event.button.button] = CA_FALSE;
                break;
            case SDL_WINDOWEVENT: {
                switch (cafe()->event.window.event) {
                    case SDL_WINDOWEVENT_RESIZED: {
                        cafe()->window.width = cafe()->event.window.data1;
                        cafe()->window.height = cafe()->event.window.data2;
                    }
                    break;
                }
            }
            break;
        }
    }
    render()->begin(render());
	counter = 0;
}

void cafe_end() {
    render()->end(render());
    SDL_GL_SwapWindow(cafe()->window.handle);
}

static void s_step_function(float dt) {}

void cafe_run(ca_StepFunction step) {
    step = step ? step : s_step_function;
    while (cafe_is_running())  {
        cafe_begin();
        step(1.f);
        cafe_end();
    }
}

/*=====================*
 * AUDIO FUNCTIONS     *
 *=====================*/

int cafe_audio_play(ca_Audio* audio) {
    return mo_play_audio(audio);
}

void cafe_audio_stop_all(ca_Audio* audio) {
    mo_audio_stop_all(audio);
}

void cafe_audio_pause_all(ca_Audio* audio) {
    mo_audio_pause_all(audio);
}


int cafe_audio_is_any_playing(ca_Audio* audio) {
    return mo_audio_is_any_playing(audio);
}


/******************
 * Audio Instance *
 ******************/

void cafe_audio_play_instance(int id) {
    mo_audio_instance_play(id);
}

void cafe_audio_stop_instance(int id) {
    mo_audio_instance_stop(id);
}

void cafe_audio_pause_instance(int id) {
    mo_audio_instance_pause(id);
}

void cafe_audio_set_instance_volume(int id, float volume) {
    mo_audio_instance_set_volume(id, volume);
}

void cafe_audio_set_instance_loop(int id, int loop) {
}


/*****************
 * Sound         *
 *****************/
ca_Sound* cafe_audio_load_sound(const char* filepath) {
    if (!filepath) return NULL;
    la_file_t* fp = la_fopen(filepath, LA_READ_MODE);
    la_header_t h;
    la_fheader(fp, &h);
    void* data = malloc(h.size);
    la_fread(fp, data, h.size);
    ca_Sound* snd = mo_load_audio_from_memory(data, h.size, MO_AUDIO_STATIC);
    la_fclose(fp);
    return snd;
}

ca_Sound* cafe_audio_new_sound(void* data, int size) {
    return mo_load_audio_from_memory(data, size, MO_AUDIO_STATIC);;
}

void cafe_sound_destroy(ca_Sound* sound) {
    mo_destroy_audio(sound);
}


/*****************
 * Music         *
 *****************/
ca_Music* cafe_audio_load_music(const char* filepath) {
    if (!filepath) return NULL;
    la_file_t* fp = la_fopen(filepath, LA_READ_MODE);
    la_header_t h;
    la_fheader(fp, &h);
    void* data = malloc(h.size);
    la_fread(fp, data, h.size);
    ca_Music* msc = mo_load_audio_from_memory(data, h.size, MO_AUDIO_STREAM);
    la_fclose(fp);
    return msc;
}

ca_Music* cafe_audio_new_music(void* data, int size) {
    return mo_load_audio_from_memory(data, size, MO_AUDIO_STREAM);
}

void cafe_music_destroy(ca_Music* music) {
    mo_destroy_audio(music);
}


/*=====================*
 * INPUT FUNCTIONS      *
 *=====================*/

/*****************
 * Keyboard      *
 ****************/

ca_i32 cafe_keyboard_from_name(const ca_i8* name) {
    return SDL_GetScancodeFromName(name);
}

const ca_i8* cafe_keyboard_get_name(ca_i32 key) {
    return SDL_GetScancodeName(key);
}

ca_bool cafe_keyboard_is_down(ca_i32 key) {
    return cafe()->input.keys[key];
}

ca_bool cafe_keyboard_is_up(ca_i32 key) {
    return !cafe()->input.keys[key];
}

ca_bool cafe_keyboard_wasPressed(ca_i32 key);
ca_bool cafe_keyboard_wasReleased(ca_i32 key);

/*****************
 * Mouse         *
 ****************/

void cafe_mouse_get_pos(ca_f32* x, ca_f32* y) {
    int xx, yy;
    SDL_GetMouseState(&xx, &yy);
    if (x) *x = xx;
    if (y) *y = yy;
}

void cafe_mouse_set_pos(ca_f32 x, ca_f32 y) {
    SDL_WarpMouseInWindow(cafe()->window.handle, x, y);
}

void cafe_mouse_get_delta_pos(ca_f32* x, ca_f32* y) {
    int xx, yy;
    SDL_GetRelativeMouseState(&xx, &yy);
    if (x) *x = xx;
    if (y) *y = yy;
}

ca_bool cafe_mouse_is_down(ca_i32 button) {
    return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(button);
}

ca_bool cafe_mouse_is_up(ca_i32 button) {
    return !(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(button));
}

ca_bool cafe_mouse_wasPressed(ca_i32 button) {
    return CA_TRUE;
}

ca_bool cafe_mouse_wasReleased(ca_i32 button) {
    return CA_TRUE;
}

/*=============================*
 *                             *
 *          Graphics           *
 *                             *
 *=============================*/
#if 1 // graphics module
static void s_render_clear(Render* render) {
    tea_buffer_seek(render->vbo, 0);
}

static void s_render_flush(Render* render) {
    tea_buffer_flush(render->vbo);
}

static void s_render_draw(Render* render) {
    if (tea_buffer_tell(render->vbo) <= 0) return;
	counter++;
    s_render_flush(render);
    tea_draw(render->draw_mode);
}

static void s_render_begin(Render* render) {
    ca_Canvas *c = &render()->defaults.canvas;
    c->drawable.width = cafe()->window.width;
    c->drawable.height = cafe()->window.height;

    cafe_graphics_set_color(CAFE_RGBA(255, 255, 255, 255));

    tea_matrix_mode(TEA_PERSPECTIVE);
    tea_load_identity();
    tea_ortho(0, cafe()->window.width, cafe()->window.height, 0, 0, 1);
    tea_matrix_mode(TEA_MODELVIEW);
    tea_load_identity();

    cafe_graphics_set_shader(render()->defaults.shader);
    render()->image = render()->defaults.white;
    tea_bind_texture(render()->image->drawable.handle);
    cafe_graphics_set_canvas(c);

    render()->clear(render());
}

static void s_render_end(Render* render) {
    render()->draw(render());
}

ca_bool s_cafe_render_init(Render* render) {
    memset(render, 0, sizeof(Render));
    render->defaults.vbo = tea_buffer(TEA_VERTEX_BUFFER, 10000);
    render->vbo = render->defaults.vbo;
    ca_u8 pixels[] = {255, 255, 255, 255};
    render->defaults.white = cafe_graphics_new_image(1, 1, pixels);
    render->defaults.shader = cafe_graphics_new_shader(NULL, NULL);

    ca_Canvas* c = &render->defaults.canvas;
    c->drawable.width = cafe()->window.width;
    c->drawable.height = cafe()->window.height;

    render->format = tea_vertex_format();
    tea_vertex_format_add(render->format, TEA_ATTRIB_FLOAT2);
    tea_vertex_format_add(render->format, TEA_ATTRIB_FLOAT4);
    tea_vertex_format_add(render->format, TEA_ATTRIB_FLOAT2);
    tea_setup_buffer(render->format, render->vbo);

    render->begin = s_render_begin;
    render->end = s_render_end;
    render->flush = s_render_flush;
    render->draw = s_render_draw;
    render->clear = s_render_clear;

    return CA_TRUE;
}

void s_cafe_render_deinit(Render* render) {
    if (!render) return;
    tea_buffer_free(render->defaults.vbo);
    tea_free_vertex_format(render->format);
    cafe_image_destroy(render->defaults.white);
    cafe_shader_destroy(render->defaults.shader);
}

void cafe_graphics_clear(ca_Color color) {
    ca_f32 c[4];
    c[0] = color.r / 255.f;
    c[1] = color.g / 255.f;
    c[2] = color.b / 255.f;
    c[3] = color.a / 255.f;
    tea_clear(c[0], c[1], c[2], c[3]);
}

void cafe_graphics_draw_mode(ca_i32 mode) {
    if (mode < 0 || mode > CAFE_FILL) return;
    if (render()->draw_mode != mode) {
        render()->draw(render());
        render()->clear(render());
        render()->draw_mode = mode;
    }
}

void cafe_graphics_set_shader(ca_Shader* shader) {
    shader = shader ? shader : render()->defaults.shader;
    if (render()->shader != shader) {
        render()->draw(render());
        render()->clear(render());
        render()->shader = shader;
    }
    tea_use_program(shader->program);
    tea_program_set_uniform_matrix4fv(shader->world, 1, TEA_FALSE, tea_get_matrix(TEA_PERSPECTIVE));
    tea_program_set_uniform_matrix4fv(shader->modelview, 1, TEA_FALSE, tea_get_matrix(TEA_MODELVIEW));
}

void cafe_graphics_set_canvas(ca_Canvas* canvas) {
    canvas = canvas ? canvas : &render()->defaults.canvas;
    if (render()->canvas != canvas) {
        render()->draw(render());
        render()->clear(render());
        render()->canvas = canvas;
    }
    tea_bind_framebuffer(canvas->fbo);
    tea_viewport(0, 0, canvas->drawable.width, canvas->drawable.height);
    tea_matrix_mode(TEA_PERSPECTIVE);
    tea_load_identity();
    tea_ortho(0, canvas->drawable.width, canvas->drawable.height, 0, 0, 1);
    tea_matrix_mode(TEA_MODELVIEW);
    tea_program_set_uniform_matrix4fv(render()->shader->world, 1, CA_FALSE, tea_get_matrix(TEA_PERSPECTIVE));
}

void cafe_graphics_set_color(ca_Color color) {
    ca_f32 c[4];
    c[0] = color.r / 255.f;
    c[1] = color.g / 255.f;
    c[2] = color.b / 255.f;
    c[3] = color.a / 255.f;
    tea_buffer_color4f(render()->vbo, c[0], c[1], c[2], c[3]);
}

static void s_graphics_set_image(ca_Image *i) {
    i = i ? i : render()->defaults.white;
    if (i != render()->image) {
        render()->draw(render());
        render()->clear(render());
        render()->image = i;
    }
    tea_bind_texture(i->drawable.handle);
}

void cafe_graphics_draw_point(ca_f32 x, ca_f32 y) {

}

void cafe_graphics_draw_line(ca_f32 x0, ca_f32 y0, ca_f32 x1, ca_f32 y1) {
    cafe_graphics_draw_mode(CAFE_LINE);
    s_graphics_set_image(render()->defaults.white);
    tea_buffer_line(render()->vbo, (vec2){x0, y0}, (vec2){x1, y1});
}

typedef void(*DrawRect)(te_buffer_t, vec2, vec2);
static DrawRect rect_fn[] = {
    tea_buffer_line_rectangle,
    tea_buffer_fill_rectangle
};

void cafe_graphics_draw_rectangle(ca_f32 x, ca_f32 y, ca_f32 w, ca_f32 h) {
    ca_i32 mode = render()->draw_mode;
    s_graphics_set_image(render()->defaults.white);
    rect_fn[mode](render()->vbo, (vec2){x, y}, (vec2){w, h});
}

typedef void(*DrawTriang)(te_buffer_t, vec2, vec2, vec2);
static DrawTriang triang_fn[] = {
    tea_buffer_line_triangle,
    tea_buffer_fill_triangle
};

void cafe_graphics_draw_triangle(ca_f32 x0, ca_f32 y0, ca_f32 x1, ca_f32 y1, ca_f32 x2, ca_f32 y2) {
    ca_i32 mode = render()->draw_mode;
    s_graphics_set_image(render()->defaults.white);
    triang_fn[mode](render()->vbo, (vec2){x0, y0}, (vec2){x1, y1}, (vec2){x2, y2});
}

typedef void(*DrawCircle)(te_buffer_t, vec2, ca_f32, ca_u32);
static DrawCircle circ_fn[] = {
    tea_buffer_line_circle,
    tea_buffer_fill_circle
};

void cafe_graphics_draw_circle(ca_f32 x, ca_f32 y, ca_f32 radius) {
    ca_i32 mode = render()->draw_mode;
    s_graphics_set_image(render()->defaults.white);
    circ_fn[mode](render()->vbo, (vec2){x, y}, radius, 16);
}

void cafe_graphics_draw_circle_ex(ca_f32 x, ca_f32 y, ca_f32 radius, ca_u32 s) {
    ca_i32 mode = render()->draw_mode;
    s_graphics_set_image(render()->defaults.white);
    circ_fn[mode](render()->vbo, (vec2){x, y}, radius, s);
}

static void s_draw_drawable(struct Drawable *drawable, ca_Rect* d, ca_Rect* s, ca_bool flip_y) {
    ca_Rect dest, src;
    ca_f32 w, h;
    w = drawable->width;
    h = drawable->height;
    dest.x = 0;
    dest.y = 0;
    dest.w = w;
    dest.h = h;

    src.x = src.y = 0;
    src.w = dest.w;
    src.h = dest.h;

    d = d ? d : &dest;
    s = s ? s : &src;

    ca_f32 vertices[] = {
        d->x, d->y, 1.f, 1.f, 1.f, 1.f, s->x / w, s->y / h,
        d->x+d->w, d->y, 1.f, 1.f, 1.f, 1.f, (s->x+s->w) / w, s->y / h,
        d->x+d->w, d->y+d->h, 1.f, 1.f, 1.f, 1.f, (s->x+s->w) / w, (s->y+s->h) / h,

        d->x, d->y, 1.f, 1.f, 1.f, 1.f, s->x / w, s->y / h,
        d->x, d->y+d->h, 1.f, 1.f, 1.f, 1.f, s->x / w, (s->y+s->h) / h,
        d->x+d->w, d->y+d->h, 1.f, 1.f, 1.f, 1.f, (s->x+s->w) / w, (s->y+s->h) / h
    };

    tea_buffer_send_vertices(render()->vbo, 6, vertices);
}

void cafe_graphics_draw_image(ca_Image* image, ca_Rect *dest, ca_Rect *src) {
    s_graphics_set_image(image);
    s_draw_drawable((struct Drawable*)image, dest, src, 0);
}

/*****************
 * Image         *
 *****************/

ca_Image* cafe_graphics_new_image(ca_i32 width, ca_i32 height, ca_u8* data) {
    ca_Image* image = NULL;
    if (width <= 0 || height <= 0) return image;
    image = (ca_Image*)malloc(sizeof(*image));
    image->drawable.handle = tea_texture(TEA_RGBA, width, height, data, 0);
    image->drawable.width = width;
    image->drawable.height = height;
    image->drawable.filter[0] = CAFE_NEAREST;
    image->drawable.filter[1] = CAFE_NEAREST;
    image->drawable.wrap[0] = CAFE_CLAMP;
    image->drawable.wrap[1] = CAFE_CLAMP;

    image->data = NULL;

    return image;
}

ca_Image* cafe_graphics_load_image(const ca_i8* path) {
    ca_Image* image = NULL;
    la_file_t* file = la_fopen(path, LA_READ_MODE);
    la_header_t h;
    la_fheader(file, &h);

    ca_u8 data[h.size+1];
    la_fread(file, (ca_i8*)data, h.size);
    data[h.size] = '\0';

    ca_i32 width, height, channels;
    ca_u8* pixels = stbi_load_from_memory(data, h.size, &width, &height, &channels, STBI_rgb_alpha);
    image = cafe_graphics_new_image(width, height, pixels);
    stbi_image_free(pixels);
    return image;
}

void cafe_image_destroy(ca_Image* image) {
    if (!image) return;
    if (image->drawable.handle) tea_texture_free(image->drawable.handle);
    free(image);
}

/*****************
 * Canvas        *
 *****************/

ca_Canvas* cafe_graphics_new_canvas(ca_i32 width, ca_i32 height) {
    ca_Canvas* canvas = NULL;
    if (width <= 0 || height <= 0) return canvas;
    canvas = (ca_Canvas*)malloc(sizeof(*canvas));
    canvas->drawable.handle = tea_texture(TEA_RGBA, width, height, NULL, 0);
    canvas->drawable.width = width;
    canvas->drawable.height = height;
    canvas->drawable.filter[0] = CAFE_NEAREST;
    canvas->drawable.filter[1] = CAFE_NEAREST;
    canvas->drawable.wrap[0] = CAFE_CLAMP;
    canvas->drawable.wrap[1] = CAFE_CLAMP;
    canvas->fbo = tea_framebuffer(canvas->drawable.handle);
    return canvas;
}

void cafe_canvas_destroy(ca_Canvas* canvas) {
    if (!canvas) return;
    if (canvas->drawable.handle) tea_texture_free(canvas->drawable.handle);
    if (canvas->fbo) tea_framebuffer_free(canvas->fbo);
    free(canvas);
}
/*****************
 * Shader        *
 *****************/

ca_Shader* cafe_graphics_new_shader(const ca_i8* vert, const ca_i8* frag) {
    vert = vert ? vert : _default_vert_function;
    frag = frag ? frag : _default_frag_function;

    ca_Shader* shader = (ca_Shader*)malloc(sizeof(*shader));
    shader->program = tea_simple_program(vert, frag);
    shader->world = tea_program_uniform_location(shader->program, "u_World");
    shader->modelview = tea_program_uniform_location(shader->program, "u_ModelView");
    return shader;
}

void cafe_shader_destroy(ca_Shader* shader) {
    if (!shader) return;
    if (shader->program) tea_program_free(shader->program);
    free(shader);
}

void cafe_shader_set_uniform1i(ca_Shader* shader, const ca_i8* name, ca_i32 value) {
    if (!shader) return;
    ca_i32 loc = tea_program_uniform_location(shader->program, name);
    if (!loc) return;
    ca_Shader* current = render()->shader;
    te_program_t p = 0;
    if (current) p = current->program;
    tea_use_program(shader->program);
    tea_program_set_uniform1i(loc, value);
    tea_use_program(p);
}

void cafe_shader_set_uniform1f(ca_Shader* shader, const ca_i8* name, ca_f32 value) {
    if (!shader) return;
    ca_i32 loc = tea_program_uniform_location(shader->program, name);
    if (!loc) return;
    ca_Shader* current = render()->shader;
    te_program_t p = 0;
    if (current) p = current->program;
    tea_use_program(shader->program);
    tea_program_set_uniform1f(loc, value);
    tea_use_program(p);
}

void cafe_shader_set_uniform2f(ca_Shader* shader, const ca_i8* name, ca_f32 v0, ca_f32 v1) {
    if (!shader) return;
    ca_i32 loc = tea_program_uniform_location(shader->program, name);
    if (!loc) return;
    ca_Shader* current = render()->shader;
    te_program_t p = 0;
    if (current) p = current->program;
    tea_use_program(shader->program);
    tea_program_set_uniform2f(loc, v0, v1);
    tea_use_program(p);
}

void cafe_shader_set_uniform3f(ca_Shader* shader, const ca_i8* name, ca_f32 v0, ca_f32 v1, ca_f32 v2) {
    if (!shader) return;
    ca_i32 loc = tea_program_uniform_location(shader->program, name);
    if (!loc) return;
    ca_Shader* current = render()->shader;
    te_program_t p = 0;
    if (current) p = current->program;
    tea_use_program(shader->program);
    tea_program_set_uniform3f(loc, v0, v1, v2);
    tea_use_program(p);
}

void cafe_shader_set_uniform4f(ca_Shader* shader, const ca_i8* name, ca_f32 v0, ca_f32 v1, ca_f32 v2, ca_f32 v3) {
    if (!shader) return;
    ca_i32 loc = tea_program_uniform_location(shader->program, name);
    if (!loc) return;
    ca_Shader* current = render()->shader;
    te_program_t p = 0;
    if (current) p = current->program;
    tea_use_program(shader->program);
    tea_program_set_uniform4f(loc, v0, v1, v2, v3);
    tea_use_program(p);
}

void cafe_shader_set_uniform_mat4(ca_Shader* shader, const ca_i8* name, ca_Mat4 value) {
    if (!shader) return;
    ca_i32 loc = tea_program_uniform_location(shader->program, name);
    if (!loc) return;
    ca_Shader* current = render()->shader;
    te_program_t p = 0;
    if (current) p = current->program;
    tea_use_program(shader->program);
    tea_program_set_uniform_matrix4fv(loc, 1, TEA_FALSE, (const ca_f32*)value);
    tea_use_program(p);
}
#endif

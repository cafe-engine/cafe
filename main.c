#include "cafe.h"
// #include "tea.h"
#include <SDL2/SDL.h>

float x, y;

const char *_frag_shader =
"vec4 pixel(vec4 color, vec2 tex_coord, sampler2D sample) {\n"
"    return color * texture(sample, vec2(tex_coord.y, tex_coord.x));\n"
"}\n";

// ca_Shader *shader;
// ca_Texture *texture;
// ca_Texture *target;
ca_Image *image;
ca_Canvas *canvas;

void update(float dt)
{
    if (cafe_keyboard_is_down(CAFEK_LEFT)) x -= 10.f;
    else if (cafe_keyboard_is_down(CAFEK_RIGHT)) x += 10.f;
    // cafe_graphics_set_canvas(canvas);
    // cafe_graphics_draw_mode(CAFE_FILL);
    // cafe_graphics_set_color(CAFE_RGBA(255, 0, 255, 255));
    // cafe_graphics_draw_image(image, NULL, NULL);
    // cafe_graphics_draw_rectangle(x, y, 32, 128);
    // cafe_graphics_set_canvas(NULL);

    // cafe_graphics_draw_canvas()
    ca_Rect dest = {0, 0, 640, 380};
    // cafe_graphics_draw_image((ca_Image*)image, &dest, NULL);
    cafe_graphics_clear(CAFE_RGB(0, 0, 0));

    cafe_graphics_draw_mode(CAFE_FILL);
    cafe_graphics_draw_circle(x, y+64, 16);
    cafe_graphics_draw_rectangle(64, 64, 64, 32);
}

int main(int argc, char **argv) {
    ca_Config config = cafe_config(NULL, 640, 380);
    x = y = 0;
    if (!cafe_open(&config)) {
        fprintf(stderr, "Failed to init cafe\n");
        return -1;
    }

    image = cafe_graphics_load_image("penguin.jpg");
    canvas = cafe_graphics_new_canvas(160, 95);

    cafe_run(update);
    cafe_close();

    return 0;
}

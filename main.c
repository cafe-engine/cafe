#include <stdio.h>
#include "src/cafe.h"

int main(int argc, char ** argv) {
    cf_Config c = {0};
    cafe_init_config(&c, "adventure", 640, 380);
    cafe_init(&c);
    cf_Canvas canvas = cafe_canvas(160, 95);


    cf_Image img = cafe_image_load("modules/tea/goblin.png");

    while (!cafe_should_close()) {
        cafe_begin();

        cafe_graphics_set_canvas(canvas);
        cafe_graphics_clear((cf_Color){255, 255, 255, 255});
        cafe_graphics_draw_color((cf_Color){0, 255, 255, 255});
        cafe_graphics_rectangle(0, 0, 32, 32, CAFE_LINE);

        cafe_graphics_draw_color((cf_Color){255, 0, 255, 255});
        cafe_graphics_circle(32, 32, 8, CAFE_FILL);
        cafe_graphics_set_canvas(0);
        
        cafe_graphics_draw_canvas(canvas, NULL, 0, 64);

        cf_Rect r = {0, 0, 16, 16};
        cafe_graphics_draw_image_ex(img, &r, cafe_point(0, 0), 0, cafe_point(4, 4), cafe_point(0, 0));

        cafe_end();
    }

    cafe_terminate();
}

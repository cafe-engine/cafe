#include "src/cafe.h"

int main(int argc, char ** argv) {
    cf_Config c = {0};
    cafe_init_config(&c, "adventure", 640, 380);
    cafe_init(&c);


    while (!cafe_should_close()) {
        cafe_begin();

        cafe_graphics_draw_color((cf_Color){255, 255, 255, 255});
        cafe_graphics_rectangle(0, 0, 32, 32, CAFE_LINE);

        cafe_graphics_draw_color((cf_Color){255, 0, 255, 255});
        cafe_graphics_circle(32, 32, 8, CAFE_FILL);

        cafe_end();
    }

    cafe_terminate();
}
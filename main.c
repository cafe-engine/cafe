#include <stdio.h>
#include "cafe.h"

int main(int argc, char ** argv) {
    cf_Config c = {0};
    cafe_init_config(&c, NULL, 640, 380);
    c.argc = argc;
    c.argv = argv;
    c.window_flags |= 1; // RESIZABLE
    cafe_init(&c);

    while (cafe_main_loop());

    cafe_terminate();
}

#include <stdio.h>
#include "cafe.h"

int main(int argc, char ** argv) {
    cf_Config c = {0};
    cafe_init_config(&c, "adventure", 640, 380);
    c.argc = argc;
    c.argv = argv;
    cafe_init(&c);

    while (cafe_main_loop());

    cafe_terminate();
}

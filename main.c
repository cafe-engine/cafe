#include <stdio.h>
#include "cafe.h"
#include "tea.h"

float x, y;

int cafe_load() {
    x = 32;
    y = 32;

    return 0;
}
int cafe_update(float dt) {
    if (tea_key_down(TEA_KEY_D)) x += 32;
    return 0;
}
int cafe_draw() {

    tea_color(TEA_WHITE);
    tea_circle(x, y, 16);
    return 0;
}

int main(int argc, char ** argv) { 
    cafe_init(argc, argv);
    cafe_loop();
    cafe_deinit();
    return 1;
}

#include "tea.h"

#include <SDL.h>

int main(int argc, char ** argv) {
    te_config_t conf = tea_config_init("hello", 640, 380);
    tea_init(&conf);

    float x;
    x = 0;

    tea_window_title("testando", NULL);
    tea_window_width(NULL, 128);

    while (!tea_should_close()) {
        tea_update_input();
        tea_begin();
        tea_clear(0xff0000ff);
        tea_color(TEA_WHITE);

        if (tea_key_down(TEA_KEY_A)) x += 10;

        tea_rect(x, 0, 32, 32);
        tea_circle(x+16, 16, 8);
        tea_print("olar", 0, 0);


        tea_end();
    }

    tea_deinit();

    return 0;
}

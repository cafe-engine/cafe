#include <stdio.h>
#include "cafe.h"

int main(int argc, char ** argv) {
    cf_Config c = {0};
    cafe_init_config(&c, NULL, 640, 380);
    c.argc = argc;
    c.argv = argv;
    c.window_flags |= 1; // RESIZABLE
    cafe_init(&c);
    cf_Audio *buf = cafe_audio_load("assets/music.wav", 0);
    cafe_audio_play(buf);

    while (cafe_main_loop());

    cafe_terminate();
}

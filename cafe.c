#include "cafe.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#include "lua/cafe.h"

#include "tea.h"
#include "mocha.h"
#include "coffee.h"
#include "latte.h"

/************************
 * Core
 ************************/

int cafe_init(int argc, char **argv) {
    te_config_t conf = tea_config_init("cafe 0.1.0", 640, 380);
    const char *path = argv[1] ? argv[1] : ".";
#ifdef __EMSCRIPTEN__
    conf.window_flags |= TEA_WINDOW_RESIZABLE;
#endif
    la_init(path);
    tea_init(&conf);
    mo_init(0);

    l_cafe_init(argc, argv);

    return 1;
}

int cafe_deinit() { l_cafe_deinit(); return 0; }

void _loop() {
    tea_begin();
    l_cafe_run(L);
    tea_end();
}

int cafe_loop() {
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(_loop, 0, 1);
#else
    while (!tea_should_close()) _loop();
#endif
    return 0;
}

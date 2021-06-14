#include "cafe.h"

#include "lua/cafe.h"

#include "tea.h"
#include "mocha.h"
#include "coffee.h"

/************************
 * Core
 ************************/

int cafe_init(int argc, char **argv) {
    te_config_t conf = tea_config_init("cafe 0.1.0", 640, 380);
    tea_init(&conf);
    mo_init(0);

    l_cafe_init(argc, argv);

    return 1;
}

int cafe_deinit() { l_cafe_deinit(); return 0; }

int cafe_loop() {
    while (!tea_should_close()) {
        tea_begin();
        l_cafe_run(L);
        tea_end();
    }
    return 0;
}

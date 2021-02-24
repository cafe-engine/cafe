#include <tea.h>

int main(int argc, char ** argv) {
    te_Config conf;
    tea_config_init(&conf, NULL, 640, 380);
    tea_init(&conf);
    te_Point p = tea_point(0, 0);    
    while (!tea_should_close()) {
        tea_begin();
        tea_draw_color(WHITE);
        
        if (tea_keyboard_was_pressed(TEA_KEY_A)) p.x -= 10;
        tea_draw_rect_point(p, tea_point(32, 32));        
        tea_end();
    }
    
    tea_terminate();
}
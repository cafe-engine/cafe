#include "src/tea.h"

#define GRAVITY 100

#include <math.h>
#define MATH_PI 3.14159


#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

te_Image img;
te_Canvas canvas;
te_Font *font;
int frame = 0;

int main_loop() {
  tea_begin();

  float delta = tea_get_delta();

  if (tea_keyboard_was_pressed(TEA_KEY_A)) frame = frame - 1 < 0 ? 5 : frame-1;
  else if (tea_keyboard_was_pressed(TEA_KEY_D)) frame = frame + 1 > 5 ? 0 : frame + 1;
  
  te_Rect r = tea_rect(frame*16, 0, 16, 16);

  tea_set_canvas(canvas);
  tea_clear_color(BLACK);
  tea_clear();
  tea_draw_color(WHITE);
  tea_draw_image(img, NULL, tea_point(0, 0));
  tea_draw_mode(TEA_LINE);
  tea_draw_rect(tea_rect(frame*16, 0, 16, 16));
  tea_draw_image(img, &r, tea_point(frame*16, 32));
  
  tea_draw_text(font, "TTTTTTTTTTT", tea_point(0, 48));

  char txt[25];
  sprintf(txt, "%f", delta);
  tea_draw_text(font, txt, tea_point(0, 64));

  tea_draw_mode(TEA_FILL);
  tea_draw_circle(tea_point(frame*32, 64), 8);
  
  // tea_draw_circle(tea_point(x, 0), 8);
  tea_set_canvas(0);

  if (tea_keyboard_was_pressed(TEA_KEY_ESCAPE)) return 0;


  //te_Texture *ct = tea_canvas_texture(canvas);
  //tea_draw_texture_ex(ct, &tea_rect(0, 0, 160*4, 95*4), NULL, 0, tea_point(0, 0), TEA_FLIP_NONE);
  tea_draw_canvas(canvas, NULL, tea_point(0, 0));
  

  tea_end();

  return 1;
}

int main(int argc, char ** argv) {
  te_Config conf;
  tea_config_init(&conf, NULL, 640, 380);
  conf.window_flags |= SDL_WINDOW_RESIZABLE;

  tea_init(&conf);

  img = tea_image_load("goblin.png");
  canvas = tea_canvas(160, 95);
  font = tea_font_load("extrude.ttf", 16);

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(main_loop, 30, 1);
#else
  while (!tea_should_close()) if (!main_loop()) break;
#endif

  // while (!tea_should_close()) {
    
  // }

  tea_terminate();

  return 0;
}

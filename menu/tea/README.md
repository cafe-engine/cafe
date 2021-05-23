# tea

Tea is a tiny C lib that handle graphics, window and input (uses SDL)

```c
#include "tea.h"

int main(int argc, char ** argv) {
    te_config_t c = tea_config_init("example", 640, 380);
    tea_init(&c);

    while (!tea_should_close()) {
        tea_update_input();
        tea_begin();
        
        tea_clear(TEA_BLACK);
        tea_color(TEA_COLOR(255, 0, 0, 255));

        tea_mode(TEA_LINE);
        tea_rect(32, 32, 32, 32);
        tea_circle(32, 32, 8);

        tea_end();
    }

    tea_deinit();
    return 0;
}
```

## TODOS:

- [x] Load images (stb_image.h)
- [x] Load fonts (stb_truetype.h)
- [ ] Window
	- [ ] wrap SDL get and set functions (title, width, height, fullscreen, resizable, ...)
- [ ] Input
	- [x] mouse input
	- [x] keyboard input
	- [ ] joystick input
- [ ] Software Renderer
	- [ ] primitives
		- [ ] point
		- [ ] line
		- [ ] rectangle
		- [ ] circle
		- [ ] triangle
		- [ ] polygon
	- [ ] textures
 	- [ ] render texture
 	- [ ] render text
- [ ] SDL Renderer (Hardware Accelerated)
	- [ ] primitives
		- [x] circle
		- [x] triangle
		- [ ] polygon
 	- [ ] CPU transforms
 	- [x] render text
- [ ] OpenGL Batched Renderer
 	- [ ] primitives
		- [ ] circle
		- [ ] polygon
 	- [ ] shaders
 	- [ ] framebuffers
 	- [ ] coordinate system (GPU transforms)
 	- [ ] render text

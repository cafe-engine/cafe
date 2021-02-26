# Tea

a tiny c lib that wraps SDL2 (and possibly GLFW in the future) for handle input, render and window.

The idea is to create a modular render, that can be compiled with different renders. 

## TODOS:

- [x] Load images (stb_image.h)
- [x] Load fonts (stb_truetype.h)
- [ ] Window
	- [ ] wrap SDL get and set functions (title, width, height, fullscreen, resizable, ...)
- [ ] Input
	- [ ] mouse input
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
- [ ] SDL Renderer
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

#ifndef CAFE_LUA_H
#define CAFE_LUA_H

#include <SDL.h>

#include "../cafe.h"
#include "tea.h"
#include "mocha.h"
#include "latte.h"

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#define AUDIO_CLASS "Audio"
#define TEXTURE_CLASS "Texture"
#define FILE_CLASS "File"
#define FONT_CLASS "Font"
#define RECT_CLASS "Rect"
#define JOYSTICK_CLASS "Joystick"
#define CONTROLLER_CLASS "Controller"

static const char *boot_lua = "local traceback = debug.traceback\n"
"local filepath = cafe.filesystem.path()\n"
"package.path = package.path .. ';' .. filepath .. '/?.lua;' .. filepath .. '/?/init.lua'\n"
"local function _err(msg)\n"
"   local trace = traceback('', 1)\n"
"   cafe.error(msg, trace)\n"
"   print(msg, trace)\n"
"end\n"
"function cafe.error(msg, trace)\n"
"   cafe.update = function() end\n"
"   cafe.draw = function()\n" 
//"       cafe.print('error', 32)\n"
//"       cafe.print(msg, 32, 16)\n"
//"       cafe.print(trace, 32, 32)\n"
"   end\n"
"end\n"
"function cafe.run()\n"
"   if cafe.load then cafe.load() end\n"
"   local delta = 0\n"
"   return function()\n"
"       dt = cafe.delta()\n"
"       if cafe.update then cafe.update(dt) end\n"
"       if cafe.draw then cafe.draw() end\n"
"   end\n"
"end\n"
"function cafe.window_move(id, x, y) end\n"
"function cafe.window_resize(id, w, h) end\n"
"function cafe.window_focus(id, focused) end\n"
"function cafe.window_visible(id, visible) end\n"
"function cafe.window_mouse(id, enter) end\n"
"function cafe.window_close(id) end\n"
"xpcall(function() require 'main' end, _err)";

extern lua_State *L;

CAFE_API int luaopen_cafe(lua_State *L);
CAFE_API int luaopen_render(lua_State *L);
CAFE_API int luaopen_keyboard(lua_State *L);
CAFE_API int luaopen_mouse(lua_State *L);
CAFE_API int luaopen_joystick(lua_State *L);
CAFE_API int luaopen_window(lua_State *L);

CAFE_API int l_cafe_init(int argc, char **argv);
CAFE_API int l_cafe_deinit();
CAFE_API int l_cafe_load(lua_State *L);
CAFE_API int l_cafe_run(lua_State *L);

/* Rect */
CAFE_API int luaopen_rect(lua_State *L);
CAFE_API int l_cafe_rect(lua_State *L);
CAFE_API int l_cafe_rect_set(lua_State *L);
CAFE_API int l_cafe_rect_x(lua_State *L);
CAFE_API int l_cafe_rect_y(lua_State *L);
CAFE_API int l_cafe_rect_w(lua_State *L);
CAFE_API int l_cafe_rect_h(lua_State *L);

/* File */
CAFE_API int luaopen_file(lua_State *L);
CAFE_API int l_cafe_file(lua_State *L);
CAFE_API int l_cafe_file_read(lua_State *L);
CAFE_API int l_cafe_file_write(lua_State *L);
CAFE_API int l_cafe_file_insert(lua_State *L);
CAFE_API int l_cafe_file_append(lua_State *L);
CAFE_API int l_cafe_file_close(lua_State *L);
CAFE_API int l_cafe_file_info(lua_State *L);
CAFE_API int l_cafe_file_size(lua_State *L);
CAFE_API int l_cafe_file__gc(lua_State *L);

/* Audio */
CAFE_API int luaopen_audio(lua_State *L);
CAFE_API int l_cafe_audio(lua_State *L);
CAFE_API int l_cafe_audio_play(lua_State *L);
CAFE_API int l_cafe_audio_pause(lua_State *L);
CAFE_API int l_cafe_audio_volume(lua_State *L);
CAFE_API int l_cafe_audio_stop(lua_State *L);
CAFE_API int l_cafe_audio__gc(lua_State *L);

/* Font */
CAFE_API int luaopen_font(lua_State *L);
CAFE_API int l_cafe_font(lua_State *L);
CAFE_API int l_cafe_font_print(lua_State *L);
CAFE_API int l_cafe_font__gc(lua_State *L);

/* Texture */
CAFE_API int luaopen_texture(lua_State *L);
CAFE_API int l_cafe_texture(lua_State *L);
CAFE_API int l_cafe_texture_info(lua_State *L);
CAFE_API int l_cafe_texture_width(lua_State *L);
CAFE_API int l_cafe_texture_height(lua_State *L);
CAFE_API int l_cafe_texture_size(lua_State *L);
CAFE_API int l_cafe_texture_draw(lua_State *L);
CAFE_API int l_cafe_texture__gc(lua_State *L);

/* Filesystem */

CAFE_API int luaopen_filesystem(lua_State *L);
CAFE_API int l_cafe_filesystem_fopen(lua_State *L);
CAFE_API int l_cafe_filesystem_info(lua_State *L);
CAFE_API int l_cafe_filesystem_path(lua_State *L);

/* Render */

CAFE_API int l_cafe_render_clear(lua_State *L);
CAFE_API int l_cafe_render_mode(lua_State *L);
CAFE_API int l_cafe_render_color(lua_State *L);

CAFE_API int l_cafe_render_target(lua_State *L);

CAFE_API int l_cafe_render_point(lua_State *L);
CAFE_API int l_cafe_render_line(lua_State *L);
CAFE_API int l_cafe_render_rectangle(lua_State *L);
CAFE_API int l_cafe_render_circle(lua_State *L);
CAFE_API int l_cafe_render_triangle(lua_State *L);
CAFE_API int l_cafe_render_text(lua_State *L);

/* Window */

CAFE_API int l_cafe_window_title(lua_State *L);
CAFE_API int l_cafe_window_pos(lua_State *L);
CAFE_API int l_cafe_window_size(lua_State *L);
CAFE_API int l_cafe_window_minimize(lua_State *L);
CAFE_API int l_cafe_window_maximize(lua_State *L);
CAFE_API int l_cafe_window_restore(lua_State *L);
CAFE_API int l_cafe_window_fullscreen(lua_State *L);

/****************
 * Input 
 ****************/

/* Keyboard */
CAFE_API int l_cafe_keyboard_down(lua_State *L);
CAFE_API int l_cafe_keyboard_up(lua_State *L);
CAFE_API int l_cafe_keyboard_pressed(lua_State *L);
CAFE_API int l_cafe_keyboard_released(lua_State *L);

/* Mouse */
CAFE_API int l_cafe_mouse_down(lua_State *L);
CAFE_API int l_cafe_mouse_up(lua_State *L);
CAFE_API int l_cafe_mouse_pressed(lua_State *L);
CAFE_API int l_cafe_mouse_released(lua_State *L);

/* Joystick */
CAFE_API int l_cafe_joystick(lua_State *L);
CAFE_API int l_cafe_joystick_count(lua_State *L);
CAFE_API int l_cafe_joystick_vibration(lua_State *L);
CAFE_API int l_cafe_joystick_name(lua_State *L);

CAFE_API int l_cafe_joystick_button_count(lua_State *L);
CAFE_API int l_cafe_joystick_axis_count(lua_State *L);
CAFE_API int l_cafe_joystick_hat_count(lua_State *L);
CAFE_API int l_cafe_joystick_ball_count(lua_State *L);

CAFE_API int l_cafe_joystick_button(lua_State *L);
CAFE_API int l_cafe_joystick_axis(lua_State *L);
CAFE_API int l_cafe_joystick_axes(lua_State *L);
CAFE_API int l_cafe_joystick_hat(lua_State *L);
CAFE_API int l_cafe_joystick_ball(lua_State *L);

/* Callbacks */

CAFE_API int l_cafe_callback_controller_device(int state, int id);
CAFE_API int l_cafe_callback_keyboard(int window_id, int down, int repeat, struct te_keysym_t sym);

CAFE_API int l_cafe_callback_window_move(int id, int x, int y);
CAFE_API int l_cafe_callback_window_resize(int id, int w, int h);
CAFE_API int l_cafe_callback_window_visible(int id, int visible);
CAFE_API int l_cafe_callback_window_focus(int id, int focused);
CAFE_API int l_cafe_callback_window_mouse(int id, int mouse_enter);
CAFE_API int l_cafe_callback_window_close(int id);

#endif /* CAFE_LUA_H */

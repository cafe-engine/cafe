#ifndef CAFE_LUA_H
#define CAFE_LUA_H

#include "cafe.h"

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "cstar.h"

#define RECT_CLASS "Rect"

#define IMAGE_CLASS "Image"
#define CANVAS_CLASS "Canvas"
#define SHADER_CLASS "Shader"

#define FILE_CLASS "File"
#define DIR_CLASS "Dir"
#define VDRIVE_CLASS "VDrive"

CAFE_API int cafe_lua_init();
CAFE_API int cafe_lua_load();
CAFE_API int cafe_lua_step();

CAFE_API int luaopen_cafe(lua_State *L);

CAFE_API int luaopen_audio(lua_State *L);
CAFE_API int luaopen_graphics(lua_State *L);
/*CAFE_API int luaopen_input(lua_State *L);*/
CAFE_API int luaopen_keyboard(lua_State *L);
CAFE_API int luaopen_filesystem(lua_State *L);

CAFE_API int luaopen_types(lua_State *L);

#endif /* CAFE_LUA_H */

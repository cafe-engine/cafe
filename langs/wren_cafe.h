#ifndef _CAFE_WREN_H
#define _CAFE_WREN_H

#include "cafe.h"

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#define AUDIO_CLASS "Audio"

#define TEXTURE_CLASS "Texture"
#define FONT_CLASS "Font"

#define FILE_CLASS "File"

#define JOYSTICK_CLASS "Joystick"
#define CONTROLLER_CLASS "Controller"

extern lua_State *L;
CAFE_API int luaopen_cafe(lua_State *L);

#endif

#ifdef LUA_CAFE_IMPLEMENTATION

#endif
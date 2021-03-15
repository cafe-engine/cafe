#ifndef CAFE_LUA_H
#define CAFE_LUA_H

#include "cafe.h"

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "cstar.h"

CAFE_API int cafe_lua_init();
CAFE_API int cafe_lua_load();
CAFE_API int cafe_lua_step();

CAFE_API int luaopen_cafe(lua_State *L);

#endif /* CAFE_LUA_H */

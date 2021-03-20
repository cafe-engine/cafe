#include "cafe_lua.h"

static int cafe_lua_audio_load(lua_State *L) {
    const char *filename = luaL_checkstring(L, 1);

    return 1;
}

int luaopen_audio(lua_State *L) {
    luaL_Reg reg[] = {
        {NULL, NULL}
    };

    luaL_newlib(L, reg);

    return 1;
}

#include "cafe_lua.h"

static int cafe_lua_audio_load(lua_State *L) {
    const char *filename = luaL_checkstring(L, 1);

    cf_Audio **laudio = lua_newuserdata(L, sizeof(*laudio));
    *laudio = cafe_audio_load(filename, 0);

    return 1;
}

int luaopen_audio(lua_State *L) {
    luaL_Reg reg[] = {
        {"Buffer", cafe_lua_audio_load},
        {NULL, NULL}
    };

    luaL_newlib(L, reg);

    return 1;
}

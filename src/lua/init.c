#include "cafe_lua.h"

static lua_State *L;

int cafe_lua_init() {
    L = luaL_newstate();

    luaL_openlibs(L);

    luaL_requiref(L, "cafe", luaopen_cafe, 1);

    luaL_dofile(L, "init.lua");

    cafe_lua_load();
    return 1;
}

int cafe_lua_load() {
    lua_getglobal(L, "cafe");
    if (!lua_isnil(L, -1)) {
        lua_getfield(L, -1, "_load");
        if (!lua_isnil(L, -1)) {
            int err = lua_pcall(L, 0, 0, 0);
            if (err) {
                const char *str = lua_tostring(L, -1);
                cst_error("%s", str);
            }
        }
        lua_pop(L, 1);
        lua_settop(L, 1);
    }
    return 1;
}

int cafe_lua_step() {
    lua_getglobal(L, "cafe");
    if (!lua_isnil(L, -1)) {
        lua_getfield(L, -1, "_step");
        if (!lua_isnil(L, -1)) {
            int err = lua_pcall(L, 0, 0, 0);
            if (err) {
                const char *str = lua_tostring(L, -1);
                cst_error("%s", str);
            }
        }
        lua_pop(L, 1);
        lua_settop(L, 1);
    }
    return 1;
}

int cafe_lua_get_version(lua_State *L) {
    lua_pushstring(L, CAFE_VERSION);

    return 1;
}

int luaopen_cafe(lua_State *L) {
    luaL_Reg reg[] = {
        {"getVersion", cafe_lua_get_version},
        {NULL, NULL}
    };

    luaL_newlib(L, reg);

    return 1;
}
/* Graphics */



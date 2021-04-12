#include "cafe_lua.h"

static int lua_cafe_new_audio(lua_State *L) {
    const char *filename = luaL_checkstring(L, 1);

    cf_Audio *laudio = lua_newuserdata(L, sizeof(*laudio));
    luaL_setmetatable(L, AUDIO_CLASS);
    *laudio = cafe_audio_load(filename, 0);

    return 1;
}

static int lua_cafe_new_sound(lua_State *L) {
    const char *filename = luaL_checkstring(L, 1);

    cf_Sound *sound = lua_newuserdata(L, sizeof(*sound));
    luaL_setmetatable(L, AUDIO_CLASS);
    *sound = cafe_sound_load(filename);

    return 1;
}

static int lua_cafe_new_music(lua_State *L) {
    const char *filename = luaL_checkstring(L, 1);

    cf_Music *music = lua_newuserdata(L, sizeof(*music));
    luaL_setmetatable(L, AUDIO_CLASS);
    *music = cafe_music_load(filename); 

    return 1;
}

int luaopen_audio(lua_State *L) {
    luaL_Reg reg[] = {
        {"Buffer", lua_cafe_new_audio},
        {"Sound", lua_cafe_new_sound},
        {"Music", lua_cafe_new_music},
        {NULL, NULL}
    };

    luaL_newlib(L, reg);

    return 1;
}

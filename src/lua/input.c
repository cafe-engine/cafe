#include "cafe_lua.h"


static int cafe_lua_keyboard_isdown(lua_State *L) {
   const char *keyname = luaL_checkstring(L, 1); 


   int key = cafe_key_from_name(keyname);
   int down = cafe_keyboard_isdown(key);

   lua_pushboolean(L, down);
   return 1;
}

int luaopen_keyboard(lua_State *L) {
    luaL_Reg reg[] = {
        {"isDown", cafe_lua_keyboard_isdown},
        {NULL, NULL}
    };

    luaL_newlib(L, reg);

    return 1;
}

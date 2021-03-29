#include "cafe_lua.h"


static int cafe_lua_keyboard_isdown(lua_State *L) {
   const char *keyname = luaL_checkstring(L, 1); 


   int key = cafe_key_from_name(keyname);
   int down = cafe_keyboard_isdown(key);

   lua_pushboolean(L, down);
   return 1;
}

static int cafe_lua_keyboard_isup(lua_State *L) {
   const char *keyname = luaL_checkstring(L, 1); 


   int key = cafe_key_from_name(keyname);
   int down = cafe_keyboard_isup(key);

   lua_pushboolean(L, down);
   return 1;
}

static int cafe_lua_keyboard_waspressed(lua_State *L) {
   const char *keyname = luaL_checkstring(L, 1); 


   int key = cafe_key_from_name(keyname);
   int down = cafe_keyboard_waspressed(key);

   lua_pushboolean(L, down);
   return 1;
}
static int cafe_lua_keyboard_wasreleased(lua_State *L) {
   const char *keyname = luaL_checkstring(L, 1); 


   int key = cafe_key_from_name(keyname);
   int down = cafe_keyboard_wasreleased(key);

   lua_pushboolean(L, down);
   return 1;
}
int luaopen_keyboard(lua_State *L) {
    luaL_Reg reg[] = {
        {"isDown", cafe_lua_keyboard_isdown},
        {"isUp", cafe_lua_keyboard_isup},
        {"wasPressed", cafe_lua_keyboard_waspressed},
        {"wasReleased", cafe_lua_keyboard_wasreleased},
        {NULL, NULL}
    };

    luaL_newlib(L, reg);

    return 1;
}

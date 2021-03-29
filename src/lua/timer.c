/*
 * =====================================================================================
 *
 *       Filename:  timer.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  27/03/2021 11:53:38
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "cafe_lua.h"
#include "tea.h"

static int lua_cafe_delta(lua_State *L) {

    lua_pushnumber(L, tea_delta());
    return 1;
}

int luaopen_timer(lua_State *L) {
    luaL_Reg reg[] = {
        {"delta", lua_cafe_delta},
        {NULL, NULL}
    };

    luaL_newlib(L, reg);

    return 1;
}

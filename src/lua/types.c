#include "cafe.h"
#include "cafe_lua.h"
#include "lua.h"

/* Image */

static int cafe_lua_image_draw(lua_State *L) {
    cf_Image *img = luaL_checkudata(L, 1, IMAGE_CLASS);

    CAFE_VALUE x = luaL_optnumber(L, 2, 0);
    CAFE_VALUE y = luaL_optnumber(L, 3, 0);

    cafe_graphics_draw_image(*img, NULL, x, y);

   return 0; 
}

int cafeopen_image(lua_State *L) {
    luaL_Reg reg[] = {
        {"draw", cafe_lua_image_draw},
        {"filter", NULL},
        {"wrap", NULL},
        {NULL, NULL}
    };

    luaL_newmetatable(L, IMAGE_CLASS);
    luaL_setfuncs(L, reg, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    return 1;
}

/* Canvas */

/*static int cafe_lua_new_canvas(lua_State *L) {
    int w, h;

    w = luaL_checknumber(L, 1);
    cst_assert(w > 0);

    h = luaL_checknumber(L, 2);
    cst_assert(h > 0);

    cf_Canvas *canvas = lua_newuserdata(L, sizeof(*canvas));
    luaL_setmetatable(L, CANVAS_CLASS);

    *canvas = cafe_canvas(w, h);
    
    return 1;
}*/

static int lua_cafe_canvas_draw(lua_State *L) {

    cf_Canvas *canvas = luaL_checkudata(L, 1, CANVAS_CLASS);

    cf_Point p;
    p.x = luaL_optnumber(L, 2, 0);
    p.y = luaL_optnumber(L, 3, 0);

    cafe_graphics_draw_canvas(*canvas, NULL, p.x, p.y);
    
    return 0;
}

static int lua_cafe_canvas_set(lua_State *L) {
    cf_Canvas *canvas = luaL_checkudata(L, 1, CANVAS_CLASS);

    cafe_graphics_set_canvas(*canvas);
    return 0;
}

static int lua_cafe_canvas_unset(lua_State *L) {
    cafe_graphics_set_canvas(0);
    return 0;
}

int cafeopen_canvas(lua_State *L) {
    luaL_Reg reg[] = {
        {"draw", lua_cafe_canvas_draw},
        {"set", lua_cafe_canvas_set},
        {"unset", lua_cafe_canvas_unset},
        {NULL, NULL}
    };

    luaL_newmetatable(L, CANVAS_CLASS);
    luaL_setfuncs(L, reg, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    
    return 1;
}

int luaopen_types(lua_State *L) {
    lua_newtable(L);

    struct { char *name; int (*fn)(lua_State*); } libs[] = {
        {"Image", cafeopen_image},
        {"Canvas", cafeopen_canvas},
        {NULL, NULL}
    };

    for (int i = 0; libs[i].name != NULL; i++) {
        libs[i].fn(L);
        lua_setfield(L, -2, libs[i].name);
    }

    return 1;
}

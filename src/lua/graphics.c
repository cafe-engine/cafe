#include "cafe_lua.h"

static int cafe_lua_graphics_clear(lua_State *L) {
    cf_Color color = {0, 0, 0, 255};
    
    cafe_graphics_clear(color);
    return 0;
}

static int cafe_lua_graphics_setcolor(lua_State *L) {
    cafe_graphics_draw_color((cf_Color){255, 255, 255, 255});

    return 0;
}

static int cafe_lua_graphics_circle(lua_State *L) {
   int mode = CAFE_FILL;

   cf_Point pos;
   pos.x = luaL_checknumber(L, 1);
   pos.y = luaL_checknumber(L, 2);
   CAFE_VALUE radius = luaL_checknumber(L, 3);

   cafe_graphics_circle(pos.x, pos.y, radius, mode);
    
   return 0;
}

static int cafe_lua_graphics_rectangle(lua_State *L) {
    int mode = CAFE_FILL;

    CAFE_VALUE x = luaL_checknumber(L, 1);
    CAFE_VALUE y = luaL_checknumber(L, 2);
    CAFE_VALUE w = luaL_checknumber(L, 3);
    CAFE_VALUE h = luaL_checknumber(L, 4);

    cafe_graphics_rectangle(x, y, w, h, mode);

    return 0;
}

static int cafe_lua_graphics_triangle(lua_State *L) {
   int mode = CAFE_FILL;

   CAFE_VALUE points[6];
   for (int i = 0; i < 6; i++) {
       points[i] = luaL_checknumber(L, i+1);
   }

   cafe_graphics_triangle(points[0], points[1], points[2], points[3], points[4], points[5], mode);           
    
   return 0;
}

static int cafe_lua_new_image(lua_State *L) {
    const char *filename = luaL_checkstring(L, 1);
    
    cf_Image *img = lua_newuserdata(L, sizeof(*img));
    luaL_setmetatable(L, "Image");
    *img = cafe_image_load(filename);

    return 1;
}

static int cafe_lua_new_canvas(lua_State *L) {
    int w = luaL_checknumber(L, 1);
    cst_assert(w > 0);
    int h = luaL_checknumber(L, 2);
    cst_assert(h > 0);

    cf_Canvas *canvas = lua_newuserdata(L, sizeof(*canvas));
    luaL_setmetatable(L, CANVAS_CLASS);
    
    *canvas = cafe_canvas(w, h);

    return 1;
}

int luaopen_graphics(lua_State *L) {
    luaL_Reg reg[] = {
        {"Image", cafe_lua_new_image},
        {"Canvas", cafe_lua_new_canvas},
        {"clear", cafe_lua_graphics_clear},
        {"setColor", cafe_lua_graphics_setcolor},
        {"rectangle", cafe_lua_graphics_rectangle},
        {"circle", cafe_lua_graphics_circle},
        {"triangle", cafe_lua_graphics_triangle},
        {NULL, NULL}
    };

    luaL_newlib(L, reg);

    return 1;
}

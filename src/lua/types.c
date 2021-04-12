#include "cafe.h"
#include "cafe_lua.h"
#include "lua.h"

/* Audio */

static int lua_cafe_audio_play(lua_State *L) {
    cf_Audio *data = luaL_checkudata(L, 1, AUDIO_CLASS);
    cafe_audio_play(*data);

    return 0;
}

static int lua_cafe_audio_pause(lua_State *L) {
    cf_Audio *data = luaL_checkudata(L, 1, AUDIO_CLASS);
    cafe_audio_pause(*data);

    return 0;
}

static int lua_cafe_audio_stop(lua_State *L) {
    cf_Audio *data = luaL_checkudata(L, 1, AUDIO_CLASS);
    cafe_audio_stop(*data);

    return 0;
}


static int cafeopen_audio(lua_State *L) {
    luaL_Reg reg[] = {
        {"play", lua_cafe_audio_play},
        {"stop", lua_cafe_audio_stop},
        {"pause", lua_cafe_audio_pause},
        {NULL, NULL}
    };

    luaL_newmetatable(L, AUDIO_CLASS);
    luaL_setfuncs(L, reg, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    return 1;
}

/* Rect */

static int cafe_lua_rect_x(lua_State *L) {
    cf_Rect *r = luaL_checkudata(L, 1, RECT_CLASS);

    r->x = luaL_optnumber(L, 2, r->x);
    lua_pushnumber(L, r->x);

    return 1;
}

static int cafe_lua_rect_y(lua_State *L) {
    cf_Rect *r = luaL_checkudata(L, 1, RECT_CLASS);

    r->y = luaL_optnumber(L, 2, r->y);
    lua_pushnumber(L, r->y);

    return 1;
}

static int cafe_lua_rect_w(lua_State *L) {
    cf_Rect *r = luaL_checkudata(L, 1, RECT_CLASS);

    r->w = luaL_optnumber(L, 2, r->w);
    lua_pushnumber(L, r->w);

    return 1;
}

static int cafe_lua_rect_h(lua_State *L) {
    cf_Rect *r = luaL_checkudata(L, 1, RECT_CLASS);

    r->h = luaL_optnumber(L, 2, r->h);
    lua_pushnumber(L, r->h);

    return 1;
}

static int cafe_lua_rect_set(lua_State *L) {
    cf_Rect *r = luaL_checkudata(L, 1, RECT_CLASS);

    r->x = luaL_optnumber(L, 2, r->x);
    r->y = luaL_optnumber(L, 3, r->y);
    r->w = luaL_optnumber(L, 4, r->w);
    r->h = luaL_optnumber(L, 5, r->h);

    return 0;
}

int cafeopen_rect(lua_State *L) {
    luaL_Reg reg[] = {
        {"x", cafe_lua_rect_x},
        {"y", cafe_lua_rect_y},
        {"w", cafe_lua_rect_w},
        {"h", cafe_lua_rect_h},
        {"set", cafe_lua_rect_set},
        {NULL, NULL}
    };

    luaL_newmetatable(L, RECT_CLASS);
    luaL_setfuncs(L, reg, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    return 1;
}

/* Image */

static int cafe_lua_image_draw(lua_State *L) {
    cf_Image *img = luaL_checkudata(L, 1, IMAGE_CLASS);
    cf_Rect *r = NULL;

    int arg = 2;
    int args = lua_gettop(L);
    if (luaL_testudata(L, arg, RECT_CLASS)) r = lua_touserdata(L, arg++);
    

    CAFE_VALUE x = luaL_optnumber(L, arg++, 0);
    CAFE_VALUE y = luaL_optnumber(L, arg++, 0);
    if (args < 5) {
        cafe_graphics_draw_image(*img, r, x, y);
        return 0;
    }

    CAFE_VALUE angle = luaL_optnumber(L, arg++, 0);
    cf_Point scale;
    scale.x = luaL_optnumber(L, arg++, 1);
    scale.y = luaL_optnumber(L, arg++, 1);

    cf_Point origin;
    origin.x = luaL_optnumber(L, arg++, 0);
    origin.y = luaL_optnumber(L, arg++, 0);

    cafe_graphics_draw_image_ex(*img, r, CAFE_POINT(x, y), angle, scale, origin);

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
    cf_Rect *r = NULL;

    int arg = 2;
    int args = lua_gettop(L);
    if (luaL_testudata(L, arg, RECT_CLASS)) r = lua_touserdata(L, arg++);
    

    CAFE_VALUE x = luaL_optnumber(L, arg++, 0);
    CAFE_VALUE y = luaL_optnumber(L, arg++, 0);
    if (args < 5) {
        cafe_graphics_draw_image(*canvas, r, x, y);
        return 0;
    }

    CAFE_VALUE angle = luaL_optnumber(L, arg++, 0);
    cf_Point scale;
    scale.x = luaL_optnumber(L, arg++, 1);
    scale.y = luaL_optnumber(L, arg++, 1);

    cf_Point origin;
    origin.x = luaL_optnumber(L, arg++, 0);
    origin.y = luaL_optnumber(L, arg++, 0);

    cafe_graphics_draw_image_ex(*canvas, r, CAFE_POINT(x, y), angle, scale, origin);

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

/* File */

static int lua_cafe_file_read(lua_State *L) {
    cf_File **ffp = (cf_File**)luaL_checkudata(L, 1, FILE_CLASS);
    cf_File *fp = *ffp;
    cf_Header h;

    cafe_file_header(fp, &h);
    char buf[h.size+1];

    cafe_file_read(fp, buf, h.size);
    buf[h.size] = '\0';

    lua_pushstring(L, buf);
    return 1;
}

static int lua_cafe_file_write(lua_State *L) {
    cf_File **ffp = (cf_File**)luaL_checkudata(L, 1, FILE_CLASS);
    cf_File *fp = *ffp;
    size_t len;
    const char *text = luaL_checklstring(L, 2, &len);

    cafe_file_write(fp, text, len);
    return 0;
}

static int lua_cafe_file_seek(lua_State *L) {
    cf_File **ffp = (cf_File**)luaL_checkudata(L, 1, FILE_CLASS);
    cf_File *fp = *ffp;
    int o = cafe_file_tell(fp);

    int offset = luaL_optnumber(L, 2, o);

    cafe_file_seek(fp, offset);
    return 0;
}

static int lua_cafe_file_tell(lua_State *L) {
    cf_File **ffp = (cf_File**)luaL_checkudata(L, 1, FILE_CLASS);
    cf_File *fp = *ffp;
    
    int offset = cafe_file_tell(fp);
    lua_pushnumber(L, offset);
    return 1;
}

static int lua_cafe_file_size(lua_State *L) {
    cf_File **ffp = (cf_File**)luaL_checkudata(L, 1, FILE_CLASS);
    cf_File *fp = *ffp;
    
    cf_Header h;
    cafe_file_header(fp, &h);

    lua_pushnumber(L, h.size);
    return 1;
}

static int lua_cafe_file_header(lua_State *L) {
    cf_File **fp = luaL_checkudata(L, 1, FILE_CLASS);
    cf_Header h;

    cafe_file_header(*fp, &h);
    lua_newtable(L);

    lua_pushstring(L, h.name);
    lua_setfield(L, -2, "name");

    lua_pushstring(L, h.linkname);
    lua_setfield(L, -2, "linkname");

    lua_pushnumber(L, h.size);
    lua_setfield(L, -2, "size");

    lua_pushstring(L, h.uname);
    lua_setfield(L, -2, "uname");

    lua_pushstring(L, h.gname);
    lua_setfield(L, -2, "gname");

    return 1;
}

static int lua_cafe_file_close(lua_State *L) {
    cf_File **fp = luaL_checkudata(L, 1, FILE_CLASS);

    cafe_file_close(*fp);

    return 0;
}

int cafeopen_file(lua_State *L) {
    luaL_Reg reg[] = {
        {"read", lua_cafe_file_read},
        {"write", lua_cafe_file_write},
        {"seek", lua_cafe_file_seek},
        {"tell", lua_cafe_file_tell},
        {"size", lua_cafe_file_size},
        {"header", lua_cafe_file_header},
        {"close", lua_cafe_file_close},
        {NULL, NULL}
    };

    luaL_newmetatable(L, FILE_CLASS);
    luaL_setfuncs(L, reg, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    return 1;
}

/* Directory */

static int lua_cafe_dir_read(lua_State *L) {
    cf_Dir **dir = (cf_Dir**)luaL_checkudata(L, 1, DIR_CLASS);

    return 1;
}

static int lua_cafe_dir_close(lua_State *L) {
    cf_Dir **dir = (cf_Dir**)luaL_checkudata(L, 1, DIR_CLASS);
    cafe_dir_close(*dir);

    return 1;
}

int cafeopen_dir(lua_State *L) {
    luaL_Reg reg[] = {
        {"read", lua_cafe_dir_read},
        {"close", lua_cafe_dir_close},
        {NULL, NULL}
    };

    luaL_newmetatable(L, DIR_CLASS);
    luaL_setfuncs(L, reg, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    return 1;
}

int luaopen_types(lua_State *L) {
    lua_newtable(L);

    struct { char *name; int (*fn)(lua_State*); } libs[] = {
        {"Audio", cafeopen_audio},
        {"Rect", cafeopen_rect},
        {"Image", cafeopen_image},
        {"Canvas", cafeopen_canvas},
        {"File", cafeopen_file},
        {"Dir", cafeopen_dir},
        {NULL, NULL}
    };

    for (int i = 0; libs[i].name != NULL; i++) {
        libs[i].fn(L);
        lua_setfield(L, -2, libs[i].name);
    }

    return 1;
}

#ifndef _CAFE_LUA_H
#define _CAFE_LUA_H

#include "cafe.h"

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#define AUDIO_CLASS "Audio"

#define TEXTURE_CLASS "Texture"
#define FONT_CLASS "Font"

#define FILE_CLASS "File"

#define JOYSTICK_CLASS "Joystick"
#define CONTROLLER_CLASS "Controller"

CAFE_API ca_bool cafe_lua_setup(void);
CAFE_API ca_StepFunction cafe_lua_step_function(void);

extern lua_State *L;
CAFE_API int luaopen_cafe(lua_State *L);

CAFE_API int luaopen_render(lua_State *L);
CAFE_API int luaopen_keyboard(lua_State *L);
CAFE_API int luaopen_mouse(lua_State *L);

CAFE_API int luaopen_texture(lua_State *L);

#endif

#ifdef CAFE_LUA_IMPLEMENTATION

lua_State *L;

const char *init_lua_str =
"local traceback = debug.traceback\n"
"local function _step()\n"
"if cafe.update then cafe.update(0) end\n"
"if cafe.draw then cafe.draw() end\n"
"end\n"
"function _error()\n"
"local trace = traceback('', 1)\n"
"print(msg, trace)\n"
"end\n"
"function cafe._load()\n"
"if cafe.load then xpcall(cafe.load, _error) end\n"
"end\n"
"function cafe._step()\n"
"xpcall(_step, _error)\n"
"end\n"
"xpcall(function() require 'main' end, _error)\n";

ca_bool cafe_lua_setup(void) {
    L = luaL_newstate();
    luaL_openlibs(L);
    luaL_requiref(L, "cafe", luaopen_cafe, 1);

    int lerr = luaL_loadbuffer(L, init_lua_str, strlen(init_lua_str), "init.lua");
    int err = lua_pcall(L, 0, 0, 0);
    if (lerr || err) {
        const char *error = lua_tostring(L, -1);
        fprintf(stderr, "Failed to init Lua: %s\n", error);
        return CA_FALSE;
    }
    return CA_TRUE;
}

void l_cafe_step_function(float dt) {
    lua_getglobal(L, "cafe");
    if (!lua_isnil(L, -1)) {
        lua_getfield(L, -1, "_step");
        if (!lua_isnil(L, -1)) {
            int err = lua_pcall(L, 0, 0, 0);
            if (err) {
                const char *str = lua_tostring(L, -1);
            }
        }
        lua_pop(L, 1);
        lua_settop(L, 1);
    }
}

ca_StepFunction cafe_lua_step_function(void) {
    return l_cafe_step_function;
}

static int l_cafe_getVersion(lua_State *L) {
    lua_pushstring(L, CAFE_VERSION);
    return 1;
}

int luaopen_cafe(lua_State *L) {
    luaL_Reg reg[] = {
        { "getVersion", l_cafe_getVersion },
        { NULL, NULL }
    };

    luaL_newlib(L, reg);

    struct {
        char *name;
        int (*fn)(lua_State*);
    } libs[] = {
        { "_Audio", NULL },
        { "_Sound", NULL },
        { "_Music", NULL },
        { "_Texture", luaopen_texture },
        { "_File", NULL },
        { "audio", NULL },
        { "filesystem", NULL },
        { "keyboard", luaopen_keyboard },
        { "mouse", luaopen_mouse },
        { "joystick", NULL },
        { "render", luaopen_render },
        { "window", NULL },
        { NULL, NULL }
    };

    int i;
    for (i = 0; libs[i].name != NULL; i++) {
        if (libs[i].fn) {
            libs[i].fn(L);
            lua_setfield(L, -2, libs[i].name);
        }
    }

    return 1;
}

/*=====================*
 * RENDER FUNCTIONS    *
 *=====================*/

static int l_cafe_render_clear(lua_State *L) {
    ca_Color color = CAFE_RGB(0, 0, 0);
    ca_u8 colors[4] = {0, 0, 0, 255};
    for (int i = 0; i < 4; i++)
        colors[i] = luaL_optnumber(L, 1, colors[i]);

    memcpy(&color, colors, sizeof color);
    cafe_render_clear(color);

    return 0;
}

static int l_cafe_render_setMode(lua_State *L) {
    const char *mode_str = luaL_checkstring(L, 1);
    ca_i32 mode = CAFE_LINE;
    if (strcmp(mode_str, "line") == 0) mode = CAFE_LINE;
    else if (strcmp(mode_str, "fill") == 0) mode = CAFE_FILL;

    cafe_render_setMode(mode);
    return 0;
}

static int l_cafe_render_setTarget(lua_State *L) {
    ca_Texture **tex = luaL_testudata(L, 1, TEXTURE_CLASS);
    ca_Texture *ttex = NULL;
    if (tex) ttex = *tex;
    cafe_render_setTarget(ttex);
    return 0;
}

static int l_cafe_render_setColor(lua_State *L) {
    ca_Color color = CAFE_RGB(0, 0, 0);
    ca_u8 colors[4] = {0, 0, 0, 255};
    for (int i = 0; i < 4; i++)
        colors[i] = luaL_optnumber(L, 1, colors[i]);

    memcpy(&color, colors, sizeof color);
    cafe_render_setColor(color);
    return 0;
}

static int l_cafe_render_newTexture(lua_State *L) {
    ca_Texture **texture = (ca_Texture*)lua_newuserdata(L, sizeof *texture);
    luaL_setmetatable(L, TEXTURE_CLASS);
    if (lua_type(L, 1) == LUA_TSTRING) {
        const ca_i8 *path = luaL_checkstring(L, 1);
        *texture = cafe_render_loadTexture(path);
    } else if (lua_type(L, 1) == LUA_TNUMBER) {
        ca_i32 width, height;
        width = luaL_checknumber(L, 1);
        height = luaL_checknumber(L, 2);

        *texture = cafe_render_createTexture(width, height, NULL, CAFE_TEXTURE_TARGET);
    }
    return 1;
}

static int l_cafe_render_rectangle(lua_State *L) {
    int x, y, w, h;
    x = luaL_checknumber(L, 1);
    y = luaL_checknumber(L, 2);
    w = luaL_checknumber(L, 3);
    h = luaL_checknumber(L, 4);
    cafe_render_rect(x, y, w, h);
    return 0;
}

static int l_cafe_render_triangle(lua_State *L) {
    int pos[6];
    for (int i = 0; i < 6; i++)
        pos[i] = luaL_checknumber(L, i+1);

    cafe_render_triangle(pos[0], pos[1], pos[2], pos[3], pos[4], pos[5]);
    return 0;
}

static int l_cafe_render_circle(lua_State *L) {
    int x, y;
    int radius;
    x = luaL_checknumber(L, 1);
    y = luaL_checknumber(L, 2);
    radius = luaL_checknumber(L, 3);
    cafe_render_circle(x, y, radius);
    return 0;
}

static int l_cafe_render_texture(lua_State *L) {
    ca_Texture **tex = luaL_checkudata(L, 1, TEXTURE_CLASS);
    ca_Rect dest, src;
    ca_i32 width, height;
    cafe_texture_getSize(*tex, &width, &height);
    dest.x = luaL_optnumber(L, 2, 0);
    dest.y = luaL_optnumber(L, 3, 0);
    dest.w = width;
    dest.h = height;
    cafe_render_texture(*tex, &dest, NULL);
    return 0;
}

int luaopen_render(lua_State *L) {
    luaL_Reg reg[] = {
        { "clear", l_cafe_render_clear },
        { "setMode", l_cafe_render_setMode },
        { "setTarget", l_cafe_render_setTarget },
        // { "setShader", l_cafe_render_setShader },
        { "newTexture", l_cafe_render_newTexture },
        { "setColor", l_cafe_render_setColor },
        { "rectangle", l_cafe_render_rectangle },
        { "triangle", l_cafe_render_triangle },
        { "circle", l_cafe_render_circle },
        { "texture", l_cafe_render_texture },
        { NULL, NULL }
    };

    luaL_newlib(L, reg);

    return 1;
}

/* Texture */

static int l_cafe_texture_getWidth(lua_State *L) {
    ca_Texture **tex = luaL_checkudata(L, 1, TEXTURE_CLASS);
    lua_pushnumber(L, cafe_texture_getWidth(*tex));
    return 1;
}

static int l_cafe_texture_getHeight(lua_State *L) {
    ca_Texture **tex = luaL_checkudata(L, 1, TEXTURE_CLASS);
    lua_pushnumber(L, cafe_texture_getHeight(*tex));
    return 1;
}

int luaopen_texture(lua_State *L) {
    luaL_Reg reg[] = {
        { "getWidth", l_cafe_texture_getWidth },
        { "getHeight", l_cafe_texture_getHeight },
        { NULL, NULL }
    };
    luaL_newmetatable(L, TEXTURE_CLASS);
    luaL_setfuncs(L, reg, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    return 1;
}

static int l_cafe_keyboard_isDown(lua_State *L) {
    const char *btn_str = luaL_checkstring(L, 1);
    int btn = cafe_keyboard_fromName(btn_str);
    lua_pushboolean(L, cafe_keyboard_isDown(btn));
    return 1;
}

static int l_cafe_keyboard_isUp(lua_State *L) {
    const char *btn_str = luaL_checkstring(L, 1);
    int btn = cafe_keyboard_fromName(btn_str);
    lua_pushboolean(L, cafe_keyboard_isUp(btn));
    return 1;
}

int luaopen_keyboard(lua_State *L) {
    luaL_Reg reg[] = {
        { "isDown", l_cafe_keyboard_isDown },
        { "isUp", l_cafe_keyboard_isUp },
        { NULL, NULL }
    };

    luaL_newlib(L, reg);
    return 1;
}

static int l_cafe_mouse_isDown(lua_State *L) {
    int btn = luaL_checknumber(L, 1);
    lua_pushboolean(L, cafe_mouse_isDown(btn));
    return 1;
}

static int l_cafe_mouse_isUp(lua_State *L) {
    int btn = luaL_checknumber(L, 1);
    lua_pushboolean(L, cafe_mouse_isUp(btn));
    return 1;
}

int luaopen_mouse(lua_State *L) {
    luaL_Reg reg[] = {
        { "isDown", l_cafe_mouse_isDown },
        { "isUp", l_cafe_mouse_isUp },
        { NULL, NULL }
    };

    luaL_newlib(L, reg);
    return 1;
}

#endif
#include "cafe.h"
#include "mocha.h"

lua_State *L;

int l_cafe_init(int args, char **argv) {
    L = luaL_newstate();

    tea_event_window_move(l_cafe_callback_window_move);
    tea_event_window_resize(l_cafe_callback_window_resize);
    tea_event_window_visible(l_cafe_callback_window_visible);
    tea_event_window_focus(l_cafe_callback_window_focus);
    tea_event_window_mouse(l_cafe_callback_window_mouse);
    tea_event_window_close(l_cafe_callback_window_close);
    tea_event_key(l_cafe_callback_keyboard);

    luaL_openlibs(L);
    luaL_requiref(L, "cafe", luaopen_cafe, 1);

    if (luaL_dostring(L, boot_lua) != LUA_OK) {
        const char *error_buf = lua_tostring(L, -1);
        fprintf(stderr, "Failed to load Lua boot: %s\n", error_buf);
        exit(0);
    }
    
    lua_getglobal(L, "cafe");
    if (!lua_isnil(L, -1)) {
        lua_getfield(L, -1, "run");
        if (!lua_isnil(L, -1)) {
            int err = lua_pcall(L, 0, 1, 0);
            if (err) {
                const char *str = lua_tostring(L, -1);
                fprintf(stderr, "Lua error: %s\n", str);
                exit(0);
            }
            lua_rawsetp(L, LUA_REGISTRYINDEX, boot_lua);
        }
    }

    return 0;
}

int l_cafe_deinit() {
    lua_close(L);
    return 0;
}

int l_cafe_run(lua_State *L) {
    lua_rawgetp(L, LUA_REGISTRYINDEX, boot_lua);
    if (lua_pcall(L, 0, 0, 0) != 0) {
        const char *msg = lua_tostring(L, -1);
        fprintf(stderr, "cafe error: %s\n", msg);
        exit(0);
    }
    return 0;
}

int l_cafe_ver(lua_State *L) {
    lua_pushstring(L, "cafe "CAFE_VER);
    return 1;
}

int l_cafe_delta(lua_State *L) {
    lua_pushnumber(L, tea_delta());
    return 1;
}

int luaopen_cafe(lua_State *L) {
    luaL_Reg reg[] = {
        {"ver", l_cafe_ver},
        {"delta", l_cafe_delta},
        {"Font", l_cafe_font},
        {"Texture", l_cafe_texture},
        {"Rect", l_cafe_rect},
        {"File", l_cafe_file},
	{"Audio", l_cafe_audio},
        {NULL, NULL}
    };
    luaL_newlib(L, reg);

    struct { char *name; int(*fn)(lua_State *); } libs[] = {
        {"_Rect", luaopen_rect},
        {"_Font", luaopen_font},
        {"_Texture", luaopen_texture},
        {"_File", luaopen_file},
	{"_Audio", luaopen_audio},
        {"Joystick", luaopen_joystick},
        {"render", luaopen_render},
        {"keyboard", luaopen_keyboard},
        {"mouse", luaopen_mouse},
        {"window", luaopen_window},
        {"filesystem", luaopen_filesystem},
        {NULL, NULL}
    };

    for (int i = 0; libs[i].name; i++) {
        libs[i].fn(L);
        lua_setfield(L, -2, libs[i].name);
    }

    return 1;
}

int luaopen_rect(lua_State *L) {
    luaL_Reg reg[] = {
        {"__call", l_cafe_rect_set},
        {"x", l_cafe_rect_x},
        {"y", l_cafe_rect_y},
        {"w", l_cafe_rect_w},
        {"h", l_cafe_rect_h},
        {"set", l_cafe_rect_set},
        {NULL, NULL}
    };

    luaL_newmetatable(L, RECT_CLASS);
    luaL_setfuncs(L, reg, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    return 1;
}

int l_cafe_rect(lua_State *L) {
    TEA_TNUM val[4];
    int top = lua_gettop(L);
    for (int i = 0; i < top; i++) {
        val[i] = luaL_optnumber(L, i+1, 0);
    }

    te_rect_t *r = lua_newuserdata(L, sizeof(*r));
    luaL_setmetatable(L, RECT_CLASS);
    *r = TEA_RECT(val[0], val[1], val[2], val[3]);

    return 1;
}

int l_cafe_rect_x(lua_State *L) {
    te_rect_t *r = luaL_checkudata(L, 1, RECT_CLASS);
    r->x = luaL_optnumber(L, 2, r->x);
    lua_pushnumber(L, r->x);
    return 1;
}

int l_cafe_rect_y(lua_State *L) {
    te_rect_t *r = luaL_checkudata(L, 1, RECT_CLASS);
    r->y = luaL_optnumber(L, 2, r->y);
    lua_pushnumber(L, r->y);
    return 1;
}

int l_cafe_rect_w(lua_State *L) {
    te_rect_t *r = luaL_checkudata(L, 1, RECT_CLASS);
    r->w = luaL_optnumber(L, 2, r->w);
    lua_pushnumber(L, r->w);
    return 1;
}

int l_cafe_rect_h(lua_State *L) {
    te_rect_t *r = luaL_checkudata(L, 1, RECT_CLASS);
    r->h = luaL_optnumber(L, 2, r->h);
    lua_pushnumber(L, r->h);
    return 1;
}

int l_cafe_rect_set(lua_State *L) {
    te_rect_t *r = luaL_checkudata(L, 1, RECT_CLASS);

    TEA_TNUM *d = (TEA_TNUM*)r;
    TEA_TNUM val[4];
    int top = lua_gettop(L)-1;
    for (int i = 0; i < top; i++) {
        val[i] = luaL_optnumber(L, i+2, d[i]);
        lua_pushnumber(L, val[i]);
    }
    *r = TEA_RECT(val[0], val[1], val[2], val[3]);

    return 4;
}

int luaopen_file(lua_State *L) {
    luaL_Reg reg[] = {
        {"read", l_cafe_file_read},
        {"write", l_cafe_file_write},
        {"append", l_cafe_file_append},
        {"close", l_cafe_file_close},
        {"info", l_cafe_file_info},
        {"__gc", l_cafe_file_close},
        {NULL, NULL}
    };

    luaL_newmetatable(L, FILE_CLASS);
    luaL_setfuncs(L, reg, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    return 1;
}

int l_cafe_file(lua_State *L) {
    la_file_t **fp = lua_newuserdata(L, sizeof(la_file_t*));
    luaL_setmetatable(L, FILE_CLASS);
    const char *path = luaL_checkstring(L, 1);
    int mode = luaL_optnumber(L, 2, 0);
    *fp = la_fopen(path, mode);

    return 1;
}

int l_cafe_file_close(lua_State *L) {
    la_file_t **fp = luaL_checkudata(L, 1, FILE_CLASS);
    la_fclose(*fp);
    return 0;
}

int l_cafe_file_read(lua_State *L) {
    la_file_t **fp = luaL_checkudata(L, 1, FILE_CLASS);
    la_header_t h;
    la_fheader(*fp, &h);

    int bytes = luaL_optnumber(L, 1, h.size);
    
    char data[bytes];
    la_fread(*fp, data, bytes);

    lua_pushstring(L, data);

    return 1;
}

int l_cafe_file_write(lua_State *L) {
    la_file_t **fp = luaL_checkudata(L, 1, FILE_CLASS);
    const char *text = luaL_checkstring(L, 2);
    int bytes = luaL_optnumber(L, 2, strlen(text));

    la_fwrite(*fp, text, bytes);

    return 0;
}

int l_cafe_file_append(lua_State *L) {
    la_file_t **fp = luaL_checkudata(L, 1, FILE_CLASS);
    const char *text = luaL_checkstring(L, 2);
    int bytes = luaL_optnumber(L, 2, strlen(text));

    la_fappend(*fp, text, bytes);

    return 0;
}

int l_cafe_file_info(lua_State *L) {
    la_file_t **fp = luaL_checkudata(L, 1, FILE_CLASS);

    la_header_t h;
    la_fheader(*fp, &h);

    lua_newtable(L);
    lua_pushnumber(L, h.size);
    lua_setfield(L, -2, "size");
    lua_pushstring(L, h.name);
    lua_setfield(L, -2, "name");

    return 1;
}

int luaopen_audio(lua_State *L) {
    luaL_Reg reg[] = {
	{"play", l_cafe_audio_play},
	{"stop", l_cafe_audio_stop},
	{"pause", l_cafe_audio_pause},
	{"volume", l_cafe_audio_volume},
	{"__gc", l_cafe_audio__gc},
	{NULL, NULL}
    };

    luaL_newmetatable(L, AUDIO_CLASS);
    luaL_setfuncs(L, reg, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    return 1;
}

int l_cafe_audio(lua_State *L) {
    const char *path = luaL_checkstring(L, 1);
    int usage = luaL_optnumber(L, 2, 0);

    mo_audio_t **audio = lua_newuserdata(L, sizeof(mo_audio_t*));
    luaL_setmetatable(L, AUDIO_CLASS);
    
    la_file_t *fp = la_fopen(path, LA_READ_MODE);
    if (!fp) {
	lua_pushstring(L, "Failed to load audio");
	lua_error(L);
	return 0;
    }
    la_header_t h;
    la_fheader(fp, &h);

    char *data = malloc(h.size);
    la_fread(fp, data, h.size);
    free(data);
    la_fclose(fp);

    *audio = mo_audio(data, h.size, usage);

    return 1;
}

int l_cafe_audio_play(lua_State *L) {
    mo_audio_t **audio = luaL_checkudata(L, 1, AUDIO_CLASS);
    mo_play(*audio);
    return 0;
}

int l_cafe_audio_stop(lua_State *L) {
    mo_audio_t **audio = luaL_checkudata(L, 1, AUDIO_CLASS);
    mo_stop(*audio);
    return 0;
}

int l_cafe_audio_pause(lua_State *L) {
    mo_audio_t **audio = luaL_checkudata(L, 1, AUDIO_CLASS);
    mo_pause(*audio);
    return 0;
}

int l_cafe_audio_volume(lua_State *L) {
    mo_audio_t **audio = luaL_checkudata(L, 1, AUDIO_CLASS);
    float volume = luaL_optnumber(L, 2, 0);
    mo_volume(*audio, volume);
    return 0;
}

int l_cafe_audio__gc(lua_State *L) {
    mo_audio_t **audio = luaL_checkudata(L, 1, AUDIO_CLASS);
    if (*audio) mo_audio_destroy(*audio);
    *audio = NULL;

    return 0;
}

int luaopen_font(lua_State *L) {
    luaL_Reg reg[] = {
        {"__call", l_cafe_font},
        {"print", l_cafe_font_print},
        {NULL, NULL}
    };

    luaL_newmetatable(L, FONT_CLASS);
    luaL_setfuncs(L, reg, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    return 1;
}

int l_cafe_font(lua_State *L) {
    const char *str = luaL_checkstring(L, 1);
    int size = luaL_optnumber(L, 2, 16);
    te_font_t **font = lua_newuserdata(L, sizeof(*font)); 
    luaL_setmetatable(L, FONT_CLASS);
    la_file_t *fp = la_fopen(str, LA_READ_MODE);
    la_header_t h;
    la_fheader(fp, &h);

    char data[h.size];
    la_fread(fp, data, h.size); 

    *font = tea_font(data, h.size, size);
    return 1;
}

int l_cafe_font_print(lua_State *L) {
    te_font_t **font = luaL_checkudata(L, 1, FONT_CLASS);
    const char *str = luaL_checkstring(L, 2);
    TEA_TNUM x, y;
    x = luaL_optnumber(L, 3, 0);
    y = luaL_optnumber(L, 4, 0);

    tea_font_print(*font, str, x, y);

    return 0;
}

int luaopen_texture(lua_State *L) {
    luaL_Reg reg[] = {
        {"__call", l_cafe_texture},
	{"info", l_cafe_texture_info},
	{"width", l_cafe_texture_width},
	{"height", l_cafe_texture_height},
	{"size", l_cafe_texture_size},
        {"draw", l_cafe_texture_draw},
        {NULL, NULL}
    };

    luaL_newmetatable(L, TEXTURE_CLASS);
    luaL_setfuncs(L, reg, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    return 1;
}

int _texture_from_path(lua_State *L) {
    const char *filepath = luaL_checkstring(L, 1);
    int usage = luaL_optnumber(L, 2, 1)-1;

    la_file_t *f = la_fopen(filepath, LA_READ_MODE);
    la_header_t h;
    la_fheader(f, &h);

    char data[h.size];
    la_fread(f, data, h.size);
    la_fclose(f);

    te_texture_t **tex = lua_newuserdata(L, sizeof(*tex));
    luaL_setmetatable(L, TEXTURE_CLASS);
    *tex = tea_texture_from_memory(data, h.size, usage);

    return 1;
}

int _texture_from_size(lua_State *L) {
    int w, h;
    w = luaL_checknumber(L, 1);
    h = luaL_checknumber(L, 2);
    int usage = luaL_optnumber(L, 3, 1)-1;

    te_texture_t **tex = lua_newuserdata(L, sizeof(*tex));
    luaL_setmetatable(L, TEXTURE_CLASS);
    *tex = tea_texture(NULL, w, h, TEA_RGBA, usage);
    return 1;
}

int l_cafe_texture(lua_State *L) {
    switch (lua_type(L, 1)) {
        case LUA_TSTRING: return _texture_from_path(L);
        case LUA_TNUMBER: return _texture_from_size(L);
    }
    return 1;
}

int l_cafe_texture_info(lua_State *L) {
    te_texture_t **tex = luaL_checkudata(L, 1, TEXTURE_CLASS);
    te_texinfo_t info;
    tea_texture_info(*tex, &info);

    lua_newtable(L);
    lua_newtable(L);
    lua_pushnumber(L, info.size.w);
    lua_setfield(L, -2, "x");
    lua_pushnumber(L, info.size.h);
    lua_setfield(L, -2, "y");

    lua_setfield(L, -2, "size");
    lua_pushnumber(L, info.usage);
    lua_setfield(L, -2, "usage");
    
    return 1;
}

int l_cafe_texture_width(lua_State *L) {
    te_texture_t **tex = luaL_checkudata(L, 1, TEXTURE_CLASS);
    te_texinfo_t info;
    tea_texture_info(*tex, &info);
    lua_pushnumber(L, info.size.w);
    return 1;
}

int l_cafe_texture_height(lua_State *L) {
    te_texture_t **tex = luaL_checkudata(L, 1, TEXTURE_CLASS);
    te_texinfo_t info;
    tea_texture_info(*tex, &info);
    lua_pushnumber(L, info.size.h);
    return 1;
}

int l_cafe_texture_size(lua_State *L) {
    te_texture_t **tex = luaL_checkudata(L, 1, TEXTURE_CLASS);
    te_texinfo_t info;
    tea_texture_info(*tex, &info);
    lua_pushnumber(L, info.size.w);
    lua_pushnumber(L, info.size.h);
    return 2;
}

int l_cafe_texture_draw(lua_State *L) {
    int arg = 0;
    te_texture_t **tex = luaL_checkudata(L, ++arg, TEXTURE_CLASS);
    te_texinfo_t info;
    tea_texture_info(*tex, &info);
    te_rect_t *part = luaL_testudata(L, ++arg, RECT_CLASS);
    te_rect_t d, s;
    s = TEA_RECT(0, 0, info.size.w, info.size.h);

    if (part) memcpy(&s, part, sizeof(*part));
    else arg--;

    d.x = luaL_optnumber(L, ++arg, 0);
    d.y = luaL_optnumber(L, ++arg, 0);
    TEA_TNUM angle = luaL_optnumber(L, ++arg, 0);
    TEA_TNUM sx, sy;
    sx = luaL_optnumber(L, ++arg, 1);
    sy = luaL_optnumber(L, ++arg, 1);
    d.w = sx*s.w;
    d.h = sy*s.h;

    tea_texture_draw(*tex, &d, &s);
//    tea_texture_draw_ex(*tex, &d, &s, angle, NULL, 0);


    return 0;
}

int luaopen_filesystem(lua_State *L) {
    luaL_Reg reg[] = {
        {"fopen", l_cafe_file},
        {"path", l_cafe_filesystem_path},
        {NULL, NULL}
    };

    luaL_newlib(L, reg);

    return 1;
}

int l_cafe_filesystem_path(lua_State *L) {
    char out[100];
    la_get_basedir(out);
    lua_pushstring(L, out);
    return 1;
}

int luaopen_render(lua_State *L) {
    luaL_Reg reg[] = {
        {"clear", l_cafe_render_clear},
        {"color", l_cafe_render_color},
        {"mode", l_cafe_render_mode},
        {"point", l_cafe_render_point},
        {"line", l_cafe_render_line},
        {"rectangle", l_cafe_render_rectangle},
        {"circle", l_cafe_render_circle},
        {"triangle", l_cafe_render_triangle},
        {"text", l_cafe_render_text},
        {"target", l_cafe_render_target},
        {NULL, NULL}
    };

    luaL_newlib(L, reg);

    return 1;
}

int l_cafe_render_clear(lua_State *L) {
    unsigned char col[4] = {0, 0, 0, 255};
    int top = lua_gettop(L);
    for (int i = 0; i < top; i++) {
       col[i] = luaL_checknumber(L, i+1); 
    }

    tea_clear(TEA_COLOR(col[0], col[1], col[2], col[3]));
    return 0;
}

int l_cafe_render_mode(lua_State *L) {
    TEA_TNUM mode;
    mode = luaL_optnumber(L, 1, 0);

    tea_mode(mode);
    lua_pushnumber(L, mode);

    return 1;
}

int l_cafe_render_color(lua_State *L) {
    unsigned char col[4] = {0, 0, 0, 255};
    int top = lua_gettop(L);
    for (int i = 0; i < top; i++) {
       col[i] = luaL_checknumber(L, i+1); 
    }

    tea_color(TEA_COLOR(col[0], col[1], col[2], col[3]));
    return 0;
}

int l_cafe_render_target(lua_State *L) {
    te_texture_t **tex = luaL_testudata(L, 1, TEXTURE_CLASS);
    tea_set_target(tex ? *tex : NULL);
    return 0;
}

int l_cafe_render_point(lua_State *L) {
    TEA_TNUM x, y; 
    x = luaL_optnumber(L, 1, 0);
    y = luaL_optnumber(L, 2, 0);

    tea_point(x, y);

    return 0;
}

int l_cafe_render_line(lua_State *L) {
    TEA_TNUM x0, y0;
    TEA_TNUM x1, y1;

    x0 = luaL_optnumber(L, 1, 0);
    y0 = luaL_optnumber(L, 2, 0);

    x1 = luaL_optnumber(L, 3, 0);
    y1 = luaL_optnumber(L, 4, 0);

    tea_line(x0, y0, x1, y1);

    return 0;
}

int l_cafe_render_rectangle(lua_State *L) {
    TEA_TNUM x, y;
    TEA_TNUM w, h;

    x = luaL_optnumber(L, 1, 0);
    y = luaL_optnumber(L, 2, 0);

    w = luaL_optnumber(L, 3, 0);
    h = luaL_optnumber(L, 4, 0);

    tea_rect(x, y, w, h);

    return 0;
}

int l_cafe_render_circle(lua_State *L) {
    TEA_TNUM x, y, radius;
    x = luaL_optnumber(L, 1, 0);
    y = luaL_optnumber(L, 2, 0);
    radius = luaL_optnumber(L, 3, 0);

    tea_circle(x, y, radius);

    return 0;
}

int l_cafe_render_triangle(lua_State *L) {
    TEA_TNUM pos[6];
    int top = lua_gettop(L);

    for (int i = 0; i < top; i++) {
        pos[i] = luaL_optnumber(L, i+1, 0);
    }

    tea_triangle(pos[0], pos[1], pos[2], pos[3], pos[4], pos[5]);

    return 0;
}

int l_cafe_render_text(lua_State *L) {
    const char *str = luaL_checkstring(L, 1);
    TEA_TNUM x, y;
    x = luaL_optnumber(L, 2, 0);
    y = luaL_optnumber(L, 3, 0);

    tea_print(str, x, y);

    return 0;
}

int luaopen_window(lua_State *L) {
    luaL_Reg reg[] = {
        {"title", l_cafe_window_title},
        {"pos", l_cafe_window_pos},
        {"size", l_cafe_window_size},
        {"minimize", l_cafe_window_minimize},
        {"maximize", l_cafe_window_maximize},
        {"restore", l_cafe_window_restore},
        {"fullscreen", l_cafe_window_fullscreen},
        {NULL, NULL}
    };

    luaL_newlib(L, reg);

    return 1;
}

int l_cafe_window_title(lua_State *L) {
    const char *title = luaL_checkstring(L, 1);
    tea_window_title(title, NULL);
    return 0;
}

int l_cafe_window_pos(lua_State *L) {
    te_point_t pos;
    tea_window_pos(&pos, 0, 0);
    lua_pushnumber(L, pos.x);
    lua_pushnumber(L, pos.y);
    return 2;
}

int l_cafe_window_size(lua_State *L) {
    te_point_t size;
    tea_window_size(&size, 0, 0);
    lua_pushnumber(L, size.x);
    lua_pushnumber(L, size.y);
    return 2;
}

int l_cafe_window_minimize(lua_State *L) {
    tea_window_minimize();
    return 0;
}

int l_cafe_window_maximize(lua_State *L) {
    tea_window_maximize();
    return 0;
}

int l_cafe_window_restore(lua_State *L) {
    tea_window_restore();
    return 0;
}

int l_cafe_window_fullscreen(lua_State *L) {
    return 0;
}

int luaopen_keyboard(lua_State *L) {
    luaL_Reg reg[] = {
        {"down", l_cafe_keyboard_down},
        {"up", l_cafe_keyboard_up},
        {"pressed", l_cafe_keyboard_pressed},
        {"released", l_cafe_keyboard_released},
        {NULL, NULL}
    };

    luaL_newlib(L, reg);

    return 1;
}

int l_cafe_keyboard_down(lua_State *L) {
    const char *name = luaL_checkstring(L, 1);

    int key = tea_key_from_name(name);

    int b = tea_key_down(key);
    lua_pushboolean(L, b);
    
    return 1;
}

int l_cafe_keyboard_up(lua_State *L) {
    const char *name = luaL_checkstring(L, 1);

    int key = tea_key_from_name(name);

    int b = tea_key_up(key);
    lua_pushboolean(L, b);
    
    return 1;
}

int l_cafe_keyboard_pressed(lua_State *L) {
    const char *name = luaL_checkstring(L, 1);

    int key = tea_key_from_name(name);

    int b = tea_key_pressed(key);
    lua_pushboolean(L, b);
    
    return 1;
}

int luaopen_mouse(lua_State *L) {
    luaL_Reg reg[] = {
        {"down", l_cafe_mouse_down},
        {"up", l_cafe_mouse_up},
        {"pressed", l_cafe_mouse_pressed},
        {"released", l_cafe_mouse_released},
        {NULL, NULL}
    };

    luaL_newlib(L, reg);

    return 1;
}

int l_cafe_mouse_down(lua_State *L) {
    int btn = luaL_checknumber(L, 1) - 1;
    lua_pushboolean(L, tea_mouse_down(btn));
    return 1;
}

int l_cafe_mouse_up(lua_State *L) {
    int btn = luaL_checknumber(L, 1) - 1;
    lua_pushboolean(L, tea_mouse_up(btn));
    return 1;
}

int l_cafe_mouse_pressed(lua_State *L) {
    int btn = luaL_checknumber(L, 1) - 1;
    lua_pushboolean(L, tea_mouse_pressed(btn));
    return 1;
}

int l_cafe_mouse_released(lua_State *L) {
    int btn = luaL_checknumber(L, 1) - 1;
    lua_pushboolean(L, tea_mouse_released(btn));
    return 1;
}

int l_cafe_keyboard_released(lua_State *L) {
    const char *name = luaL_checkstring(L, 1);

    int key = tea_key_from_name(name);

    int b = tea_key_released(key);
    lua_pushboolean(L, b);
    
    return 1;
}

int luaopen_joystick(lua_State *L) {
    luaL_Reg reg[] = {
        {"__call", l_cafe_joystick},
        {"count", l_cafe_joystick_count},
        {"vibration", l_cafe_joystick_vibration},
        {"name", l_cafe_joystick_name},
        {"button_count", l_cafe_joystick_button_count},
        {"axis_count", l_cafe_joystick_axis_count},
        {"hat_count", l_cafe_joystick_hat_count},
        {"ball_count", l_cafe_joystick_ball_count},
        {"button", l_cafe_joystick_button},
        {"axis", l_cafe_joystick_axis},
        {"axes", l_cafe_joystick_axes},
        {"hat", l_cafe_joystick_hat},
        {"ball", l_cafe_joystick_ball},
        {NULL, NULL}
    };

    lua_newtable(L);
    luaL_newmetatable(L, JOYSTICK_CLASS);
    luaL_setfuncs(L, reg, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    lua_setfield(L, -2, "__mt");
    luaL_setmetatable(L, JOYSTICK_CLASS);

    return 1;
}

int l_cafe_joystick(lua_State *L) {
    int id = luaL_checknumber(L, 2);
    te_joystick_t **joy = lua_newuserdata(L, sizeof(*joy));
    luaL_setmetatable(L, JOYSTICK_CLASS);
    *joy = tea_joystick(id);
    return 1;
}

int l_cafe_joystick_count(lua_State *L) {
    lua_pushnumber(L, tea_joystick_count());
    return 1;
}

int l_cafe_joystick_vibration(lua_State *L) {
    te_joystick_t **joy = luaL_checkudata(L, 1, JOYSTICK_CLASS);
    int lfreq = luaL_optnumber(L, 2, 0);
    int hfreq = luaL_optnumber(L, 3, 0);
    int ms = luaL_optnumber(L, 4, 0);
    tea_joystick_vibration(*joy, lfreq, hfreq, ms);
    return 1;
}

int l_cafe_joystick_name(lua_State *L) {
    te_joystick_t **joy = luaL_checkudata(L, 1, JOYSTICK_CLASS);
    lua_pushstring(L, tea_joystick_name(*joy));
    return 1;
}

int l_cafe_joystick_button_count(lua_State *L) {
    te_joystick_t **joy = luaL_checkudata(L, 1, JOYSTICK_CLASS);
    lua_pushnumber(L, tea_joystick_button_count(*joy));
    return 1;
}

int l_cafe_joystick_axis_count(lua_State *L) {
    te_joystick_t **joy = luaL_checkudata(L, 1, JOYSTICK_CLASS); 
    lua_pushnumber(L, tea_joystick_axis_count(*joy));
    return 1;
}

int l_cafe_joystick_hat_count(lua_State *L) {
    te_joystick_t **joy = luaL_checkudata(L, 1, JOYSTICK_CLASS);
    lua_pushnumber(L, tea_joystick_hat_count(*joy));
    return 1;
}

int l_cafe_joystick_ball_count(lua_State *L) {
    te_joystick_t **joy = luaL_checkudata(L, 1, JOYSTICK_CLASS);
    lua_pushnumber(L, tea_joystick_ball_count(*joy));
    return 1;
}

int l_cafe_joystick_button(lua_State *L) {
    te_joystick_t **joy = luaL_checkudata(L, 1, JOYSTICK_CLASS);
    int button = luaL_checknumber(L, 2);
    lua_pushboolean(L, tea_joystick_button(*joy, button));
    return 1;
}

int l_cafe_joystick_axis(lua_State *L) {
    te_joystick_t **joy = luaL_checkudata(L, 1, JOYSTICK_CLASS);
    int axis = luaL_checknumber(L, 2);
    lua_pushnumber(L, tea_joystick_axis(*joy, axis));
    return 1;
}

int l_cafe_joystick_axes(lua_State *L) {
    te_joystick_t **joy = luaL_checkudata(L, 1, JOYSTICK_CLASS);
    int n = tea_joystick_axis_count(*joy);
    for (int i = 0; i < n; i++) lua_pushnumber(L, tea_joystick_axis(*joy, i));
    return n;
}

const char *hats[] = {
    [SDL_HAT_CENTERED] = "c\0",
    [SDL_HAT_DOWN] = "d\0",
    [SDL_HAT_LEFT] = "l\0",
    [SDL_HAT_LEFTDOWN] = "ld\0",
    [SDL_HAT_LEFTUP] = "lu\0",
    [SDL_HAT_RIGHT] = "r\0",
    [SDL_HAT_RIGHTDOWN] = "rd\0",
    [SDL_HAT_RIGHTUP] = "ru\0",
    [SDL_HAT_UP] = "u\0"
};

int l_cafe_joystick_hat(lua_State *L) {
    te_joystick_t **joy = luaL_checkudata(L, 1, JOYSTICK_CLASS);
    int btn = luaL_checknumber(L, 2);
    int hat = tea_joystick_hat(*joy, btn);
    lua_pushstring(L, hats[hat]);

    return 1;
}

int l_cafe_joystick_ball(lua_State *L) {
    te_joystick_t **joy = luaL_checkudata(L, 1, JOYSTICK_CLASS);
    int ball = luaL_checknumber(L, 2);

    int dx, dy;
    tea_joystick_ball(*joy, ball, &dx, &dy);
    lua_pushnumber(L, dx);
    lua_pushnumber(L, dy);
    return 2;
}

int l_cafe_callback_keyboard(int window_id, int down, int repeat, struct te_keysym_t sym) {
    lua_getglobal(L, "cafe");
    if (!lua_isnil(L, -1)) {
	const char* fn_str[2] = { "keyreleased", "keypressed"};
	lua_getfield(L, -1, fn_str[down]);
	if (!lua_isnil(L, -1)) {
	    lua_pushnumber(L, window_id);
	    lua_pushstring(L, tea_key_name(sym.scancode));
	    lua_pushboolean(L, repeat);
	    int err = lua_pcall(L, 3, 0, 0);
	    if (err) {
		fprintf(stderr, "cafe lua error: %s\n", lua_tostring(L, -1)); 
		exit(0);
	    }
	}
    }
    return 0;
}

int l_cafe_callback_window_move(int id, int x, int y) {
    lua_getglobal(L, "cafe");
    if (!lua_isnil(L, -1)) {
        lua_getfield(L, -1, "window_move");
        if (!lua_isnil(L, -1)) {
            lua_pushnumber(L, id);
            lua_pushnumber(L, x);
            lua_pushnumber(L, y);
            int err = lua_pcall(L, 3, 0, 0);
            if (err) {
                fprintf(stderr, "cafe lua error: %s\n", lua_tostring(L, -1));
                exit(0);
            }
        }
    }
    return 0;
}

int l_cafe_callback_window_resize(int id, int w, int h) {
    lua_getglobal(L, "cafe");
    if (!lua_isnil(L, -1)) {
        lua_getfield(L, -1, "window_resize");
        if (!lua_isnil(L, -1)) {
            lua_pushnumber(L, id);
            lua_pushnumber(L, w);
            lua_pushnumber(L, h);
            int err = lua_pcall(L, 3, 0, 0);
            if (err) {
                fprintf(stderr, "cafe lua error: %s\n", lua_tostring(L, -1));
                exit(0);
            }
        }
    }
    return 0;
}

int l_cafe_callback_window_visible(int id, int visible) {
    lua_getglobal(L, "cafe");
    if (!lua_isnil(L, -1)) {
        lua_getfield(L, -1, "window_visible");
        if (!lua_isnil(L, -1)) {
            lua_pushnumber(L, id);
            lua_pushboolean(L, visible);
            int err = lua_pcall(L, 2, 0, 0);
            if (err) {
                fprintf(stderr, "cafe lua error: %s\n", lua_tostring(L, -1));
                exit(0);
            }
        }
    }
    return 0;
}

int l_cafe_callback_window_focus(int id, int focused) {
    lua_getglobal(L, "cafe");
    if (!lua_isnil(L, -1)) {
        lua_getfield(L, -1, "window_focus");
        if (!lua_isnil(L, -1)) {
            lua_pushnumber(L, id);
            lua_pushboolean(L, focused);
            int err = lua_pcall(L, 2, 0, 0);
            if (err) {
                fprintf(stderr, "cafe lua error: %s\n", lua_tostring(L, -1));
                exit(0);
            }
        }
    }
    return 0;
}

int l_cafe_callback_window_mouse(int id, int enter) {
    lua_getglobal(L, "cafe");
    if (!lua_isnil(L, -1)) {
        lua_getfield(L, -1, "window_mouse");
        if (!lua_isnil(L, -1)) {
            lua_pushnumber(L, id);
            lua_pushboolean(L, enter);
            int err = lua_pcall(L, 2, 0, 0);
            if (err) {
                fprintf(stderr, "cafe lua error: %s\n", lua_tostring(L, -1));
                exit(0);
            }
        }
    }
    return 0;
}

int l_cafe_callback_window_close(int id) {
    lua_getglobal(L, "cafe");
    if (!lua_isnil(L, -1)) {
        lua_getfield(L, -1, "window_close");
        if (!lua_isnil(L, -1)) {
            lua_pushnumber(L, id);
            int err = lua_pcall(L, 1, 0, 0);
            if (err) {
                fprintf(stderr, "cafe lua error: %s\n", lua_tostring(L, -1));
                exit(0);
            }
        }
    }
    return 0;
}

#if 0
int l_cafe_callback_window_resize(lua_State *L);
int l_cafe_callback_window_visible(lua_State *L);
int l_cafe_callback_window_focus(lua_State *L);
int l_cafe_callback_window_mouse(lua_State *L);
int l_cafe_callback_window_close(lua_State *L);
#endif

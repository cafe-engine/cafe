#include "cafe_lua.h"
#include "lua.h"

static int lua_cafe_fs_size(lua_State *L) {
    const char *filename = luaL_checkstring(L, 1);

    long size = cafe_filesystem_size(filename);

    lua_pushnumber(L, size);
    return 1;
}

static int lua_cafe_fs_read(lua_State *L) {
    return 0;
}

static int lua_cafe_fs_mkdir(lua_State *L) {
    const char *path = luaL_checkstring(L, 1);

    int res = cafe_filesystem_mkdir(path);

    lua_pushboolean(L, res);
    return 1;
}

static int lua_cafe_fs_rmdir(lua_State *L) {
    const char *path = luaL_checkstring(L, 1);
    
    int res = cafe_filesystem_rmdir(path);

    lua_pushboolean(L, res);
    return 1;
}

static int lua_cafe_fs_touch(lua_State *L) {
    const char *filename = luaL_checkstring(L, 1);

    int res = cafe_filesystem_touch(filename);
    lua_pushboolean(L, res);
    return 1;
}

static int lua_cafe_fs_rm(lua_State *L) {
    const char *filename = luaL_checkstring(L, 1);

    int res = cafe_filesystem_rm(filename);
    lua_pushboolean(L, res);

    return 1;
}

static int lua_cafe_fs_print(lua_State *L) {
    const char *filename = luaL_checkstring(L, 1);

    cf_File *fp = cafe_file_open(filename, 0);

    cf_Header h;
    cafe_file_header(fp, &h);

    char buf[h.size+1];

    cafe_file_read(fp, buf, h.size);
    buf[h.size] = '\0';


    cafe_file_close(fp);

    printf("%s\n", buf);
    return 0;
}

int luaopen_filesystem(lua_State *L) {
    luaL_Reg reg[] = {
        /*{"size", lua_cafe_fs_size},
        {"read", lua_cafe_fs_read},
        {"write", lua_cafe_fs_write},*/
        {"mkdir", lua_cafe_fs_mkdir},
        {"rmdir", lua_cafe_fs_rmdir},
        {"touch", lua_cafe_fs_touch},
        {"rm", lua_cafe_fs_rm},
        {"print", lua_cafe_fs_print},
        {NULL, NULL}
    };

    luaL_newlib(L, reg);

    return 1;
}

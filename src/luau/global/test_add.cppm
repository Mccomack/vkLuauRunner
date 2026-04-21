module;
#include <lua.h>
#include <lualib.h>

export module luau:global.testadd;

export namespace global {
    int laddTest(lua_State* L) {
        double v1 = luaL_checknumber(L, 1);
        double v2 = luaL_checknumber(L, 2);

        lua_pushnumber(L, v1 + v2);

        return 1;
    }
}
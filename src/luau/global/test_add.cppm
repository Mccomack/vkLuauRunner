module;
#include <lua.h>
#include <lualib.h>

export module luau:global.testadd;
import :common;

export namespace global {
    int laddTest(lua_State* L) {
        double v1 = luaL_checknumber(L, 1);
        double v2 = luaL_checknumber(L, 2);

        lua_pushnumber(L, v1 + v2);

        return 1;
    }
}

namespace {
    inline int _ = []()->int {
        luau::getDefaultFunctions().push_back(luau::funcRegInfo{
            .name = "add",
            .reqLevel = luau::SecurityType::eNone,
            .func = global::laddTest
        });

        return 0;
    }();
}
module;
#include <lua.h>
#include <lualib.h>

export module luau:global.print;
import logger;
import std;

namespace {
    Logger logger("luau/global/print");
}

export namespace global {
    int lprint(lua_State* L) {
        int n = lua_gettop(L);

        std::string tmp = "";

        for (int i = 1; i <= n; i++) {
            tmp += luaL_tolstring(L, i, nullptr);

            if (i != n) tmp += " ";

            lua_pop(L, 1);
        }

        logger.Log(tmp);

        return 0;
    }
}
module;
#include <lua.h>
#include <lualib.h>

export module luau:global.print;
import :common;
import logger;
import std;

namespace {
    Logger logger("luau/global/print");
}

export namespace luau::global {
    int lprint(lua_State* L) {
        int n = lua_gettop(L);

        std::string tmp = "";

        for (int i = 1; i <= n; i++) {
            tmp += luaL_tolstring(L, i, nullptr);

            if (i != n) tmp += "\t";

            lua_pop(L, 1);
        }

        logger.Log(tmp);

        return 0;
    }
}

namespace {
    inline static int _ = []() -> int {
        luau::getDefaultFunctions().push_back(luau::funcRegInfo{
            .name = "print",
            .reqLevel = luau::SecurityType::eNone,
            .func = luau::global::lprint
        });

        return 0;
    }();
}
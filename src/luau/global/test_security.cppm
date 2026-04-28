module;
#include <lua.h>
#include <lualib.h>

export module luau:global.testsecurity;
import :common;

export namespace global {
    int lsecurity(lua_State* L) {
        lua_pushstring(L, "very secret");

        return 1;
    }
}

namespace {
    inline int _ = []() -> int {
        luau::getDefaultFunctions().push_back(luau::funcRegInfo{
            .name = "security",
            .reqLevel = luau::SecurityType::eDefault,
            .func = global::lsecurity
        });

        return 0;
    }();
}
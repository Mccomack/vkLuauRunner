module;
#include <lua.h>
#include <lualib.h>

export module luau:global.testsecurity;
import :common;

export namespace luau::global {
    int lsecurity(lua_State* L) {
        lua_pushstring(L, "very secret");

        return 1;
    }
}

namespace {
    inline int _ = []() -> int {
        // clang-format off
        luau::getDefaultFunctions().push_back(luau::funcRegInfo{
            .name = "security",
            .reqLevel = luau::SecurityType::eDefault,
            .func = luau::global::lsecurity
        });
        // clang-format on

        return 0;
    }();
}
module;
#include <lua.h>

export module luau:library.test.somemethod;
import :common;

import std;

namespace _lib__test {
    int test_lsomemethod(lua_State* L) {
        lua_pushstring(L, "🤑");

        return 1;
    }

    luau::funcRegInfo somemethod{
        .name = "somemethod",
        .reqLevel = luau::SecurityType::eNone,
        .func = test_lsomemethod
    };
}
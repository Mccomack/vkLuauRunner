module;
#include <lua.h>
#include <cstddef>
#include "lualib.h"

export module luau:library.test.luau;
import :library.test;
import :common;

import std;

namespace luau::library {
    int test_lsomemethod(lua_State* L) {
        lua_pushstring(L, "🤑");

        return 1;
    }

    int test_ltoMoneyFace(lua_State* L) {
        int n = lua_gettop(L);
        if (n < 1) {
            luaL_error(L, "expected string, got no arguments");
        }

        luau::library::test& test = luau::library::test::getInstance();

        std::string str;
        const char* _str = lua_tolstring(L, 1, nullptr);

        if (_str == nullptr)
            str = "nil";
        else
            str = _str;

        lua_pushstring(L, test.toMoneyFace(str).c_str());

        return 1;
    }

    luau::libRegInfo test{
        .name = "test",

        .methods = {
            luau::funcRegInfo{
                .name = "somemethod",
                .reqLevel = luau::SecurityType::eNone,
                .func = test_lsomemethod
            },
            luau::funcRegInfo{
                .name = "toMoneyFace",
                .reqLevel = luau::SecurityType::eNone,
                .func = test_ltoMoneyFace
            }
        }
    };
}

namespace {
    inline static int _ = []() -> int {
        luau::getDefaultLibraries().push_back(&luau::library::test);

        return 0;
    }();
}
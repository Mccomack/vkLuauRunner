module;
#include <lua.h>

export module luau:library.test;
import :library.test.somemethod;
import :common;

import std;

namespace luau::library {
    luau::libRegInfo test{
        .name = "test",

        .methods = {
            _lib__test::somemethod
        }
    };
}

namespace {
    inline static int _ = []() -> int {
        luau::getDefaultLibraries().push_back(luau::library::test);
        
        return 0;
    }();
}
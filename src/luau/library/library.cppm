module;
#include <lua.h>

export module luau:library;
export import :library.test;
export import :library.test.luau;
import :common;

import std;

export namespace luau::library {
    constexpr std::array kDefaultLibraries{
        &luau::library::kTestLibrary
    };

    void registerDefaultLibraries(lua_State* L);
}

void luau::library::registerDefaultLibraries(lua_State* L) {
    for (const luau::libRegInfo* libInfo : kDefaultLibraries)
        luau::registerLibrary(L, *libInfo);
}

module;
#include <lua.h>

export module luau:library;
export import :library.test;
import :common;

export namespace luau::library {
    void registerDefaultLibraries(lua_State* L);
}

void luau::library::registerDefaultLibraries(lua_State* L) {
    for (luau::libRegInfo* libInfo : luau::getDefaultLibraries()) {
        luau::registerLibrary(L, *libInfo);
    }
}
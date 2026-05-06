module;
#include <lua.h>

export module luau:library;
import :common;

export namespace luau::library {
    void registerDefaultLibraries(lua_State* L);
}

void luau::library::registerDefaultLibraries(lua_State *L) {
    for (const luau::libRegInfo& libInfo : luau::getDefaultLibraries()) {
        luau::registerLibrary(L, libInfo);
    }
}
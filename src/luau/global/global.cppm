module;
#include <lua.h>

export module luau:global;
export import :global.testadd;
export import :global.print;
export import :global.testsecurity;
export import :common;

export namespace luau::global {
    void registerDefaultFunctions(lua_State* L);
}

void luau::global::registerDefaultFunctions(lua_State* L) {
    for (luau::funcRegInfo funcRegInfo : luau::getDefaultFunctions()) {
        luau::registerFunction(L, funcRegInfo.name, funcRegInfo.func, funcRegInfo.reqLevel);
    }
}
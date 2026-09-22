module;
#include <lua.h>

export module luau:global;
export import :global.testadd;
export import :global.print;
export import :global.testsecurity;
export import :global.printSecurityLevel;
import :common;

export namespace luau::global {
    constexpr luau::funcRegInfo kDefaultFunctions[] = {
        {.name = "print",
        .reqLevel = luau::SecurityType::eNone,
        .func = luau::global::lprint},
        {.name = "printSecurityLevel",
        .reqLevel = luau::SecurityType::eNone,
        .func = luau::global::lprintSecurityLevel},
        {.name = "add",
        .reqLevel = luau::SecurityType::eNone,
        .func = luau::global::laddTest},
        {.name = "security",
        .reqLevel = luau::SecurityType::eDefault,
        .func = luau::global::lsecurity}
    };

    constexpr void registerDefaultFunctions(lua_State* L);
}

constexpr void luau::global::registerDefaultFunctions(lua_State* L) {
    for (luau::funcRegInfo funcRegInfo : kDefaultFunctions)
        luau::registerFunction(L, funcRegInfo.name, funcRegInfo.func, funcRegInfo.reqLevel);
}

module;
#include <lua.h>

export module luau:global;
export import :global.testadd;
export import :global.print;
export import :global.testsecurity;

export namespace global {
    int laddTest(lua_State* L);
}
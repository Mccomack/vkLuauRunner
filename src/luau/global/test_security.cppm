module;
#include <lua.h>
#include <lualib.h>

export module luau:global.testsecurity;

export namespace global {
    int lsecurity(lua_State* L) {
        lua_pushstring(L, "very secret");

        return 1;
    }
}
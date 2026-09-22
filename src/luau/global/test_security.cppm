module;
#include <lua.h>
#include <lualib.h>

export module luau:global.testsecurity;
import :common;

export namespace luau::global {
    int lsecurity(lua_State* L) {
        lua_pushstring(L, "very secret");

        return 1;
    }
}

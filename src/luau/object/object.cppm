module;
#include <lua.h>

export module luau:object;
// export import :object.base;
export import :common;

namespace luau::object {
    void registerDefaultObjects(lua_State* L);
}

void registerDefaultObjects(lua_State* L) {

}
module;
#include <lua.h>

export module luau:object;
export import :object.base;
import :common;

export namespace luau::object {
    void registerDefaultObjects(lua_State* L);
}

void luau::object::registerDefaultObjects(lua_State* L) {
    for (luau::objRegInfo* objInfo : luau::getDefaultObjects()) {
        luau::registerObject(L, *objInfo);
    }
}
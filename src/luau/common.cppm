module;
#include <lua.h>
#include <lualib.h>
#include <vector>

#define SECURITY_TYPE_LIST \
    X(eNone) \
    X(eUntrusted) \
    X(eDefault) \
    X(ePlugin) \
    X(eEngineScript) \

export module luau:common;

import std;

namespace luau {
    #define X(name) name,
    export enum SecurityType { SECURITY_TYPE_LIST };
    #undef X

    struct environmentContext {
        SecurityType secType = SecurityType::eNone;
    };

    struct funcRegInfo {
        std::string name;
        SecurityType reqLevel = SecurityType::eDefault;
        lua_CFunction func;
    };

    std::vector<funcRegInfo>& getDefaultFunctions();

    int securityWrapper(lua_State* L);
    void registerFunction(lua_State* L, const std::string& name, lua_CFunction func, SecurityType requiredLevel);
}

std::vector<luau::funcRegInfo>& luau::getDefaultFunctions() {
    static std::vector<funcRegInfo> defaultFunctions;
    return defaultFunctions;
};

int luau::securityWrapper(lua_State* L) {
    int requiredLevel = lua_tointeger(L, lua_upvalueindex(1));

    auto* envData = static_cast<struct environmentContext*>(lua_getthreaddata(L));
    int currentLevel = envData ? static_cast<int>(envData->secType) : 0;

    if (currentLevel < requiredLevel)
        luaL_error(L, "Cannot access. required level: %d, current level: %d. ", requiredLevel, currentLevel);

    lua_CFunction func = lua_tocfunction(L, lua_upvalueindex(2));
    return func(L);
}

void luau::registerFunction(lua_State* L, const std::string& name, lua_CFunction func, luau::SecurityType requiredLevel) {
    lua_pushinteger(L, static_cast<int>(requiredLevel));
    lua_pushcfunction(L, func, name.c_str());
    lua_pushcclosure(L, &luau::securityWrapper, name.c_str(), 2);
    lua_setglobal(L, name.c_str());
}
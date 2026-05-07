module;
#include <lua.h>
#include <lualib.h>

#define SECURITY_TYPE_LIST \
    X(eNone) \
    X(eUntrusted) \
    X(eDefault) \
    X(ePlugin) \
    X(eEngineScript) \

export module luau:common;
import logger;

import std;

namespace {
    Logger logger("luau");
}

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

    template <typename T>
    struct propRegInfo {
        std::string name;
        SecurityType readLevel = SecurityType::eDefault;
        SecurityType writeLevel = SecurityType::eDefault;
        std::optional<T> defaultValue = std::nullopt;
    };

    export using anyProp = std::variant<
        propRegInfo<std::string>, 
        propRegInfo<short>, 
        propRegInfo<int>, 
        propRegInfo<long long>, 
        propRegInfo<float>, 
        propRegInfo<double>, 
        propRegInfo<bool>
    >;

    struct libRegInfo {
        std::string name;

        std::vector<funcRegInfo> methods;
    };

    struct objRegInfo {
        std::string name;

        std::vector<funcRegInfo> methods;
        std::vector<anyProp> properties;
    };

    std::vector<funcRegInfo>& getDefaultFunctions();
    std::vector<libRegInfo*>&  getDefaultLibraries();
    std::vector<objRegInfo>&  getDefaultObjects();

    int securityWrapper(lua_State* L);
    void registerFunction(lua_State* L, const std::string& name, lua_CFunction func, SecurityType requiredLevel);
    void registerLibrary(lua_State* L, const libRegInfo& regInfo);
    void registerObject(lua_State* L, const objRegInfo& objInfo);
}

std::vector<luau::funcRegInfo>& luau::getDefaultFunctions() {
    static std::vector<funcRegInfo> defaultFunctions;
    return defaultFunctions;
};

std::vector<luau::libRegInfo*>& luau::getDefaultLibraries() {
    static std::vector<libRegInfo*> defaultLibraries;
    return defaultLibraries;
}

std::vector<luau::objRegInfo>& luau::getDefaultObjects() {
    static std::vector<objRegInfo> defaultObjects;
    return defaultObjects;
}

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

void luau::registerLibrary(lua_State *L, const libRegInfo& libInfo) {
    lua_newtable(L);

    for (const luau::funcRegInfo& funcInfo : libInfo.methods) {
        lua_pushinteger(L, static_cast<int>(funcInfo.reqLevel));
        lua_pushcfunction(L, funcInfo.func, funcInfo.name.c_str());
        lua_pushcclosure(L, &luau::securityWrapper, funcInfo.name.c_str(), 2);
        lua_setfield(L, -2, funcInfo.name.c_str());
    }

    lua_setreadonly(L, -1, true);
    lua_setglobal(L, libInfo.name.c_str());
}

void luau::registerObject(lua_State *L, const objRegInfo& objInfo) {
    
}
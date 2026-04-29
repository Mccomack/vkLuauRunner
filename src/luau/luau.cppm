module;
#include <lua.h>
#include <luacode.h>
#include <lualib.h>

export module luau;
export import :global;
export import :runfile;
export import :common;

import std;

export namespace luau {
    class State;
    class Environment;
}

class luau::State {
private:
    lua_State* L = nullptr;
public:
    struct environmentContext* __sharedEnvData = nullptr; // optional

    State() {
        L = luaL_newstate();
        luaL_openlibs(L);
    }

    State(SecurityType secType) : State() {
        __sharedEnvData = new struct environmentContext{
            .secType = secType
        };
    }

    State(const State&) = delete;

    State(State&& other) noexcept : L(other.L) {
        other.L = nullptr;
    }

    State& operator= (const State&) = delete;

    State& operator=(State&& other) noexcept {
        if (this != &other) {
            if (L) lua_close(L);

            L = other.L;
            other.L = nullptr;
        }

        return *this;
    }

    lua_State* operator*() {
        return L;
    }

    void registerFunction(const std::string& name, lua_CFunction func, luau::SecurityType requiredLevel) {return luau::registerFunction(L, name, func, requiredLevel);};

    ~State() {
        if (L) lua_close(L); 
        if (__sharedEnvData) delete __sharedEnvData;
    }
};

class luau::Environment {
private:
    State& host;
    lua_State* thread = nullptr;

public:
    struct environmentContext* envData = nullptr;

    Environment() = delete;
    Environment(State& _host) : host(_host), thread(lua_newthread(*host)) {
        if (_host.__sharedEnvData) {
            envData = new struct environmentContext{
                .secType = _host.__sharedEnvData->secType
            };
        } else {
            envData = new struct environmentContext{
                .secType = SecurityType::eNone
            };
        }

        lua_setthreaddata(thread, envData);
    };
    Environment(State& _host, SecurityType _secType) : Environment(_host) {
        envData->secType = _secType;
    };

    Environment(const Environment&) = delete;
    Environment(Environment&& other) noexcept : host(other.host), thread(other.thread), envData(other.envData) {
        other.thread = nullptr; other.envData = nullptr;
    };

    Environment& operator=(const Environment&) = delete;
    Environment& operator=(Environment&& other) noexcept {
        if (this != &other) {
            host = std::move(other.host);
            thread = other.thread;
            envData = other.envData;

            other.thread = nullptr;
            other.envData = nullptr;
        }

        return *this;
    }

    lua_State* operator*() {
        return thread;
    }

    void registerFunction(const std::string& name, lua_CFunction func, luau::SecurityType requiredLevel) {return luau::registerFunction(thread, name, func, requiredLevel);};

    ~Environment() {
        if (envData) delete envData;
    }
};
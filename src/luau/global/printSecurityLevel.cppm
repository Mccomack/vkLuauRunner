module;
#include <lua.h>
#include <lualib.h>

export module luau:global.printSecurityLevel;
import :common;
import logger;
import std;

namespace {
    Logger logger("luau/global/printSecurityLevel");
}

namespace luau::global {
    int lprintSecurityLevel(lua_State* L) {
        auto* envData = static_cast<struct luau::environmentContext*>(lua_getthreaddata(L));
        int currentLevel = envData ? static_cast<int>(envData->secType) : 0;

        logger.Logf("Current level: {}", currentLevel);

        return 0;
    }
}

namespace {
    inline int _ = []()->int {
        luau::getDefaultFunctions().push_back(luau::funcRegInfo{
            .name = "printSecurityLevel",
            .reqLevel = luau::SecurityType::eNone,
            .func = luau::global::lprintSecurityLevel
        });

        return 0;
    }();
}
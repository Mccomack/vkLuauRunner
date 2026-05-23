module;
#include <cstdint>

export module projinfo;

import std;

export namespace project {
    std::string name = "vkLuauRunner";

    std::string version = APP_VERSION;

    uint8_t versionMajor = APP_VERSION_MAJOR;
    uint8_t versionMinor = APP_VERSION_MINOR;
    uint8_t versionPatch = APP_VERSION_PATCH;

    std::string gitHash = GIT_HASH;

#ifdef NDEBUG
    bool isDebugBuild = false;
#else
    bool isDebugBuild = true;
#endif
}
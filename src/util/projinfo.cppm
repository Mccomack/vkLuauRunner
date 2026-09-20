module;
#include <cstdint>

export module projinfo;

import std;

export namespace project {
    constexpr std::string_view name = "vkLuauRunner";

    constexpr std::string version = APP_VERSION;

    constexpr uint8_t versionMajor = APP_VERSION_MAJOR;
    constexpr uint8_t versionMinor = APP_VERSION_MINOR;
    constexpr uint8_t versionPatch = APP_VERSION_PATCH;

    constexpr std::string gitHash = GIT_HASH;

#ifdef NDEBUG
    constexpr bool isDebugBuild = false;
#else
    constexpr bool isDebugBuild = true;
#endif
}

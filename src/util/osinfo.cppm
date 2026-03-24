module;
#include <cstdlib>
#include <string>
#include <string_view>
#include <filesystem>

#define OS_LIST     \
    X(Windows)      \
    X(macOS)        \
    X(Linux)        \
    X(Unknown)      \

export module osinfo;

namespace fs = std::filesystem;

export namespace os {
    #define X(name) name,
    enum OS { OS_LIST };
    #undef X

    #define X(name) #name,
    std::string_view OSname[] = { OS_LIST };
    #undef X

#if defined(_WIN32)
    inline OS os = Windows;
#elif defined(__linux__)
    inline OS os = Linux;
#elif defined(__APPLE__)
    inline OS os = macOS;
#else
    inline OS os = Unknown;
#endif

    inline std::string_view name = OSname[os];

    inline fs::path appPath = []()->fs::path {
#ifndef NDEBUG
        return fs::current_path();
#endif

        std::string tmp;

        fs::path path;

        if (os == Windows) {
            const char* appdata = std::getenv("LOCALAPPDATA");
            path = fs::path(std::string(appdata) + "\\vkLuauRunner\\");
        } else if (os == Linux) {
            const char* xdg = std::getenv("XDG_DATA_HOME");
            if (xdg) {
                path = fs::path(std::string(xdg) + "/vkLuauRunner/");
            } else {
                const char* home = std::getenv("HOME");
                path = fs::path(std::string(home) + "/.local/share/vkLuauRunner/");
            } ;
        } else if (os == macOS) {
            const char* home = std::getenv("HOME");
            path = fs::path(std::string(home) + "/Library/Application Support/vkLuauRunner/");
        }

        if (!fs::exists(path) || !fs::is_directory(path)) {
            fs::create_directory(path);
        }

        return path;
    }();
}
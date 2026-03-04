module;
#include <cstdlib>
#include <string>
#include <string_view>

#define OS_LIST     \
    X(Windows)      \
    X(macOS)        \
    X(Linux)        \
    X(Unknown)      \

export module osinfo;

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

    inline std::string appPath = []()->std::string {
        std::string tmp;

        if (os == Windows) {
            const char* appdata = std::getenv("APPDATA");
            return std::string(appdata) + "\\vkLuauRunner\\";
        } else if (os == Linux) {
            const char* xdg = std::getenv("XDG_DATA_HOME");
            if (xdg) return std::string(xdg) + "/vkLuauRunner/";

            const char* home = std::getenv("HOME");
            return std::string(home) + "/.local/share/vkLuauRunner/";
        } else if (os == macOS) {
            const char* home = std::getenv("HOME");
            return std::string(home) + "/Library/Application Support/vkLuauRunner/";
        }

        return "";
    }();
    
}
module;
#define OS_LIST \
    X(Windows)  \
    X(macOS)    \
    X(Linux)    \
    X(Unknown)

export module osinfo;
import projinfo;

import std;

namespace fs = std::filesystem;

export namespace os {
#define X(name) name,
    enum OS { OS_LIST };
#undef X

#define X(name) #name,
    std::string_view OSname[] = {OS_LIST};
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

    inline fs::path appPath = []() -> fs::path {
#ifndef NDEBUG
        return fs::current_path();
#endif

        std::string tmp;

        fs::path path;

        switch (os) {
            case Windows: {
                const char* appdata = std::getenv("LOCALAPPDATA");
                path = fs::path(
                    std::format("{}\\{}\\", std::string(appdata), project::name)
                );
                break;
            }
            case Linux: {
                const char* xdg = std::getenv("XDG_DATA_HOME");
                if (xdg) {
                    path = fs::path(
                        std::format("{}/{}/", std::string(xdg), project::name)
                    );
                } else {
                    const char* home = std::getenv("HOME");
                    path = fs::path(
                        std::format(
                            "{}/.local/share/{}/",
                            std::string(home),
                            project::name
                        )
                    );
                };
                break;
            }
            case macOS: {
                const char* home = std::getenv("HOME");
                path = fs::path(
                    std::format(
                        "{}/Library/Application Support/{}/",
                        std::string(home),
                        project::name
                    )
                );
                break;
            }
            default:
                return fs::path("/");
        }

        if (!fs::exists(path) || !fs::is_directory(path)) {
            fs::create_directory(path);
        }

        return path;
    }();
}
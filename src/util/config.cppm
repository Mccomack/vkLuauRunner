module;
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string_view>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;

export module Config;
import osinfo;

export class Config {
    Config();
    Config(fs::path Path);
    ~Config();

    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    bool loaded;
    const fs::path path;

    nlohmann::json j;
public:
    static Config& getInstance();

    bool New();

    bool Load();
    bool Save();

    template <typename configType>
    configType Get(std::string_view index) {
        return j[index].get<configType>();
    }

    template <typename configType>
    configType Get(std::string_view index, configType defaultValue) {
        if (!j.contains(index)) {
            this->Set(index, defaultValue);

            return defaultValue;
        }

        return j[index].get<configType>();
    }

    template <typename configType>
    void Set(std::string_view index, configType value) {
        j[index] = value;
    }
};

Config::Config() : loaded(false), path(os::appPath / "config.json"), j(nullptr) {}

Config::Config(std::filesystem::path Path) : loaded(false), path(Path), j(nullptr) {}

Config::~Config() {
    if (loaded) {
        this->Save();
    }
}

Config& Config::getInstance() {
    static Config instance;

    return instance;
}

bool Config::New() {
    if (fs::exists(path)) {
        return false;
    }

    std::ofstream file(path, std::ios::out);

    if (!file.is_open()) {
        std::cerr << std::format("Error opening {}", fs::absolute(path).string()) << std::endl;

        return false;
    }

    file << std::setw(4) << "{\n\t\n}\n" << std::endl;

    file.close();
    return true;
}

bool Config::Load() {
    if (loaded) {
        std::cout << "Reloading config" << std::endl;
    }

    std::ifstream file(path, std::ios::in);

    if (!file.is_open()) {
        std::cerr << std::format("Error opening {}", fs::absolute(path).string()) << std::endl;

        return false;
    }

    loaded = true;

    file >> j;

    file.close();
    return true;
}

bool Config::Save() {
    if (!loaded) {
        std::cerr << "bro we have nothing to save" << std::endl;

        return false;
    }

    std::ofstream file(path, std::ios::out);

    if (!file.is_open()) {
        std::cerr << std::format("Error opening {}", fs::absolute(path).string()) << std::endl;

        return false;
    }

    file << std::setw(4) << j << std::endl;

    file.close();
    return true;
}
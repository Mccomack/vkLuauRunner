#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <nlohmann/json.hpp>
#include "config.hpp"

namespace fs = std::filesystem;

using std::string;

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
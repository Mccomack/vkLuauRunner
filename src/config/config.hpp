#include <iostream>
#include <string>
#include <optional>
#include <nlohmann/json.hpp>

using std::string;

class Config {
    bool loaded;
    string path;

    nlohmann::json j;
public:
    Config() : loaded(false), path("config.json"), j(nullptr) {};
    Config(std::optional<string> Path) : loaded(false), path(Path.value_or("config.json")), j(nullptr) {};

    bool New();

    bool Load();
    bool Save();

    template <typename configType>
    std::optional<configType> Get(string index) {
        if (!j.contains(index)) {
            return std::nullopt;
        }

        return j[index].get<configType>();
    }

    template <typename configType>
    void Set(string index, configType value) {
        j[index] = value;
    }
};
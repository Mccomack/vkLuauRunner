#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

using std::string;

class Config {
    bool loaded;
    const string path;

    nlohmann::json j;
public:
    Config();
    Config(string Path);
    ~Config();

    bool New();

    bool Load();
    bool Save();

    template <typename configType>
    configType Get(string index, configType defaultValue) {
        if (!j.contains(index)) {
            this->Set(index, defaultValue);

            return defaultValue;
        }

        return j[index].get<configType>();
    }

    template <typename configType>
    void Set(string index, configType value) {
        j[index] = value;
    }
};

#endif
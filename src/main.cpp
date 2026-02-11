#include <iostream>
#include <string>
#include "config/config.hpp"
#include "logger/logger.hpp"

using std::string;

int main() {
    Config& config = Config::getInstance();
    Logger logger("main");
    Logger anotherLogger("totallyNotMain");

    config.New();
    config.Load();

    int v = config.Get<int>("asdf", 1);

    logger.Log("asdf");
    anotherLogger.Log("zxcv");

    return 0;
}
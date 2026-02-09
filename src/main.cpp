#include <iostream>
#include <string>
#include "config/config.hpp"
#include "logger/logger.hpp"

using std::cout;
using std::endl;

using std::string;

int main() {
    Config& config = Config::getInstance();
    Logger logger("main");

    config.New();
    config.Load();

    int v = config.Get<int>("asdf", 1);

    logger.Log("asdf");

    return 0;
}
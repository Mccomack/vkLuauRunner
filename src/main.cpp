#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include "config/config.hpp"
#include "logger/logger.hpp"
#include "graphic/graphic.hpp"

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

#ifdef NDEBUG
    logger.Log("Current build type: Release");
#else
    logger.Log("Current build type: Debug");
#endif

    //graphic::graphicLearning();

    graphic::triangle helloTriangle;

    try {
        helloTriangle.run();
    } catch (const std::exception& e) {
        logger.Log(e.what(), "Error");

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
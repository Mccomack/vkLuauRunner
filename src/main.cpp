#include <format>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

import osinfo;
import Config;
import Logger;
import graphic;

using std::string;

int main() {
    Config& config = Config::getInstance();
    Logger logger("main");

    config.New();
    config.Load();

    logger.Log(std::format("Current OS: {}", os::name));
    logger.Log(std::format("appData folder path: {}", os::appPath));

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
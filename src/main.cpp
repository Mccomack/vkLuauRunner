#include <format>
#include <string>
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

    logger.Logf("Current OS: {}", os::name);

#ifdef NDEBUG
    logger.Log("Current build type: Release");
#else
    logger.Log("Current build type: Debug");
#endif

    logger.Logf("appData folder path: {}", os::appPath.generic_string());

    //graphic::graphicLearning();

    graphic::app helloTriangle;

    try {
        helloTriangle.run();
    } catch (const std::exception& e) {
        logger.Log(e.what(), "Error");

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
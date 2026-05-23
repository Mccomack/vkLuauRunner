#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#include <GLFW/glfw3.h>

import std;
import osinfo;
import projinfo;
import Config;
import logger;
import app;
import luau;

namespace {
    Config& config = Config::getInstance();
    Logger logger("main");
}

int main() {
    std::ios_base::sync_with_stdio(false);

    config.New();
    config.Load();

    logger.Logf("{} v{}", project::name, project::version);
    logger.Logf("Git hash: {}", project::gitHash);

    logger.Logf("Current OS: {}", os::name);

    logger.Logf(
        "Current build type: {}", (project::isDebugBuild ? "Debug" : "Release")
    );

    logger.Logf("appData folder path: {}", os::appPath.generic_string());

    try {
        app::app app;
        app.run();
    } catch (const std::exception& e) {
        logger.Log(e.what(), "Error");

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
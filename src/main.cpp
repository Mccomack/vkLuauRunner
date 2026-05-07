#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

import std;
import osinfo;
import appinfo;
import Config;
import logger;
import graphic;
import luau;

int main() {
    std::ios_base::sync_with_stdio(false);
    
    Config& config = Config::getInstance();
    Logger logger("main");

    config.New();
    config.Load();

    logger.Logf("{} v{}", app::name, app::version);
    logger.Logf("Git hash: {}", app::gitHash);

    logger.Logf("Current OS: {}", os::name);

    logger.Logf("Current build type: {}", (app::isDebugBuild ? "Debug" : "Release"));

    logger.Logf("appData folder path: {}", os::appPath.generic_string());

    luau::State main(luau::SecurityType::eDefault);
    luau::global::registerDefaultFunctions(*main);
    luau::library::registerDefaultLibraries(*main);
    luau::object::registerDefaultObjects(*main); 
    luau::sandbox(main);
    luau::Environment script1(main);
    luau::sandbox(script1);

    try {
        luau::run::runLuauFile(*script1, os::appPath / "asset" / "script" / "main.luau");
    } catch (const std::exception& e) {
        logger.Log(e.what(), "Error");
    }

    graphic::app helloTriangle;

    try {
        helloTriangle.run();
    } catch (const std::exception& e) {
        logger.Log(e.what(), "Error");

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
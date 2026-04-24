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
    Config& config = Config::getInstance();
    Logger logger("main");

    config.New();
    config.Load();

    logger.Logf("{} v{}", app::name, app::version);
    logger.Logf("Git hash: {}", app::gitHash);

    logger.Logf("Current OS: {}", os::name);

#ifdef NDEBUG
    logger.Log("Current build type: Release");
#else
    logger.Log("Current build type: Debug");
#endif

    logger.Logf("appData folder path: {}", os::appPath.generic_string());

    luau::State main(luau::SecurityType::eDefault);
    main.registerFunction("print", luau::global::lprint, luau::SecurityType::eNone);
    main.registerFunction("add", luau::global::laddTest, luau::SecurityType::eNone);
    main.registerFunction("security", luau::global::lsecurity, luau::SecurityType::eDefault);
    luau::Environment script1(main);

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
module;

export module app;
import window;
import graphic;
import input;
import luau;

import osinfo;
import logger;

import std;

namespace {
    Logger logger("app");
}

export namespace app {
    class app;
}

void afterInit() {
    luau::State main(luau::SecurityType::eDefault);
    luau::global::registerDefaultFunctions(*main);
    luau::library::registerDefaultLibraries(*main);
    luau::object::registerDefaultObjects(*main);
    luau::sandbox(main);
    luau::Environment script1(main);
    luau::sandbox(script1);
    luau::Environment script2(main);
    luau::sandbox(script2);

    try {
        luau::run::runLuauFile(
            *script1, os::appPath / "asset" / "script" / "main.luau"
        );
        logger.Log("---");
        luau::run::runLuauFile(
            *script2, os::appPath / "asset" / "script" / "2ndrankedppl.luau"
        );
    } catch (const std::exception& e) {
        logger.Log(e.what(), "Error");
    }
}

class app::app {
   private:
    static void beforeRender(app& obj);
    static void afterRender(app& obj);

    Window mainWindow;
    graphic::app renderer;
    input::handler inputHandler;

   public:
    app()
        : renderer(mainWindow),
          inputHandler(mainWindow) {
        afterInit();
    };

    void run();
};

void app::app::beforeRender(app& obj) {
    // TODO: impl
    // input handle
    // 구스범스바보 factos 👀

    static input::Status beforeStatus = input::Status::eNone;
    input::Status status = obj.inputHandler.getKeyStatus(input::keyboard::W);

    if (status == beforeStatus)
        return;

    beforeStatus = status;
    logger.Log(status == input::Status::ePress ? "press" : "not press");
}

void app::app::afterRender(app& obj) {
    // TODO: impl
}

void app::app::run() {
    renderer.run(
        [this]() -> void { beforeRender(*this); },
        [this]() -> void { afterRender(*this); }
    );
}
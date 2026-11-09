module;
#include <SDL3/SDL.h>

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

class app::app {
   private:
    bool quit;
    static void beforeRender(app& obj);
    static void afterRender(app& obj);

    Window mainWindow;
    graphic::app renderer;
    input::handler inputHandler;

   public:
    app()
        : renderer(mainWindow),
          inputHandler(mainWindow) {
        SDL_AddEventWatch(resizingEventWatcher, this);

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
    };

    static bool resizingEventWatcher(void* userdata, SDL_Event* e);

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

bool app::app::resizingEventWatcher(void* userdata, SDL_Event* e) {
    auto* self = static_cast<class app::app*>(userdata);

    switch (e->type) {
        case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
            self->mainWindow.setPixelSize(e->window.data1, e->window.data2);
            self->renderer.drawFrame();
            break;
        case SDL_EVENT_WINDOW_RESIZED:
            self->mainWindow.setFinalSize(e->window.data1, e->window.data2);
            break;
    }

    return true;
}

void app::app::run() {
    bool quit = false;
    SDL_Event e;

    while (!quit) {
        beforeRender(*this);

        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_EVENT_QUIT:
                    quit = true;
                    break;
                case SDL_EVENT_WINDOW_RESIZED:
                    mainWindow.setFinalSize(e.window.data1, e.window.data2);
                    break;
                case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
                    mainWindow.setPixelSize(e.window.data1, e.window.data2);
                    break;
                case SDL_EVENT_WINDOW_MINIMIZED:
                    mainWindow.setMinimizedFlag(true);
                    break;
                case SDL_EVENT_WINDOW_RESTORED:
                    mainWindow.setMinimizedFlag(false);
                    break;
                default:
                    logger.Debugf(
                        "SDL Event didn't handled. event type: {}", e.type
                    );
                    break;
            }
        }

        renderer.drawFrame();

        afterRender(*this);
    }
}

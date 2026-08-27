module;
#include <SDL3/SDL.h>

export module input;
export import :common;
export import :keyboard;

import window;

import std;

export namespace input {
    class handler;
}

class input::handler {
   private:
    handler(const handler&) = delete;
    handler& operator=(const handler&) = delete;

    handler(handler&&) = delete;
    handler& operator=(handler&&) = delete;

    Window& window;

    std::unordered_map<keyboard, Status> keyStatus;

   public:
    handler() = delete;
    handler(Window& window_) noexcept;
    ~handler();

    Status getKeyStatus(keyboard key);
};

using handler = input::handler;

handler::handler(Window& window_) noexcept
    : window(window_) {
    //
}

handler::~handler() {
    //
}

input::Status handler::getKeyStatus(keyboard key) {
    const bool* keyState = SDL_GetKeyboardState(nullptr);

    return keyState[keyboardMap.getB(key)] ? Status::ePress : Status::eNone;
}
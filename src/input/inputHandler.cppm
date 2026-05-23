module;
#include <GLFW/glfw3.h>

export module input;
export import :common;
export import :keyboard;

import window;

export namespace input {
    class handler;
}

class input::handler {
   private:
    handler(const handler&) = delete;
    handler& operator=(const handler&) = delete;

    handler(handler&&) = delete;
    handler& operator=(handler&&) = delete;

    window& window_;  // only reference

   public:
    handler() = delete;
    handler(window& _window) noexcept;
    ~handler();

    Status getKeyStatus(keyboard key);
};

using handler = input::handler;

handler::handler(window& _window) noexcept
    : window_(_window) {
    //
}

handler::~handler() {
    //
}

input::Status handler::getKeyStatus(keyboard key) {
    int keyStatus = glfwGetKey(*window_, getKeyboardKeyMap().at(key));

    return keyStatus == GLFW_PRESS ? Status::ePress : Status::eNone;
}
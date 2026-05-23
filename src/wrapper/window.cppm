module;
#include <GLFW/glfw3.h>
#include <new>

export module window;

import std;

export class window {
   private:
    window(const window&) = delete;
    window& operator=(const window&) = delete;

    window(window&&) = delete;
    window& operator=(window&&) = delete;

    GLFWwindow* _window;

   public:
    window(int width = 800, int height = 600)
        : _window([&]() -> GLFWwindow* {
              glfwInit();
              glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

              return glfwCreateWindow(
                  width, height, "window", nullptr, nullptr
              );
          }()) {}

    ~window() {
        glfwDestroyWindow(_window);
        glfwTerminate();
    }

    GLFWwindow* operator*() { return _window; }

    std::tuple<int, int> getFramebufferSize() {
        int width, height;

        glfwGetFramebufferSize(_window, &width, &height);

        return std::make_tuple(width, height);
    }
};
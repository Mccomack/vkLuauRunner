module;
#include <GLFW/glfw3.h>
#include <new>

export module window;

import std;

export class Window {
   private:
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    GLFWwindow* window;

   public:
    Window(int width = 800, int height = 600)
        : window([&]() -> GLFWwindow* {
              glfwInit();
              glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

              return glfwCreateWindow(
                  width, height, "Window", nullptr, nullptr
              );
          }()) {}

    ~Window() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    GLFWwindow* operator*() { return window; }

    std::tuple<int, int> getFramebufferSize() {
        int width, height;

        glfwGetFramebufferSize(window, &width, &height);

        return std::make_tuple(width, height);
    }
};
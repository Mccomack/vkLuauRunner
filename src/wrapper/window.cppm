module;
// #include <GLFW/glfw3.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <cstddef>

export module window;

import std;
import vulkan;

export class Window {
   private:
    // 복사생성자 금지
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    // free windows 🤑🤑🤑
    SDL_Window* window;

   public:
    // 창 생성하는 거. 앱 만드는 게 아니라서 여러 개 있어도 문제 없을 듯?
    Window(int width = 800, int height = 600)
        : window([&]() -> SDL_Window* {
              SDL_Init(SDL_INIT_VIDEO);

              SDL_Window* window_ = SDL_CreateWindow(
                  "Window",
                  width,
                  height,
                  SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE
              );

              if (!window_)
                  throw std::runtime_error(SDL_GetError());

              return window_;
          }()) {}

    // 홍명보당한 경우
    ~Window() {
        if (!window)
            return;

        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    // pointer 관련 편의성 함수. SDL_Window* 반환
    SDL_Window* operator*() const { return window; }
    operator SDL_Window*() const { return window; }

    std::tuple<int, int> getFramebufferSize() const {
        int width, height;

        SDL_GetWindowSizeInPixels(window, &width, &height);

        return std::make_tuple(width, height);
    }

    std::tuple<int, int> getFinalSize() const {
        int width, height;

        SDL_GetWindowSize(window, &width, &height);

        return std::make_tuple(width, height);
    }

    vk::raii::SurfaceKHR createSurface(
        const vk::raii::Instance& instance
    ) const {
        VkSurfaceKHR surface;

        SDL_Vulkan_CreateSurface(window, *instance, nullptr, &surface);

        return vk::raii::SurfaceKHR(instance, surface);
    }

    std::span<const char*> getRequiredInstanceExtensions() const {
        uint32_t count;
        const char* const* extensions =
            SDL_Vulkan_GetInstanceExtensions(&count);

        return std::span{const_cast<const char**>(extensions), count};
    }
};

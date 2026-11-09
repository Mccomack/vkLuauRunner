module;
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

    std::atomic<uint64_t> position;

    // 각각 hdr 적용 전 화면 사이즈, hdr 적용 후 화면 사이즈, 화면 사이즈 변경
    // 여부.
    std::atomic<uint64_t> pixelSize;
    std::atomic<uint64_t> finalSize;
    std::atomic<bool> sizeChanged;
    std::atomic<bool> minimized;

    // 비트연산자 🤓
    // width 먼저, height 나중
    uint64_t pack(uint32_t w, uint32_t h) {
        return (static_cast<uint64_t>(w) << 32) | h;
    }

    std::pair<uint32_t, uint32_t> unpack(uint64_t packed) const {
        return {
            static_cast<uint32_t>(packed >> 32),
            static_cast<uint32_t>(packed & 0xFFFFFFFF)
        };
    }

    void initSDL(uint32_t width, uint32_t height);
    void cleanup();

   public:
    Window(uint32_t width = 800, uint32_t height = 600) {
        initSDL(width, height);
    }

    ~Window() { cleanup(); }

    SDL_Window* operator*() const { return window; }
    operator SDL_Window*() const { return window; }

    vk::raii::SurfaceKHR createSurface(
        const vk::raii::Instance& instance
    ) const;
    std::span<const char*> getRequiredInstanceExtensions() const;

    // 생 pixel 기준 화면 사이즈
    std::pair<uint32_t, uint32_t> getPixelSize() const {
        return unpack(pixelSize);
    }

    // hdr 처리 후 화면 사이즈
    std::pair<uint32_t, uint32_t> getFinalSize() const {
        return unpack(finalSize);
    }

    bool isSizeChangedFlagSet() const { return sizeChanged.load(); }
    bool isMinimizedFlagSet() const { return minimized.load(); }

    void setPixelSize(uint32_t w, uint32_t h) {
        pixelSize.store(pack(w, h), std::memory_order_relaxed);
        sizeChanged.store(true, std::memory_order_relaxed);
    }

    void setFinalSize(uint32_t w, uint32_t h) {
        finalSize.store(pack(w, h), std::memory_order_relaxed);
        sizeChanged.store(true, std::memory_order_relaxed);
    }

    void resetSizeChangedFlag() {
        sizeChanged.store(false, std::memory_order_relaxed);
    }

    void setMinimizedFlag(bool flag) {
        minimized.store(flag, std::memory_order_relaxed);
    }
};

void Window::initSDL(uint32_t width, uint32_t height) {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "Window", width, height, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE
    );

    if (!window)
        throw std::runtime_error(SDL_GetError());

    setPixelSize(width, height);
    setFinalSize(width, height);
}

void Window::cleanup() {
    if (!window)
        return;

    SDL_DestroyWindow(window);
    SDL_Quit();
}

vk::raii::SurfaceKHR Window::createSurface(
    const vk::raii::Instance& instance
) const {
    VkSurfaceKHR surface;

    SDL_Vulkan_CreateSurface(window, *instance, nullptr, &surface);

    return vk::raii::SurfaceKHR(instance, surface);
}

std::span<const char*> Window::getRequiredInstanceExtensions() const {
    uint32_t count;
    const char* const* extensions = SDL_Vulkan_GetInstanceExtensions(&count);

    return std::span{const_cast<const char**>(extensions), count};
}

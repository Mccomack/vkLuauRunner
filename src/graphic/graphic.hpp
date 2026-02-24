#ifndef GRAPHIC_HPP
#define GRAPHIC_HPP

#include <optional>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "validationLayer/validationLayer.hpp"
#include "device/device.hpp"

namespace graphic {
    void graphicLearning();

    namespace validationLayer {
        using namespace ::validationLayer;
    }

    namespace device {
        using namespace ::device;
    }

    class triangle;
}

class graphic::triangle {
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    GLFWwindow* window;

    VkInstance instance;
    VkSurfaceKHR surface;

    VkDebugUtilsMessengerEXT debugMessenger;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;

    void createInstance();
    void createSurface();
    
    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();

public:
    void run();
};

#endif
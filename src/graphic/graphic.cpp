#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <vector>
#include <tuple>
#include <cstring>
#include <format>
#include <optional>
#include "./../logger/logger.hpp"

#include "graphic.hpp"

using namespace graphic::validationLayer;
using namespace graphic::device;

using graphic::triangle;

namespace {
    Logger logger("graphic");
}

void graphic::graphicLearning() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    logger.Log(std::format("{} extensions supported", extensionCount));

    glm::mat4 matrix;
    glm::vec4 vec;
    auto test = matrix * vec;

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return;
}

void triangle::createInstance() {
    if (validationLayer::enableValidationLayers && !validationLayer::checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    // some shits
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    // vaildation layer support
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

    if (validationLayer::enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayer::validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayer::validationLayers.data();

        validationLayer::populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;

        createInfo.pNext = nullptr;
    }

    // get instance extensions
    std::vector<const char*> extensions;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    for (int i = 0; i < glfwExtensionCount; i++) {
        extensions.push_back(glfwExtensions[i]);
    }

    validationLayer::pushRequiredExtensions(extensions);

    // macOS Settings
    extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);

    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

    // put instance extensions
    createInfo.enabledExtensionCount = (uint32_t) extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();

#ifndef NDEBUG
    logger.Log("Enabled instance extensions: ", "Debug");

    for (const char* extension : extensions) {
        logger.Log(std::format("\t{}", extension), "Debug");
    }
#endif

    // the time has come
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

    if (result != VK_SUCCESS) {
        logger.Log(std::format("vkCreateInstance error. Error Code: {}", static_cast<int>(result)), "Error");

        throw std::runtime_error("failed to create instance!"); // u stupid ass
    }

    logger.Log("vkCreateInstance success", "Info");
}

void triangle::createSurface() {
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("Cannot create window surface. ");
    }
}

void triangle::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    this->window = window;
}

void triangle::initVulkan() {
    createInstance();

    if (validationLayer::enableValidationLayers) {
        debugMessenger = validationLayer::createDebugMessenger(instance);
    }

    createSurface();

    physicalDevice = device::pickPhysicalDevice(instance, surface);
    
    std::tuple<VkDevice, VkQueue, VkQueue> tmpdev = device::createLogicalDevice(physicalDevice, surface);
    device = std::get<0>(tmpdev);
    graphicsQueue = std::get<1>(tmpdev);
    presentQueue = std::get<2>(tmpdev);

}

void triangle::mainLoop() {
    logger.Log("mainLoop start", "Info");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    logger.Log("close", "Info");
}

void triangle::cleanup() {
    vkDestroyDevice(device, nullptr);

    if (debugMessenger) {
        validationLayer::DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }

    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window);

    glfwTerminate();
}

void triangle::run() {
    triangle::initWindow();
    triangle::initVulkan();
    triangle::mainLoop();
    triangle::cleanup();
}
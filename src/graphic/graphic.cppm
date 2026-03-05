module;
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <vector>
#include <tuple>
#include <cstring>
#include <format>

import Logger;

export module graphic;
export import :validationLayer;
export import :device;
export import :swapchain;
export import :pipeline;
export import :framebuffer;
export import :common;

export namespace graphic {
    namespace validationLayer {
        using namespace ::validationLayer;
    }

    namespace device {
        using namespace ::device;
    }

    namespace swapchain {
        using namespace ::swapchain;
    }

    namespace pipeline {
        using namespace ::pipeline;
    }

    namespace framebuffer {
        using namespace ::framebuffer;
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

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    VkPipelineLayout pipelineLayout;
    VkRenderPass renderPass;
    VkPipeline graphicsPipeline;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    void createInstance();
    void createSurface();
    
    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();

public:
    void run();
};

namespace {
    Logger logger("graphic");
}

void graphic::triangle::createInstance() {
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
    logger.Debug("Enabled instance extensions: ");

    for (const char* extension : extensions) {
        logger.Debug(std::format("\t{}", extension));
    }
#endif

    // the time has come
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

    if (result != VK_SUCCESS) {
        logger.Log(std::format("vkCreateInstance error. Error Code: {}", static_cast<int>(result)), "Error");

        throw std::runtime_error("failed to create instance!"); // u stupid ass
    }
}

void graphic::triangle::createSurface() {
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("Cannot create window surface. ");
    }
}

void graphic::triangle::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    this->window = window;
}

void graphic::triangle::initVulkan() {
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

    swapChain = swapchain::createSwapChain(physicalDevice, device, surface, window);
    swapChainImages = swapchain::createImages(device, swapChain);

    SwapChainSupportDetails swapchainSupport = querySwapChainSupport(physicalDevice, surface);
    swapChainImageFormat = swapchain::chooseSwapSurfaceFormat(swapchainSupport.formats).format;
    swapChainExtent = swapchain::chooseSwapExtent(swapchainSupport.capabilities, window);

    swapChainImageViews = swapchain::createImageViews(device, swapChainImages, swapChainImageFormat);

    std::tuple<VkShaderModule, VkShaderModule> tmpshd = pipeline::getShaderModule(device);
    VkShaderModule vertShaderModule = std::get<0>(tmpshd);
    VkShaderModule fragShaderModule = std::get<1>(tmpshd);

    pipelineLayout = pipeline::createPipelineLayout(device, vertShaderModule, fragShaderModule);
    renderPass = pipeline::createRenderPass(device, swapChainImageFormat);

    graphicsPipeline = pipeline::createGraphicsPipeline(device, pipelineLayout, renderPass, vertShaderModule, fragShaderModule, swapChainExtent);
    
    vkDestroyShaderModule(device, vertShaderModule, nullptr);
    vkDestroyShaderModule(device, fragShaderModule, nullptr);

    swapChainFramebuffers.resize(swapChainImageViews.size());

    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        swapChainFramebuffers[i] = framebuffer::createFramebuffer(device, renderPass, swapChainImageViews[i], swapChainExtent);
    }
}

void graphic::triangle::mainLoop() {
    logger.Log("mainLoop start", "Info");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}

void graphic::triangle::cleanup() {
    for (VkFramebuffer framebuffer : swapChainFramebuffers) {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }

    vkDestroyPipeline(device, graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    vkDestroyRenderPass(device, renderPass, nullptr);

    for (VkImageView imageView : swapChainImageViews) {
        vkDestroyImageView(device, imageView, nullptr);
    }

    vkDestroySwapchainKHR(device, swapChain, nullptr);
    vkDestroyDevice(device, nullptr);

    if (debugMessenger) {
        validationLayer::DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }

    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window);

    glfwTerminate();
}

void graphic::triangle::run() {
    graphic::triangle::initWindow();
    graphic::triangle::initVulkan();
    graphic::triangle::mainLoop();
    graphic::triangle::cleanup();
}
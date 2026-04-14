module;
#include "vulkan/vulkan.hpp"
#include <algorithm>
#include <vulkan/vulkan_core.h>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_raii.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

export module graphic;
export import :validationLayer;
export import :device;
export import :swapchain;
export import :pipeline;
export import :framebuffer;
export import :command;
export import :synchronization;
export import :buffer;
export import :descriptor;
export import :uniformBufferObject;
export import :common;

import vulkan;

import Logger;

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

    namespace command {
        using namespace ::command;
    }

    namespace synchronization {
        using namespace ::synchronization;
    }

    namespace buffer {
        using namespace ::buffer;
    }

    namespace descriptor {
        using namespace ::descriptor;
    }

    namespace uniformBufferObejct {
        using namespace ::uniformBufferObject;
    }

    class app;
}

class graphic::app {
    static constexpr uint32_t WIDTH = 800;
    static constexpr uint32_t HEIGHT = 600;

    uint32_t currentFrame = 0;

    const std::vector<graphic::Vertex> vertices = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}}, 
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}}, 
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    };

    const std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0
    };

    GLFWwindow* window;

    vk::raii::Context context;
    vk::raii::Instance instance = nullptr;
    vk::raii::SurfaceKHR surface = nullptr;

    vk::DebugUtilsMessengerEXT debugMessenger = nullptr;

    vk::raii::PhysicalDevice physicalDevice = nullptr;
    vk::raii::Device device;
    vk::raii::Queue graphicsQueue;
    vk::raii::Queue presentQueue;

    vk::raii::SwapchainKHR swapChain;
    std::vector<vk::Image> swapChainImages;
    vk::SurfaceFormatKHR swapChainSurfaceFormat;
    vk::Extent2D swapChainExtent;
    std::vector<vk::raii::ImageView> swapChainImageViews;

    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    VkPipelineLayout pipelineLayout;
    VkRenderPass renderPass;
    VkPipeline graphicsPipeline;

    std::vector<vk::raii::Framebuffer> swapChainFramebuffers;

    vk::raii::CommandPool commandPool;

    graphic::Buffer indexBuffers;
    graphic::Buffer vertexBuffers;

    std::vector<vk::raii::Buffer> uniformBuffers;
    std::vector<vk::raii::DeviceMemory> uniformBufferMemories;
    std::vector<void*> uniformBufferMapped;

    std::vector<vk::raii::CommandBuffer> commandBuffers;

    std::vector<vk::raii::Semaphore> imageAvaliableSemaphores;
    std::vector<vk::raii::Semaphore> renderFinishedSemaphores;
    std::vector<vk::raii::Fence> inFlightFences;
    std::vector<vk::raii::Fence> presentFences;

    void createInstance();
    void createSurface();

    void recreateSwapchain();

    void drawFrame();
  
    void initWindow();
    void initVulkan();
    void mainLoop();

    void cleanupSwapchain();
    void cleanup();

public:
    bool framebufferResized = false;

    void run();
};

namespace {
    Logger logger("graphic");
}

void graphic::app::createInstance() {
    if (validationLayer::enableValidationLayers && !validationLayer::checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    constexpr vk::ApplicationInfo appInfo{
        .pApplicationName = "Hello Quadrangle",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "No Engine",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = vk::ApiVersion14
    };

    vk::InstanceCreateInfo createInfo{};
    createInfo.pApplicationInfo = &appInfo;

    // instance layers
    std::vector<const char*> requiredLayers;
    if (validationLayer::enableValidationLayers) {
        requiredLayers.assign(validationLayer::validationLayers.begin(), validationLayer::validationLayers.end());
    }

    auto layerProperties = context.enumerateInstanceLayerProperties();
    auto unsupportedLayer = std::ranges::find_if(requiredLayers, 
        [&layerProperties](auto const &requiredLayer) {
            return std::ranges::none_of(layerProperties, 
                [requiredLayer](auto const &layerProperty) {
                    return strcmp(layerProperty.layerName, requiredLayer) == 0;
                }
            );
        }
    );

    if (unsupportedLayer != requiredLayers.end()) {
        throw std::runtime_error(std::format("Required layer not supported: {}", std::string(*unsupportedLayer)));
    }

    createInfo.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size());
    createInfo.ppEnabledLayerNames = requiredExtensions.data();

    // vaildation layer support
    if (validationLayer::enableValidationLayers) {
        vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo{};

        validationLayer::populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (vk::DebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    } else {
        createInfo.pNext = nullptr;
    }

    // instance extensions
    std::vector<const char*> requiredExtensions;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    for (int i = 0; i < glfwExtensionCount; i++) {
        requiredExtensions.push_back(glfwExtensions[i]);
    }

    validationLayer::pushRequiredInstanceExtensions(requiredExtensions);

    requiredExtensions.push_back(vk::KHRGetSurfaceCapabilities2ExtensionName);
    requiredExtensions.push_back(vk::EXTSurfaceMaintenance1ExtensionName);

    // macOS Settings
    requiredExtensions.push_back(vk::KHRPortabilityEnumerationExtensionName);
    requiredExtensions.push_back(vk::KHRGetPhysicalDeviceProperties2ExtensionName);

    //createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

    // put instance extensions
    createInfo.enabledExtensionCount = (uint32_t) extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();

    logger.Debug("Enabled instance extensions: ");

    for (const char* extension : extensions) {
        logger.Debugf("\t{}", extension);
    }

    instance = vk::raii::Instance(context, createInfo);
}

void graphic::app::createSurface() {
    VkSurfaceKHR _surface;

    if (glfwCreateWindowSurface(*instance, window, nullptr, &_surface) != VK_SUCCESS) {
        throw std::runtime_error("Cannot create window surface. ");
    }

    surface = vk::raii::SurfaceKHR(instance, _surface);
}

void graphic::app::recreateSwapchain() {
    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }

    device.waitIdle();

    cleanupSwapchain();

    swapChain = swapchain::createSwapChain(physicalDevice, device, surface, window, swapChain);

    SwapChainSupportDetails swapchainSupport = querySwapChainSupport(physicalDevice, surface);
    swapChainSurfaceFormat = swapchain::chooseSwapSurfaceFormat(swapchainSupport.formats);
    swapChainExtent = swapchain::chooseSwapExtent(swapchainSupport.capabilities, window);

    swapChainImages = swapchain::createImages(device, swapChain);
    swapChainImageViews = swapchain::createImageViews(device, swapChainImages, swapChainSurfaceFormat);

    swapChainFramebuffers.resize(swapChainImageViews.size());

    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        swapChainFramebuffers[i] = framebuffer::createFramebuffer(device, renderPass, swapChainImageViews[i], swapChainExtent);
    }

    if (swapChainImages.size() != renderFinishedSemaphores.size()) {
        for (size_t i = 0; i < imageAvaliableSemaphores.size(); i++) {
            vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
        }

        renderFinishedSemaphores.resize(swapChainImages.size());

        for (size_t i = 0; i < swapChainImages.size(); i++) {
            renderFinishedSemaphores[i] = synchronization::craeteSemaphore(device);
        }
    }
}

void graphic::app::drawFrame() {
    vkWaitForFences(device, 1, &presentFences[currentFrame], VK_TRUE, UINT64_MAX);
    

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvaliableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        framebufferResized = false;
        recreateSwapchain();

        return;
    } else if (result == VK_SUBOPTIMAL_KHR || framebufferResized) {
        VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &imageAvaliableSemaphores[currentFrame];
        submitInfo.pWaitDstStageMask = &waitStage;

        vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(graphicsQueue);

        framebufferResized = false;
        recreateSwapchain();

        return;
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("cannot acquire swapchain image");
    }

    vkResetFences(device, 1, &presentFences[currentFrame]);

    vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
    
    vkResetFences(device, 1, &inFlightFences[currentFrame]);

    vkResetCommandBuffer(commandBuffers[currentFrame], 0);

    uniformBufferObject::updateUniformBuffer(swapChainExtent, uniformBufferMapped[currentFrame]);

    RenderState renderState{};
    renderState.renderPass = renderPass;
    renderState.graphicsPipeline = graphicsPipeline;
    renderState.pipelineLayout = pipelineLayout;
    renderState.vertexBuffer = vertexBuffers.buffer;
    renderState.indexBuffer = indexBuffers.buffer;
    renderState.descriptorSet = descriptorSets[currentFrame];
    renderState.vertices = vertices;
    renderState.indices = indices;

    FrameTarget frameTarget{};
    frameTarget.framebuffer = swapChainFramebuffers[imageIndex];
    frameTarget.extent = swapChainExtent;

    command::recordCommandBuffer(commandBuffers[currentFrame], renderState, frameTarget);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvaliableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[currentFrame];
    
    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[imageIndex]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("cannot submit draw command buffer");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;

    VkSwapchainPresentFenceInfoKHR presentFenceInfo{};
    presentFenceInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_PRESENT_FENCE_INFO_EXT;
    presentFenceInfo.swapchainCount = 1;
    presentFenceInfo.pFences = &presentFences[currentFrame];

    presentInfo.pNext = &presentFenceInfo;

    result = vkQueuePresentKHR(presentQueue, &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
        framebufferResized = false;
        recreateSwapchain();

        return;
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("cannot present swapchain image");
    }
    
    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    graphic::app* app = reinterpret_cast<graphic::app*>(glfwGetWindowUserPointer(window));

    app->framebufferResized = true;
}

void graphic::app::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void graphic::app::initVulkan() {
    createInstance();

    if (validationLayer::enableValidationLayers) {
        debugMessenger = validationLayer::createDebugMessenger(instance);
    }

    createSurface();

    physicalDevice = device::pickPhysicalDevice(instance, surface);
    
    std::tuple<vk::raii::Device, vk::raii::Queue, vk::raii::Queue> tmpdev = device::createLogicalDevice(physicalDevice, surface, context);
    device = std::get<0>(tmpdev);
    graphicsQueue = std::get<1>(tmpdev);
    presentQueue = std::get<2>(tmpdev);

    swapChain = swapchain::createSwapChain(physicalDevice, device, surface, window);
    swapChainImages = swapchain::createImages(device, swapChain);

    SwapChainSupportDetails swapchainSupport = querySwapChainSupport(physicalDevice, surface);
    swapChainSurfaceFormat = swapchain::chooseSwapSurfaceFormat(swapchainSupport.formats);
    swapChainExtent = swapchain::chooseSwapExtent(swapchainSupport.capabilities, window);

    swapChainImageViews = swapchain::createImageViews(device, swapChainImages, swapChainSurfaceFormat);

    commandPool = command::createCommandPool(physicalDevice, device, surface);

    std::tuple<VkShaderModule, VkShaderModule> tmpshd = pipeline::getShaderModule(device);
    vk::raii::ShaderModule vertShaderModule = std::get<0>(tmpshd);
    vk::raii::ShaderModule fragShaderModule = std::get<1>(tmpshd);

    vk::DeviceSize size = sizeof(vertices[0]) * vertices.size();
    vk::DeviceSize indSize = sizeof(indices[0]) * indices.size();

    vertexBuffers = buffer::createVertexBuffer(physicalDevice, device, commandPool, graphicsQueue, vertices.data(), size);
    indexBuffers = buffer::createIndexBffer(physicalDevice, device, commandPool, graphicsQueue, indices.data(), indSize);

    auto tmpubo = buffer::createUniformBuffer(physicalDevice, device, commandPool, graphicsQueue);
    uniformBuffers = std::get<0>(tmpubo);
    uniformBufferMemories = std::get<1>(tmpubo);
    uniformBufferMapped = std::get<2>(tmpubo);

    descriptorSetLayout = descriptor::createDescriptorSetLayout(device);
    descriptorPool = descriptor::createDescriptorPool(device);
    descriptorSets = descriptor::createDescriptorSets(device, descriptorSetLayout, descriptorPool, uniformBuffers);

    pipelineLayout = pipeline::createPipelineLayout(device, descriptorSetLayout, vertShaderModule, fragShaderModule);
    renderPass = pipeline::createRenderPass(device, swapChainImageFormat);

    graphicsPipeline = pipeline::createGraphicsPipeline(device, pipelineLayout, renderPass, vertShaderModule, fragShaderModule, swapChainExtent);
    
    // vkDestroyShaderModule(device, vertShaderModule, nullptr);
    // vkDestroyShaderModule(device, fragShaderModule, nullptr);

    swapChainFramebuffers.resize(swapChainImageViews.size());

    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        swapChainFramebuffers[i] = framebuffer::createFramebuffer(device, renderPass, swapChainImageViews[i], swapChainExtent);
    }

    commandBuffers = command::createCommandBuffer(device, commandPool);

    imageAvaliableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(swapChainImages.size());

    for (size_t i = 0; i < swapChainImages.size(); i++) {
        renderFinishedSemaphores[i] = std::move(synchronization::craeteSemaphore(device));
    }

    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    presentFences.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        imageAvaliableSemaphores[i] = std::move(synchronization::craeteSemaphore(device));

        inFlightFences[i] = std::move(synchronization::createFence(device));
        presentFences[i] = std::move(synchronization::createFence(device));
    }
}

void graphic::app::mainLoop() {
    logger.Log("mainLoop start", "Info");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(device);
}

void graphic::app::cleanupSwapchain() {
    for (VkFramebuffer framebuffer : swapChainFramebuffers) {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }

    for (VkImageView imageView : swapChainImageViews) {
        vkDestroyImageView(device, imageView, nullptr);
    }
}

void graphic::app::cleanup() {
    for (VkSemaphore semaphore : renderFinishedSemaphores) {
        vkDestroySemaphore(device, semaphore, nullptr);
    }

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(device, imageAvaliableSemaphores[i], nullptr);

        vkDestroyFence(device, inFlightFences[i], nullptr);
        vkDestroyFence(device, presentFences[i], nullptr);
    }

    vkDestroyBuffer(device, indexBuffers.buffer, nullptr);
    vkFreeMemory(device, indexBuffers.bufferMemory, nullptr);

    vkDestroyBuffer(device, vertexBuffers.buffer, nullptr);
    vkFreeMemory(device, vertexBuffers.bufferMemory, nullptr);

    for (size_t i = 0; i < graphic::MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroyBuffer(device, uniformBuffers[i], nullptr);
        vkFreeMemory(device, uniformBufferMemories[i], nullptr);
    }

    vkDestroyDescriptorPool(device, descriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

    vkDestroyCommandPool(device, commandPool, nullptr);

    vkDestroyPipeline(device, graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    vkDestroyRenderPass(device, renderPass, nullptr);

    cleanupSwapchain();
    vkDestroySwapchainKHR(device, swapChain, nullptr);

    vkDestroyDevice(device, nullptr);

    if (debugMessenger) {
        //validationLayer::DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }

    //vkDestroySurfaceKHR(instance, surface, nullptr);
    //vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window);

    glfwTerminate();
}

void graphic::app::run() {
    graphic::app::initWindow();
    graphic::app::initVulkan();
    graphic::app::mainLoop();
    graphic::app::cleanup();
}
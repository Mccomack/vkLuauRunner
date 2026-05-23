module;
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

export module graphic;

import :validationLayer;
import :device;
import :swapchain;
import :pipeline;
import :framebuffer;
import :command;
import :synchronization;
import :buffer;
import :descriptor;
import :uniformBufferObject;
import :common;

import osinfo;
import logger;

import window;

import std;

import vulkan;

export namespace graphic {
    class app;
}

class graphic::app {
    uint32_t currentFrame = 0;

    const std::vector<graphic::Vertex> vertices = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    };

    const std::vector<uint16_t> indices = {0, 1, 2, 2, 3, 0};

    vk::raii::Context context;
    vk::raii::Instance instance = nullptr;
    vk::raii::SurfaceKHR surface = nullptr;

    vk::DebugUtilsMessengerEXT debugMessenger = nullptr;

    vk::raii::PhysicalDevice physicalDevice = nullptr;
    vk::raii::Device device = nullptr;
    vk::raii::Queue graphicsQueue = nullptr;
    vk::raii::Queue presentQueue = nullptr;

    vk::raii::SwapchainKHR swapChain = nullptr;
    std::vector<vk::Image> swapChainImages;
    vk::SurfaceFormatKHR swapChainSurfaceFormat;
    vk::Extent2D swapChainExtent;
    std::vector<vk::raii::ImageView> swapChainImageViews;

    vk::raii::DescriptorSetLayout descriptorSetLayout = nullptr;
    vk::raii::DescriptorPool descriptorPool = nullptr;
    std::vector<vk::raii::DescriptorSet> descriptorSets;

    vk::raii::PipelineLayout pipelineLayout = nullptr;
    vk::raii::RenderPass renderPass = nullptr;
    vk::raii::Pipeline graphicsPipeline = nullptr;

    std::vector<vk::raii::Framebuffer> swapChainFramebuffers;

    vk::raii::CommandPool commandPool = nullptr;

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
    void mainLoop(
        std::function<void()> beforeRenderCallback = nullptr,
        std::function<void()> afterRenderCallback = nullptr
    );

    void cleanupSwapchain();
    void cleanup();

   public:
    window& window_;

    bool framebufferResized = false;

    app(window& _window)
        : window_(_window) {
        initWindow();
        initVulkan();
    };
    ~app() { cleanup(); };

    void run(
        std::function<void()> beforeRenderCallback = nullptr,
        std::function<void()> afterRenderCallback = nullptr
    );
};

namespace {
    Logger logger("graphic");
}

void graphic::app::createInstance() {
    if (validationLayer::enableValidationLayers &&
        !validationLayer::checkValidationLayerSupport(context))
        throw std::runtime_error(
            "validation layers requested, but not available!"
        );

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
    if (validationLayer::enableValidationLayers)
        requiredLayers.assign(
            validationLayer::validationLayers.begin(),
            validationLayer::validationLayers.end()
        );

    auto layerProperties = context.enumerateInstanceLayerProperties();
    auto unsupportedLayer = std::ranges::find_if(
        requiredLayers, [&layerProperties](auto const& requiredLayer) {
            return std::ranges::none_of(
                layerProperties, [requiredLayer](auto const& layerProperty) {
                    return strcmp(layerProperty.layerName, requiredLayer) == 0;
                }
            );
        }
    );

    if (unsupportedLayer != requiredLayers.end())
        throw std::runtime_error(
            std::format(
                "Required layer not supported: {}",
                std::string(*unsupportedLayer)
            )
        );

    createInfo.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size());
    createInfo.ppEnabledLayerNames = requiredLayers.data();

    // vaildation layer support
    if (validationLayer::enableValidationLayers) {
        vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo{};

        validationLayer::populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext =
            (vk::DebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    } else {
        createInfo.pNext = nullptr;
    }

    // instance extensions
    std::vector<const char*> requiredExtensions;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    for (int i = 0; i < glfwExtensionCount; i++)
        requiredExtensions.push_back(glfwExtensions[i]);

    validationLayer::pushRequiredInstanceExtensions(requiredExtensions);

    requiredExtensions.push_back(vk::KHRGetSurfaceCapabilities2ExtensionName);
    requiredExtensions.push_back(vk::EXTSurfaceMaintenance1ExtensionName);

    // macOS Settings
    if (os::os == os::macOS) {
        requiredExtensions.push_back(
            vk::KHRPortabilityEnumerationExtensionName
        );
        requiredExtensions.push_back(
            vk::KHRGetPhysicalDeviceProperties2ExtensionName
        );

        createInfo.flags |=
            vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR;
    }

    // put instance extensions
    createInfo.enabledExtensionCount = (uint32_t)requiredExtensions.size();
    createInfo.ppEnabledExtensionNames = requiredExtensions.data();

    logger.Debug("Enabled instance extensions: ");

    for (const char* extension : requiredExtensions)
        logger.Debugf("\t{}", extension);

    instance = vk::raii::Instance(context, createInfo);
}

void graphic::app::createSurface() {
    VkSurfaceKHR _surface;

    if (glfwCreateWindowSurface(*instance, *window_, nullptr, &_surface) !=
        VK_SUCCESS)
        throw std::runtime_error("Cannot create window surface. ");

    surface = vk::raii::SurfaceKHR(instance, _surface);
}

void graphic::app::recreateSwapchain() {
    auto [width, height] = window_.getFramebufferSize();
    while (width == 0 || height == 0) {
        std::tie(width, height) = window_.getFramebufferSize();
        glfwWaitEvents();
    }

    device.waitIdle();

    cleanupSwapchain();

    swapChain = swapchain::createSwapChain(
        physicalDevice, device, surface, *window_, swapChain
    );

    SwapChainSupportDetails swapchainSupport =
        querySwapChainSupport(physicalDevice, surface);
    swapChainSurfaceFormat =
        swapchain::chooseSwapSurfaceFormat(swapchainSupport.formats);
    swapChainExtent =
        swapchain::chooseSwapExtent(swapchainSupport.capabilities, *window_);

    cleanupSwapchain();

    swapChainImages = swapchain::createImages(device, swapChain);
    swapChainImageViews = swapchain::createImageViews(
        device, swapChainImages, swapChainSurfaceFormat
    );

    for (size_t i = 0; i < swapChainImageViews.size(); i++)
        swapChainFramebuffers.push_back(
            std::move(
                framebuffer::createFramebuffer(
                    device, renderPass, swapChainImageViews[i], swapChainExtent
                )
            )
        );

    if (swapChainImages.size() != renderFinishedSemaphores.size()) {
        // for (size_t i = 0; i < imageAvaliableSemaphores.size(); i++) {
        //     vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
        // }

        renderFinishedSemaphores.clear();

        for (size_t i = 0; i < swapChainImages.size(); i++) {
            renderFinishedSemaphores.push_back(
                std::move(synchronization::craeteSemaphore(device))
            );
        }
    }
}

void graphic::app::drawFrame() {
    {
        auto result = device.waitForFences(
            *presentFences[currentFrame], vk::True, UINT64_MAX
        );
        if (result != vk::Result::eSuccess) {
            throw std::runtime_error("cannot wait for fence(presentFence)");
        }
    }

    auto [result, imageIndex] = swapChain.acquireNextImage(
        UINT64_MAX, *imageAvaliableSemaphores[currentFrame], nullptr
    );
    if (result == vk::Result::eErrorOutOfDateKHR) {
        framebufferResized = false;
        recreateSwapchain();

        return;
    } else if (result == vk::Result::eSuboptimalKHR || framebufferResized) {
        vk::PipelineStageFlags waitStage =
            vk::PipelineStageFlagBits::eBottomOfPipe;
        vk::SubmitInfo submitInfo{
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &*imageAvaliableSemaphores[currentFrame],
            .pWaitDstStageMask = &waitStage
        };

        graphicsQueue.submit(submitInfo, nullptr);
        graphicsQueue.waitIdle();

        framebufferResized = false;
        recreateSwapchain();

        return;
    } else if (result != vk::Result::eSuccess) {
        assert(
            result == vk::Result::eTimeout || result == vk::Result::eNotReady
        );

        throw std::runtime_error("cannot acquire swapchain image");
    }

    device.resetFences(*presentFences[currentFrame]);

    std::ignore = device.waitForFences(
        *inFlightFences[currentFrame], vk::True, UINT64_MAX
    );
    device.resetFences(*inFlightFences[currentFrame]);

    commandBuffers[currentFrame].reset();

    uniformBufferObject::updateUniformBuffer(
        swapChainExtent, uniformBufferMapped[currentFrame]
    );

    RenderStateView renderState{
        .renderPass = renderPass,
        .graphicsPipeline = graphicsPipeline,
        .pipelineLayout = pipelineLayout,
        .vertexBuffer = vertexBuffers.buffer,
        .indexBuffer = indexBuffers.buffer,
        .descriptorSet = descriptorSets[currentFrame],
        .vertices = vertices,
        .indices = indices
    };

    FrameTargetView frameTarget{
        .framebuffer = swapChainFramebuffers[imageIndex],
        .extent = swapChainExtent
    };

    command::recordCommandBuffer(
        commandBuffers[currentFrame], renderState, frameTarget
    );

    vk::Semaphore waitSemaphores[] = {*imageAvaliableSemaphores[currentFrame]};
    vk::PipelineStageFlags waitStages[] = {
        vk::PipelineStageFlagBits::eColorAttachmentOutput
    };
    vk::Semaphore signalSemaphores[] = {*renderFinishedSemaphores[imageIndex]};

    vk::SubmitInfo submitInfo{
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = waitSemaphores,
        .pWaitDstStageMask = waitStages,
        .commandBufferCount = 1,
        .pCommandBuffers = &*commandBuffers[currentFrame],
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = signalSemaphores
    };
    graphicsQueue.submit(submitInfo, *inFlightFences[currentFrame]);

    vk::SwapchainKHR swapChains[] = {*swapChain};

    vk::SwapchainPresentFenceInfoKHR presentFenceInfo{
        .swapchainCount = 1, .pFences = &*presentFences[currentFrame]
    };

    vk::PresentInfoKHR presentInfo{
        .pNext = &presentFenceInfo,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = signalSemaphores,
        .swapchainCount = 1,
        .pSwapchains = swapChains,
        .pImageIndices = &imageIndex,
        .pResults = nullptr
    };

    result = presentQueue.presentKHR(presentInfo);
    if (result == vk::Result::eErrorOutOfDateKHR ||
        result == vk::Result::eSuboptimalKHR || framebufferResized) {
        framebufferResized = false;
        recreateSwapchain();

        return;
    } else if (result != vk::Result::eSuccess)
        throw std::runtime_error("cannot present swapchain image");

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

static void framebufferResizeCallback(
    GLFWwindow* window,
    int width,
    int height
) {
    graphic::app* app =
        reinterpret_cast<graphic::app*>(glfwGetWindowUserPointer(window));

    app->framebufferResized = true;
}

void graphic::app::initWindow() {
    glfwSetFramebufferSizeCallback(*window_, framebufferResizeCallback);
}

void graphic::app::initVulkan() {
    createInstance();

    if (validationLayer::enableValidationLayers)
        debugMessenger = validationLayer::createDebugMessenger(instance);

    createSurface();

    physicalDevice = device::pickPhysicalDevice(instance, surface);

    std::tie(device, graphicsQueue, presentQueue) =
        device::createLogicalDevice(physicalDevice, surface);

    swapChain =
        swapchain::createSwapChain(physicalDevice, device, surface, *window_);
    swapChainImages = swapchain::createImages(device, swapChain);

    SwapChainSupportDetails swapchainSupport =
        querySwapChainSupport(physicalDevice, surface);
    swapChainSurfaceFormat =
        swapchain::chooseSwapSurfaceFormat(swapchainSupport.formats);
    swapChainExtent =
        swapchain::chooseSwapExtent(swapchainSupport.capabilities, *window_);

    swapChainImageViews = swapchain::createImageViews(
        device, swapChainImages, swapChainSurfaceFormat
    );

    commandPool = command::createCommandPool(physicalDevice, device, surface);

    auto [vertShaderModule, fragShaderModule] =
        pipeline::getShaderModule(device);

    vk::DeviceSize size = sizeof(vertices[0]) * vertices.size();
    vk::DeviceSize indSize = sizeof(indices[0]) * indices.size();

    vertexBuffers = buffer::createVertexBuffer(
        physicalDevice,
        device,
        commandPool,
        graphicsQueue,
        vertices.data(),
        size
    );
    indexBuffers = buffer::createIndexBffer(
        physicalDevice,
        device,
        commandPool,
        graphicsQueue,
        indices.data(),
        indSize
    );

    std::tie(uniformBuffers, uniformBufferMemories, uniformBufferMapped) =
        buffer::createUniformBuffer(
            physicalDevice, device, commandPool, graphicsQueue
        );

    descriptorSetLayout = descriptor::createDescriptorSetLayout(device);
    descriptorPool = descriptor::createDescriptorPool(device);
    descriptorSets = descriptor::createDescriptorSets(
        device, descriptorSetLayout, descriptorPool, uniformBuffers
    );

    pipelineLayout = pipeline::createPipelineLayout(
        device, descriptorSetLayout, vertShaderModule, fragShaderModule
    );
    renderPass =
        pipeline::createRenderPass(device, swapChainSurfaceFormat.format);

    graphicsPipeline = pipeline::createGraphicsPipeline(
        device,
        pipelineLayout,
        renderPass,
        vertShaderModule,
        fragShaderModule,
        swapChainExtent
    );

    for (size_t i = 0; i < swapChainImageViews.size(); i++)
        swapChainFramebuffers.push_back(
            std::move(
                framebuffer::createFramebuffer(
                    device, renderPass, swapChainImageViews[i], swapChainExtent
                )
            )
        );

    commandBuffers = command::createCommandBuffer(device, commandPool);

    for (size_t i = 0; i < swapChainImages.size(); i++)
        renderFinishedSemaphores.push_back(
            std::move(synchronization::craeteSemaphore(device))
        );

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        imageAvaliableSemaphores.push_back(
            std::move(synchronization::craeteSemaphore(device))
        );

        inFlightFences.push_back(
            std::move(synchronization::createFence(device))
        );
        presentFences.push_back(
            std::move(synchronization::createFence(device))
        );
    }
}

void graphic::app::mainLoop(
    std::function<void()> beforeRenderCallback,
    std::function<void()> afterRenderCallback
) {
    logger.Log("mainLoop start", "Info");

    while (!glfwWindowShouldClose(*window_)) {
        glfwPollEvents();

        if (beforeRenderCallback)
            beforeRenderCallback();

        drawFrame();

        if (afterRenderCallback)
            afterRenderCallback();
    }

    device.waitIdle();
}

void graphic::app::cleanupSwapchain() {
    swapChainFramebuffers.clear();

    swapChainImageViews.clear();
}

void graphic::app::cleanup() {
    // cleanupSwapchain();

    // glfwDestroyWindow(window);

    // glfwTerminate();
}

void graphic::app::run(
    std::function<void()> beforeRenderCallback,
    std::function<void()> afterRenderCallback
) {
    graphic::app::mainLoop(beforeRenderCallback, afterRenderCallback);
}
module;
#include <GLFW/glfw3.h>

#include <cstring>
#include "vulkan/vulkan.hpp"

export module graphic:validationLayer;
import logger;
import projinfo;

import std;

import vulkan;

export namespace graphic::validationLayer {
    // clang-format off
    constexpr std::array<const char*, 1> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
    // clang-format on

    const bool enableValidationLayers = project::isDebugBuild;

    bool checkValidationLayerSupport(const vk::raii::Context& context);
    void pushRequiredInstanceExtensions(std::vector<const char*>& extensions);

    void populateDebugMessengerCreateInfo(
        vk::DebugUtilsMessengerCreateInfoEXT& createInfo
    );

    vk::DebugUtilsMessengerEXT createDebugMessenger(
        const vk::raii::Instance& instance
    );
};

namespace {
    Logger logger("graphic/validationLayer");
}

bool graphic::validationLayer::checkValidationLayerSupport(
    const vk::raii::Context& context
) {
    std::vector<vk::LayerProperties> availableLayers =
        context.enumerateInstanceLayerProperties();
    std::unordered_set<std::string> availableLayerNames;

    for (const vk::LayerProperties& layerProperties : availableLayers) {
        availableLayerNames.insert(layerProperties.layerName);
    }

    for (const char* layerName : validationLayers) {
        if (!availableLayerNames.contains(layerName))
            return false;
    }

    return true;
}

void graphic::validationLayer::pushRequiredInstanceExtensions(
    std::vector<const char*>& extensions
) {
    if (enableValidationLayers)
        extensions.push_back(vk::EXTDebugUtilsExtensionName);
}

VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(
    vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    vk::DebugUtilsMessageTypeFlagsEXT messageType,
    const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData
) {
    logger.Debugf("validation layer: {}", pCallbackData->pMessage);

    return vk::False;
}

void graphic::validationLayer::populateDebugMessengerCreateInfo(
    vk::DebugUtilsMessengerCreateInfoEXT& createInfo
) {
    vk::DebugUtilsMessageSeverityFlagsEXT severityFlags{
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
        // vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo |
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
    };

    vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags{
        vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
        vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
        vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
    };

    createInfo.messageSeverity = severityFlags,
    createInfo.messageType = messageTypeFlags,
    createInfo.pfnUserCallback = &debugCallback;
}

vk::DebugUtilsMessengerEXT graphic::validationLayer::createDebugMessenger(
    const vk::raii::Instance& instance
) {
    if (!enableValidationLayers)
        return nullptr;

    vk::DebugUtilsMessengerCreateInfoEXT createInfo{};
    populateDebugMessengerCreateInfo(createInfo);

    return instance.createDebugUtilsMessengerEXT(createInfo);
}
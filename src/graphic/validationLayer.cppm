module;
#include <GLFW/glfw3.h>

#include <cstring>

export module graphic:validationLayer;
import logger;

import std;

import vulkan;

export namespace graphic::validationLayer {
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    #ifdef NDEBUG
        const bool enableValidationLayers = false;
    #else
        const bool enableValidationLayers = true;
    #endif

    bool checkValidationLayerSupport();
    void pushRequiredInstanceExtensions(std::vector<const char*> &extensions);

    // static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    //     VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
    //     VkDebugUtilsMessageTypeFlagsEXT messageType, 
    //     const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    //     void* pUserData
    // );

    void populateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT& createInfo);

    vk::DebugUtilsMessengerEXT createDebugMessenger(const vk::raii::Instance& instance);
};

namespace {
    Logger logger("graphic/validationLayer");
}

bool graphic::validationLayer::checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}

void graphic::validationLayer::pushRequiredInstanceExtensions(std::vector<const char*> &extensions) {
    if (enableValidationLayers) {
        extensions.push_back(vk::EXTDebugUtilsExtensionName);
    }
}

VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(
    vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
    vk::DebugUtilsMessageTypeFlagsEXT messageType, 
    const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {
    
    logger.Debugf("validation layer: {}", pCallbackData->pMessage);

    return vk::False;
}

void graphic::validationLayer::populateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT& createInfo) {
    vk::DebugUtilsMessageSeverityFlagsEXT severityFlags{
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | 
        //vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo | 
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

vk::DebugUtilsMessengerEXT graphic::validationLayer::createDebugMessenger(const vk::raii::Instance &instance) {
    if (!enableValidationLayers)
        return nullptr;

    vk::DebugUtilsMessengerCreateInfoEXT createInfo{};
    populateDebugMessengerCreateInfo(createInfo);
   
    return instance.createDebugUtilsMessengerEXT(createInfo);
}
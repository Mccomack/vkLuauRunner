#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <optional>
#include <tuple>
#include <vector>

#include <vulkan/vulkan.hpp>

namespace device {
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
    
    VkPhysicalDevice pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
    std::tuple<VkDevice, VkQueue, VkQueue> createLogicalDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
}

#endif
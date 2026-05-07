module;
#include <cstdint>

export module graphic:device;
import :common;
import osinfo;
import logger;

import std;

import vulkan;

export namespace graphic::device {
    const std::vector<const char*> deviceExtensions = {
        vk::KHRSwapchainExtensionName,
        vk::EXTSwapchainMaintenance1ExtensionName
    };

    bool checkDeviceExtensionSupport(
        const vk::raii::PhysicalDevice& physicalDevice
    );
    bool checkDeviceFeatureSupport(
        const vk::raii::PhysicalDevice& physicalDevice
    );
    bool isDeviceSuitable(
        const vk::raii::PhysicalDevice& physicalDevice,
        const vk::raii::SurfaceKHR& surface
    );

    vk::raii::PhysicalDevice pickPhysicalDevice(
        const vk::raii::Instance& instance,
        const vk::raii::SurfaceKHR& surface
    );
    std::tuple<vk::raii::Device, vk::raii::Queue, vk::raii::Queue>
    createLogicalDevice(
        const vk::raii::PhysicalDevice& physicalDevice,
        const vk::raii::SurfaceKHR& surface
    );
}

namespace {
    Logger logger("graphic/device");
}

bool graphic::device::checkDeviceExtensionSupport(
    const vk::raii::PhysicalDevice& physicalDevice
) {
    std::vector<vk::ExtensionProperties> availableExtensions =
        physicalDevice.enumerateDeviceExtensionProperties();

    std::set<std::string> requiredExtensions(
        deviceExtensions.begin(), deviceExtensions.end()
    );

    for (const vk::ExtensionProperties& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

bool graphic::device::checkDeviceFeatureSupport(
    const vk::raii::PhysicalDevice& physicalDevice
) {
    vk::StructureChain<
        vk::PhysicalDeviceFeatures2,
        vk::PhysicalDeviceVulkan13Features,
        vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT,
        vk::PhysicalDeviceSwapchainMaintenance1FeaturesEXT>
        features = physicalDevice.template getFeatures2<
            vk::PhysicalDeviceFeatures2,
            vk::PhysicalDeviceVulkan13Features,
            vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT,
            vk::PhysicalDeviceSwapchainMaintenance1FeaturesEXT>();

    bool dynamicRendering =
        features.template get<vk::PhysicalDeviceVulkan13Features>()
            .dynamicRendering;
    bool extDynamicState =
        features
            .template get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>()
            .extendedDynamicState;
    bool swapMaintenance1 =
        features
            .template get<vk::PhysicalDeviceSwapchainMaintenance1FeaturesEXT>()
            .swapchainMaintenance1;

    bool supportsRequiredFeatures =
        dynamicRendering && extDynamicState && swapMaintenance1;

    logger.Debugf(
        "dynamicRendering: {}, extendedDynamicState: {}, "
        "swapchainMaintenance1: {}",
        dynamicRendering,
        extDynamicState,
        swapMaintenance1
    );

    return supportsRequiredFeatures;
}

bool graphic::device::isDeviceSuitable(
    const vk::raii::PhysicalDevice& physicalDevice,
    const vk::raii::SurfaceKHR& surface
) {
    auto props = physicalDevice.getProperties();

    logger.Debugf(
        "Device: {}, API: {}.{}.{}",
        props.deviceName.data(),
        vk::apiVersionMajor(props.apiVersion),
        vk::apiVersionMinor(props.apiVersion),
        vk::apiVersionPatch(props.apiVersion)
    );

    if (props.apiVersion < vk::ApiVersion14) {
        return false;
    }

    graphic::QueueFamilyIndices indices =
        graphic::findQueueFamilies(physicalDevice, surface);
    if (!indices.isComplete()) {
        return false;
    }

    bool extensionsSupported = checkDeviceExtensionSupport(physicalDevice);
    bool featuresSupported = checkDeviceFeatureSupport(physicalDevice);
    if (!(extensionsSupported && featuresSupported)) {
        return false;
    }

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        graphic::SwapChainSupportDetails swapChainSupport =
            graphic::querySwapChainSupport(physicalDevice, surface);
        swapChainAdequate = !swapChainSupport.formats.empty() &&
                            !swapChainSupport.presentModes.empty();
    }

    return swapChainAdequate;
}

vk::raii::PhysicalDevice graphic::device::pickPhysicalDevice(
    const vk::raii::Instance& instance,
    const vk::raii::SurfaceKHR& surface
) {
    vk::raii::PhysicalDevice physicalDevice = nullptr;

    std::vector<vk::raii::PhysicalDevice> devices =
        instance.enumeratePhysicalDevices();

    if (devices.empty()) {
        throw std::runtime_error("Cannot find GPUs with vulkan support. ");
    }

    for (const auto& device : devices) {
        if (isDeviceSuitable(device, surface)) {
            physicalDevice = device;
            break;
        }
    }

    if (physicalDevice == nullptr) {
        throw std::runtime_error("Cannot find a suitable device. ");
    }

    return physicalDevice;
}

std::tuple<vk::raii::Device, vk::raii::Queue, vk::raii::Queue> graphic::device::
    createLogicalDevice(
        const vk::raii::PhysicalDevice& physicalDevice,
        const vk::raii::SurfaceKHR& surface
    ) {
    vk::raii::Device device = nullptr;
    vk::raii::Queue graphicsQueue = nullptr;
    vk::raii::Queue presentQueue = nullptr;

    graphic::QueueFamilyIndices indices =
        graphic::findQueueFamilies(physicalDevice, surface);

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {
        indices.graphicsFamily.value(), indices.presentFamily.value()
    };

    // what the hell is this
    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        vk::DeviceQueueCreateInfo queueCreateInfo{
            .queueFamilyIndex = queueFamily,
            .queueCount = 1,
            .pQueuePriorities = &queuePriority
        };

        queueCreateInfos.push_back(queueCreateInfo);
    }

    vk::PhysicalDeviceFeatures deviceFeatures{};
    // add some things later

    vk::PhysicalDeviceSwapchainMaintenance1FeaturesEXT maintenance1Features{
        .swapchainMaintenance1 = vk::True
    };

    vk::DeviceCreateInfo createInfo{};

    createInfo.queueCreateInfoCount =
        static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.pNext = &maintenance1Features;

    std::vector<const char*> extensions(deviceExtensions);

    // macOS settings
    if (os::os == os::macOS) {
        extensions.push_back("VK_KHR_portability_subset");
    }

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

#ifndef NDEBUG
    logger.Debug("Enabled device extensions: ");

    for (const char* extensionName : extensions) {
        logger.Debugf("\t{}", extensionName);
    }
#endif

    device = vk::raii::Device(physicalDevice, createInfo);

    graphicsQueue = vk::raii::Queue(device, indices.graphicsFamily.value(), 0);
    presentQueue = vk::raii::Queue(device, indices.presentFamily.value(), 0);

    std::tuple<vk::raii::Device, vk::raii::Queue, vk::raii::Queue> tup =
        std::make_tuple(
            std::move(device), std::move(graphicsQueue), std::move(presentQueue)
        );

    return tup;
}
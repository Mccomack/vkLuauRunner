module;
#include <GLFW/glfw3.h>
#include "vulkan/vulkan.hpp"

export module graphic:swapchain;
import :common;

import std;

import vulkan;

export namespace graphic::swapchain {
    vk::SurfaceFormatKHR chooseSwapSurfaceFormat(
        const std::vector<vk::SurfaceFormatKHR>& avaliableFormats
    );
    vk::PresentModeKHR chooseSwapPresentMode(
        const std::vector<vk::PresentModeKHR>& avaliablePresentModes
    );
    vk::Extent2D chooseSwapExtent(
        const vk::SurfaceCapabilitiesKHR& capabilities,
        GLFWwindow* window
    );
    uint32_t chooseSwapMinImageCount(const vk::SurfaceCapabilitiesKHR& surfaceCapabilities);

    vk::raii::SwapchainKHR createSwapChain(
        const vk::raii::PhysicalDevice& physicalDevice,
        const vk::raii::Device& device,
        const vk::raii::SurfaceKHR& surface,
        GLFWwindow* window,
        vk::SwapchainKHR oldSwapchain = nullptr
    );
    std::vector<vk::Image> createImages(
        const vk::raii::Device& device,
        const vk::raii::SwapchainKHR& swapChain
    );
    std::vector<vk::raii::ImageView> createImageViews(
        const vk::raii::Device& device,
        const std::vector<vk::Image>& swapChainImages,
        vk::SurfaceFormatKHR swapChainSurfaceFormat
    );
}

vk::SurfaceFormatKHR graphic::swapchain::chooseSwapSurfaceFormat(
    const std::vector<vk::SurfaceFormatKHR>& avaliableFormats
) {
    for (const vk::SurfaceFormatKHR& avaliableFormat : avaliableFormats) {
        if (avaliableFormat.format == vk::Format::eB8G8R8A8Srgb &&
            avaliableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
            return avaliableFormat;
        }
    }

    return avaliableFormats[0];
}

vk::PresentModeKHR graphic::swapchain::chooseSwapPresentMode(
    const std::vector<vk::PresentModeKHR>& avaliablePresentModes
) {
    for (const vk::PresentModeKHR& avaliablePresentMode : avaliablePresentModes) {
        if (avaliablePresentMode == vk::PresentModeKHR::eMailbox) {
            return avaliablePresentMode;
        }
    }

    return vk::PresentModeKHR::eFifo;
}

vk::Extent2D graphic::swapchain::chooseSwapExtent(
    const vk::SurfaceCapabilitiesKHR& capabilities,
    GLFWwindow* window
) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    vk::Extent2D actualExtend{static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

    actualExtend.width = std::clamp(
        actualExtend.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width
    );
    actualExtend.height = std::clamp(
        actualExtend.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height
    );

    return actualExtend;
}

uint32_t graphic::swapchain::chooseSwapMinImageCount(
    const vk::SurfaceCapabilitiesKHR& surfaceCapabilities
) {
    auto minImageCount = std::max(3u, surfaceCapabilities.minImageCount);

    if ((0 < surfaceCapabilities.maxImageCount) &&
        (surfaceCapabilities.maxImageCount < minImageCount)) {
        minImageCount = surfaceCapabilities.maxImageCount;
    }

    return minImageCount;
}

vk::raii::SwapchainKHR graphic::swapchain::createSwapChain(
    const vk::raii::PhysicalDevice& physicalDevice,
    const vk::raii::Device& device,
    const vk::raii::SurfaceKHR& surface,
    GLFWwindow* window,
    vk::SwapchainKHR oldSwapchain
) {
    graphic::QueueFamilyIndices indices = graphic::findQueueFamilies(physicalDevice, surface);
    graphic::SwapChainSupportDetails swapChainSupport =
        graphic::querySwapChainSupport(physicalDevice, surface);

    vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    vk::PresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    vk::Extent2D extent = chooseSwapExtent(swapChainSupport.capabilities, window);

    uint32_t imageCount = chooseSwapMinImageCount(swapChainSupport.capabilities);

    vk::SwapchainCreateInfoKHR createInfo{
        .surface = *surface,
        .minImageCount = imageCount,
        .imageFormat = surfaceFormat.format,
        .imageColorSpace = surfaceFormat.colorSpace,
        .imageExtent = extent,
        .imageArrayLayers = 1,
        .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
        .imageSharingMode = vk::SharingMode::eExclusive,
        .preTransform = swapChainSupport.capabilities.currentTransform,
        .compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
        .presentMode = presentMode,
        .clipped = true,
        .oldSwapchain = oldSwapchain
    };

    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }

    vk::raii::SwapchainKHR swapChain(device, createInfo);

    return swapChain;
}

std::vector<vk::Image> graphic::swapchain::createImages(
    const vk::raii::Device& device,
    const vk::raii::SwapchainKHR& swapChain
) {
    std::vector<vk::Image> swapChainImages = swapChain.getImages();

    return swapChainImages;
}

std::vector<vk::raii::ImageView> graphic::swapchain::createImageViews(
    const vk::raii::Device& device,
    const std::vector<vk::Image>& swapChainImages,
    vk::SurfaceFormatKHR swapChainSurfaceFormat
) {
    std::vector<vk::raii::ImageView> imageViews;

    vk::ComponentMapping components{
        .r = vk::ComponentSwizzle::eIdentity,
        .g = vk::ComponentSwizzle::eIdentity,
        .b = vk::ComponentSwizzle::eIdentity,
        .a = vk::ComponentSwizzle::eIdentity
    };

    vk::ImageSubresourceRange subresourceRange{
        .aspectMask = vk::ImageAspectFlagBits::eColor,
        .baseMipLevel = 0,
        .levelCount = 1,
        .baseArrayLayer = 0,
        .layerCount = 1
    };

    vk::ImageViewCreateInfo createInfo{
        .viewType = vk::ImageViewType::e2D,
        .format = swapChainSurfaceFormat.format,

        .components = components,

        .subresourceRange = subresourceRange,
    };

    for (const vk::Image& image : swapChainImages) {
        createInfo.image = image;

        imageViews.emplace_back(device, createInfo);
    }

    return imageViews;
}
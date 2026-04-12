module;
#include "glm/ext/vector_float3.hpp"
#include <array>
#include <cstddef>
#include <span>
#include <stdexcept>
#include <vector>
#include <optional>
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
#include <vulkan/vulkan_core.h>

export module graphic:common;

import vulkan;

export namespace graphic {
    struct SwapChainSupportDetails {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presentModes;
    };

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct Vertex {
        glm::vec2 pos;
        glm::vec3 color;

        static vk::VertexInputBindingDescription getBindingDescription() {
            vk::VertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = vk::VertexInputRate::eVertex;

            return bindingDescription;
        }

        static std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescriptions() {
            std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions{};
            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = vk::Format::eR32G32Sfloat;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
            attributeDescriptions[1].offset = offsetof(Vertex, color);

            return attributeDescriptions;
        }
    };

    struct UniformBufferObject {
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };

    struct RenderStateView {
        const vk::raii::RenderPass& renderPass;
        const vk::raii::Pipeline& graphicsPipeline;
        const vk::raii::PipelineLayout& pipelineLayout;
        const vk::raii::Buffer& vertexBuffer;
        const vk::raii::Buffer& indexBuffer;
        const vk::raii::DescriptorSet& descriptorSet;

        std::span<const Vertex> vertices;
        std::span<const uint16_t> indices;
    };

    struct FrameTargetView {
        const vk::raii::Framebuffer& framebuffer;
        vk::Extent2D extent;
    };

    struct Buffer {
        vk::raii::Buffer buffer;
        vk::raii::DeviceMemory bufferMemory;
    };

    const int MAX_FRAMES_IN_FLIGHT = 2;

    QueueFamilyIndices findQueueFamilies(const vk::raii::PhysicalDevice& physicalDevice, const vk::raii::SurfaceKHR& surface);
    SwapChainSupportDetails querySwapChainSupport(const vk::raii::PhysicalDevice& physicalDevice, const vk::raii::SurfaceKHR& surface);

    uint32_t findMemoryType(const vk::raii::PhysicalDevice& physicalDevice, uint32_t typeFilter, vk::MemoryPropertyFlags properties);

    vk::Viewport newViewport(vk::Extent2D swapchainExtent);
    vk::Rect2D newScissor(vk::Extent2D swapchainExtent);
}

graphic::QueueFamilyIndices graphic::findQueueFamilies(const vk::raii::PhysicalDevice& physicalDevice, const vk::raii::SurfaceKHR& surface) {
    graphic::QueueFamilyIndices indices;

    std::vector<vk::QueueFamilyProperties> queueFamilies = physicalDevice.getQueueFamilyProperties();

    int i = 0;
    for (const vk::QueueFamilyProperties& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
            indices.graphicsFamily = i;
        }

        vk::Bool32 presentSupport = physicalDevice.getSurfaceSupportKHR(i, surface);

        if (presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}

graphic::SwapChainSupportDetails graphic::querySwapChainSupport(const vk::raii::PhysicalDevice& physicalDevice, const vk::raii::SurfaceKHR& surface) {
    SwapChainSupportDetails details{
        .capabilities = physicalDevice.getSurfaceCapabilitiesKHR(*surface),
        .formats = physicalDevice.getSurfaceFormatsKHR(*surface),
        .presentModes = physicalDevice.getSurfacePresentModesKHR(*surface)
    };

    return details;
}

uint32_t graphic::findMemoryType(const vk::raii::PhysicalDevice& physicalDevice, uint32_t typeFilter, vk::MemoryPropertyFlags properties) {
    vk::PhysicalDeviceMemoryProperties memProperties = physicalDevice.getMemoryProperties();

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("cannot find SIUUUUUUUitable memory type");
}

vk::Viewport graphic::newViewport(vk::Extent2D swapchainExtent) {
    vk::Viewport viewport{
        .x = 0.0f,
        .y = 0.0f,
        .width = static_cast<float>(swapchainExtent.width),
        .height = static_cast<float>(swapchainExtent.height),
        .minDepth = 0.0f,
        .maxDepth = 1.0f,
    };

    return viewport;
}

vk::Rect2D graphic::newScissor(vk::Extent2D swapchainExtent) {
    vk::Rect2D scissor{
        .offset = {0, 0},
        .extent = swapchainExtent,
    };

    return scissor;
}
module;

#include <stdexcept>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>

export module graphic:framebuffer;
import vulkan;

namespace framebuffer {
    vk::raii::Framebuffer createFramebuffer(const vk::raii::Device& device, const vk::raii::RenderPass& renderPass, const vk::raii::ImageView& imageView, vk::Extent2D extent);
}

vk::raii::Framebuffer framebuffer::createFramebuffer(const vk::raii::Device& device, const vk::raii::RenderPass& renderPass, const vk::raii::ImageView& imageView, vk::Extent2D extent) {
    vk::FramebufferCreateInfo framebufferInfo{
        .renderPass = renderPass,
        .attachmentCount = 1,
        .pAttachments = &imageView,
        .width = extent.width,
        .height = extent.height,
        .layers = 1
    };

    return vk::raii::Framebuffer(device, framebufferInfo);
}
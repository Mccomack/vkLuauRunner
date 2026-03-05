module;

#include <stdexcept>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>

export module graphic:framebuffer;

namespace framebuffer {
    VkFramebuffer createFramebuffer(VkDevice device, VkRenderPass renderPass, VkImageView imageView, VkExtent2D extent);
}

VkFramebuffer framebuffer::createFramebuffer(VkDevice device, VkRenderPass renderPass, VkImageView imageView, VkExtent2D extent) {
    VkFramebuffer framebuffer;

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = renderPass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = &imageView;
    framebufferInfo.width = extent.width;
    framebufferInfo.height = extent.height;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &framebuffer) != VK_SUCCESS) {
        throw std::runtime_error("cannot create VkFramebuffer");
    }

    return framebuffer;
}
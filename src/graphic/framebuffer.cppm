module;

export module graphic:framebuffer;
import vulkan;

namespace graphic::framebuffer {
    vk::raii::Framebuffer createFramebuffer(const vk::raii::Device& device, const vk::raii::RenderPass& renderPass, const vk::raii::ImageView& imageView, vk::Extent2D extent);
}

vk::raii::Framebuffer graphic::framebuffer::createFramebuffer(const vk::raii::Device& device, const vk::raii::RenderPass& renderPass, const vk::raii::ImageView& imageView, vk::Extent2D extent) {
    vk::FramebufferCreateInfo framebufferInfo{
        .renderPass = renderPass,
        .attachmentCount = 1,
        .pAttachments = &*imageView,
        .width = extent.width,
        .height = extent.height,
        .layers = 1
    };

    return vk::raii::Framebuffer(device, framebufferInfo);
}
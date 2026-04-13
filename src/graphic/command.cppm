module;
#include <cstdint>
#include <span>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>

export module graphic:command;
import :common;

export namespace command {
    vk::raii::CommandPool createCommandPool(const vk::raii::PhysicalDevice& physicalDevice, const vk::raii::Device& device, const vk::raii::SurfaceKHR& surface);
    std::vector<vk::raii::CommandBuffer> createCommandBuffer(const vk::raii::Device& device, const vk::raii::CommandPool& commandPool);

    void recordCommandBuffer(const vk::raii::CommandBuffer& commandBuffer, const graphic::RenderStateView& renderState, const graphic::FrameTargetView& frameTarget);
}

vk::raii::CommandPool command::createCommandPool(const vk::raii::PhysicalDevice& physicalDevice, const vk::raii::Device& device, const vk::raii::SurfaceKHR& surface) {
    graphic::QueueFamilyIndices queueFamilyIndices = graphic::findQueueFamilies(physicalDevice, surface);

    vk::CommandPoolCreateInfo commandPoolInfo{
        .flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
        .queueFamilyIndex = queueFamilyIndices.graphicsFamily.value()
    };

    vk::raii::CommandPool commandPool(device, commandPoolInfo);

    return commandPool;
}

std::vector<vk::raii::CommandBuffer> command::createCommandBuffer(const vk::raii::Device& device, const vk::raii::CommandPool& commandPool) {
    vk::CommandBufferAllocateInfo allocInfo{
        .commandPool = *commandPool,
        .level = vk::CommandBufferLevel::ePrimary,
        .commandBufferCount = static_cast<uint32_t>(graphic::MAX_FRAMES_IN_FLIGHT)
    };

    return vk::raii::CommandBuffers(device, allocInfo);
}

void command::recordCommandBuffer(const vk::raii::CommandBuffer& commandBuffer, const graphic::RenderStateView& renderState, const graphic::FrameTargetView& frameTarget) {
    VkRenderPass renderPass = renderState.renderPass;
    VkPipeline graphicsPipeline = renderState.graphicsPipeline;
    VkPipelineLayout pipelineLayout = renderState.pipelineLayout;
    VkBuffer vertexBuffer = renderState.vertexBuffer;
    VkBuffer indexBuffer = renderState.indexBuffer;
    VkDescriptorSet descriptorSet = renderState.descriptorSet;
    std::span<const graphic::Vertex> vertices = renderState.vertices;
    std::span<const uint16_t> indices = renderState.indices;

    VkFramebuffer framebuffer = frameTarget.framebuffer;
    VkExtent2D extent = frameTarget.extent;

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("cannot begin record command buffer");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = framebuffer;
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = extent;

    VkClearValue clearColor{{0.0f, 0.0f, 0.0f, 1.0f}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

        VkBuffer vertexBuffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);

        VkViewport viewport = graphic::newViewport(extent);
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor = graphic::newScissor(extent);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);

        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("cannot record VkCommandBuffer");
    }
}
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
    VkCommandPool createCommandPool(VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface);
    std::vector<VkCommandBuffer> createCommandBuffer(VkDevice device, VkCommandPool commandPool);

    void recordCommandBuffer(VkCommandBuffer commandBuffer, const graphic::RenderState& renderState, const graphic::FrameTarget& frameTarget);
}

VkCommandPool command::createCommandPool(VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface) {
    graphic::QueueFamilyIndices queueFamilyIndices = graphic::findQueueFamilies(physicalDevice, surface);

    VkCommandPool commandPool;

    VkCommandPoolCreateInfo commandPoolInfo{};
    commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    commandPoolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(device, &commandPoolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("cannot create VkCommandPool");
    }

    return commandPool;
}

std::vector<VkCommandBuffer> command::createCommandBuffer(VkDevice device, VkCommandPool commandPool) {
    std::vector<VkCommandBuffer> commandBuffers(graphic::MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandBufferCount = static_cast<uint32_t>(graphic::MAX_FRAMES_IN_FLIGHT);
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("cannot create VkCommandBuffer");
    }

    return commandBuffers;
}

void command::recordCommandBuffer(VkCommandBuffer commandBuffer, const graphic::RenderState& renderState, const graphic::FrameTarget& frameTarget) {
    VkRenderPass renderPass = renderState.renderPass;
    VkPipeline graphicsPipeline = renderState.graphicsPipeline;
    VkBuffer vertexBuffer = renderState.vertexBuffer;
    VkBuffer indexBuffer = renderState.indexBuffer;
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

        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("cannot record VkCommandBuffer");
    }
}
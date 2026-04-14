module;
#include <cstdint>
#include <span>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>

export module graphic:command;
import :common;
import vulkan;

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
    // VkRenderPass renderPass = renderState.renderPass;
    // VkPipeline graphicsPipeline = renderState.graphicsPipeline;
    // VkPipelineLayout pipelineLayout = renderState.pipelineLayout;
    // VkBuffer vertexBuffer = renderState.vertexBuffer;
    // VkBuffer indexBuffer = renderState.indexBuffer;
    // VkDescriptorSet descriptorSet = renderState.descriptorSet;
    // std::span<const graphic::Vertex> vertices = renderState.vertices;
    // std::span<const uint16_t> indices = renderState.indices;

    // VkFramebuffer framebuffer = frameTarget.framebuffer;
    // VkExtent2D extent = frameTarget.extent;

    commandBuffer.begin(vk::CommandBufferBeginInfo{});

    vk::ClearValue clearColor{ vk::ClearColorValue{0.0f, 0.0f, 0.0f, 1.0f} };

    vk::RenderPassBeginInfo renderPassInfo{
        .renderPass = *renderState.renderPass,
        .framebuffer = *frameTarget.framebuffer,

        .renderArea = vk::Rect2D{
            .offset = {0, 0},
            .extent = frameTarget.extent
        },

        .clearValueCount = 1,
        .pClearValues = &clearColor
    };

    commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

        commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *renderState.graphicsPipeline);

        vk::DeviceSize offsets[] = {0};
        commandBuffer.bindVertexBuffers(0, {*renderState.vertexBuffer}, offsets);
        //vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

        commandBuffer.bindIndexBuffer({*renderState.indexBuffer}, 0, vk::IndexTypeValue<decltype(renderState.indices)::value_type>::value);
        //vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);

        vk::Viewport viewport = graphic::newViewport(frameTarget.extent);
        commandBuffer.setViewport(0, viewport);
        //vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        vk::Rect2D scissor = graphic::newScissor(frameTarget.extent);
        commandBuffer.setScissor(0, scissor);
        //vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, *renderState.pipelineLayout, 0, {*renderState.descriptorSet}, {});
        //vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);

        commandBuffer.drawIndexed(static_cast<uint32_t>(renderState.indices.size()), 1, 0, 0, 0);
        //vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

    commandBuffer.endRenderPass();
    //vkCmdEndRenderPass(commandBuffer);

    commandBuffer.end();
    // if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
    //     throw std::runtime_error("cannot record VkCommandBuffer");
    // }
}
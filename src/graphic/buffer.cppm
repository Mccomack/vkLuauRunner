module;
#include <cstring>

export module graphic:buffer;
import :common;
import logger;

import std;

import vulkan;

namespace {
    Logger logger("graphic/buffer");
}

export namespace graphic::buffer {
    graphic::Buffer createBuffer(
        const vk::raii::PhysicalDevice& physicalDevice,
        const vk::raii::Device& device,
        vk::DeviceSize size,
        vk::BufferUsageFlags usage,
        vk::MemoryPropertyFlags memProperties
    );

    void copyBuffer(
        const vk::raii::Device& device,
        const vk::raii::CommandPool& commandPool,
        const vk::raii::Queue& graphicsQueue,
        const vk::raii::Buffer& srcBuffer,
        const vk::raii::Buffer& dstBuffer,
        vk::DeviceSize size
    );

    graphic::Buffer createVertexBuffer(
        const vk::raii::PhysicalDevice& physicalDevice,
        const vk::raii::Device& device,
        const vk::raii::CommandPool& commandPool,
        const vk::raii::Queue& graphicsQueue,
        const void* bufData,
        vk::DeviceSize size
    );
    graphic::Buffer createIndexBffer(
        const vk::raii::PhysicalDevice& physicalDevice,
        const vk::raii::Device& device,
        const vk::raii::CommandPool& commandPool,
        const vk::raii::Queue& graphicsQueue,
        const void* bufData,
        vk::DeviceSize size
    );
    std::tuple<
        std::vector<vk::raii::Buffer>,
        std::vector<vk::raii::DeviceMemory>,
        std::vector<void*>>
    createUniformBuffer(
        const vk::raii::PhysicalDevice& physicalDevice,
        const vk::raii::Device& device,
        const vk::raii::CommandPool& commandPool,
        const vk::raii::Queue& graphicsQueue
    );
}

graphic::Buffer graphic::buffer::createBuffer(
    const vk::raii::PhysicalDevice& physicalDevice,
    const vk::raii::Device& device,
    vk::DeviceSize size,
    vk::BufferUsageFlags usage,
    vk::MemoryPropertyFlags memProperties
) {
    vk::BufferCreateInfo bufferInfo{
        .size = size, .usage = usage, .sharingMode = vk::SharingMode::eExclusive
    };

    vk::raii::Buffer buffer(device, bufferInfo);

    vk::MemoryRequirements memRequirements = buffer.getMemoryRequirements();

    vk::MemoryAllocateInfo allocInfo{
        .allocationSize = memRequirements.size,
        .memoryTypeIndex = graphic::findMemoryType(
            physicalDevice, memRequirements.memoryTypeBits, memProperties
        )
    };

    vk::raii::DeviceMemory bufferMemory(device, allocInfo);
    buffer.bindMemory(bufferMemory, 0);

    return graphic::Buffer{std::move(buffer), std::move(bufferMemory)};
}

void graphic::buffer::copyBuffer(
    const vk::raii::Device& device,
    const vk::raii::CommandPool& commandPool,
    const vk::raii::Queue& graphicsQueue,
    const vk::raii::Buffer& srcBuffer,
    const vk::raii::Buffer& dstBuffer,
    vk::DeviceSize size
) {
    vk::CommandBufferAllocateInfo allocInfo{
        .commandPool = commandPool,
        .level = vk::CommandBufferLevel::ePrimary,
        .commandBufferCount = 1
    };
    vk::raii::CommandBuffer commandBuffer =
        std::move(device.allocateCommandBuffers(allocInfo).front());

    vk::CommandBufferBeginInfo beginInfo{
        .flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit
    };
    commandBuffer.begin(beginInfo);

    vk::BufferCopy copyRegion{.srcOffset = 0, .dstOffset = 0, .size = size};
    commandBuffer.copyBuffer(*srcBuffer, *dstBuffer, copyRegion);

    commandBuffer.end();

    vk::SubmitInfo submitInfo{
        .commandBufferCount = 1, .pCommandBuffers = &*commandBuffer
    };

    graphicsQueue.submit(submitInfo, nullptr);
    graphicsQueue.waitIdle();
}

graphic::Buffer graphic::buffer::createVertexBuffer(
    const vk::raii::PhysicalDevice& physicalDevice,
    const vk::raii::Device& device,
    const vk::raii::CommandPool& commandPool,
    const vk::raii::Queue& graphicsQueue,
    const void* bufData,
    vk::DeviceSize size
) {
    graphic::Buffer stagingBuffers = createBuffer(
        physicalDevice,
        device,
        size,
        vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostVisible |
            vk::MemoryPropertyFlagBits::eHostCoherent
    );

    void* data = stagingBuffers.bufferMemory.mapMemory(0, size);
    memcpy(data, bufData, (size_t)size);
    stagingBuffers.bufferMemory.unmapMemory();

    graphic::Buffer vertexBuffers = createBuffer(
        physicalDevice,
        device,
        size,
        vk::BufferUsageFlagBits::eTransferDst |
            vk::BufferUsageFlagBits::eVertexBuffer,
        vk::MemoryPropertyFlagBits::eDeviceLocal
    );

    copyBuffer(
        device,
        commandPool,
        graphicsQueue,
        stagingBuffers.buffer,
        vertexBuffers.buffer,
        size
    );

    return vertexBuffers;
}

graphic::Buffer graphic::buffer::createIndexBffer(
    const vk::raii::PhysicalDevice& physicalDevice,
    const vk::raii::Device& device,
    const vk::raii::CommandPool& commandPool,
    const vk::raii::Queue& graphicsQueue,
    const void* indData,
    vk::DeviceSize size
) {
    graphic::Buffer stagingBuffers = createBuffer(
        physicalDevice,
        device,
        size,
        vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostVisible |
            vk::MemoryPropertyFlagBits::eHostCoherent
    );

    void* data = stagingBuffers.bufferMemory.mapMemory(0, size);
    memcpy(data, indData, (size_t)size);
    stagingBuffers.bufferMemory.unmapMemory();

    graphic::Buffer indexBuffer = createBuffer(
        physicalDevice,
        device,
        size,
        vk::BufferUsageFlagBits::eTransferDst |
            vk::BufferUsageFlagBits::eIndexBuffer,
        vk::MemoryPropertyFlagBits::eDeviceLocal
    );

    copyBuffer(
        device,
        commandPool,
        graphicsQueue,
        stagingBuffers.buffer,
        indexBuffer.buffer,
        size
    );

    return indexBuffer;
}

std::tuple<
    std::vector<vk::raii::Buffer>,
    std::vector<vk::raii::DeviceMemory>,
    std::vector<void*>>
graphic::buffer::createUniformBuffer(
    const vk::raii::PhysicalDevice& physicalDevice,
    const vk::raii::Device& device,
    const vk::raii::CommandPool& commandPool,
    const vk::raii::Queue& graphicsQueue
) {
    std::vector<vk::raii::Buffer> uniformBuffers;
    std::vector<vk::raii::DeviceMemory> uniformBufferMemories;
    std::vector<void*> uniformBufferMapped;

    for (size_t i = 0; i < graphic::MAX_FRAMES_IN_FLIGHT; i++) {
        vk::DeviceSize bufferSize = sizeof(graphic::UniformBufferObject);

        graphic::Buffer uniformBuffer = createBuffer(
            physicalDevice,
            device,
            bufferSize,
            vk::BufferUsageFlagBits::eUniformBuffer,
            vk::MemoryPropertyFlagBits::eHostVisible |
                vk::MemoryPropertyFlagBits::eHostCoherent
        );

        uniformBuffers.push_back(std::move(uniformBuffer.buffer));
        uniformBufferMemories.push_back(std::move(uniformBuffer.bufferMemory));
        uniformBufferMapped.push_back(
            uniformBufferMemories[i].mapMemory(0, bufferSize)
        );
    }

    return std::make_tuple(
        std::move(uniformBuffers),
        std::move(uniformBufferMemories),
        uniformBufferMapped
    );
}
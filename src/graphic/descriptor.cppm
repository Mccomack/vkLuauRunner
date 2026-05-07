module;
#include <cstdint>

export module graphic:descriptor;
import :common;

import std;

import vulkan;

export namespace graphic::descriptor {
    vk::raii::DescriptorSetLayout createDescriptorSetLayout(
        const vk::raii::Device& device
    );
    vk::raii::DescriptorPool createDescriptorPool(
        const vk::raii::Device& device
    );

    std::vector<vk::raii::DescriptorSet> createDescriptorSets(
        const vk::raii::Device& device,
        const vk::raii::DescriptorSetLayout& descriptorSetLayout,
        const vk::raii::DescriptorPool& descriptorPool,
        const std::vector<vk::raii::Buffer>& uniformBuffers
    );
}

vk::raii::DescriptorSetLayout graphic::descriptor::createDescriptorSetLayout(
    const vk::raii::Device& device
) {
    vk::DescriptorSetLayoutBinding uboLayoutBinding{
        .binding = 0,
        .descriptorType = vk::DescriptorType::eUniformBuffer,
        .descriptorCount = 1,
        .stageFlags = vk::ShaderStageFlagBits::eVertex,
        .pImmutableSamplers = nullptr
    };

    vk::DescriptorSetLayoutCreateInfo layoutInfo{
        .bindingCount = 1, .pBindings = &uboLayoutBinding
    };

    return vk::raii::DescriptorSetLayout(device, layoutInfo);
}

vk::raii::DescriptorPool graphic::descriptor::createDescriptorPool(
    const vk::raii::Device& device
) {
    vk::DescriptorPoolSize poolSize{
        .type = vk::DescriptorType::eUniformBuffer,
        .descriptorCount = static_cast<uint32_t>(graphic::MAX_FRAMES_IN_FLIGHT)
    };

    vk::DescriptorPoolCreateInfo poolInfo{
        .flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
        .maxSets = static_cast<uint32_t>(graphic::MAX_FRAMES_IN_FLIGHT),
        .poolSizeCount = 1,
        .pPoolSizes = &poolSize
    };

    return vk::raii::DescriptorPool(device, poolInfo);
}

std::vector<vk::raii::DescriptorSet> graphic::descriptor::createDescriptorSets(
    const vk::raii::Device& device,
    const vk::raii::DescriptorSetLayout& descriptorSetLayout,
    const vk::raii::DescriptorPool& descriptorPool,
    const std::vector<vk::raii::Buffer>& uniformBuffers
) {
    std::vector<vk::DescriptorSetLayout> layouts(
        graphic::MAX_FRAMES_IN_FLIGHT, *descriptorSetLayout
    );

    vk::DescriptorSetAllocateInfo allocInfo{
        .descriptorPool = descriptorPool,
        .descriptorSetCount =
            static_cast<uint32_t>(graphic::MAX_FRAMES_IN_FLIGHT),
        .pSetLayouts = layouts.data()
    };

    std::vector<vk::raii::DescriptorSet> descriptorSets =
        device.allocateDescriptorSets(allocInfo);

    for (size_t i = 0; i < graphic::MAX_FRAMES_IN_FLIGHT; i++) {
        vk::DescriptorBufferInfo bufferInfo{
            .buffer = uniformBuffers[i],
            .offset = 0,
            .range = sizeof(graphic::UniformBufferObject)
        };

        vk::WriteDescriptorSet descriptorWrite{
            .dstSet = descriptorSets[i],
            .dstBinding = 0,
            .dstArrayElement = 0,

            .descriptorCount = 1,
            .descriptorType = vk::DescriptorType::eUniformBuffer,

            .pImageInfo = nullptr,
            .pBufferInfo = &bufferInfo,
            .pTexelBufferView = nullptr
        };

        device.updateDescriptorSets(descriptorWrite, {});
    }

    return descriptorSets;
}
module;
#include <stdexcept>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>

export module graphic:synchronization;

export namespace synchronization {
    VkSemaphore craeteSemaphore(VkDevice device);
    VkFence createFence(VkDevice device);
}

VkSemaphore synchronization::craeteSemaphore(VkDevice device) {
    VkSemaphore semaphore;

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &semaphore) != VK_SUCCESS) {
        throw std::runtime_error("cannot create VkSemaphore");
    }

    return semaphore;
}

VkFence synchronization::createFence(VkDevice device) {
    VkFence fence;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if (vkCreateFence(device, &fenceInfo, nullptr, &fence) != VK_SUCCESS) {
        throw std::runtime_error("cannot create VkFence");
    }

    return fence;
}
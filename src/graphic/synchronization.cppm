module;

export module graphic:synchronization;
import vulkan;

export namespace synchronization {
    vk::raii::Semaphore craeteSemaphore(const vk::raii::Device& device);
    vk::raii::Fence createFence(const vk::raii::Device& device);
}

vk::raii::Semaphore synchronization::craeteSemaphore(const vk::raii::Device& device) {
    vk::SemaphoreCreateInfo semaphoreInfo{};

    vk::raii::Semaphore semaphore(device, semaphoreInfo);

    return semaphore;
}

vk::raii::Fence synchronization::createFence(const vk::raii::Device& device) {
    vk::FenceCreateInfo fenceInfo{ .flags = vk::FenceCreateFlagBits::eSignaled };

    vk::raii::Fence fence(device, fenceInfo);

    return fence;
}
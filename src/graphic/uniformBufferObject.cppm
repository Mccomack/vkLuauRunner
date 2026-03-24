module;
#include "glm/ext/matrix_clip_space.hpp"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vulkan/vulkan.hpp>

export module graphic:uniformBufferObject;
import :common;

import std;

export namespace uniformBufferObject {
    void updateUniformBuffer(VkExtent2D swapchainExtent, void* uniformBufferMapped);
}

void uniformBufferObject::updateUniformBuffer(VkExtent2D swapchainExtent, void* uniformBufferMapped) {
    static std::chrono::time_point startTime = std::chrono::high_resolution_clock::now();

    std::chrono::time_point currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    graphic::UniformBufferObject ubo{
        glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        glm::perspective(glm::radians(45.0f), swapchainExtent.width / (float)swapchainExtent.height, 0.1f, 10.0f)
    };

    ubo.proj[1][1] *= -1;

    memcpy(uniformBufferMapped, &ubo, sizeof(ubo));
}
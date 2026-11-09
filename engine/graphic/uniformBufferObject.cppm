module;
#define GLM_FORCE_RADIANS
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

export module graphic:uniformBufferObject;
import :common;

import std;

import vulkan;

namespace ch = std::chrono;

export namespace graphic::uniformBufferObject {
    void updateUniformBuffer(
        vk::Extent2D swapchainExtent,
        void* uniformBufferMapped
    );
}

void graphic::uniformBufferObject::updateUniformBuffer(
    vk::Extent2D swapchainExtent,
    void* uniformBufferMapped
) {
    static ch::time_point startTime = ch::high_resolution_clock::now();

    ch::time_point currentTime = ch::high_resolution_clock::now();
    float time =
        ch::duration<float, ch::seconds::period>(currentTime - startTime)
            .count();

    graphic::UniformBufferObject ubo{
        glm::rotate(
            glm::mat4(1.0f),
            time * glm::radians(90.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        ),
        glm::lookAt(
            glm::vec3(2.0f, 2.0f, 2.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        ),
        glm::perspective(
            glm::radians(45.0f),
            swapchainExtent.width / (float)swapchainExtent.height,
            0.1f,
            10.0f
        )
    };

    ubo.proj[1][1] *= -1;

    memcpy(uniformBufferMapped, &ubo, sizeof(ubo));
}
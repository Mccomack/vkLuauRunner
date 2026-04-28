module;

#include <glm/glm.hpp>

export module graphic:pipeline;
import :common;
import osinfo;

import std;

import vulkan;

export namespace graphic::pipeline {
    vk::raii::ShaderModule createShaderModule(const vk::raii::Device& device, const std::vector<char>& code);
    std::tuple<vk::raii::ShaderModule, vk::raii::ShaderModule> getShaderModule(const vk::raii::Device& device);

    vk::raii::PipelineLayout createPipelineLayout(const vk::raii::Device& device, const vk::raii::DescriptorSetLayout& descriptorSetLayout, const vk::raii::ShaderModule& vertShaderModule, const vk::raii::ShaderModule& fragShaderModule);
    vk::raii::RenderPass createRenderPass(const vk::raii::Device& device, vk::Format swapChainImageFormat);

    vk::raii::Pipeline createGraphicsPipeline(const vk::raii::Device& device, const vk::raii::PipelineLayout& pipelineLayout, const vk::raii::RenderPass& renderPass, const vk::raii::ShaderModule& vertShaderModule, const vk::raii::ShaderModule& fragShaderModule, vk::Extent2D extent);
}

static std::vector<char> readFile(const std::filesystem::path& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error(std::format("cannot open file \"{}\"", filename.generic_string()));
    }

    size_t fileSize = file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

vk::raii::ShaderModule graphic::pipeline::createShaderModule(const vk::raii::Device& device, const std::vector<char> &code) {
    vk::ShaderModuleCreateInfo createInfo{
        .codeSize = code.size(),
        .pCode = reinterpret_cast<const uint32_t*>(code.data())
    };

    return vk::raii::ShaderModule(device, createInfo);
}

std::tuple<vk::raii::ShaderModule, vk::raii::ShaderModule> graphic::pipeline::getShaderModule(const vk::raii::Device& device) {
    std::filesystem::path shaderPath = os::appPath / "shaders";

    std::vector<char> shaderCode = readFile(shaderPath / "shader.slang.spv");

    vk::raii::ShaderModule vertShaderModule = createShaderModule(device, shaderCode);
    vk::raii::ShaderModule fragShaderModule = createShaderModule(device, shaderCode);

    return std::make_tuple(std::move(vertShaderModule), std::move(fragShaderModule));
}

vk::raii::PipelineLayout graphic::pipeline::createPipelineLayout(const vk::raii::Device& device, const vk::raii::DescriptorSetLayout& descriptorSetLayout, const vk::raii::ShaderModule& vertShaderModule, const vk::raii::ShaderModule& fragShaderModule) {
    vk::PipelineLayoutCreateInfo pipelineLayoutInfo{
        .setLayoutCount = 1,
        .pSetLayouts = &*descriptorSetLayout,
        .pushConstantRangeCount = 0,
        .pPushConstantRanges = nullptr
    };

    return vk::raii::PipelineLayout(device, pipelineLayoutInfo);
}

vk::raii::RenderPass graphic::pipeline::createRenderPass(const vk::raii::Device& device, vk::Format swapChainImageFormat) {
    vk::AttachmentDescription colorAttachment{
        .format = swapChainImageFormat,
        .samples = vk::SampleCountFlagBits::e1,
        .loadOp = vk::AttachmentLoadOp::eClear,
        .storeOp = vk::AttachmentStoreOp::eStore,
        .finalLayout = vk::ImageLayout::ePresentSrcKHR
    };

    vk::AttachmentReference colorAttachmentRef{
        .attachment = 0,
        .layout = vk::ImageLayout::eColorAttachmentOptimal
    };

    vk::SubpassDescription subpass{
        .pipelineBindPoint = vk::PipelineBindPoint::eGraphics,
        .colorAttachmentCount = 1,
        .pColorAttachments = &colorAttachmentRef
    };

    vk::SubpassDependency dependency{
        .srcSubpass = vk::SubpassExternal,
        .dstSubpass = 0,
        .srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput,
        .dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput,
        .srcAccessMask = vk::AccessFlagBits::eNone,
        .dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite
    };

    vk::RenderPassCreateInfo renderPassInfo{
        .attachmentCount = 1,
        .pAttachments = &colorAttachment,
        .subpassCount = 1,
        .pSubpasses = &subpass,
        .dependencyCount = 1,
        .pDependencies = &dependency
    };

    return vk::raii::RenderPass(device, renderPassInfo);
}

vk::raii::Pipeline graphic::pipeline::createGraphicsPipeline(const vk::raii::Device& device, const vk::raii::PipelineLayout& pipelineLayout, const vk::raii::RenderPass& renderPass, const vk::raii::ShaderModule& vertShaderModule, const vk::raii::ShaderModule& fragShaderModule, vk::Extent2D extent) {
    std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages = {
        vk::PipelineShaderStageCreateInfo{
            .stage  = vk::ShaderStageFlagBits::eVertex,
            .module = *vertShaderModule,
            .pName  = "vertMain",
        },
        vk::PipelineShaderStageCreateInfo{
            .stage  = vk::ShaderStageFlagBits::eFragment,
            .module = *fragShaderModule,
            .pName  = "fragMain",
        },
    };

    std::array<vk::DynamicState, 2> dynamicStates = {
        vk::DynamicState::eViewport,
        vk::DynamicState::eScissor,
    };

    vk::PipelineDynamicStateCreateInfo dynamicStateInfo{
        .dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
        .pDynamicStates    = dynamicStates.data(),
    };

    auto bindingDescription    = graphic::Vertex::getBindingDescription();
    auto attributeDescriptions = graphic::Vertex::getAttributeDescriptions();

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo{
        .vertexBindingDescriptionCount   = 1,
        .pVertexBindingDescriptions      = &bindingDescription,
        .vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size()),
        .pVertexAttributeDescriptions    = attributeDescriptions.data(),
    };

    vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo{
        .topology               = vk::PrimitiveTopology::eTriangleList,
        .primitiveRestartEnable = vk::False,
    };

    vk::Viewport viewport = graphic::newViewport(extent);
    vk::Rect2D   scissor  = graphic::newScissor(extent);

    vk::PipelineViewportStateCreateInfo viewportStateInfo{
        .viewportCount = 1,
        .pViewports    = &viewport,
        .scissorCount  = 1,
        .pScissors     = &scissor,
    };

    vk::PipelineRasterizationStateCreateInfo rasterizationStateInfo{
        .depthClampEnable        = vk::False,
        .rasterizerDiscardEnable = vk::False,
        .polygonMode             = vk::PolygonMode::eFill,
        .cullMode                = vk::CullModeFlagBits::eBack,
        .frontFace               = vk::FrontFace::eCounterClockwise,
        .depthBiasEnable         = vk::False,
        .depthBiasConstantFactor = 0.0f,
        .depthBiasClamp          = 0.0f,
        .depthBiasSlopeFactor    = 0.0f,
        .lineWidth               = 1.0f,
    };

    vk::PipelineMultisampleStateCreateInfo multisampleStateInfo{
        .rasterizationSamples  = vk::SampleCountFlagBits::e1,
        .sampleShadingEnable   = vk::False,
        .minSampleShading      = 1.0f,
        .pSampleMask           = nullptr,
        .alphaToCoverageEnable = vk::False,
        .alphaToOneEnable      = vk::False,
    };

    vk::PipelineColorBlendAttachmentState colorBlendAttachment{
        .blendEnable         = vk::False,
        .srcColorBlendFactor = vk::BlendFactor::eOne,
        .dstColorBlendFactor = vk::BlendFactor::eZero,
        .colorBlendOp        = vk::BlendOp::eAdd,
        .srcAlphaBlendFactor = vk::BlendFactor::eOne,
        .dstAlphaBlendFactor = vk::BlendFactor::eZero,
        .alphaBlendOp        = vk::BlendOp::eAdd,
        .colorWriteMask      = vk::ColorComponentFlagBits::eR
                             | vk::ColorComponentFlagBits::eG
                             | vk::ColorComponentFlagBits::eB
                             | vk::ColorComponentFlagBits::eA,
    };

    vk::PipelineColorBlendStateCreateInfo colorBlendStateInfo{
        .logicOpEnable   = vk::False,
        .logicOp         = vk::LogicOp::eCopy,
        .attachmentCount = 1,
        .pAttachments    = &colorBlendAttachment,
        .blendConstants  = {{ 0.0f, 0.0f, 0.0f, 0.0f }},
    };

    vk::GraphicsPipelineCreateInfo pipelineInfo{
        .stageCount          = static_cast<uint32_t>(shaderStages.size()),
        .pStages             = shaderStages.data(),
        .pVertexInputState   = &vertexInputInfo,
        .pInputAssemblyState = &inputAssemblyInfo,
        .pViewportState      = &viewportStateInfo,
        .pRasterizationState = &rasterizationStateInfo,
        .pMultisampleState   = &multisampleStateInfo,
        .pDepthStencilState  = nullptr,
        .pColorBlendState    = &colorBlendStateInfo,
        .pDynamicState       = &dynamicStateInfo,
        .layout              = *pipelineLayout,
        .renderPass          = *renderPass,
        .subpass             = 0,
        .basePipelineHandle  = nullptr,
        .basePipelineIndex   = -1,
    };

    return device.createGraphicsPipeline(nullptr, pipelineInfo);
}
//
// Created by oster on 09.09.2024.
//

#include "../../VulkanEngine.hpp"
#include "../IRenderable.hpp"

struct MetallicRoughness {
    MaterialPipeline opaquePipeline;
    MaterialPipeline transparentPipeline;

    VkDescriptorSetLayout materialLayout;

    struct MaterialConstants {
        glm::vec4 colorFactors;
        glm::vec4 metalRoughFactors;
        glm::vec4 extra[14];
    };

    struct MaterialResources {
        AllocatedImage colorImage;
        VkSampler colorSampler;
        AllocatedImage metalRoughImage;
        VkSampler metalRoughSampler;
        VkBuffer dataBuffer;
        uint32_t bufferOffset;
    };

    DescriptorAllocator descriptorAllocator;

    void buildPipelines(VulkanEngine* engine);
    void clearRessources();

    MaterialInstance writeMaterial(VkDevice device, MaterialPass pass,
                                   const MaterialResources& resources, DescriptorAllocator allocator);
};

void MetallicRoughness::buildPipelines(VulkanEngine* engine) {
    VkShaderModule vertShader = VulkanUtil::createShaderModule(engine-> device, "shaders, vert.spv");
    VkShaderModule fragShader = VulkanUtil::createShaderModule(engine->device, "shaders/frag.spv");

    DescriptorLayoutBuilder layoutBuilder;
    layoutBuilder.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
    layoutBuilder.addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
    layoutBuilder.addBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);

    materialLayout = layoutBuilder.build(engine->device, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);

    VkDescriptorSetLayout layouts[] = {engine->sceneDataDescriptorLayout, materialLayout, engine->objectDataDescriptorLayout};

    RenderPassBuilder renderPassBuilder;
    renderPassBuilder.setColorAttachmentFormat(engine->getColorAttachmentFormat());
    renderPassBuilder.setDepthFormat(engine->getDepthFormat());
    VkRenderPass renderPass = renderPassBuilder.createRenderPass(engine->device);

    PipelineBuilder pipelineBuilder;
    pipelineBuilder.setDescriptorSetLayouts(layouts);
    pipelineBuilder.setShaders(vertShader, fragShader);
    pipelineBuilder.setInputTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
    pipelineBuilder.setPolygonMode(VK_POLYGON_MODE_FILL);
    pipelineBuilder.setCullMode(VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE);
    pipelineBuilder.setMultisamplingNone();
    pipelineBuilder.disableColorBlending();
    pipelineBuilder.enableDepthTest(VK_TRUE, VK_COMPARE_OP_GREATER_OR_EQUAL);

    pipelineBuilder.buildPipelineLayout(engine->device, &opaquePipeline.pipelineLayout);
    pipelineBuilder.buildPipeline(engine->device, renderPass, &opaquePipeline.pipeline, opaquePipeline.pipelineLayout);

    pipelineBuilder.enableAdditiveBlending();
    pipelineBuilder.enableDepthTest(VK_FALSE, VK_COMPARE_OP_GREATER_OR_EQUAL);

    pipelineBuilder.buildPipelineLayout(engine->device, &transparentPipeline.pipelineLayout);
    pipelineBuilder.buildPipeline(engine->device, renderPass, &transparentPipeline.pipeline, transparentPipeline.pipelineLayout);

    vkDestroyShaderModule(engine->device, vertShader, nullptr);
    vkDestroyShaderModule(engine->device, fragShader, nullptr);
}

MaterialInstance MetallicRoughness::writeMaterial(VkDevice device, MaterialPass pass,
                                                  const MaterialResources& resources, DescriptorAllocator allocator) {
    MaterialInstance instance;
    instance.materialPass = pass;
    if (pass == MaterialPass::Transparent) {
        instance.pipeline = &transparentPipeline;
    } else {
        instance.pipeline = &opaquePipeline;
    }

    instance.materialSet = allocator.allocate(device, materialLayout);

    allocator.clearWrites();
    allocator.writeBuffer(0, resources.dataBuffer, sizeof(MaterialConstants), resources.bufferOffset, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
    allocator.writeImage(1, resources.colorImage.imageView, resources.colorSampler, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ,VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
    allocator.writeImage(2, resources.metalRoughImage.imageView, resources.metalRoughSampler, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ,VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);

    allocator.updateSet(device, instance.materialSet);

    return instance;
}

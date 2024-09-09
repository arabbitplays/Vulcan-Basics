//
// Created by oster on 09.09.2024.
//

#include <glm/vec4.hpp>
#include "../IRenderable.hpp"
#include "../../builders/RessourceBuilder.hpp"
#include "../../DescriptorAllocator.hpp"

struct metallicRoughness {
    MaterialPipeline opaquePipeline;
    MaterialPipeline transparentPipeline;

    VkDescriptorSetLayout materialLayout;

    struct MaterialConstants {
        glm::vec4 colorFactors;
        glm::vec4 metalRoughFactors;
        glm::vec4 extra[14];
    };

    struct MaterialRessources {
        AllocatedImage colorImage;
        VkSampler colorSampler;
        AllocatedImage metalRoughImage;
        VkSampler metalRoughSampler;
        VkBuffer dataBuffer;
        uint32_t bufferOffset;
    };

    DescriptorAllocator descriptorAllocator;

    void buildPipelines();
    void clearRessources();

    MaterialInstance writeMaterial() {}
};

void metallicRoughness::buildPipelines() {

}

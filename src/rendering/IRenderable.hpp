//
// Created by oster on 09.09.2024.
//

#include <vulkan/vulkan_core.h>

#ifndef BASICS_IRENDERABLE_HPP
#define BASICS_IRENDERABLE_HPP

enum class MaterialPass : uint8_t {
    MainColor,
    Transparent,
    Other
};

struct MaterialPipeline {
    VkPipeline pipeline;
    VkPipelineLayout pipelineLayout;
};

struct MaterialInstance {
    MaterialPipeline* pipeline;
    VkDescriptorSet materialSet;
    MaterialPass materialPass;
};

class IRenderable {
};

#endif //BASICS_IRENDERABLE_HPP


//
// Created by oster on 09.09.2024.
//

#ifndef BASICS_RESSOURCEBUILDER_HPP
#define BASICS_RESSOURCEBUILDER_HPP


#include <vulkan/vulkan_core.h>
#include "../CommandManager.hpp"

struct AllocatedBuffer {
    VkBuffer buffer;
    VkDeviceMemory bufferMemory;
};

struct AllocatedImage {
    VkImage image;
    VkDeviceMemory imageMemory;
    VkImageView imageView;
};

class RessourceBuilder {
public:
    RessourceBuilder() = default;
    RessourceBuilder(VkPhysicalDevice physicalDevice, VkDevice device, CommandManager commandManager);
    AllocatedBuffer createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
    void copyBuffer(AllocatedBuffer src, AllocatedBuffer dst, VkDeviceSize size);
    void destroyBuffer(AllocatedBuffer buffer);

    AllocatedImage createImage(uint32_t width, uint32_t height,
                                    VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
                                    VkMemoryPropertyFlags properties, VkImageAspectFlags aspectFlags);
    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
    void destroyImage(AllocatedImage image);

private:
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

        VkPhysicalDevice physicalDevice;
    VkDevice device;
    CommandManager commandManager;
};


#endif //BASICS_RESSOURCEBUILDER_HPP

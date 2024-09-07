//
// Created by oster on 07.09.2024.
//

#ifndef BASICS_MESHASSETBUILDER_HPP
#define BASICS_MESHASSETBUILDER_HPP


#include <vulkan/vulkan.h>
#include <vector>
#include "CommandManager.hpp"
#include "Vertex.hpp"

class MeshAssetBuilder {
public:
    struct AllocatedBuffer {
        VkBuffer buffer;
        VkDeviceMemory bufferMemory;
    };

    struct MeshBuffers {
        AllocatedBuffer vertexBuffer;
        AllocatedBuffer indexBuffer;
    };

    struct MeshSurface {
        uint32_t startIndex;
        uint32_t count;
    };

    struct MeshAsset {
        std::vector<MeshSurface> surfaces;
        MeshBuffers meshBuffers;
    };

    MeshAssetBuilder() = default;
    MeshAssetBuilder(VkPhysicalDevice physicalDevice, VkDevice device, CommandManager commandManager);
    MeshAsset LoadMeshAsset(std::string path);
    void destroyMeshAsset(MeshAsset meshAsset);

private:
    void loadModel(std::string path, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
    AllocatedBuffer createVertexBuffer(std::vector<Vertex> vertices);
    AllocatedBuffer createIndexBuffer(std::vector<uint32_t> indices);
    AllocatedBuffer createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
    void copyBuffer(AllocatedBuffer src, AllocatedBuffer dst, VkDeviceSize size);
    void destroyBuffer(AllocatedBuffer buffer);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    VkPhysicalDevice physicalDevice;
    VkDevice device;
    CommandManager commandManager;
};


#endif //BASICS_MESHASSETBUILDER_HPP

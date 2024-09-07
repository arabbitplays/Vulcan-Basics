//
// Created by oster on 07.09.2024.
//

#include <stdexcept>
#include <tiny_obj_loader.h>
#include <iostream>
#include "MeshAssetBuilder.hpp"

MeshAssetBuilder::MeshAssetBuilder(VkPhysicalDevice physicalDevice, VkDevice device, CommandManager commandManager) {
    this->physicalDevice = physicalDevice;
    this->device = device;
    this->commandManager = commandManager;
}

MeshAssetBuilder::MeshAsset MeshAssetBuilder::LoadMeshAsset(std::string path) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    loadModel(path, vertices, indices);

    MeshBuffers meshBuffers{};
    meshBuffers.vertexBuffer = createVertexBuffer(vertices);
    meshBuffers.indexBuffer = createIndexBuffer(indices);

    MeshAsset meshAsset{};
    meshAsset.meshBuffers = meshBuffers;
    std::vector<MeshSurface> surfaces;
    surfaces.push_back({0, static_cast<uint32_t>(indices.size())});
    meshAsset.surfaces = surfaces;

    return meshAsset;
}

void MeshAssetBuilder::loadModel(std::string path, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    std::cout << "Load model " << path << std::endl;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str())) {
        throw std::runtime_error("warn + err");
    }

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Vertex vertex{};

            vertex.pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.color = {1.0f, 1.0f, 1.0f};

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }
    }

    std::cout << "Finished loading model!" << std::endl;
}

MeshAssetBuilder::AllocatedBuffer MeshAssetBuilder::createVertexBuffer(std::vector<Vertex> vertices) {
    VkDeviceSize size = sizeof(vertices[0]) * vertices.size();

    AllocatedBuffer stagingBuffer = createBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT
            , VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    void* data;
    vkMapMemory(device, stagingBuffer.bufferMemory, 0, size, 0, &data);
    memcpy(data, vertices.data(), (size_t) size);
    vkUnmapMemory(device, stagingBuffer.bufferMemory);

    AllocatedBuffer vertexBuffer = createBuffer(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    copyBuffer(stagingBuffer, vertexBuffer, size);

    destroyBuffer(stagingBuffer);
    return vertexBuffer;
}

MeshAssetBuilder::AllocatedBuffer MeshAssetBuilder::createIndexBuffer(std::vector<uint32_t> indices) {
    VkDeviceSize size = sizeof(indices[0]) * indices.size();

    AllocatedBuffer stagingBuffer = createBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT
            , VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    void* data;
    vkMapMemory(device, stagingBuffer.bufferMemory, 0, size, 0, &data);
    memcpy(data, indices.data(), (size_t) size);
    vkUnmapMemory(device, stagingBuffer.bufferMemory);

    AllocatedBuffer indexBuffer = createBuffer(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    copyBuffer(stagingBuffer, indexBuffer, size);

    destroyBuffer(stagingBuffer);
    return indexBuffer;
}

MeshAssetBuilder::AllocatedBuffer MeshAssetBuilder::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) {
    MeshAssetBuilder::AllocatedBuffer allocatedBuffer{};

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.flags = 0;

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &allocatedBuffer.buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create vertex buffer!");
    }

    VkMemoryRequirements memRequirements{};
    vkGetBufferMemoryRequirements(device, allocatedBuffer.buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device, &allocInfo, nullptr, &allocatedBuffer.bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate vertex buffer memory!");
    }

    vkBindBufferMemory(device, allocatedBuffer.buffer, allocatedBuffer.bufferMemory, 0);

    return allocatedBuffer;
}

uint32_t MeshAssetBuilder::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperies;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperies);

    for (uint32_t i = 0; i < memProperies.memoryTypeCount; i++) {
        if (typeFilter & (1 << i) && (memProperies.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

void MeshAssetBuilder::copyBuffer(MeshAssetBuilder::AllocatedBuffer src, MeshAssetBuilder::AllocatedBuffer dst, VkDeviceSize size) {
    VkCommandBuffer commandBuffer = commandManager.beginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, src.buffer, dst.buffer, 1, &copyRegion);

    commandManager.endSingleTimeCommand(commandBuffer);
}

void MeshAssetBuilder::destroyBuffer(AllocatedBuffer buffer) {
    vkDestroyBuffer(device, buffer.buffer, nullptr);
    vkFreeMemory(device, buffer.bufferMemory, nullptr);
}

void MeshAssetBuilder::destroyMeshAsset(MeshAssetBuilder::MeshAsset meshAsset) {
    destroyBuffer(meshAsset.meshBuffers.vertexBuffer);
    destroyBuffer(meshAsset.meshBuffers.indexBuffer);
}

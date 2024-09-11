//
// Created by oster on 09.09.2024.
//

#ifndef BASICS_VULKANENGINE_HPP
#define BASICS_VULKANENGINE_HPP

#include <iostream>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <cstring>
#include <optional>
#include <set>
#include <limits>
#include <algorithm>
#include <fstream>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>
#include <chrono>
#include <stb_image.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <unordered_map>
#include "Vertex.hpp"
#include "builders/PipelineBuilder.hpp"
#include "DescriptorAllocator.hpp"
#include "CommandManager.hpp"
#include "builders/MeshAssetBuilder.hpp"
#include "VulkanUtil.hpp"
#include "builders/DescriptorLayoutBuilder.hpp"
#include "builders/RenderPassBuilder.hpp"
#include "rendering/IRenderable.hpp"

class VulkanEngine;

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
    void clearRessources(VkDevice device);

    MaterialInstance writeMaterial(VkDevice device, MaterialPass pass,
                                   const MaterialResources& resources, DescriptorAllocator allocator);
};

class VulkanEngine {
public:
    VkDevice device;
    CommandManager commandManager;
    RessourceBuilder ressourceBuilder;
    MeshAssetBuilder meshAssetBuilder;

    VkDescriptorSetLayout sceneDataDescriptorLayout;
    VkDescriptorSetLayout objectDataDescriptorLayout;

    void run();
    VkFormat getColorAttachmentFormat();
    VkFormat getDepthFormat();

    struct SceneData {
        glm::mat4 view;
        glm::mat4 proj;
        glm::mat4 viewProj;
        glm::vec4 ambientColor;
        glm::vec4 sunlightDirection; // w for sun power
        glm::vec4 sunlightPower;
    };

    struct ObjectData {
        glm::mat4 model;
    };

private:
    GLFWwindow* window;
    VkInstance instance;
    VkSurfaceKHR surface;
    VkDebugUtilsMessengerEXT debugMessenger;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkQueue graphicsQueue, presentQueue;

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;

    AllocatedImage depthImage;

    MaterialPipeline graphicsPipeline;

    std::vector<VkFramebuffer> swapChainFrameBuffers;
    std::vector<VkCommandBuffer> commandBuffers;

    MeshAssetBuilder::MeshAsset meshAsset;

    AllocatedImage textureImage;
    AllocatedImage whiteImage;
    AllocatedImage greyImage;
    AllocatedImage blackImage;
    AllocatedImage errorCheckerboardImage;

    VkSampler defaultSamplerLinear;
    VkSampler defaultSamplerNearest;

    std::vector<AllocatedBuffer> sceneUniformBuffers;
    std::vector<void*> sceneUniformBuffersMapped;
    std::vector<AllocatedBuffer> objectUniformBuffers;
    std::vector<void*> objectUniformBuffersMapped;

    DescriptorAllocator descriptorAllocator;
    std::vector<VkDescriptorSet> sceneDescriptorSets;
    std::vector<VkDescriptorSet> objectDescriptorSets;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    uint32_t currentFrame = 0;
    bool framebufferResized = false;

    MaterialInstance defaultMetalRough;
    MetallicRoughness metalRoughMaterial;
    AllocatedBuffer materialBuffer; // TODO weg damit da nur da zum destroyen am End

    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();

    static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
    void createInstance();
    bool checkValidationLayerSupport();
    std::vector<const char *> getRequiredExtensions();
    void setupDebugMessenger();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
    void createSurface();
    void pickPhysicalDevice();
    bool isDeviceSuitable(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    void createLogicalDevice();
    void recreateSwapChain();
    void createSwapChain();
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
    VkExtent2D chooseSwapExtend(const VkSurfaceCapabilitiesKHR &capabilities);
    void createImageViews();
    void createDescriptorSetLayout();
    void initPipelines();
    void createFrameBuffers();
    void createCommandManager();
    void createRessourceBuilder();
    void createDepthResources();
    VkFormat findDepthFormat();
    VkFormat findSupportedFormat(const std::vector<VkFormat> candidates, VkImageTiling tiling,
                                 VkFormatFeatureFlags features);
    bool hasStencilComponent(VkFormat format);
    void createTextureImage();
    void initDefaultResources();
    void createDefaultTextures();
    void createDefaultSamplers();
    void createDefaultMaterials();
    void loadMeshes();
    void createUniformBuffers();
    void createDescriptorAllocator();
    void createDescriptorSets();
    void createCommandBuffers();
    void createSyncObjects();
    void drawFrame();
    void cleanupSwapChain();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    void updateUniformBuffers(uint32_t currentImage);
    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData) {

        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

        return VK_FALSE;
    }
};

#endif //BASICS_VULKANENGINE_HPP


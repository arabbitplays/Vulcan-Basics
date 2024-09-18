cmake_minimum_required(VERSION 3.27)
project(Basics LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Vulkan REQUIRED)
find_package(stb REQUIRED)
find_package(tinyobjloader)

add_executable(${PROJECT_NAME} src/rendering/engine/VulkanEngine.cpp
        src/rendering/Vertex.hpp
        src/builders/PipelineBuilder.cpp
        src/builders/PipelineBuilder.hpp
        src/rendering/engine/DescriptorAllocator.cpp
        src/rendering/engine/DescriptorAllocator.hpp
        src/builders/MeshAssetBuilder.cpp
        src/builders/MeshAssetBuilder.hpp
        src/rendering/engine/CommandManager.cpp
        src/rendering/engine/CommandManager.hpp
        src/builders/RessourceBuilder.cpp
        src/builders/RessourceBuilder.hpp
        src/rendering/IRenderable.hpp
        src/util/VulkanUtil.hpp
        src/builders/DescriptorLayoutBuilder.cpp
        src/builders/DescriptorLayoutBuilder.hpp
        src/main.cpp
        src/rendering/engine/VulkanEngine.hpp
        src/builders/RenderPassBuilder.cpp
        src/builders/RenderPassBuilder.hpp
        src/rendering/nodes/Node.cpp
        src/rendering/nodes/Node.hpp
        src/rendering/nodes/MeshNode.cpp
        src/rendering/nodes/MeshNode.hpp
        src/Analytics.hpp
        src/rendering/engine/DeletionQueue.hpp
)

target_link_libraries(${PROJECT_NAME} glfw glm::glm Vulkan::Vulkan stb::stb tinyobjloader::tinyobjloader)
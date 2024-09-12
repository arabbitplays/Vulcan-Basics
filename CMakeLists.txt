cmake_minimum_required(VERSION 3.27)
project(Basics LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Vulkan REQUIRED)
find_package(stb REQUIRED)
find_package(tinyobjloader)

add_executable(${PROJECT_NAME} src/VulkanEngine.cpp
        src/Vertex.hpp
        src/builders/PipelineBuilder.cpp
        src/builders/PipelineBuilder.hpp
        src/DescriptorAllocator.cpp
        src/DescriptorAllocator.hpp
        src/builders/MeshAssetBuilder.cpp
        src/builders/MeshAssetBuilder.hpp
        src/CommandManager.cpp
        src/CommandManager.hpp
        src/builders/RessourceBuilder.cpp
        src/builders/RessourceBuilder.hpp
        src/rendering/IRenderable.hpp
        src/VulkanUtil.hpp
        src/builders/DescriptorLayoutBuilder.cpp
        src/builders/DescriptorLayoutBuilder.hpp
        src/main.cpp
        src/VulkanEngine.hpp
        src/builders/RenderPassBuilder.cpp
        src/builders/RenderPassBuilder.hpp
        src/rendering/Node.cpp
        src/rendering/Node.hpp
        src/rendering/MeshNode.cpp
        src/rendering/MeshNode.hpp
)

target_link_libraries(${PROJECT_NAME} glfw glm::glm Vulkan::Vulkan stb::stb tinyobjloader::tinyobjloader)
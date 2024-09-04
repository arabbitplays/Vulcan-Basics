cmake_minimum_required(VERSION 3.27)
project(Basics LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Vulkan REQUIRED)
find_package(stb REQUIRED)
find_package(tinyobjloader)

add_executable(${PROJECT_NAME} src/main.cpp
        src/Vertex.hpp)

target_link_libraries(${PROJECT_NAME} glfw glm::glm Vulkan::Vulkan stb::stb tinyobjloader::tinyobjloader)
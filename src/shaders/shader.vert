#version 450

#include "input_structures.glsl"

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inUV;
layout(location = 3) in vec3 inNormal;

layout(location = 0) out vec3 outNormal;
layout(location = 1) out vec3 outColor;
layout(location = 2) out vec2 outUV;

void main() {
    gl_Position = sceneData.proj * sceneData.view * objectData.model * vec4(inPosition, 1.0);
    outColor = inColor;
    outUV = inUV;
    outNormal = (objectData.model * vec4(inNormal, 1.0)).xyz;
}

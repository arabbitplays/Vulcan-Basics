#version 450

#include "input_structures.glsl"

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inColor;
layout(location = 3) in vec2 inUV;

layout(location = 0) out vec3 outNormal;
layout(location = 1) out vec3 outColor;
layout(location = 2) out vec2 outUV;

void main() {
    gl_Position = sceneData.viewProj * objectData.model * vec4(inPosition, 1.0f);

    outNormal = (objectData.model * vec4(inNormal, 1.0f)).xyz;
    outColor = inColor.xyz * materialData.colorFactors.xyz;
    outUV = inUV;
}
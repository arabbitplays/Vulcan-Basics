#version 450

#include "input_structures.glsl"

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTextCoord;

void main() {
    gl_Position = sceneData.proj * sceneData.view * objectData.model * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTextCoord = inTexCoord;
}

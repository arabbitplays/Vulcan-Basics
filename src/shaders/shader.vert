#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTextCoord;


layout(set = 0, binding = 0) uniform SceneData {
    mat4 view;
    mat4 proj;
    mat4 viewProj;
    vec4 ambientColor;
    vec4 sunlightDirection; // w for sun power
    vec4 sunlightPower;
} sceneData;

layout(set = 1, binding = 0) uniform ObjectData {
    mat4 model;
} objectData;

void main() {
    gl_Position = sceneData.proj * sceneData.view * objectData.model * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTextCoord = inTexCoord;
}

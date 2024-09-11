#version 450

#include "input_structures.glsl"

layout(location = 0) in vec3 inNormal;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inUV;

layout(location = 0) out vec4 outColor;


void main() {
    outColor = vec4(inNormal, 1.0);
}
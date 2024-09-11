#version 450

#include "input_structures.glsl"

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;


void main() {
    outColor = vec4(fragColor * texture(colorTex, fragTexCoord).rgb, 1.0);
}
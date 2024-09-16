#version 450

#include "input_structures.glsl"
#include "lighting.glsl"

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inColor;
layout(location = 3) in vec2 inUV;

layout(location = 0) out vec4 outColor;


void main() {
    vec3 n = normalize(inNormal);
    vec3 v = normalize(sceneData.viewPos.xyz - inPos);
    vec3 l = normalize(sceneData.sunlightDirection.xyz);
    vec3 h = normalize(l + v);

    vec3 lightColor = sceneData.sunlightColor.xyz;
    float lightStrength = sceneData.sunlightColor.w;
    vec3 inRadiance = sceneData.sunlightColor.xyz * lightStrength;

    vec3 albedo = inColor * texture(colorTex, inUV).xyz;
    // TODO include texture
    float metallic = materialData.metalRoughFactors.x;
    float roughness = materialData.metalRoughFactors.y;

    vec3 brdf = calcBRDF(l, v, n, h, albedo, metallic, roughness);
    vec3 outRadiance = brdf * inRadiance * max(dot(n, l), 0.0);
    vec3 ambient = vec3(0.01) * albedo;

    vec3 color = ambient + outRadiance;

    // gamma correction
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    outColor = vec4(color, 1.0);
}
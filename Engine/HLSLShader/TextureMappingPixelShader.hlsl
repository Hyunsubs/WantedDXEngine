#include "Common.hlsli"

struct PixelInput
{
    float4 position : SV_Position;
    float3 color : COLOR;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 cameraDirection : TEXCOORD1;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
};

// Texture.
Texture2D diffuseMap : register(t0);
SamplerState diffuseSampler : register(s0);

float4 main(PixelInput input) : SV_TARGET
{
    // Sampling.
    float4 texColor = diffuseMap.Sample(diffuseSampler, input.texCoord);
    
    // Light Dir.
    float3 lightDir = -float3(500.0f, 500.0f, -500.0f);
    lightDir = normalize(lightDir);
    
    // World Normal.
    float3 worldNormal = normalize(input.normal);
    
    // Dot (Lambert Cosine Law).
    float nDotl = CalcHalfLambert(worldNormal, lightDir, 0.5f);
    float4 ambient = texColor * float4(0.1f, 0.1, 0.1f, 1);
    float4 diffuse = texColor * nDotl;
    
    // Phong Reflection Model
    float4 finalColor = ambient + diffuse;
    // finalColor += CalcPhong(worldNormal, lightDir, input.cameraDirection);
    
    float specular = CalcBlinnPhong(worldNormal, lightDir, input.cameraDirection);
    finalColor += float4(0.4f, 0.6f, 0.8f, 1.f) * specular;
    
    return finalColor;
}
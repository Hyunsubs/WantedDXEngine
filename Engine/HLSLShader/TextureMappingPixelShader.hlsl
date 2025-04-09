struct PixelInput
{
    float4 position : SV_Position;
    float3 color : COLOR;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
};

// Texture.
Texture2D diffuseMap : register(t0);
SamplerState diffuseSampler : register(s0);

float4 main(PixelInput input) : SV_TARGET
{
    // Sampling.
    float4 texColor = diffuseMap.Sample(diffuseSampler, input.texCoord);
    
    // Light Dir
    float3 lightDir = -float3(500.f, 500.f, -500.f);
    lightDir = normalize(lightDir);
    
    // World Normal 
    float3 worldNormal = normalize(input.normal);
    
    // Dot (Lambert)
    float lightIntensity = saturate(dot(worldNormal, -lightDir));
    
    float finalColor = texColor * lightIntensity;
    
    return finalColor;
}
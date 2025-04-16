// Lambert cosine law.
float CalcLambert(float3 worldNormal, float3 lightDirection)
{
    return saturate(dot(worldNormal, -lightDirection));
}

// Half Lambert 
float CalcHalfLambert(float3 worldNormal, float3 lightDirection, float weight, float param = 2)
{
    float nDotL = CalcLambert(worldNormal, lightDirection);
    return pow(nDotL * weight + (1.f - weight), param);
}


// Phong
float CalcPhong(float3 worldNormal, float3 lightDirection, float3 cameraDirection, float shineness = 16.f)
{
    float lightIntensity = CalcHalfLambert(worldNormal, lightDirection, 0.5f);
    
    float specular = 0.f;
    if (lightIntensity)
    {
        float3 reflection = reflect(lightDirection, worldNormal);
        float3 viewDirection = normalize(cameraDirection);
        float rDotv = saturate(dot(reflection, -viewDirection));
        float shineness = 16.f;
        specular = pow(rDotv, shineness);
    }
    
    return specular;
}

float CalcBlinnPhong(float3 worldNormal, float3 lightDirection, float3 cameraDirection, float shineness = 32.f)
{
    float NoL = CalcLambert(worldNormal, lightDirection);
    // Blinn-Phong(specular)
    float specular = 0.f;
    if (NoL > 0)
    {
        // HalfVector
        float3 viewDirection = normalize(cameraDirection);
        float3 halfVector = normalize((-lightDirection) + (-viewDirection));
        
        // nDoth
        float NoH = saturate(dot(worldNormal, halfVector));
        float shineness = 32.f;
        specular = pow(NoH, shineness);

    }
    
    return specular;
}
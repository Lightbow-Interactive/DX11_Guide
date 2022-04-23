cbuffer globalLightsCBuffer : register(b0)
{
    float ambientLightFactor;
    float3 padding;

    float3 directionalLightColor;
    float directionalLightStrength;
    float3 directionalLightDirection;
    float padding2;

    float3 pointLightColor;
    float pointLightStrength;
    float3 pointLightPosition;
    float padding3;

    float3 spotLightColor;
    float spotLightStrength;
    float3 spotLightDirection;
    float spotLightCutoff;
    float3 spotLightPosition;
    float padding4;
}

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float3 worldPosition : WORLD_POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float3 eyePos : EYEPOS;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 ambient = { 0.f, 0.f, 0.f, 0.f };
    float4 diffuse = { 0.f, 0.f, 0.f, 0.f };
    float4 specular = { 0.f, 0.f, 0.f, 0.f };

    // Ambient Light
    ambient += input.color * ambientLightFactor;

    // Directional Light
    float dirFactor = max(dot(input.normal, -directionalLightDirection), 0.f);
    diffuse += input.color * float4((directionalLightColor * directionalLightStrength) * dirFactor, 1.f);

    // Point Light
    float3 vectorToLight = normalize(pointLightPosition - input.worldPosition);
    float diffFactor = max(dot(vectorToLight, input.normal), 0.f);
    float distanceToLight = distance(pointLightPosition, input.worldPosition);
    diffuse += input.color * float4((pointLightColor * pointLightStrength / (distanceToLight/2)) * diffFactor, 1.f);

    float3 eyeToPos = normalize(input.eyePos - input.worldPosition);
    float3 reflectDir = reflect(-vectorToLight, input.normal);
    float specFactor = pow(max(dot(eyeToPos, reflectDir), 0.f), 32);
    specular += input.color * float4((pointLightColor * pointLightStrength) * specFactor, 1.f);

    // Spot Light
    float3 lightToFrag = normalize(input.worldPosition - spotLightPosition);
    float theta = acos(dot(lightToFrag, normalize(spotLightDirection)));
    float thetaDeg = theta * 180 / 3.14159265f;

    if (thetaDeg < spotLightCutoff)
    {
        float edgeIntensity = clamp((spotLightCutoff - thetaDeg) / 10, 0.f, 1.f);

        float splDiffFactor = max(dot(-lightToFrag, input.normal), 0.f);
        float splDistance = distance(spotLightPosition, input.worldPosition);
        diffuse += input.color * float4(((spotLightColor * spotLightStrength / (splDistance / 2)) * diffFactor) * edgeIntensity, 1.f);
    }

    return ambient + diffuse + specular;
}
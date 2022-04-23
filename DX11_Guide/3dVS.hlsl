#pragma pack_matrix(row_major)

cbuffer globalCBuffer : register(b0) // Register global constant buffer to slot 0
{
    float4x4 viewProjectionMatrix;
    float3 eyePos;
    float padding;
}

cbuffer objectCBuffer : register(b1) // Register the per object constant buffer to slot 1
{
    float4x4 worldMatrix;
}

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 color : COLOR;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 worldPosition : WORLD_POSITION;
    float4 color : COLOR;
    float4 normal : NORMAL;
    float3 eyePos : EYEPOS;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(float4(input.position, 1.f), mul(worldMatrix, viewProjectionMatrix));
    output.color = float4(input.color, 1.f);
    output.worldPosition = mul(float4(input.position, 1.f), worldMatrix);
    output.normal = normalize(mul(float4(input.normal, 0.f), worldMatrix));
    output.eyePos = eyePos;
    return output;
}
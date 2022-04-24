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
    return float4(1.f, 1.f, 1.f, 1.f);
}
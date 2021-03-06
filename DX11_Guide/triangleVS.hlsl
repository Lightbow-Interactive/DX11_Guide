struct VS_INPUT
{
    float3 position : POSITION;
    float2 textureCoord : TEXTURE_COORD;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 textureCoord : TEXTURE_COORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = float4(input.position, 1.f);
    output.textureCoord = input.textureCoord;
    return output;
}
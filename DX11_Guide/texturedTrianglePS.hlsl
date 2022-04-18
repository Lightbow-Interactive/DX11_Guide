struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 textureCoord : TEXTURE_COORD;
};

Texture2D objTexture : TEXTURE : register(t0); // register at texture slot 0
										       //   -> slot needs to be the same as the one in the Bind() function
SamplerState objSamplerState : SAMPLER : register(s0); // register at sampler state slot 0

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 sampledColor = objTexture.Sample(objSamplerState, input.textureCoord); // Sample the texture using the sampler state and texture coordinate
    return sampledColor;
}
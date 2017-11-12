Texture2D ColorTexture;
SamplerState ColorSampler;

struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float2 TextureCoordinate : TEXCOORD;
};

float4 main(VS_OUTPUT IN) : SV_TARGET
{
    float4 color = ColorTexture.Sample(ColorSampler, IN.TextureCoordinate);
    return float4(1 - color.rgb, color.a);
}
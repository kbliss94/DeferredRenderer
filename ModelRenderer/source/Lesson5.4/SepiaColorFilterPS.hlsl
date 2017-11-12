static const float3x3 SepiaFilter =
{
    0.393f, 0.349f, 0.272f,
									  0.769f, 0.686f, 0.534f,
									  0.189f, 0.168f, 0.131f
};

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
    return float4(mul(color.rgb, SepiaFilter), color.a);
}
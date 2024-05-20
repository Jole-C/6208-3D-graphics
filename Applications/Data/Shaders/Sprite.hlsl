cbuffer VSConstants
{
	float4 TexAndViewportSize;
}

Texture2D	SpriteTexture : register(t0);
SamplerState SpriteSampler : register(s0);

struct VS_INPUT
{
	// Per-vertex data
    float2 Position : POSITION;
    float2 TexCoord : TEXCOORD;

	// Instance data
    float4x4 Transform : TRANSFORM;
    float4 Color : COLOR;
    float4 SourceRect : SOURCERECT;
};

struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD;
    float4 Color : COLOR;
};

VS_OUTPUT SpriteVS(in VS_INPUT input)
{
    float4 positionSS = float4(input.Position * input.SourceRect.zw, 0.0f, 1.0f);

    positionSS = mul(positionSS, input.Transform);

    float4 positionDS = positionSS;
    positionDS.xy /= TexAndViewportSize.zw;
    positionDS = positionDS * 2 - 1;
    positionDS.y *= -1;

    float2 outTexCoord = input.TexCoord;
    outTexCoord.xy *= input.SourceRect.zw / TexAndViewportSize.xy;
    outTexCoord.xy += input.SourceRect.xy / TexAndViewportSize.xy;

    VS_OUTPUT output;
    output.Position = positionDS;
    output.TexCoord = outTexCoord;
    output.Color = input.Color;

    return output;
}

float4 SpritePS(in VS_OUTPUT input) : SV_Target
{
    float4 texColor = SpriteTexture.Sample(SpriteSampler, input.TexCoord);
    texColor = texColor * input.Color;
    return texColor;
}

Texture2D       DiffuseTexture0 : register(t0);   
Texture2D		DiffuseTexture1 : register(t1);
SamplerState    TextureSampler : register(s0);

cbuffer TransformMatrices
{
	matrix WorldViewProjMatrix;	
};

cbuffer TextureParams
{
    float4 texWeight;
};

struct VS_INPUT
{
	float3 position : POSITION;
	float2 tex		: TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float2 tex		: TEXCOORD0;
};


VS_OUTPUT VSMain( in VS_INPUT input )
{
	VS_OUTPUT output;
	
	output.position = mul(float4(input.position, 1.0f), WorldViewProjMatrix);
	output.tex = input.tex;

	return output;
}


float4 PSMain(in VS_OUTPUT input) : SV_Target
{
	float4 sampledTextureColour = DiffuseTexture0.Sample(TextureSampler, input.tex);
    float4 sampledTextureColour1 = DiffuseTexture1.Sample(TextureSampler, input.tex);
	
    return (texWeight.x * sampledTextureColour + (1 - texWeight.x) * sampledTextureColour1);
}
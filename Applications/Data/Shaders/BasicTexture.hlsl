Texture2D       DiffuseTexture : register(t0);   
SamplerState    TextureSampler : register(s0);

cbuffer TransformMatrices
{
	matrix WorldViewProjMatrix;	
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
	float4 sampledTextureColour = DiffuseTexture.Sample(TextureSampler, input.tex);
	return(sampledTextureColour);
}
//---------------------------------
cbuffer Transforms
{
	matrix WorldMatrix;
	matrix WorldViewProjectionMatrix;
};

cbuffer Lights
{
	Light Lights[100];
}


cbuffer SurfaceReflectanceInfo
{
	float4 SurfaceEmissiveColour;
	float4 SurfaceConstants;
};


cbuffer AmbientLightInfo
{
	float4 AmbientLightColour;
};


cbuffer DirectionalLightInfo
{
	float4 DirectionalLightColour;
	float3 DirectionalLightDirection;
};


struct PointLightInfo
{
	float4 PointLightColour;
	float3 PointLightPosition;
	float2 PointLightRange;
};


struct SpotLightInfo
{
	float4 SpotLightColour;
	float3 SpotLightPosition;
	float3 SpotLightDirection;
	float2 SpotLightRange;
	float2 SpotLightFocus;
};

//---------------------------------
struct VS_INPUT
{
	float4 position : POSITION;
	float2 tex		: TEXCOORD0;
	float3 normal	: NORMAL;
};


struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 worldPos : POSITION;
};

//----------------------------------
VS_OUTPUT VSMain(in VS_INPUT input)
{
	VS_OUTPUT output;

	output.position = mul(float4(input.position, 1.0f), WorldViewProjectionMatrix);
	output.tex = input.tex;

	output.worldPos = mul(float4(input.position, 1.0f), WorldMatrix);
	output.normal = mul(normalize(input.normal), WorldMatrix);

	return output;
}


//-----------------------------------
float4 PSMain(in VS_OUTPUT input) : SV_Target
{

	float ambientlightIntensity = AmbientLightColour * SurfaceConstants.x;
	return(color);
}
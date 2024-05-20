TextureCube     SkyboxTexture : register( t0 );           
SamplerState    LinearSampler : register( s0 );

cbuffer SkyboxData
{
	matrix ViewMatrix;	
	matrix ProjMatrix;
	float4 ViewPosition;
};

struct VS_INPUT
{
	float3 position : POSITION;
	float2 tex		: TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float3 normal   : NORMAL;
};


VS_OUTPUT VSMAIN( in VS_INPUT input )
{
	VS_OUTPUT output;
	float4 positionVS = mul( float4( input.position.xyz + ViewPosition.xyz, 1.0f ), ViewMatrix );
	output.normal = input.position.xyz;
	output.position = mul( positionVS, ProjMatrix ).xyww;
	return output;
}


float4 PSMAIN( in VS_OUTPUT input ) : SV_Target
{
	float4 sampledColor = SkyboxTexture.Sample( LinearSampler, input.normal );
	return( sampledColor );
}


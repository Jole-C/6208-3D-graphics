//---------------------------------
cbuffer Transforms
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjMatrix;
};

//---------------------------------
struct VS_INPUT
{
	float4 position : POSITION;
	float4 color : COLOR;
};


struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};



//----------------------------------
VS_OUTPUT VSMain(in VS_INPUT v)
{
	VS_OUTPUT o;

	float4 WorldSpace = mul(v.position, WorldMatrix);
	float4 ViewSpace = mul(WorldSpace, ViewMatrix);
	float4 ScreenSpace = mul(ViewSpace, ProjMatrix);

	o.position = ScreenSpace;
	o.color = v.color;

	return o;
}


//-----------------------------------
float4 PSMain(in VS_OUTPUT input) : SV_Target
{
	float4 color = input.color;
	return(color);
}
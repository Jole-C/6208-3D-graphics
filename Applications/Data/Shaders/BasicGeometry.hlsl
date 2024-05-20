Texture2D       DiffuseTexture1 : register(t0);   
Texture2D       DiffuseTexture2 : register(t1);   
Texture2D       DiffuseTexture3 : register(t2);   
Texture2D       DiffuseTexture4 : register(t3);
SamplerState    TextureSampler : register(s0);

cbuffer GSTransformMatrices
{
	matrix WorldMatrix;
    matrix ViewMatrix;
    matrix ProjMatrix;
    
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

struct GS_INPUTOUTPUT
{
    float4 position : SV_Position;
    float2 tex : TEXCOORD0;
    float2 texID : TEXCOORD1;
};

cbuffer instancePositions
{
    float4 pos1;
    float4 pos2;
    float4 pos3;
    float4 pos4;
};


GS_INPUTOUTPUT VSMain(in VS_INPUT input)
{
    GS_INPUTOUTPUT output;
	
	output.position = float4(input.position, 1);
    output.tex = input.tex;
    output.texID = 0;

	return output;
}


float4 PSMain(in GS_INPUTOUTPUT input) : SV_Target
{
    float4 sampledTextureColour;
    int textype = round(input.texID.x);
    if (textype == 0)
    {
        sampledTextureColour = DiffuseTexture1.Sample(TextureSampler, input.tex);
    }
    else if(textype == 1)
    {
        sampledTextureColour = DiffuseTexture2.Sample(TextureSampler, input.tex);
    }
    else if (textype == 2)
    {
        sampledTextureColour = DiffuseTexture3.Sample(TextureSampler, input.tex);
    }
    else if (textype == 3)
    {
        sampledTextureColour = DiffuseTexture4.Sample(TextureSampler, input.tex);
    }
    
    return sampledTextureColour;
}


float4x4 TransMatrix(float4 trans)
{
    return float4x4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        trans.x, trans.y, trans.z, 1
    );
}


[instance(4)]
[maxvertexcount(3)]
void GSMain(triangle GS_INPUTOUTPUT input[3], inout TriangleStream<GS_INPUTOUTPUT> TriangleOutputStream, uint InstanceID : SV_GSInstanceID)
{
    GS_INPUTOUTPUT output;
    
    float4 instancePositions[4];
    instancePositions[0] = pos1;
    instancePositions[1] = pos2;
    instancePositions[2] = pos3;
    instancePositions[3] = pos4;
    
    float4x4 transMatrix = TransMatrix(instancePositions[InstanceID]);

    for (int i = 0; i < 3; i++)
    {
        float4 position = input[i].position;
        position = mul(position, WorldMatrix);
        position = mul(position, transMatrix);
        position = mul(position, ViewMatrix);
        output.position = mul(position, ProjMatrix);
        output.tex = input[i].tex;
        output.texID = float2(InstanceID, 0);
        TriangleOutputStream.Append(output);
    }
	
    TriangleOutputStream.RestartStrip();
}
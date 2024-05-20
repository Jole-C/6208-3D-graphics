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
    float4 rotationData : COLOR;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
    float2 tex : TEXCOORD0;
    float4 rotationData : COLOR;
};

struct GS_INPUTOUTPUT
{
    float4 position : SV_Position;
    float2 tex : TEXCOORD0;
    float4 rotationData : COLOR;
};

cbuffer Time
{
    float4 time;
};


float4x4 CreateAxisAngleRotMatrix(float3 axis, float angle)
{
    float c, s;
    sincos(angle, s, c);

    float t = 1 - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    return float4x4(
		t * x * x + c, t * x * y - s * z, t * x * z + s * y, 0,
		t * x * y + s * z, t * y * y + c, t * y * z - s * x, 0,
		t * x * z - s * y, t * y * z + s * x, t * z * z + c, 0,
		0, 0, 0, 1
		);
}

float sigmoid(float t, float k)
{
    float s = 1 / (1 + exp(-k * t));
    return s;
};


[maxvertexcount(3)]
void GSMain(triangle GS_INPUTOUTPUT input[3], inout TriangleStream<GS_INPUTOUTPUT> TriangleOutputStream)
{
    GS_INPUTOUTPUT output;

    float explosionSpeed = 2;
    float explosionRadius = 160;
    float elapsedTime = time.x;
    float totalTime = time.y;
    
    float sine, cosine;
    sincos(totalTime * explosionSpeed, sine, cosine);
    sine = saturate(sine);
    cosine = sigmoid(cosine, 20);
    
    float maxSpinSpeed = 10;
    float spinSpeedPercentage = (input[0].rotationData.x + input[1].rotationData.x + input[2].rotationData.x) / 3;
    float spinSpeed = spinSpeedPercentage * maxSpinSpeed;
    float rotationAngle = spinSpeed * totalTime;
    
    rotationAngle = rotationAngle * sine;
    rotationAngle = fmod(rotationAngle, 2 * 3.142f);
    
    float3 axis = normalize(input[0].rotationData.yzw + input[1].rotationData.yzw + input[2].rotationData.yzw);
    float4x4 rotationMatrix = CreateAxisAngleRotMatrix(axis, rotationAngle);
    
    
    float3 middlePoint = (input[0].position.xyz + input[1].position.xyz + input[2].position.xyz) / 3;
    float3 outwardVector = normalize(middlePoint);
    
    outwardVector = normalize(outwardVector + axis);
    
    float3 displacement = explosionRadius * outwardVector * sine * spinSpeedPercentage;

    for (int i = 0; i < 3; i++)
    {
        float4 position = input[i].position;
        
        position = float4(position.xyz - middlePoint, 1);
        position = mul(position, rotationMatrix);
        position = float4(position.xyz + middlePoint, 1);
        position = float4(position.xyz + displacement, 1);

        output.rotationData = float4(0, 0, 0, cosine);
        output.position = mul(position, WorldViewProjMatrix);
        output.tex = input[i].tex;

        TriangleOutputStream.Append(output);
    }

    TriangleOutputStream.RestartStrip();
}



VS_OUTPUT VSMain(in VS_INPUT input)
{
    VS_OUTPUT output;

    output.position = float4(input.position, 1);
    output.tex = input.tex;
    output.rotationData = input.rotationData;

    return output;
}


float4 PSMain(in VS_OUTPUT input) : SV_Target
{
    float4 sampledTextureColour = DiffuseTexture.Sample(TextureSampler, input.tex);
    sampledTextureColour.w = input.rotationData.w;
    return(sampledTextureColour);
}
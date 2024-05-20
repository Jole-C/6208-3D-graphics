Texture2D       ColorTexture : register(t0);
SamplerState    TextureSampler : register(s0);

cbuffer WorldTransforms_6107COMP
{
	matrix WorldMatrix;
	matrix WorldViewProjMatrix;
};

cbuffer SurfaceReflectanceInfo
{
	float4 SurfaceEmissiveColour;
	float4 SurfaceConstants;			// x: ambient, y: diffuse, z: specular, w: shininess
};

cbuffer SceneInfo_6107COMP
{
	float4 ViewPosition;
};

#define LIGHT_NONE 0
#define LIGHT_POINT 1
#define LIGHT_SPOT 2
#define LIGHT_DIRECTIONAL 3

struct Light
{
	float LightType;
	float3 Padding;
	float4 LightColour;
	float3 LightPosition;
	float3 LightDirection;
	float2 LightRange;
	float2 LightFocus;
	float2 Padding2;
};

cbuffer cLights
{
	Light Lights[10];
}

struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float4 worldPos : POSITION;
	float3 normal : NORMAL;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
};

float3 calculateDirectionalLight(Light light, float3 surfaceNormal, float3 position)
{
	float3 viewVector_d = normalize(position - ViewPosition.xyz);
	float3 directionallightreflectVector = reflect(light.LightDirection, surfaceNormal);
	//float3 directionallightreflectVector = reflect(light.LightDirection, float3(0,1,0));

	// Ambience
	float3 DirectionalLightAmbience = light.LightColour.xyz * SurfaceConstants.x;

	// Diffuse
	float3 DirectionalDiffuse = saturate(dot(surfaceNormal, -light.LightDirection)) * light.LightColour.xyz * SurfaceConstants.y;
	//float3 DirectionalDiffuse = saturate(dot(float3(0,1,0), -light.LightDirection)) * light.LightColour.xyz * SurfaceConstants.y;

	// Specular float3(0,0,0); //
	float3 DirectionalSpecular = pow(saturate(dot(directionallightreflectVector, -viewVector_d)), SurfaceConstants.w) * light.LightColour.xyz * SurfaceConstants.z;

	return (DirectionalLightAmbience + DirectionalDiffuse + DirectionalSpecular);
}

float3 calculatePointLight(Light light, float3 surfaceNormal, float3 position)
{
	float pointDistance = distance(light.LightPosition, position);
	float pointDistAtt = saturate(1 - pointDistance / light.LightRange.x);
	float3 pointlightDirection = normalize(position - light.LightPosition);
	float3 viewVector_p = normalize(position - ViewPosition.xyz);
	float3 pointlightreflectVector = reflect(light.LightDirection, surfaceNormal);

	// Ambience
	float3 PointLightAmbience = light.LightColour.xyz * SurfaceConstants.x;


	// Diffuse
	float3 PointDiffuse = saturate(dot(surfaceNormal, -pointlightDirection)) * pointDistAtt * light.LightColour.xyz * SurfaceConstants.y;

	// Specular
	float3 PointSpecular = pow(saturate(dot(pointlightreflectVector, -viewVector_p)), SurfaceConstants.w) * pointDistAtt * light.LightColour.xyz * SurfaceConstants.z;

	return (PointLightAmbience + PointDiffuse + PointSpecular);
}

float3 calculateSpotLight(Light light, float3 surfaceNormal, float3 position)
{
	float spotDistance = distance(light.LightPosition, position);
	float spotDistAtt = saturate(1 - spotDistance / light.LightRange.x);
	float3 spotlight2PixelVector = normalize(position - light.LightPosition);
	float spotAngularAtt = saturate(pow(dot(spotlight2PixelVector, light.LightDirection), light.LightFocus.x));

	// Ambience
	float3 SpotLightAmbience = light.LightColour.xyz * SurfaceConstants.x;

	// Diffuse
	float3 SpotDiffuse = saturate(dot(surfaceNormal, -light.LightDirection)) * spotDistAtt * spotAngularAtt * light.LightColour.xyz * SurfaceConstants.y;

	// Specular
	float3 viewVector_s = normalize(position - ViewPosition.xyz);
	float3 spotlightreflectVector = reflect(light.LightDirection, surfaceNormal);
	float3 SpotSpecular = pow(saturate(dot(spotlightreflectVector, -viewVector_s)), SurfaceConstants.w) * spotDistAtt * spotAngularAtt * light.LightColour.xyz * SurfaceConstants.z;

	return (SpotLightAmbience + SpotDiffuse + SpotSpecular);
}

VS_OUTPUT VSMain(in VS_INPUT input)
{
	VS_OUTPUT output;

	float4 P = mul(float4(input.position, 1.0f), WorldMatrix);
	output.position = mul(float4(input.position, 1.0f), WorldViewProjMatrix);
	output.worldPos = P;

	matrix Identity =
	{
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	};

	//output.normal = mul(float4(input.normal, 1.0f), WorldMatrix).xyz; //mul(normalize(float4(input.normal, 1.0f)), WorldMatrix).xyz;
	output.normal = mul(float4(input.normal, 1.0f), Identity).xyz;
	output.color = input.color;
	output.tex = input.tex;
	return output;
}

float4 PSMain(in VS_OUTPUT input) : SV_Target
{
	float3 pointLightIntensity = float3(0,0,0);
	float3 spotlightIntensity = float3(0,0,0);
	float3 directionalLightIntensity = float3(0,0,0);
	float3 normalVector = normalize(input.normal);

	// float3(0,1,0); //
	float3 worldPosition = input.worldPos.xyz;

	for (int i = 0; i < 10; i++)
	{
		if (Lights[i].LightType == LIGHT_NONE)
		{
			continue;
		}

		if (Lights[i].LightType == LIGHT_POINT)
		{
			pointLightIntensity += calculatePointLight(Lights[i], normalVector, worldPosition);
			continue;
		}

		if (Lights[i].LightType == LIGHT_SPOT)
		{
			spotlightIntensity += calculateSpotLight(Lights[i], normalVector, worldPosition);
			continue;
		}

		if (Lights[i].LightType == LIGHT_DIRECTIONAL)
		{
			directionalLightIntensity += calculateDirectionalLight(Lights[i], normalVector, worldPosition);
			continue;
		}
	}

	float3 lightIntensity3f = directionalLightIntensity + pointLightIntensity + spotlightIntensity + SurfaceEmissiveColour.xyz;

	lightIntensity3f.x = saturate(lightIntensity3f.x);
	lightIntensity3f.y = saturate(lightIntensity3f.y);
	lightIntensity3f.z = saturate(lightIntensity3f.z);

	float4 pixelcolour = ColorTexture.Sample(TextureSampler, input.tex) * float4(lightIntensity3f, 1);

	return pixelcolour;
}
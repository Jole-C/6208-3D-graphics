#include "LightController.h"
#include "BufferConfigDX11.h"

void LightController::Update()
{
	//Remove invalid lights and materials
	auto iterator = lights.begin();

	while (iterator != lights.end())
	{
		if ((*iterator) == nullptr)
		{
			iterator = lights.erase(iterator);
		}
		else
		{
			iterator++;
		}
	}
}

void LightController::AddLight(LightObject* newLight)
{
	lights.push_back(newLight);
}

void LightController::UpdateMaterialLights(MaterialPtr material, MaterialReflectanceInfo reflectance)
{
	Vector4f surfaceConstants = Vector4f(reflectance.ambientConstant, reflectance.diffuseConstant, reflectance.specularConstant, reflectance.shininessConstant);
	Vector4f colour = reflectance.emissiveColour;

	material->Parameters.SetVectorParameter(L"SurfaceConstants", surfaceConstants);
	material->Parameters.SetVectorParameter(L"SurfaceEmissiveColour", colour);

	for (int i = 0; i < lights.size(); i++)
	{
		lightInfo[i] = lights[i]->lightInfo;
	}

	BufferConfigDX11 config;
	config.SetByteWidth(10 * sizeof(LightInfo));
	config.SetBindFlags(D3D11_BIND_CONSTANT_BUFFER);
	config.SetMiscFlags(0);
	config.SetStructureByteStride(0);
	config.SetUsage(D3D11_USAGE_DYNAMIC); 
	config.SetCPUAccessFlags(D3D11_CPU_ACCESS_WRITE);

	BufferData data;

	for (int i = 0; i < lights.size(); i++)
	{
		data.Lights[i] = lightInfo[i];
	}

	D3D11_SUBRESOURCE_DATA dataLights;
	dataLights.pSysMem = &data;
	dataLights.SysMemPitch = 0;
	dataLights.SysMemSlicePitch = 0;

	ResourcePtr resLights = RendererDX11::Get()->CreateConstantBuffer(&config, &dataLights);
	constantBuffers.push_back(resLights);

	material->Parameters.SetConstantBufferParameter(L"cLights", resLights);
}

void LightController::ClearConstantBuffers()
{
	for (auto buffer : constantBuffers)
	{
		RendererDX11::Get()->DeleteResource(buffer);
	}

	constantBuffers.empty();
}
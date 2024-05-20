#include "MeshObject.h"
#include "RasterizerStateConfigDX11.h"
#include "LJMULevelDemo.h"
#include "LightController.h"
#include <SamplerStateConfigDX11.h>

MeshObject::MeshObject(LJMULevelDemo* worldptr) : Object(worldptr)
{
	reflectanceInfo.ambientConstant = 0.001f;
	reflectanceInfo.diffuseConstant = 4.0f;
	reflectanceInfo.specularConstant = 0.1f;
	reflectanceInfo.shininessConstant = 200.0f;
}

void MeshObject::Update()
{
	if (world->disableLightUpdating == false)
	{
		world->GetLightController()->UpdateMaterialLights(materialPointer, reflectanceInfo);
	}
}

MaterialPtr MeshObject::GenerateMaterial(Glyph3::ResourcePtr texture)
{
	Glyph3::RendererDX11* renderer = Glyph3::RendererDX11::Get();
	Glyph3::MaterialPtr material = Glyph3::MaterialPtr(new Glyph3::MaterialDX11());
	Glyph3::RenderEffectDX11* effect = new Glyph3::RenderEffectDX11;

	effect->SetVertexShader(renderer->LoadShader(Glyph3::VERTEX_SHADER,
		std::wstring(L"LitTexture.hlsl"),
		std::wstring(L"VSMain"),
		std::wstring(L"vs_4_0")));

	effect->SetPixelShader(renderer->LoadShader(Glyph3::PIXEL_SHADER,
		std::wstring(L"LitTexture.hlsl"),
		std::wstring(L"PSMain"),
		std::wstring(L"ps_4_0")));

	Glyph3::RasterizerStateConfigDX11 rsConfig;
	rsConfig.CullMode = D3D11_CULL_NONE;
	rsConfig.FillMode = D3D11_FILL_SOLID;

	int rasterizerState = renderer->CreateRasterizerState(&rsConfig);
	effect->m_iRasterizerState = rasterizerState;

	Glyph3::SamplerStateConfigDX11 SamplerConfig;
	SamplerConfig.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerConfig.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerConfig.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerConfig.MaxAnisotropy = 16;

	int TextureSampler = Glyph3::RendererDX11::Get()->CreateSamplerState(&SamplerConfig);
	material->Parameters.SetSamplerParameter(L"TextureSampler", TextureSampler);

	material->Params[Glyph3::VT_PERSPECTIVE].bRender = true;
	material->Params[Glyph3::VT_PERSPECTIVE].pEffect = effect;


	material->Parameters.SetShaderResourceParameter(L"ColorTexture", texture);

	return material;
}
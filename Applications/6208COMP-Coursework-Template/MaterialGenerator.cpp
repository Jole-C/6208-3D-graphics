#include "MaterialGenerator.h"
#include "RasterizerStateConfigDX11.h"
#include <SamplerStateConfigDX11.h>
#include "BlendStateConfigDX11.h"

using namespace Glyph3;

Glyph3::MaterialPtr MaterialGenerator::CreateLitTexturedMaterial(ResourcePtr texture)
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

Glyph3::MaterialPtr MaterialGenerator::CreateLitMultiTexturedMaterial(ResourcePtr texture, ResourcePtr texture1, ResourcePtr texture2)
{
	Glyph3::RendererDX11* renderer = Glyph3::RendererDX11::Get();
	Glyph3::MaterialPtr material = Glyph3::MaterialPtr(new Glyph3::MaterialDX11());
	Glyph3::RenderEffectDX11* effect = new Glyph3::RenderEffectDX11;

	effect->SetVertexShader(renderer->LoadShader(Glyph3::VERTEX_SHADER,
		std::wstring(L"MultiTexture.hlsl"),
		std::wstring(L"VSMain"),
		std::wstring(L"vs_4_0")));

	effect->SetPixelShader(renderer->LoadShader(Glyph3::PIXEL_SHADER,
		std::wstring(L"MultiTexture.hlsl"),
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


	material->Parameters.SetShaderResourceParameter(L"DiffuseTexture", texture);
	material->Parameters.SetShaderResourceParameter(L"DiffuseTexture1", texture1);
	material->Parameters.SetShaderResourceParameter(L"DiffuseTexture2", texture2);

	return material;
}

Glyph3::MaterialPtr MaterialGenerator::CreateTransparentLitTexturedMaterial(ResourcePtr texture)
{
	Glyph3::RendererDX11* renderer = Glyph3::RendererDX11::Get();
	RenderEffectDX11* pEffect = new RenderEffectDX11();
	Glyph3::MaterialPtr material = Glyph3::MaterialPtr(new Glyph3::MaterialDX11());

	pEffect->SetVertexShader(renderer->LoadShader(VERTEX_SHADER,
		std::wstring(L"LitTexture.hlsl"),
		std::wstring(L"VSMain"),
		std::wstring(L"vs_4_0")));

	pEffect->SetPixelShader(renderer->LoadShader(PIXEL_SHADER,
		std::wstring(L"LitTexture.hlsl"),
		std::wstring(L"PSMain"),
		std::wstring(L"ps_4_0")));

	SamplerStateConfigDX11 SamplerConfig;
	SamplerConfig.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	SamplerConfig.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	SamplerConfig.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerConfig.MaxAnisotropy = 0;

	int TextureSampler = RendererDX11::Get()->CreateSamplerState(&SamplerConfig);
	material->Parameters.SetSamplerParameter(L"TextureSampler", TextureSampler);

	BlendStateConfigDX11 blendConfig{};
	blendConfig.AlphaToCoverageEnable = false;
	blendConfig.IndependentBlendEnable = false;
	for (int i = 0; i < 8; ++i)
	{
		blendConfig.RenderTarget[i].BlendEnable = true;
		blendConfig.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		blendConfig.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendConfig.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendConfig.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendConfig.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendConfig.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;
		blendConfig.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	pEffect->m_iBlendState = RendererDX11::Get()->CreateBlendState(&blendConfig);

	// Enable the material to render the given view type, and set its effect.
	material->Params[VT_PERSPECTIVE].bRender = true;
	material->Params[VT_PERSPECTIVE].pEffect = pEffect;


	material->Parameters.SetShaderResourceParameter(L"ColorTexture", texture);

	return material;
}

Glyph3::MaterialPtr MaterialGenerator::CreateBumpLitTexturedMaterial(ResourcePtr texture, ResourcePtr bumptexture, ResourcePtr lighttexture)
{
	Glyph3::RendererDX11* renderer = Glyph3::RendererDX11::Get();
	Glyph3::MaterialPtr material = Glyph3::MaterialPtr(new Glyph3::MaterialDX11());
	Glyph3::RenderEffectDX11* effect = new Glyph3::RenderEffectDX11;

	effect->SetVertexShader(renderer->LoadShader(Glyph3::VERTEX_SHADER,
		std::wstring(L"BumpLitTexture.hlsl"),
		std::wstring(L"VSMain"),
		std::wstring(L"vs_4_0")));

	effect->SetPixelShader(renderer->LoadShader(Glyph3::PIXEL_SHADER,
		std::wstring(L"BumpLitTexture.hlsl"),
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
	material->Parameters.SetShaderResourceParameter(L"BumpTexture", bumptexture);
	material->Parameters.SetShaderResourceParameter(L"LightTexture", lighttexture);

	return material;
}

Glyph3::MaterialPtr MaterialGenerator::CreateSolidUnlitMaterial()
{
	Glyph3::RendererDX11* renderer = Glyph3::RendererDX11::Get();
	Glyph3::MaterialPtr material = Glyph3::MaterialPtr(new Glyph3::MaterialDX11());
	Glyph3::RenderEffectDX11* effect = new Glyph3::RenderEffectDX11;

	//Set up the effect
	effect->SetVertexShader(renderer->LoadShader(Glyph3::VERTEX_SHADER,
		std::wstring(L"BasicTexture.hlsl"),
		std::wstring(L"VSMain"),
		std::wstring(L"vs_4_0")));

	effect->SetPixelShader(renderer->LoadShader(Glyph3::PIXEL_SHADER,
		std::wstring(L"BasicTexture.hlsl"),
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
	material->Parameters.SetSamplerParameter(L"LinearSampler", TextureSampler);

	//Set the material effect and enable it
	material->Params[Glyph3::VT_PERSPECTIVE].bRender = true;
	material->Params[Glyph3::VT_PERSPECTIVE].pEffect = effect;

	return material;
}

Glyph3::MaterialPtr MaterialGenerator::CreateLineMaterial()
{
	MaterialPtr newMaterial = MaterialPtr(new MaterialDX11());
	Glyph3::RendererDX11* renderer = Glyph3::RendererDX11::Get();

	RenderEffectDX11* pEffect = new RenderEffectDX11();

	pEffect->SetVertexShader(renderer->LoadShader(VERTEX_SHADER, std::wstring(L"Basic.hlsl"), std::wstring(L"VSMain"), std::wstring(L"vs_4_0"), true));
	pEffect->SetPixelShader(renderer->LoadShader(PIXEL_SHADER, std::wstring(L"Basic.hlsl"), std::wstring(L"PSMain"), std::wstring(L"ps_4_0"), true));

	Glyph3::RasterizerStateConfigDX11 rsConfig;
	rsConfig.CullMode = D3D11_CULL_NONE;
	rsConfig.FillMode = D3D11_FILL_SOLID;

	int rasterizerState = renderer->CreateRasterizerState(&rsConfig);
	if (rasterizerState == -1)
	{
		assert(false);
	}

	pEffect->m_iRasterizerState = rasterizerState;

	newMaterial->Params[VT_PERSPECTIVE].bRender = true;
	newMaterial->Params[VT_PERSPECTIVE].pEffect = pEffect;

	return newMaterial;
}

Glyph3::MaterialPtr MaterialGenerator::CreateAnimatedMaterial(ResourcePtr texture1, ResourcePtr texture2)
{
	Glyph3::RendererDX11* renderer = Glyph3::RendererDX11::Get();
	Glyph3::MaterialPtr material = Glyph3::MaterialPtr(new Glyph3::MaterialDX11());
	Glyph3::RenderEffectDX11* effect = new Glyph3::RenderEffectDX11;

	effect->SetVertexShader(renderer->LoadShader(Glyph3::VERTEX_SHADER,
		std::wstring(L"AnimatedTexture.hlsl"),
		std::wstring(L"VSMain"),
		std::wstring(L"vs_4_0")));

	effect->SetPixelShader(renderer->LoadShader(Glyph3::PIXEL_SHADER,
		std::wstring(L"AnimatedTexture.hlsl"),
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


	material->Parameters.SetShaderResourceParameter(L"DiffuseTexture0", texture1);
	material->Parameters.SetShaderResourceParameter(L"DiffuseTexture1", texture2);

	return material;
}

Glyph3::MaterialPtr MaterialGenerator::CreateExplosionMaterial(ResourcePtr texture)
{
	Glyph3::RendererDX11* renderer = Glyph3::RendererDX11::Get();
	Glyph3::MaterialPtr material = Glyph3::MaterialPtr(new Glyph3::MaterialDX11());
	Glyph3::RenderEffectDX11* effect = new Glyph3::RenderEffectDX11;

	effect->SetVertexShader(renderer->LoadShader(VERTEX_SHADER,
		std::wstring(L"ProceduralAnimation.hlsl"),
		std::wstring(L"VSMain"),
		std::wstring(L"vs_4_0")));

	effect->SetPixelShader(renderer->LoadShader(PIXEL_SHADER,
		std::wstring(L"ProceduralAnimation.hlsl"),
		std::wstring(L"PSMain"),
		std::wstring(L"ps_4_0")));

	// Addition ---
	effect->SetGeometryShader(renderer->LoadShader(GEOMETRY_SHADER,
		std::wstring(L"ProceduralAnimation.hlsl"),
		std::wstring(L"GSMain"),
		std::wstring(L"gs_4_0")));
	// Addition ---

	RasterizerStateConfigDX11 rsConfig;
	rsConfig.CullMode = D3D11_CULL_NONE;
	rsConfig.FillMode = D3D11_FILL_SOLID;

	int iRasterizerState = renderer->CreateRasterizerState(&rsConfig);
	effect->m_iRasterizerState = iRasterizerState;

	SamplerStateConfigDX11 SamplerConfig;
	SamplerConfig.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerConfig.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerConfig.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerConfig.MaxAnisotropy = 1;

	BlendStateConfigDX11 blendConfig{};
	blendConfig.AlphaToCoverageEnable = false;
	blendConfig.IndependentBlendEnable = false;
	for (int i = 0; i < 8; ++i)
	{
		blendConfig.RenderTarget[i].BlendEnable = true;
		blendConfig.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		blendConfig.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendConfig.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendConfig.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendConfig.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ZERO;
		blendConfig.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendConfig.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	effect->m_iBlendState = RendererDX11::Get()->CreateBlendState(&blendConfig);

	int TextureSampler = RendererDX11::Get()->CreateSamplerState(&SamplerConfig);
	material->Parameters.SetSamplerParameter(L"TextureSampler", TextureSampler);

	// Enable the material to render the given view type, and set its effect.
	material->Params[VT_PERSPECTIVE].bRender = true;
	material->Params[VT_PERSPECTIVE].pEffect = effect;

	return material;
}

Glyph3::MaterialPtr MaterialGenerator::CreateSolarSystemShader(std::vector<Vector4f> instancePositions, ResourcePtr texture1, ResourcePtr texture2, ResourcePtr texture3, ResourcePtr texture4)
{
	Glyph3::RendererDX11* renderer = Glyph3::RendererDX11::Get();
	Glyph3::MaterialPtr material = Glyph3::MaterialPtr(new Glyph3::MaterialDX11());
	Glyph3::RenderEffectDX11* effect = new Glyph3::RenderEffectDX11;

	effect->SetVertexShader(renderer->LoadShader(Glyph3::VERTEX_SHADER,
		std::wstring(L"BasicGeometry.hlsl"),
		std::wstring(L"VSMain"),
		std::wstring(L"vs_4_0")));

	effect->SetPixelShader(renderer->LoadShader(Glyph3::PIXEL_SHADER,
		std::wstring(L"BasicGeometry.hlsl"),
		std::wstring(L"PSMain"),
		std::wstring(L"ps_4_0")));

	effect->SetGeometryShader(renderer->LoadShader(Glyph3::GEOMETRY_SHADER,
		std::wstring(L"BasicGeometry.hlsl"),
		std::wstring(L"GSMain"),
		std::wstring(L"gs_5_0")));

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

	material->Parameters.SetVectorParameter(L"pos1", instancePositions[0]);
	material->Parameters.SetVectorParameter(L"pos2", instancePositions[1]);
	material->Parameters.SetVectorParameter(L"pos3", instancePositions[2]);
	material->Parameters.SetVectorParameter(L"pos4", instancePositions[3]);

	material->Parameters.SetShaderResourceParameter(L"DiffuseTexture1", texture1);
	material->Parameters.SetShaderResourceParameter(L"DiffuseTexture2", texture2);
	material->Parameters.SetShaderResourceParameter(L"DiffuseTexture3", texture3);
	material->Parameters.SetShaderResourceParameter(L"DiffuseTexture4", texture4);

	return material;
}

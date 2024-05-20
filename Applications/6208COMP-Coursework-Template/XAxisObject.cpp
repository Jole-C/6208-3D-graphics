#include "XAxisObject.h"
#include "LJMULevelDemo.h"
#include "RasterizerStateConfigDX11.h"


XAxisObject::XAxisObject(LJMULevelDemo* worldptr) : MeshObject(worldptr)
{
	Glyph3::MaterialPtr AxisMaterial = CreateLineMaterial();
	BasicMeshPtr XAxisMesh = CreateXAxisMesh();
	GetBody()->SetGeometry(XAxisMesh);
	GetBody()->SetMaterial(AxisMaterial);
	GetNode()->Position() = Glyph3::Vector3f(0, 0, 0);
	GetNode()->Scale() = Glyph3::Vector3f(1, 1, 1);

	world->m_pScene->AddActor(this);
}

BasicMeshPtr XAxisObject::CreateXAxisMesh()
{
	std::vector<Vector3f> points;
	points.push_back(Vector3f(0, 0, 0));
	points.push_back(Vector3f(50, 0, 0));
	points.push_back(Vector3f(49, 1, 0));
	points.push_back(Vector3f(49, -1, 0));
	points.push_back(Vector3f(50, 0, 0));

	int numPoints = points.size();
	auto axisMesh = std::make_shared<DrawExecutorDX11<BasicVertexDX11::Vertex>>();
	axisMesh->SetLayoutElements(BasicVertexDX11::GetElementCount(), BasicVertexDX11::Elements);
	axisMesh->SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	axisMesh->SetMaxVertexCount(numPoints);

	BasicVertexDX11::Vertex tv;
	for (int i = 0; i < numPoints; i++)
	{
		tv.position = points[i];
		tv.color = Vector4f(1, 0, 0, 1);
		axisMesh->AddVertex(tv);
	}

	return axisMesh;
}

Glyph3::MaterialPtr XAxisObject::CreateLineMaterial()
{
	MaterialPtr newMaterial = MaterialPtr(new MaterialDX11());

	RenderEffectDX11* pEffect = new RenderEffectDX11();

	pEffect->SetVertexShader(world->GetRenderer()->LoadShader(VERTEX_SHADER, std::wstring(L"Basic.hlsl"), std::wstring(L"VSMain"), std::wstring(L"vs_4_0"), true));
	pEffect->SetPixelShader(world->GetRenderer()->LoadShader(PIXEL_SHADER, std::wstring(L"Basic.hlsl"), std::wstring(L"PSMain"), std::wstring(L"ps_4_0"), true));

	Glyph3::RasterizerStateConfigDX11 rsConfig;
	rsConfig.CullMode = D3D11_CULL_NONE;
	rsConfig.FillMode = D3D11_FILL_SOLID;

	int rasterizerState = world->GetRenderer()->CreateRasterizerState(&rsConfig);
	if (rasterizerState == -1)
	{
		Log::Get().Write(L"Failed to create light rasterizer state");
		assert(false);
	}

	pEffect->m_iRasterizerState = rasterizerState;

	newMaterial->Params[VT_PERSPECTIVE].bRender = true;
	newMaterial->Params[VT_PERSPECTIVE].pEffect = pEffect;

	return newMaterial;
}

#include "SpaceStationDomes.h"
#include "LJMULevelDemo.h"
#include "MeshGenerator.h"
#include "LightController.h"

SpaceStationDomes::SpaceStationDomes(LJMULevelDemo* worldptr) : MeshObject(worldptr)
{
	MeshGenerator meshGenerator;
	meshPointer = meshGenerator.GenerateMeshFromOBJFile(L"SpaceStationDomes.obj", Vector4f(1, 1, 1, 1));
	GetBody()->SetGeometry(meshPointer);

	texturePointer = Glyph3::RendererDX11::Get()->LoadTexture(L"SpaceStationTexture.png");
	materialPointer = GenerateMaterial(texturePointer);
	world->GetLightController()->UpdateMaterialLights(materialPointer, reflectanceInfo);
	GetBody()->SetMaterial(materialPointer);

	GetNode()->Position() = Glyph3::Vector3f(30, 70, 10);
	GetNode()->Scale() = Glyph3::Vector3f(17, 17, 17);
	world->m_pScene->AddActor(this);
}

void SpaceStationDomes::Update()
{
	MeshObject::Update();
}

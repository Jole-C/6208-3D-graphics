#include "SpaceStationRocket.h"
#include "LJMULevelDemo.h"
#include "MeshGenerator.h"
#include "LightController.h"

SpaceStationRocket::SpaceStationRocket(LJMULevelDemo* worldptr) : MeshObject(worldptr)
{
	MeshGenerator meshGenerator;
	meshPointer = meshGenerator.GenerateMeshFromOBJFile(L"SpaceStationRocket.obj", Vector4f(1, 1, 1, 1));
	GetBody()->SetGeometry(meshPointer);

	texturePointer = Glyph3::RendererDX11::Get()->LoadTexture(L"SpaceStationTexture.png");
	materialPointer = GenerateMaterial(texturePointer);
	world->GetLightController()->UpdateMaterialLights(materialPointer, reflectanceInfo);
	GetBody()->SetMaterial(materialPointer);

	GetNode()->Position() = Glyph3::Vector3f(100, 70, 150);
	GetNode()->Scale() = Glyph3::Vector3f(8, 8, 8);

	Glyph3::Entity3D* rocketBase = new Glyph3::Entity3D();
	BasicMeshPtr rocketBaseMesh = meshGenerator.GenerateMeshFromOBJFile(L"SpaceStationRocketBase.obj", Vector4f(1, 1, 1, 1));
	rocketBase->SetMaterial(materialPointer);
	rocketBase->SetGeometry(rocketBaseMesh);
	GetNode()->AttachChild(rocketBase);
	rocketBase->Position() = Vector3f(0, -1, 0);
	rocketBase->Scale() = Vector3f(1, 1, 1);

	world->m_pScene->AddActor(this);
}

void SpaceStationRocket::Update()
{
	MeshObject::Update();
}

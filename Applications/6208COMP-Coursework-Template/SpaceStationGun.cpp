#include "SpaceStationGun.h"
#include "LJMULevelDemo.h"
#include "MeshGenerator.h"
#include "LightController.h"

SpaceStationGun::SpaceStationGun(LJMULevelDemo* worldptr) : MeshObject(worldptr)
{
	MeshGenerator meshGenerator;
	meshPointer = meshGenerator.GenerateMeshFromOBJFile(L"SpaceStationGunBase.obj", Vector4f(1, 1, 1, 1));
	GetBody()->SetGeometry(meshPointer);

	texturePointer = Glyph3::RendererDX11::Get()->LoadTexture(L"SpaceStationTexture.png");
	materialPointer = GenerateMaterial(texturePointer);
	world->GetLightController()->UpdateMaterialLights(materialPointer, reflectanceInfo);
	GetBody()->SetMaterial(materialPointer);

	GetNode()->Position() = Glyph3::Vector3f(250, 70, 250);
	GetNode()->Scale() = Glyph3::Vector3f(8, 8, 8);

	Glyph3::Entity3D* rocketBase = new Glyph3::Entity3D();
	BasicMeshPtr rocketBaseMesh = meshGenerator.GenerateMeshFromOBJFile(L"SpaceStationGun.obj", Vector4f(1, 1, 1, 1));
	rocketBase->SetMaterial(materialPointer);
	rocketBase->SetGeometry(rocketBaseMesh);
	GetNode()->AttachChild(rocketBase);
	rocketBase->Position() = Vector3f(0, 1, 0);
	rocketBase->Scale() = Vector3f(1, 1, 1);

	world->m_pScene->AddActor(this);
}

void SpaceStationGun::Update()
{
	MeshObject::Update();
}

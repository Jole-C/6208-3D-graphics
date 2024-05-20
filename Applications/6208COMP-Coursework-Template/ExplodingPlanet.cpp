#include "ExplodingPlanet.h"
#include "LJMULevelDemo.h"
#include "MeshGenerator.h"
#include "MaterialGenerator.h"
#include "EarthSphere.h"

ExplodingPlanet::ExplodingPlanet(LJMULevelDemo* worldptr) : MeshObject(worldptr)
{
	MeshGenerator meshGenerator;
	meshPointer = meshGenerator.GenerateMeshFromOBJFile(L"geosphere.obj", Vector4f(1, 1, 1, 1));
	GetBody()->SetGeometry(meshPointer);

	MaterialGenerator materialGenerator;
	texturePointer = Glyph3::RendererDX11::Get()->LoadTexture(L"moon.jpg");
	materialPointer = materialGenerator.CreateExplosionMaterial(texturePointer);
	GetBody()->SetMaterial(materialPointer);
	
	world->m_pScene->AddActor(this);

	GetNode()->Position() = Glyph3::Vector3f(1700, 800, -800);
	GetNode()->Scale() = Vector3f(2, 2, 2);
}

void ExplodingPlanet::Update()
{
	time += 0.001;
	totalTime += 0.001;

	if (time > 1)
	{
		time = 0;
	}

	materialPointer->Parameters.SetVectorParameter(L"time", Vector4f(time, totalTime, 0, 0));

	angle += rotationSpeed;

	Vector3f earthPosition = world->earth->GetNode()->Position();
	Vector3f position = Vector3f(cos(angle) * radius, 0, sin(angle) * radius);
	GetNode()->Position() = earthPosition + position;
}

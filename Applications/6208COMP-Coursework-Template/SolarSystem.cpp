#include "SolarSystem.h"
#include "MaterialGenerator.h"
#include "LJMULevelDemo.h"

SolarSystem::SolarSystem(LJMULevelDemo* worldptr) : MeshObject(worldptr)
{
	std::vector<Vector3f> points;
	std::vector<Vector2f> texcoords;
	for (int y = 0; y <= verticalResolution; y++)
	{
		float elevationAngle = GLYPH_PI * float(y) / float(verticalResolution);
		float sp = (float)std::sin(elevationAngle);
		float cp = (float)std::cos(elevationAngle);

		for (int x = 0; x <= horizontalResolution; x++)
		{
			float azimuthAngle = 2.0 * GLYPH_PI * float(x) / float(horizontalResolution);
			float sa = std::sin(azimuthAngle);
			float ca = std::cos(azimuthAngle);

			Vector3f vertex = Vector3f(sp * ca, cp, sp * sa);
			Vector2f uv = Vector2f(float(x) / float(horizontalResolution), float(y) / float(verticalResolution));

			points.push_back(vertex);
			texcoords.push_back(uv);
		}
	}

	//Generate the indices
	std::vector<int> indices;

	for (int x = 0; x < horizontalResolution; x++)
	{
		for (int y = 0; y < verticalResolution; y++)
		{
			int vertUpperLeft = x * verticalResolution + y;
			int vertUpperRight = (x + 1) * verticalResolution + y;
			int vertLowerLeft = (x + 1) * verticalResolution + (y + 1);
			int vertLowerRight = x * verticalResolution + (y + 1);

			indices.push_back(vertLowerRight);
			indices.push_back(vertUpperRight);
			indices.push_back(vertUpperLeft);

			indices.push_back(vertLowerRight);
			indices.push_back(vertLowerLeft);
			indices.push_back(vertUpperRight);
		}
	}

	BasicVertexDX11::Vertex vertex;
	meshPointer = std::make_shared<DrawExecutorDX11<BasicVertexDX11::Vertex>>();
	meshPointer->SetLayoutElements(BasicVertexDX11::GetElementCount(), BasicVertexDX11::Elements);
	meshPointer->SetPrimitiveType(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	meshPointer->SetMaxVertexCount(indices.size());

	for (int i = 0; i < indices.size(); i++)
	{
		vertex.position = points[indices[i]];
		vertex.texcoords = texcoords[indices[i]];
		vertex.normal = Vector3f(0, 1, 0);

		meshPointer->AddVertex(vertex);
	}

	GetBody()->SetGeometry(meshPointer);

	planetPositions.push_back(Vector4f(600, solarSystemHeight, 600, 1));
	planetPositions.push_back(Vector4f(-600, solarSystemHeight, 600, 1));
	planetPositions.push_back(Vector4f(1700, 800, 0, 1));
	planetPositions.push_back(Vector4f(600, solarSystemHeight, -600, 1));

	PlanetPositionDefinition mars;
	mars.radius = 800;
	mars.rotationSpeed = 0.005;
	
	PlanetPositionDefinition moon;
	moon.radius = 1500;
	moon.rotationSpeed = -0.0005;
	
	PlanetPositionDefinition sun;
	sun.radius = 1800;
	sun.angle = 0.5;
	sun.rotationSpeed = 0.0007;

	planets.push_back(mars);
	planets.push_back(moon);
	planets.push_back(sun);

	texture1 = Glyph3::RendererDX11::Get()->LoadTexture(L"mars.tif");
	texture2 = Glyph3::RendererDX11::Get()->LoadTexture(L"moon.jpg");
	texture3 = Glyph3::RendererDX11::Get()->LoadTexture(L"sun.jpg");
	texture4 = Glyph3::RendererDX11::Get()->LoadTexture(L"8k_sun.jpg");

	MaterialGenerator materialGenerator;
	materialPointer = materialGenerator.CreateSolarSystemShader(planetPositions, texture1, texture2, texture3, texture4);
	GetBody()->SetMaterial(materialPointer);
	
	GetNode()->Scale() = Glyph3::Vector3f(200, 200, 200);
	world->m_pScene->AddActor(this);
}

void SolarSystem::Update()
{
	for (int i = 0; i < planets.size(); i++)
	{
		Vector4f& planetPosition = planets[i].position;
		float& angle = planets[i].angle;
		float radius = planets[i].radius;
		float rotationSpeed = planets[i].rotationSpeed;
		
		planets[i].angle += planets[i].rotationSpeed;

		planetPosition = Vector4f(1700 + cos(angle) * radius, 800, sin(angle) * radius, 1);
	}

	planetPositions[0] = planets[0].position;
	planetPositions[1] = planets[1].position;
	planetPositions[2] = Vector4f(1700, 800, 0, 1);
	planetPositions[3] = planets[2].position;

	materialPointer->Parameters.SetVectorParameter(L"pos1", planetPositions[0]);
	materialPointer->Parameters.SetVectorParameter(L"pos2", planetPositions[1]);
	materialPointer->Parameters.SetVectorParameter(L"pos3", planetPositions[2]);
	materialPointer->Parameters.SetVectorParameter(L"pos4", planetPositions[3]);
}

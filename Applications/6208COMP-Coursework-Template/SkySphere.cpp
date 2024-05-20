#include "SkySphere.h"
#include "LJMULevelDemo.h"
#include "MaterialGenerator.h"
#include "LightController.h"
#include "MathHelpers.h"

SkySphere::SkySphere(LJMULevelDemo* worldptr) : MeshObject(worldptr)
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

	//Add the vertices to the mesh, calculating the normal at the same time
	for (int i = 0; i < indices.size(); i++)
{
		vertex.position = points[indices[i]];
		vertex.texcoords = texcoords[indices[i]];
		vertex.normal = Vector3f(0, 1, 0);

		meshPointer->AddVertex(vertex);
	}

	GetBody()->SetGeometry(meshPointer);

	//Load textures
	texturePointer = Glyph3::RendererDX11::Get()->LoadTexture(L"DaySky.png");
	nightTexturePointer = Glyph3::RendererDX11::Get()->LoadTexture(L"NightSky.png");

	MaterialGenerator materialGenerator;
	materialPointer = materialGenerator.CreateAnimatedMaterial(texturePointer, nightTexturePointer);
	GetBody()->SetMaterial(materialPointer);

	GetNode()->Scale() = Glyph3::Vector3f(6000, 6000, 6000);
	GetNode()->Position() = Glyph3::Vector3f(0, 70, 0);
	world->m_pScene->AddActor(this);
}

void SkySphere::Update()
{
	angle += 0.001;

	if (angle > GLYPH_PI)
	{
		angle = 0;
	}

	time = sin(angle);

	Vector4f textureWeight = Vector4f(time, 0, 0, 0);
	materialPointer->Parameters.SetVectorParameter(L"texWeight", textureWeight);

	float s = sin(time / dayLength);
	float sigmoid = 1 / (1 + exp(-s * abruptness));
}

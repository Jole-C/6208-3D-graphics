#include "EarthSphere.h"
#include "MaterialGenerator.h"
#include "LJMULevelDemo.h"
#include "LightController.h"
#include "MathHelpers.h"

EarthSphere::EarthSphere(LJMULevelDemo* worldptr) : MeshObject(worldptr)
{
	std::vector<Vector3f> points;
	std::vector<Vector3f> normals;
	std::vector<Vector2f> texcoords;
	std::vector<Vector3f> tangent;
	std::vector<Vector3f> binormal;

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
			normals.push_back(Vector3f(sp * ca, cp, sp * sa));
			tangent.push_back(Vector3f(1, 0, 0));
			binormal.push_back(Vector3f(0, 0, 1));
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

	CustomVertexDX11::Vertex vertex;
	meshPointer = std::make_shared<DrawExecutorDX11<CustomVertexDX11::Vertex>>();
	meshPointer->SetLayoutElements(CustomVertexDX11::GetElementCount(), CustomVertexDX11::Elements);
	meshPointer->SetPrimitiveType(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	meshPointer->SetMaxVertexCount(indices.size());

	//Add the vertices to the mesh, calculating the normal at the same time
	for (int i = 0; i < indices.size(); i++)
	{
		vertex.position = points[indices[i]];
		vertex.texcoords = texcoords[indices[i]];
		vertex.normal = normals[indices[i]];
		vertex.tangent = tangent[indices[i]];
		vertex.binormal = binormal[indices[i]];
		meshPointer->AddVertex(vertex);
	}

	GetBody()->SetGeometry(meshPointer);

	//Create a duplicate entity for the clouds
	clouds = new Glyph3::Entity3D();
	clouds->SetGeometry(meshPointer);
	clouds->Scale() = Vector3f(1.05, 1.05, 1.05);
	GetNode()->AttachChild(clouds);
	AddElement(clouds);

	//Load textures
	texturePointer = Glyph3::RendererDX11::Get()->LoadTexture(L"8k_earth_daymap.jpg");
	bumpTexturePointer = Glyph3::RendererDX11::Get()->LoadTexture(L"8k_earth_normal_map.tif");
	lightTexturePointer = Glyph3::RendererDX11::Get()->LoadTexture(L"8k_earth_nightmap.jpg");
	cloudsTexturePointer = Glyph3::RendererDX11::Get()->LoadTexture(L"8k_earth_clouds.png");

	//Apply materials
	MaterialGenerator materialGenerator;
	materialPointer = materialGenerator.CreateBumpLitTexturedMaterial(texturePointer, bumpTexturePointer, lightTexturePointer);
	cloudsMaterialPointer = materialGenerator.CreateTransparentLitTexturedMaterial(cloudsTexturePointer);
	GetBody()->SetMaterial(materialPointer);
	clouds->SetMaterial(cloudsMaterialPointer);

	GetNode()->Scale() = Glyph3::Vector3f(150, 150, 150);
	GetNode()->Position() = Glyph3::Vector3f(1700, 800, 0);
	world->m_pScene->AddActor(this);
}

void EarthSphere::Update()
{
	MeshObject::Update();

	earthRotation += rotationSpeed;
	cloudsRotation -= rotationSpeed;

	Matrix3f earthMatrix;
	Matrix3f cloudsMatrix;
	earthMatrix.RotationY(earthRotation);
	cloudsMatrix.RotationY(cloudsRotation);

	GetBody()->Rotation() = earthMatrix;
	clouds->Rotation() = cloudsMatrix;

	if (world->disableLightUpdating == false)
	{
		world->GetLightController()->UpdateMaterialLights(cloudsMaterialPointer, reflectanceInfo);
	}

	angle += rotationSpeed;

	Vector3f position = Vector3f(1700 + cos(angle) * radius, 800, sin(angle) * radius);
	GetNode()->Position() = position;
}

#include "TerrainMeshHeightmap.h"
#include "LJMULevelDemo.h"
#include "LightController.h"

TerrainMeshHeightmap::TerrainMeshHeightmap(LJMULevelDemo* worldptr) : TerrainMesh(worldptr)
{
	texturePointer = Glyph3::RendererDX11::Get()->LoadTexture(L"Sand1.jpg");
	GenerateTerrain();
}

void TerrainMeshHeightmap::Update()
{
	GenerateChunks(renderingOrigin);

	if (world->disableLightUpdating == false)
	{
		world->GetLightController()->UpdateMaterialLights(materialPointer, reflectanceInfo);
	}
}

void TerrainMeshHeightmap::GenerateTerrain()
{
	dimensions = Vector2f(31, 31);
	newDimensions = Vector2f(32, 32);
	numberOfTerrainChunks = Vector2f(16, 16);

	terrainOrigin = -((numberOfTerrainChunks * (dimensions * terrainVertexSeparation)) * 0.5);
	SetPosition(Vector3f(terrainOrigin.x, 0, terrainOrigin.y));

	int actualSize = 512 * 512;
	unsigned short* rawImage = new unsigned short[actualSize];
	numberOfTerrainChunks = Vector2f(16, 16);

	FILE* fileptr;
	int file = fopen_s(&fileptr, std::string("heightmap512X512.r16").c_str(), "rb");
	int count = fread(rawImage, sizeof(unsigned short), actualSize, fileptr);
	file = fclose(fileptr);

	//Initialise the heightmap array
	for (int x = 0; x < newDimensions.x * numberOfTerrainChunks.x; x++)
	{
		std::vector<float> newColumn;
		newColumn.resize(newDimensions.y * numberOfTerrainChunks.y);
		heightdata.push_back(newColumn);
	}

	for (int x = 0; x < newDimensions.x * numberOfTerrainChunks.x; x++)
	{
		for (int y = 0; y < newDimensions.y * numberOfTerrainChunks.y; y++)
		{
			heightdata[x][y] = (double)rawImage[(y * 512) + x] * 0.005;
		}
	}

	GenerateChunks(renderingOrigin);
	world->m_pScene->AddActor(this);
	delete[] rawImage;
}

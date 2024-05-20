#include "TerrainMeshProcedural.h"
#include "FastNoiseLite.h"
#include "LJMULevelDemo.h"
#include "LightController.h"

TerrainMeshProcedural::TerrainMeshProcedural(LJMULevelDemo* worldptr) : TerrainMesh(worldptr)
{
	GenerateTerrain();
}

void TerrainMeshProcedural::Update()
{
	GenerateChunks(renderingOrigin);

	if (world->disableLightUpdating == false)
	{
		world->GetLightController()->UpdateMaterialLights(materialPointer, reflectanceInfo);
	}
}

//Generate the terrain
void TerrainMeshProcedural::GenerateTerrain()
{
	//Figure out the terrain origin
	terrainOrigin = -((numberOfTerrainChunks * (dimensions * terrainVertexSeparation)) * 0.5);
	SetPosition(Vector3f(terrainOrigin.x, 0, terrainOrigin.y));

	//Initialise the fast noise
	FastNoiseLite FastNoise(1337);
	FastNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	FastNoise.SetFrequency(0.03);

	//Initialise the noise array
	for (int x = 0; x < newDimensions.x * numberOfTerrainChunks.x; x++)
	{
		std::vector<float> newColumn;
		newColumn.resize(newDimensions.y * numberOfTerrainChunks.y);
		heightdata.push_back(newColumn);
	}

	//Get the noise values and make sure they are normalised from -1,1, to 0,1
	for (int x = 0; x < newDimensions.x * numberOfTerrainChunks.x; x++)
	{
		for (int y = 0; y < newDimensions.y * numberOfTerrainChunks.y; y++)
		{
			float normalisedX = x / newDimensions.x * numberOfTerrainChunks.x;
			float normalisedY = y / newDimensions.y * numberOfTerrainChunks.y;
			float normalisedNoise = (FastNoise.GetNoise((float)x, (float)y) + 1) / 2;

			heightdata[x][y] = normalisedNoise * 50;
		}
	}

	//Generate the initial chunks
	GenerateChunks(renderingOrigin);
	world->m_pScene->AddActor(this);
}
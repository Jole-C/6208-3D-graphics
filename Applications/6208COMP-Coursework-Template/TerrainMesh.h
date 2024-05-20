#pragma once
#include "MeshObject.h"
#include <DrawExecutorDX11.h>
#include "BasicVertexDX11.h"

class LJMULevelDemo;

struct TerrainChunk
{
	Glyph3::Entity3D* mesh;
	Vector2f index;
};

struct NoiseChunk
{
	std::vector<std::vector<float>> noise;
	Vector2f index;
};

class TerrainMesh : public MeshObject
{
public:
	TerrainMesh(LJMULevelDemo* worldptr);
	void SetRenderingOrigin(Vector2f newOrigin);
	virtual void Update() = 0;
protected:
	int terrainVertexSeparation = 10;

	//Variables for rendering
	Vector2f renderingOrigin = Vector2f(0, 0);
	Vector2f chunkRenderingRadius = Vector2f(6, 6);
	float renderingDistance;
	
	//Variables for terrain layout
	Vector2f numberOfTerrainChunks = Vector2f(20, 20);
	Vector2f dimensions = Vector2f(32, 32);
	Vector2f newDimensions = Vector2f();
	Vector2f terrainOrigin;

	ResourcePtr grassTexture;
	ResourcePtr rockTexture;
	ResourcePtr snowTexture;

	//Loaded chunks and height data
	std::vector<TerrainChunk> loadedChunks;
	std::vector<std::vector<float>> heightdata;

	virtual void GenerateTerrain();
	CustomMeshPtr TerrainMesh::GenerateGrid(Vector2f origin, std::vector<std::vector<float>> heightSegment);
	void GenerateChunks(Vector2f index);

	std::vector<std::vector<float>> FindHeightData(Vector2f topLeftCoords);
	bool IsChunkLoaded(Vector2f index);
	bool IsChunkInRange(Vector2f index);
};


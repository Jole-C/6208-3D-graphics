#pragma once
#include "TerrainMesh.h"
class TerrainMeshHeightmap : public TerrainMesh
{
public:
	TerrainMeshHeightmap(LJMULevelDemo* worldptr);
	void Update();

protected:
	void GenerateTerrain() override;

};


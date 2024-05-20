#pragma once
#include "TerrainMesh.h"
class TerrainMeshProcedural : public TerrainMesh
{
public:
	TerrainMeshProcedural(LJMULevelDemo* worldptr);
	void Update() override;

protected:
	void GenerateTerrain() override;

};


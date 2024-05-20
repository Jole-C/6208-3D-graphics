#pragma once
#include "MeshObject.h"
class SpaceStationRocket : public MeshObject
{
public:
	SpaceStationRocket(LJMULevelDemo* worldptr);
	void Update() override;
};


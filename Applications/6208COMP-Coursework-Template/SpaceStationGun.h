#pragma once
#include "MeshObject.h"
class SpaceStationGun : public MeshObject
{
public:
	SpaceStationGun(LJMULevelDemo* worldptr);
	void Update() override;
};


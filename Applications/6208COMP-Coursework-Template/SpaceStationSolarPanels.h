#pragma once
#include "MeshObject.h"
class SpaceStationSolarPanels : public MeshObject
{
public:
	SpaceStationSolarPanels(LJMULevelDemo* worldptr);
	void Update() override;
};


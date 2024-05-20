#pragma once
#include "MeshObject.h"
class ExplodingPlanet : public MeshObject
{
public:
	ExplodingPlanet(LJMULevelDemo* worldptr);
	void Update() override;

protected:
	float totalTime;
	float time;
	float angle = 0;
	float radius = 300;
	float rotationSpeed = 0.005;
};


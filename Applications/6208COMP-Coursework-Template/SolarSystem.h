#pragma once

#include "MeshObject.h"

struct PlanetPositionDefinition
{
	Vector4f position;
	float angle = 0;
	float radius;
	float rotationSpeed = 0.0005;
};

class SolarSystem : public MeshObject
{
public:
	SolarSystem(LJMULevelDemo* worldptr);
	void Update() override;

protected:
	ResourcePtr texture1;
	ResourcePtr texture2;
	ResourcePtr texture3;
	ResourcePtr texture4;
	std::vector<Vector4f> planetPositions;
	std::vector<PlanetPositionDefinition> planets;
	int verticalResolution = 32;
	int horizontalResolution = 32;
	int solarSystemHeight = 700;
};


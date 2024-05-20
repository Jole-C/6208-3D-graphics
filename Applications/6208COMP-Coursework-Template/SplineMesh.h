#pragma once
#include "MeshObject.h"
#include "Spline.h"

class SplineMesh : public MeshObject
{
public:
	SplineMesh(LJMULevelDemo* worldptr);
	void Update() override {};
	std::vector<Vector3f> splinePoints;
	Spline spline;
};


#pragma once
#include "MeshObject.h"
#include "Spline.h"

class Maxwell : public MeshObject
{
public:
	Maxwell(LJMULevelDemo* worldptr);
	void Update() override;

protected:
	std::vector<Vector3f> splinePoints;
	Spline spline;
	float time;
};


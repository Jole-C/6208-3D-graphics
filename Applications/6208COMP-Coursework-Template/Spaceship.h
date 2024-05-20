#pragma once
#include "MeshObject.h"
#include "Spline.h"

using namespace Glyph3;
class Spline;

class Spaceship : public MeshObject
{
public:
	Spaceship(LJMULevelDemo* worldptr);
	void Update() override;

protected:
	std::vector<Vector3f> splinePoints;
	Spline spline;
	float time;
	float time2;
	float timeIncrement = 0.001;
	float moveTime;
	float moveSpeed = 0.3;
};


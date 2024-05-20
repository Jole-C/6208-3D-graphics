#pragma once
#include "MeshObject.h"

class SkySphere : public MeshObject
{
public:
	SkySphere(LJMULevelDemo* worldptr);
	void Update() override;
	float GetSkyTime() { return time; }

protected:
	ResourcePtr nightTexturePointer;
	int verticalResolution = 32;
	int horizontalResolution = 32;
	float angle;
	float time;
	float dayLength = 5;
	float abruptness = 5;
};


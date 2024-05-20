#pragma once
#include "MeshObject.h"

class EarthSphere : public MeshObject
{
public:
	EarthSphere(LJMULevelDemo* worldptr);
	void Update() override;

protected:
	int verticalResolution = 32;
	int horizontalResolution = 32;
	float rotationSpeed = 0.0005;
	CustomMeshPtr meshPointer;
	ResourcePtr bumpTexturePointer;
	ResourcePtr lightTexturePointer;
	ResourcePtr cloudsTexturePointer;
	MaterialPtr cloudsMaterialPointer;
	Glyph3::Entity3D* clouds;

	float earthRotation;
	float cloudsRotation;
	float angle = 0;
	float radius = 500;
};


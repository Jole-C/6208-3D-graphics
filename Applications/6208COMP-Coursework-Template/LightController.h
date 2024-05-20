#pragma once
#include "Object.h"
#include "MaterialReflectanceInfo.h"
#include "LightObject.h"

struct BufferData
{
	LightInfo Lights[10];
};

class LightController : public Object
{
public:
	LightController(class LJMULevelDemo* worldptr) : Object(worldptr) {}

	void Update() override;
	void AddLight(LightObject* newLight);
	void UpdateMaterialLights(MaterialPtr material, MaterialReflectanceInfo reflectance);
	void ClearConstantBuffers();
protected:
	std::vector<MaterialPtr> materials;
	std::vector<LightObject*> lights;
	std::vector<ResourcePtr> constantBuffers;
	LightInfo lightInfo[10];

};


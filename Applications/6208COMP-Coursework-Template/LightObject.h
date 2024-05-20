#pragma once
#include "Object.h"

using namespace Glyph3;

enum class LIGHT_TYPES
{
	LIGHT_NONE,
	LIGHT_POINT,
	LIGHT_SPOT,
	LIGHT_DIRECTIONAL
};

struct LightInfo
{
	Vector4f iLightType = Vector4f(static_cast<float>(LIGHT_TYPES::LIGHT_NONE), 0.0f, 0.0f, 0.0f);
	Vector4f LightColour = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
	Vector4f LightPosition = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
	Vector4f LightDirection = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
	Vector4f LightRange = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
	Vector4f LightFocus = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
};

class LightObject : public Object
{
public:
	LightObject(class LJMULevelDemo* world) : Object(world) {};
	void Update() override;

	LIGHT_TYPES lightType = LIGHT_TYPES::LIGHT_NONE;
	LightInfo lightInfo;
};


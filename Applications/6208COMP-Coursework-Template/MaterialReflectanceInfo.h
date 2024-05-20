#pragma once

struct MaterialReflectanceInfo
{
	float ambientConstant = 0.001f;
	float diffuseConstant = 4.0f;
	float specularConstant = 0.1f;
	float shininessConstant = 200.0f;
	Vector4f emissiveColour = Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
};
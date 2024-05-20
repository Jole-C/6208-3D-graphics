#pragma once
#include <algorithm>
#include <Vector3f.h>
#include <Matrix3f.h>

#define PI 3.14159265359

class MathHelper
{
public:
	static float Clamp(float n, float lower, float upper)
	{
		return n <= lower ? lower : n >= upper ? upper : n;
	}

	static float Distance(Glyph3::Vector3f A, Glyph3::Vector3f B)
	{
		return (B - A).Magnitude();
	}

	static Vector3f LinearInterp(Vector3f p0, Vector3f p1, float t)
	{
		Vector3f result;
		result = p0 * (1 - t) + p1 * t;

		return result;
	}

	static Matrix3f VectorToMatrix(Vector3f rotation)
	{
		return Matrix3f();
	}

	static Vector3f Direction(Vector3f pos1, Vector3f pos2)
	{
		Vector3f direction = pos2 - pos1;
		direction.Normalize();

		return direction;
	}

	//Source: https://stackoverflow.com/questions/68516800/how-do-i-convert-degrees-to-radians
	static float DegToRad(float deg)
	{
		return deg * (PI / 180);
	}
};

#pragma once
#include "Vector3f.h"
#include "Actor.h"

using namespace Glyph3;
class LJMULevelDemo;

class Object : public Glyph3::Actor
{

public:
	Object(LJMULevelDemo* world);
	virtual void Update() = 0;

	Vector3f GetPosition();
	Matrix3f GetRotation();
	Vector3f GetScale();
	void SetPosition(Vector3f newPosition);
	void SetRotation(Matrix3f newRotation);
	void SetScale(Vector3f newScale);

	void ShouldBeRemovedNextFrame(bool yes) { isMarkedForDelete = yes; }
	bool IsMarkedForDelete() { return isMarkedForDelete; }

	bool paused = false;

protected:
	LJMULevelDemo* world;
	bool isMarkedForDelete = false;

protected:
};

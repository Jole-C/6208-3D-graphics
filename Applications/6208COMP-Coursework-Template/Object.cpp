#include "Object.h"
#include "LJMULevelDemo.h"

Object::Object(LJMULevelDemo* worldptr)
{
	world = worldptr;
}

Vector3f Object::GetPosition()
{
	return GetNode()->Position();
}

Matrix3f Object::GetRotation()
{
	return GetNode()->Rotation();
}

Vector3f Object::GetScale()
{
	return GetNode()->Scale();
}

void Object::SetPosition(Vector3f newPosition)
{
	GetNode()->Position() = newPosition;
}

void Object::SetRotation(Matrix3f newRotation)
{
	GetNode()->Rotation() = newRotation;
}

void Object::SetScale(Vector3f newScale)
{
	GetNode()->Scale() = newScale;
}

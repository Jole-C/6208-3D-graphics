#pragma once
#include <vector>
#include "Vector3f.h"
#include "Matrix3f.h"

using namespace Glyph3;

struct SplinePoint
{
	Vector3f position;
	Vector3f direction;
};

struct SplineSegment
{
	Vector3f controlStart;
	Vector3f startPoint;
	Vector3f endPoint;
	Vector3f controlEnd;
	double segmentLength;
};

class Spline
{
public:
	void SetResolution(int newResolution) { resolution = newResolution; }
	void CreateSpline(std::vector<Vector3f> splinePoints);
	SplinePoint FindPointAlongSpline(double timeAlongSpline);

protected:
	double FindSegmentLength(SplineSegment segment);
	Vector3f FindPointAlongSegment(Vector3f p0, Vector3f p1, Vector3f p2, Vector3f p3, float t);
	std::vector<SplineSegment> splineSegments;
	double splineLength = 0;
	int resolution = 100;
};


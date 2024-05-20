#include "Spline.h"
#include "ArrayHelpers.h"
#include "MathHelpers.h"

#include <string>

void Spline::CreateSpline(std::vector<Vector3f> splinePoints)
{
	const int arraySize = splinePoints.size();

	for(int i = 0; i < splinePoints.size(); i++)
	{
		//Calculate the tangents using catmull-rom
		//Wrap Array Index is used to get a circular array index
		Vector3f tangentStart = splinePoints[ArrayHelper::WrapArrayIndex(arraySize, i - 1)];
		Vector3f tangentEnd = splinePoints[ArrayHelper::WrapArrayIndex(arraySize, i + 2)];
		Vector3f startPoint = splinePoints[ArrayHelper::WrapArrayIndex(arraySize, i)];
		Vector3f endPoint = splinePoints[ArrayHelper::WrapArrayIndex(arraySize, i + 1)];

		//Set up the spline segment and add the values
		SplineSegment newSegment;
		newSegment.controlStart = tangentStart;
		newSegment.startPoint = startPoint;
		newSegment.endPoint = endPoint;
		newSegment.controlEnd = tangentEnd;

		//Find the segment length, add it to the total spline length and the new segment's length
		double length = FindSegmentLength(newSegment);
		newSegment.segmentLength = length;
		splineLength += length;

		splineSegments.push_back(newSegment);
	}
}

SplinePoint Spline::FindPointAlongSpline(double timeAlongSpline)
{
	//Set up variables:
	//Distance along spline - The distance we want to check based on t (0-1)
	//Initial spline length - incremented with each subsequent segment length until we reach the correct segment
	double lengthAlongSpline = timeAlongSpline * splineLength;
	double initialSplineLength = 0;
	Vector3f currentPoint = Vector3f();
	SplinePoint point;

	for(int i = 0; i < splineSegments.size(); i++)
	{
		//Get the current segment, add its length to the initial length
		SplineSegment segment = splineSegments[i];
		initialSplineLength += segment.segmentLength;

		//If this is greater than the distance along our spline as defined above, we know its the segment we want.
		if(initialSplineLength > lengthAlongSpline)
		{
			//Get the length along this segment, calculate t, and find the point along the segment
			double lengthAlongSegment = lengthAlongSpline - initialSplineLength + segment.segmentLength;
			double t = lengthAlongSegment / segment.segmentLength;
			currentPoint = FindPointAlongSegment(segment.controlStart, segment.startPoint, segment.endPoint, segment.controlEnd, t);
			point.position = currentPoint;
			point.direction = MathHelper::Direction(currentPoint, segment.endPoint);
			return point;
		}
	}

	return point;
}

double Spline::FindSegmentLength(SplineSegment segment)
{
	//Define the length of the segment and the number of points we want to check to accurately determine distance
	double segmentLength = 0;

	//The last point starts off as the first point
	Vector3f lastPoint = segment.startPoint;

	for(double i = 0; i < resolution; i++)
	{
		//Calculate t, and using that, find a point along the segment
		double t = i / resolution;
		Vector3f currentPoint = FindPointAlongSegment(segment.controlStart, segment.startPoint, segment.endPoint, segment.controlEnd, t);

		//Calculate the distance between the last and current points, then add it to the length of this segment
		double distance = MathHelper::Distance(currentPoint, lastPoint);
		segmentLength += distance;

		//Update the last point to the current point
		lastPoint = currentPoint;
	}

	return segmentLength;
}

//http://www.iquilezles.org/www/articles/minispline/minispline.htm
Vector3f Spline::FindPointAlongSegment(Vector3f p0, Vector3f p1, Vector3f p2, Vector3f p3, float t)
{
	Vector3f a, b, c, d;
	a.x = 2.0f * p1.x;
	a.y = 2.0f * p1.y;
	a.z = 2.0f * p1.z;

	b.x = p2.x - p0.x;
	b.y = p2.y - p0.y;
	b.z = p2.z - p0.z;

	c.x = 2.0f * p0.x - 5.0f * p1.x + 4.0f * p2.x - p3.x;
	c.y = 2.0f * p0.y - 5.0f * p1.y + 4.0f * p2.y - p3.y;
	c.z = 2.0f * p0.z - 5.0f * p1.z + 4.0f * p2.z - p3.z;

	d.x = -p0.x + 3.0f * p1.x - 3.0f * p2.x + p3.x;
	d.y = -p0.y + 3.0f * p1.y - 3.0f * p2.y + p3.y;
	d.z = -p0.z + 3.0f * p1.z - 3.0f * p2.z + p3.z;

	Vector3f pos;
	pos.x = 0.5f * (a.x + (b.x * t) + (c.x * t * t) + (d.x * t * t * t));
	pos.y = 0.5f * (a.y + (b.y * t) + (c.y * t * t) + (d.y * t * t * t));
	pos.z = 0.5f * (a.z + (b.z * t) + (c.z * t * t) + (d.z * t * t * t));

	return pos;
}
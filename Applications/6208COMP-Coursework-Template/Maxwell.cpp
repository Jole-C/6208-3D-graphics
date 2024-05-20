#include "Maxwell.h"
#include "LJMULevelDemo.h"
#include "MeshGenerator.h"
#include "LightController.h"

Maxwell::Maxwell(LJMULevelDemo* worldptr) : MeshObject(worldptr)
{
	MeshGenerator meshGenerator;
	meshPointer = meshGenerator.GenerateMeshFromOBJFile(L"maxwell.obj", Vector4f(1, 1, 1, 1));
	GetBody()->SetGeometry(meshPointer);

	texturePointer = Glyph3::RendererDX11::Get()->LoadTexture(L"dingus_nowhiskers.jpg");
	GenerateMaterial(texturePointer);
	GetBody()->SetMaterial(materialPointer);

	GetNode()->Position() = Glyph3::Vector3f(0, 0, 0);
	GetNode()->Scale() = Glyph3::Vector3f(1, 1, 1);
	world->m_pScene->AddActor(this);

	std::vector<Vector3f> splinePoints;
	splinePoints.push_back(Vector3f(-68.596825, -34.759968, -242.283463));
	splinePoints.push_back(Vector3f(-56.754181, -30.074921, -244.110703));
	splinePoints.push_back(Vector3f(-40.642731, -25.389874, -241.669144));
	splinePoints.push_back(Vector3f(-26.206858, -15.392694, -243.187286));
	splinePoints.push_back(Vector3f(-11.770983, -5.395513, -244.705414));
	splinePoints.push_back(Vector3f(22.285418, -11.897903, -241.398453));
	splinePoints.push_back(Vector3f(56.341824, -18.400295, -238.091522));
	splinePoints.push_back(Vector3f(96.970261, -20.793085, -235.374542));
	splinePoints.push_back(Vector3f(137.598694, -23.185875, -232.657593));
	splinePoints.push_back(Vector3f(160.585571, -2.660644, -230.525406));
	splinePoints.push_back(Vector3f(183.572464, 17.864586, -228.393250));
	splinePoints.push_back(Vector3f(209.760864, -4.831969, -195.748306));
	splinePoints.push_back(Vector3f(235.949265, -27.528524, -163.103348));
	splinePoints.push_back(Vector3f(268.686279, -32.426880, -107.803795));
	splinePoints.push_back(Vector3f(301.423309, -37.325237, -52.504230));
	splinePoints.push_back(Vector3f(290.302612, -0.971511, -39.325199));
	splinePoints.push_back(Vector3f(285.143463, 35.382214, -26.146164));
	splinePoints.push_back(Vector3f(291.561035, 51.610920, 3.551910));
	splinePoints.push_back(Vector3f(281.036530, 58.832546, 77.911499));
	splinePoints.push_back(Vector3f(270.549957, 85.408257, 103.916985));
	splinePoints.push_back(Vector3f(261.318878, 101.321838, 113.946877));
	splinePoints.push_back(Vector3f(254.333755, 125.548935, 116.512047));
	splinePoints.push_back(Vector3f(254.564224, 164.941696, 112.722069));
	splinePoints.push_back(Vector3f(282.922119, 181.598877, 69.741936));
	splinePoints.push_back(Vector3f(307.094666, 180.594040, 37.962898));
	splinePoints.push_back(Vector3f(322.875854, 158.791809, 24.850506));
	splinePoints.push_back(Vector3f(337.261902, 131.102203, 15.471833));
	splinePoints.push_back(Vector3f(343.686554, 94.577385, 24.753784));
	splinePoints.push_back(Vector3f(351.506287, 63.939899, 30.302017));
	splinePoints.push_back(Vector3f(326.104065, 18.329647, 76.205215));
	splinePoints.push_back(Vector3f(303.396851, -9.618616, 110.907318));
	splinePoints.push_back(Vector3f(257.414337, -21.982771, 217.551361));
	splinePoints.push_back(Vector3f(211.431793, -34.346924, 324.195404));
	splinePoints.push_back(Vector3f(128.107819, -2.399473, 319.776703));
	splinePoints.push_back(Vector3f(44.783894, 29.547977, 315.358032));
	splinePoints.push_back(Vector3f(-53.517681, 6.804305, 299.792297));
	splinePoints.push_back(Vector3f(-151.819260, -15.939364, 284.226593));
	splinePoints.push_back(Vector3f(-184.587082, 89.090744, 249.556351));
	splinePoints.push_back(Vector3f(-217.354874, 194.120865, 214.886108));
	splinePoints.push_back(Vector3f(-266.071411, 175.776657, 150.976868));
	splinePoints.push_back(Vector3f(-314.787903, 157.432449, 87.067627));
	splinePoints.push_back(Vector3f(-264.276733, 137.483109, 20.700783));
	splinePoints.push_back(Vector3f(-213.765564, 117.533775, -45.666058));
	splinePoints.push_back(Vector3f(-152.588470, 104.621147, 6.726608));
	splinePoints.push_back(Vector3f(-91.411354, 91.708511, 59.119274));
	splinePoints.push_back(Vector3f(-165.090897, 76.552963, 91.774643));
	splinePoints.push_back(Vector3f(-238.770416, 61.397423, 124.430008));
	splinePoints.push_back(Vector3f(-272.609772, 34.727917, 72.891594));
	splinePoints.push_back(Vector3f(-306.449097, 8.058414, 21.353195));
	splinePoints.push_back(Vector3f(-260.220581, -4.818566, -51.023819));
	splinePoints.push_back(Vector3f(-213.992065, -17.695547, -123.400833));
	splinePoints.push_back(Vector3f(-168.536224, -29.912556, -163.721939));
	splinePoints.push_back(Vector3f(-123.080391, -42.129566, -204.043045));
	splinePoints.push_back(Vector3f(-93.704208, -38.444767, -221.028839));

	for (auto& point : splinePoints)
	{
		point.y += 150;
	}

	spline.CreateSpline(splinePoints);
}

void Maxwell::Update()
{
	spline.CreateSpline(splinePoints);

	time += 0.0002;

	if (time >= 1)
	{
		time = 0;
	}

	SplinePoint point = spline.FindPointAlongSpline(time);

	Vector3f shipDirection = point.direction;
	shipDirection.Normalize();

	float yaw = atan2(shipDirection.x, shipDirection.z) + (GLYPH_PI / 2);
	float xz = shipDirection.Magnitude();
	float pitch = -atan2(shipDirection.y, xz);

	Matrix3f rotation;
	rotation.Rotation(Vector3f(pitch, yaw, 0));
	GetNode()->Rotation() = rotation;
	GetNode()->Position() = point.position;
}

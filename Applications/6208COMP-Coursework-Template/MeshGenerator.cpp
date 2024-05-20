#include "MeshGenerator.h"
#include "Log.h"
#include "MeshParser.h"

BasicMeshPtr MeshGenerator::GenerateMeshFromOBJFile(std::wstring meshName, Vector4f meshColour)
{
	//Create a mesh parser to parse the mesh data
	MeshParser meshParser(meshName);

	//Get the vertex count and make a vertex list
	int vertexCount = meshParser.positions.size();
	auto vertexList = std::make_shared<DrawExecutorDX11<BasicVertexDX11::Vertex>>();

	//Set up the vertex list parameters
	vertexList->SetLayoutElements(BasicVertexDX11::GetElementCount(), BasicVertexDX11::Elements);
	vertexList->SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	vertexList->SetMaxVertexCount(vertexCount);

	//Create a vertex once
	BasicVertexDX11::Vertex newVertex;

	//Change the vertex params and add a duplicate to the vertexlist for each vertex
	for (auto& object : meshParser.objects)
	{
		for (auto& face : object.faces)
		{
			for (size_t vertex = 0; vertex < 3; vertex++)
			{
				newVertex.position = meshParser.positions[face.PositionIndices[vertex]];
				newVertex.normal = meshParser.normals[face.NormalIndices[vertex]];
				newVertex.texcoords = meshParser.coords[face.CoordIndices[vertex]];

				float spinSpeed = (float)(rand() % 100) / 100;
				float spinDirX = (float)(rand() % 100) / 100;
				float spinDirY = (float)(rand() % 100) / 100;
				float spinDirZ = (float)(rand() % 100) / 100;
				newVertex.color = Vector4f(spinSpeed, spinDirX, spinDirY, spinDirZ);
				
				vertexList->AddVertex(newVertex);
			}
		}
	}

	//Return the vertex list
	return vertexList;
}

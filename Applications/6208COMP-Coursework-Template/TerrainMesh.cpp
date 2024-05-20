#include "TerrainMesh.h"
#include "RasterizerStateConfigDX11.h"
#include <memory>
#include "LJMULevelDemo.h"
#include "LightController.h"
#include "ArrayHelpers.h"
#include "MathHelpers.h"
#include "MaterialGenerator.h"
#include "Camera.h"


TerrainMesh::TerrainMesh(LJMULevelDemo* worldptr) : MeshObject(worldptr)
{
	newDimensions = Vector2f(dimensions.x + 1, dimensions.y + 1);

	grassTexture = Glyph3::RendererDX11::Get()->LoadTexture(L"DriedGrass.jpg");
	rockTexture = Glyph3::RendererDX11::Get()->LoadTexture(L"MountainRock.jpg");
	snowTexture = Glyph3::RendererDX11::Get()->LoadTexture(L"SnowScuffedGround.jpg");

	MaterialGenerator materialGenerator;
	materialPointer = materialGenerator.CreateLitMultiTexturedMaterial(snowTexture, grassTexture, rockTexture);
}

void TerrainMesh::SetRenderingOrigin(Vector2f newOrigin)
{
	//We add on the number of terrain chunks because the terrain origin places the middle of the terrain at 0,0, so we need to offset the
	//origin to account for this
	Vector2f normalisedOrigin;
	normalisedOrigin.x = floor((numberOfTerrainChunks.x / 2) + (newOrigin.x / (dimensions.x * terrainVertexSeparation)));
	normalisedOrigin.y = floor((numberOfTerrainChunks.y / 2) + (newOrigin.y / (dimensions.y * terrainVertexSeparation)));

	renderingOrigin = normalisedOrigin;
}

void TerrainMesh::GenerateTerrain()
{
}

CustomMeshPtr TerrainMesh::GenerateGrid(Vector2f origin, std::vector<std::vector<float>> heightSegment)
{
	std::vector<Vector3f> points;
	std::vector<Vector4f> colours;
	std::vector<Vector2f> uvs;
	std::vector<Vector2f> texWeights;
	Vector3f gridOrigin = Vector3f(terrainOrigin.x + origin.x, 0, terrainOrigin.y + origin.y);

	//Generate the initial vertices
	for (int x = 0; x < newDimensions.x; x++)
	{
		for (int y = 0; y < newDimensions.y; y++)
		{
			float heightValue = heightSegment[x][y];
			points.push_back(Vector3f(origin.x + (x * terrainVertexSeparation), heightValue, origin.y + (y * terrainVertexSeparation)));
			uvs.push_back(Vector2f((float)x / newDimensions.x, (float)y / newDimensions.y));
			colours.push_back(Vector4f(1, 1, 1, 1));
			texWeights.push_back(Vector2f(heightValue/50, 0));
		}
	}

	//Generate the indices
	std::vector<int> indices;

	for (int x = 0; x < newDimensions.x - 1; x++)
	{
		for (int y = 0; y < newDimensions.y - 1; y++)
		{
			int vertUpperLeft = x * newDimensions.y + y;
			int vertUpperRight = (x + 1) * newDimensions.y + y;
			int vertLowerLeft = (x + 1) * newDimensions.y + (y + 1);
			int vertLowerRight = x * newDimensions.y + (y + 1);

			indices.push_back(vertLowerRight);
			indices.push_back(vertUpperRight);
			indices.push_back(vertUpperLeft);

			indices.push_back(vertLowerRight);
			indices.push_back(vertLowerLeft);
			indices.push_back(vertUpperRight);
		}
	}

	CustomVertexDX11::Vertex vertex;
	auto terrainMesh = std::make_shared<DrawExecutorDX11<CustomVertexDX11::Vertex>>();
	terrainMesh->SetLayoutElements(CustomVertexDX11::GetElementCount(), CustomVertexDX11::Elements);
	terrainMesh->SetPrimitiveType(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	terrainMesh->SetMaxVertexCount(indices.size());

	//Add the vertices to the mesh, calculating the normal at the same time
	for (int i = 0; i < indices.size() / 3; i++)
	{
		//Calculate the normal
		Vector3f point1 = points[indices[3 * i + 0]];
		Vector3f point2 = points[indices[3 * i + 1]];
		Vector3f point3 = points[indices[3 * i + 2]];

		Vector3f ab = point2 - point1;
		Vector3f ac = point3 - point1;

		Vector3f normal = ab.Cross(ac);
		normal.Normalize();

		//Set up the vertex and add it to the mesh
		for (int j = 0; j < 3; j++)
		{
			vertex.position = points[indices[3 * i + j]];
			vertex.color = colours[indices[3 * i + j]];
			vertex.texcoords = uvs[indices[3 * i + j]];
			vertex.normal = normal;

			float dotSlope = normal.Dot(Vector3f(0, 1, 0));
			Vector2f tw = texWeights[indices[3 * i + j]];
			tw.y = dotSlope;

			vertex.texweights = tw;

			terrainMesh->AddVertex(vertex);
		}
	}

	return terrainMesh;
}

bool TerrainMesh::IsChunkLoaded(Vector2f index)
{
	for (auto& chunk : loadedChunks)
	{
		if (chunk.index == index)
		{
			return true;
		}
	}

	return false;
}

bool TerrainMesh::IsChunkInRange(Vector2f index)
{
	auto iterator = std::find_if(loadedChunks.begin(), loadedChunks.end(), [&index](const TerrainChunk& obj) {return obj.index == index; });

	for (auto& chunk : loadedChunks)
	{
		Vector2f chunkIndex = chunk.index;
		if (chunkIndex == index)
		{
			if (chunkIndex.x < renderingOrigin.x - chunkRenderingRadius.x || chunkIndex.x > renderingOrigin.x + chunkRenderingRadius.x ||
				chunkIndex.y < renderingOrigin.y - chunkRenderingRadius.y || chunkIndex.y > renderingOrigin.y + chunkRenderingRadius.y)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}

	return true;
}

void TerrainMesh::GenerateChunks(Vector2f index)
{
	std::vector<std::vector<float>> chunkNoise;
	Vector2f chunkOrigin;

	//figure out the chunks to load
	std::vector<Vector2f> chunksToGenerate;

	for (int x = renderingOrigin.x - chunkRenderingRadius.x; x < renderingOrigin.x + chunkRenderingRadius.x; x++)
	{
		for (int y = renderingOrigin.y - chunkRenderingRadius.y; y < renderingOrigin.y + chunkRenderingRadius.y; y++)
		{
			//Clamp the coordinates to be within the bounds of the number of terrain chunks
			int clampedX = MathHelper::Clamp(x, 0, numberOfTerrainChunks.x - 1);
			int clampedY = MathHelper::Clamp(y, 0, numberOfTerrainChunks.y - 1);

			if (IsChunkLoaded(Vector2f(clampedX, clampedY)) == false)
			{
				Vector3f chunk = Vector3f(clampedX * (terrainVertexSeparation * dimensions.x), 0, clampedY * (terrainVertexSeparation * dimensions.y)) + Vector3f((terrainVertexSeparation * dimensions.x) / 2, 0, (terrainVertexSeparation * dimensions.y) / 2);
				Vector3f directionToChunk = chunk - world->m_pCamera->GetNode()->Position();
				Vector3f cameraDirection = world->m_pCamera->GetNode()->Rotation() * Vector3f(0, 0, 1);

				cameraDirection.Normalize();
				directionToChunk.Normalize();

				float dotProduct = cameraDirection.Dot(directionToChunk);

				if (dotProduct > 0)
				{
					chunksToGenerate.push_back(Vector2f(clampedX, clampedY));
				}
			}
		}
	}

	//for each chunk to load
	for (int i = 0; i < chunksToGenerate.size(); i++)
	{
		Vector2f coords = chunksToGenerate[i];

		//Clamp the coordinates to be within the bounds of the number of terrain chunks
		if (IsChunkLoaded(coords) == false)
		{
			//Generate noise, origin, mesh and material
			chunkOrigin = (coords * dimensions * terrainVertexSeparation);
			chunkNoise = FindHeightData(dimensions * coords);
			CustomMeshPtr newSegment = GenerateGrid(chunkOrigin, chunkNoise);

			//Create a new entity, attach it and give it the newly generated segment
			auto* newEntity = new Glyph3::Entity3D();
			newEntity->SetGeometry(newSegment);
			newEntity->SetMaterial(materialPointer);
			GetNode()->AttachChild(newEntity);
			AddElement(newEntity);

			//Create a new chunk to hold this data
			TerrainChunk segment;
			segment.index = coords;
			segment.mesh = newEntity;

			//Add the chunk to the list of loaded chunks
			loadedChunks.push_back(segment);
		}
	}

	auto iterator = loadedChunks.begin();

	while (iterator != loadedChunks.end())
	{
		if (IsChunkInRange((*iterator).index) == false)
		{
			Glyph3::Entity3D* mesh = (*iterator).mesh;
			GetNode()->DetachChild(mesh);
			RemoveElement(mesh);
			delete(mesh);

			iterator = loadedChunks.erase(iterator);
		}
		else
		{
			iterator++;
		}
	}
}

std::vector<std::vector<float>> TerrainMesh::FindHeightData(Vector2f topLeftCoords)
{
	std::vector<std::vector<float>> noise;

	//Populate the array
	for (int x = 0; x < newDimensions.x; x++)
	{
		for (int y = 0; y < newDimensions.y; y++)
		{
			std::vector<float> newColumn;
			newColumn.resize(newDimensions.y);
			noise.push_back(newColumn);
		}
	}

	//For each element of the array, set it to the corresponding heightdata array element
	for (int x = topLeftCoords.x; x < topLeftCoords.x + newDimensions.x; x++)
	{
		for (int y = topLeftCoords.y; y < topLeftCoords.y + newDimensions.y; y++)
		{
			int normalisedX = x - topLeftCoords.x;
			int normalisedY = y - topLeftCoords.y;

			noise[normalisedX][normalisedY] = heightdata[x][y];
		}
	}

	return noise;
}

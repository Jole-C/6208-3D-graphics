#pragma once
#include <DrawExecutorDX11.h>
#include "BasicVertexDX11.h"
using namespace Glyph3;
typedef std::shared_ptr<Glyph3::DrawExecutorDX11<Glyph3::BasicVertexDX11::Vertex>> BasicMeshPtr;


class MeshGenerator
{
public:
	BasicMeshPtr GenerateMeshFromOBJFile(std::wstring meshName, Vector4f meshColour);

protected:
};


#pragma once
#include "Object.h"
#include "MaterialDX11.h"
#include "GeometryActor.h"
#include <DrawExecutorDX11.h>
#include "BasicVertexDX11.h"
#include "MaterialReflectanceInfo.h"
#include "CustomVertexDX11.h"

using namespace Glyph3;
typedef std::shared_ptr<Glyph3::DrawExecutorDX11<Glyph3::BasicVertexDX11::Vertex>> BasicMeshPtr;
typedef std::shared_ptr<Glyph3::DrawExecutorDX11<CustomVertexDX11::Vertex>> CustomMeshPtr;

class MeshObject : public Object
{
public:
	MeshObject(LJMULevelDemo* world);
	virtual void Update() = 0;

protected:
	BasicMeshPtr meshPointer;
	ResourcePtr texturePointer;
	MaterialPtr materialPointer;
	MaterialReflectanceInfo reflectanceInfo;
	
	MaterialPtr GenerateMaterial(Glyph3::ResourcePtr texture);

}; 
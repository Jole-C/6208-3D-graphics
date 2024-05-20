#pragma once
#include "MeshObject.h"
#include <DrawExecutorDX11.h>
#include "BasicVertexDX11.h"

class XAxisObject : public MeshObject
{
public:
	XAxisObject(LJMULevelDemo* worldptr);

protected:
	BasicMeshPtr CreateXAxisMesh();
	Glyph3::MaterialPtr CreateLineMaterial();
};


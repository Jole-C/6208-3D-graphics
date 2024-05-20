#pragma once 

#include <vector>
#include <array>
#include <string>
#include <sstream>
#include <fstream>
#include <assert.h>
#include "Vector2f.h"
#include "Vector3f.h"

using namespace Glyph3;

class MeshParser
{
public:
	//------------CONSTRUCTORS-----------------------------------------------------
	MeshParser(const std::wstring& pfilename);


protected:
	//-------------HELPER METHODS--------------------------------------------------
	void				parseMesh();

	Vector3f	toVec3(const std::vector<std::string>& ptokens);
	Vector2f	toVec2(const std::vector<std::string>& ptokens);

	std::array<int, 3>	toIndexTriple(const std::string pstr);
	int					wrapOffset(int pindex, int psize);

public:

	//Structure to Represent Face Mappings
	typedef struct
	{
		std::array<int, 3> PositionIndices;
		std::array<int, 3> NormalIndices;
		std::array<int, 3> CoordIndices;
	} face_t;

	//Structure to Represent Object Mappings
	typedef struct
	{
		std::string name;
		std::string material_name;
		std::vector<face_t> faces;
	} object_t;

	//--------CLASS MEMBERS-------------------------------------------

	//Vertex Data Lists
	std::vector<Vector3f> positions;
	std::vector<Vector3f> normals;
	std::vector<Vector2f> coords;

	//Lists of Objects and Material Details
	std::vector<object_t> objects;
	std::vector<std::string> materials;

	//Path to the OBJ File. 
	std::wstring			 filename;
};
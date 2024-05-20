#include "MeshParser.h"

MeshParser::MeshParser(const std::wstring& pfilename)
{
	this->filename = pfilename;
	this->parseMesh();
}

void	MeshParser::parseMesh()
{
	std::ifstream tfileobj(this->filename, std::ios::in);
	if (!tfileobj.is_open())
		return;

	std::string tline;
	size_t tcg = 0;
	this->objects.push_back(object_t());

	while (std::getline(tfileobj, tline))
	{
		std::stringstream			ss(tline);
		std::string					ttoken;
		std::vector<std::string>	ttokens;

		while (ss >> ttoken)
		{
			ttokens.push_back(ttoken);
		}

		if (ttokens.size() != 0)
		{
			if (ttokens[0] == "v")
				this->positions.emplace_back(toVec3(ttokens));
			else if (ttokens[0] == "vn")
				this->normals.emplace_back(toVec3(ttokens));
			else if (ttokens[0] == "vt")
				this->coords.emplace_back(toVec2(ttokens));
			else if (ttokens[0] == "f")
			{
				face_t tf;
				for (size_t i = 0; i < 3; ++i)
				{
					auto ttriple = this->toIndexTriple(ttokens[i + 1]);
					tf.PositionIndices[i] = this->wrapOffset(ttriple[0], this->positions.size());
					tf.CoordIndices[i] = this->wrapOffset(ttriple[1], this->coords.size());
					tf.NormalIndices[i] = this->wrapOffset(ttriple[2], this->normals.size());
				}
				this->objects.back().faces.emplace_back(tf);
			}
			else if (ttokens[0] == "o")
			{
				if (this->objects.back().faces.size() == 0)
				{
					this->objects.back().name = ttokens[1];
				}
				else
				{
					this->objects.push_back(object_t());
					this->objects.back().name = ttokens[1];
				}
			}
			else if (ttokens[0] == "mtllib")
			{
				this->materials.push_back(ttokens[1]);
			}
		}
	}
}

Vector3f MeshParser::toVec3(const std::vector<std::string>& ptokens)
{
	assert(ptokens.size() >= 4);

	return Vector3f(std::stof(ptokens[1]), std::stof(ptokens[2]), std::stof(ptokens[3]));
}

Vector2f MeshParser::toVec2(const std::vector<std::string>& ptokens)
{
	assert(ptokens.size() >= 3);
	return Vector2f(std::stof(ptokens[1]), 1.0f - std::stof(ptokens[2]));
}

std::array<int, 3>	MeshParser::toIndexTriple(const std::string pstr)
{
	// Split the string according to '/' into tokens
	std::vector<std::string> telems;
	std::stringstream ss(pstr);
	std::string titem;

	while (std::getline(ss, titem, '/'))
	{
		telems.push_back(titem);
	}

	// We need to have at least one index to do anything.
	assert(telems.size() >= 1 && telems.size() <= 3);

	// initialize to 0, then fill in indices with the available data.
	std::array<int, 3> ttriple = { 0,0,0 };

	for (size_t i = 0; i < telems.size(); ++i)
	{
		if (telems[i].size() > 0)
		{
			ttriple[i] = std::stoi(telems[i]) - 1;
		}
	}
	return ttriple;
}

int	MeshParser::wrapOffset(int pindex, int psize)
{
	if (pindex < 0)
		pindex = pindex + psize + 1;
	return pindex;
}

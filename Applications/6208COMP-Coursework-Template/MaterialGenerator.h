#pragma once
#include <MaterialDX11.h>
#include "ResourceDX11.h"

using namespace Glyph3;

class MaterialGenerator
{
public:
	Glyph3::MaterialPtr CreateLitTexturedMaterial(ResourcePtr texture);
	Glyph3::MaterialPtr CreateLitMultiTexturedMaterial(ResourcePtr texture, ResourcePtr texture1, ResourcePtr texture2);
	Glyph3::MaterialPtr CreateTransparentLitTexturedMaterial(ResourcePtr texture);
	Glyph3::MaterialPtr CreateBumpLitTexturedMaterial(ResourcePtr texture, ResourcePtr bumpTexture, ResourcePtr lighttexture);
	Glyph3::MaterialPtr CreateSolidUnlitMaterial();
	Glyph3::MaterialPtr CreateLineMaterial();
	Glyph3::MaterialPtr CreateAnimatedMaterial(ResourcePtr texture1, ResourcePtr texture2);
	Glyph3::MaterialPtr CreateExplosionMaterial(ResourcePtr texture);
	Glyph3::MaterialPtr CreateSolarSystemShader(std::vector<Vector4f> instancePositions, ResourcePtr texture1, ResourcePtr texture2, ResourcePtr texture3, ResourcePtr texture4);

};


#pragma once

struct Texture
{
	uint id;

	uint height;
	uint width;
};

namespace TextureLoader
{
	void Init();

	std::string CreateFileName(const char* assetsPath);
	Texture Load(const char* path);
	Texture CheckerTexture();
}

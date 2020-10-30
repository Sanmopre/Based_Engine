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

	Texture Load(const char* path);
}
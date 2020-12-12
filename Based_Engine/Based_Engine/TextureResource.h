#ifndef __TEXTURERESOURCE_H__
#define __TEXTURERESOURCE_H__

#include "Resource.h"

#include "TextureLoader.h"

class TextureResource : public Resource
{
public:

	TextureResource(uint uid, const char* assetsFile, const char* libraryFile);
	~TextureResource();

	const Texture GetTexture() const;
	bool LoadInMemory() override;
	bool Unload() override;

private:

	Texture texture;
};

#endif //__TEXTURERESOURCE_H__


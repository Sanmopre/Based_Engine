#ifndef __TEXTURERESOURCE_H__
#define __TEXTURERESOURCE_H__

#include "Resource.h"

#include "TextureLoader.h"

class TextureResource : public Resource
{
public:

	TextureResource(uint uid);
	~TextureResource();

	const Texture GetTexture() const;
	bool LoadInMemory() override;

private:

	Texture texture;
};

#endif //__TEXTURERESOURCE_H__


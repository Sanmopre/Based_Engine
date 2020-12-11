#include "TextureResource.h"

#include "FileSystem.h"

TextureResource::TextureResource(uint uid) : Resource(uid, FileType::IMAGE)
{
}

TextureResource::~TextureResource()
{
}

const Texture TextureResource::GetTexture() const
{
    return texture;
}

bool TextureResource::LoadInMemory()
{
    texture = TextureLoader::Load(assetsFile.c_str());
    if (texture.id != NULL)
        return true;
    return false;
}

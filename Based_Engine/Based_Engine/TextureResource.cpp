#include "TextureResource.h"

TextureResource::TextureResource(uint uid) : Resource(uid, Resource::Type::TEXTURE)
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

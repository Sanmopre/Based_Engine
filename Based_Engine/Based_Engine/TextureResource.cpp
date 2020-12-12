#include "TextureResource.h"

#include "FileSystem.h"

TextureResource::TextureResource(uint uid, const char* assetsFile, const char* libraryFile)
    : Resource(uid, FileType::IMAGE, assetsFile, libraryFile)
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

bool TextureResource::Unload()
{
    texture = { NULL, NULL, NULL };

    return true;
}

//void Input::ProccesImage(std::string file)
//{
//	GameObject* object = App->objects->selected;
//	if (!object)
//	{
//		object = App->objects->AddObject(nullptr, App->objects->selected, true, "Plane");
//		object->AddMeshComponent("Library/Meshes/plane.monki", file.c_str());
//	}
//	else
//	{
//		bool found = false;
//		for (uint c = 0; c < object->components.size(); c++)
//		{
//			if (object->components[c]->AddTexture(file.c_str()))
//			{
//				found = true;
//				break;
//			}
//		}
//		if (!found)
//			object->AddMeshComponent("Library/Meshes/plane.monki", file.c_str());
//	}
//}

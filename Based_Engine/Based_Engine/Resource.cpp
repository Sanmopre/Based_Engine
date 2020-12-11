#include "Resource.h"

#include "FileSystem.h"

Resource::Resource(uint uid, FileType type) : uid(uid), type(type)
{
}

Resource::~Resource()
{
}

FileType Resource::GetType() const
{
	return type;
}

uint Resource::GetUID() const
{
	return uid;
}

const char* Resource::GetAssetFile() const
{
	return assetsFile.c_str();
}

const char* Resource::GetLibraryFile() const
{
	return libraryFile.c_str();
}

uint Resource::GetReferenceCount() const
{
	return referenceCount;
}

bool Resource::IsLoadedToMemory() const
{
	if (referenceCount != 0)
		return true;
	return false;
}

bool Resource::LoadToMemory()
{
	if (referenceCount == 0)
		return true;
	return false;
}

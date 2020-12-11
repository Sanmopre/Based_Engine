#include "Resource.h"

Resource::Resource(uint uid, Resource::Type type) : uid(uid), type(type)
{
}

Resource::~Resource()
{
}

Resource::Type Resource::GetType() const
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

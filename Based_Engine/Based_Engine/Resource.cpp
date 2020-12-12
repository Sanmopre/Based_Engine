#include "Resource.h"

#include "FileSystem.h"

Resource::Resource(uint uid, FileType type, const char* assetsFile, const char* libraryFile)
	: uid(uid), type(type), assetsFile(assetsFile), libraryFile(libraryFile)
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

void Resource::AddReference()
{
	referenceCount++;
}

void Resource::SubstractReference()
{
	referenceCount--;
}

//uint Resource::GetReferenceCount() const
//{
//	return referenceCount;
//}

bool Resource::IsLoadedToMemory() const
{
	if (referenceCount != 0)
		return true;
	return false;
}


bool Resource::IsReferenced()
{
	if (referenceCount == 0)
		return false;
	return true;
}

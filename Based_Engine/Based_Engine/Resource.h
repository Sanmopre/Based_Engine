#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "Globals.h"
#include <config.h>

class Resource
{
public:

	enum Type 
	{
		TEXTURE,
		MESH,
		SCENE,
		UNKNOWN
	};

	Resource(uint uid, Resource::Type type);
	virtual ~Resource();

	Resource::Type GetType() const;
	uint GetUID() const;
	const char* GetAssetFile() const;
	const char* GetLibraryFile() const;
	uint GetReferenceCount() const;

	bool IsLoadedToMemory() const;
	bool LoadToMemory();

	//virtual void Save(Config& config) const;
	//virtual void Load(const Config& config);
	virtual bool LoadInMemory() = 0;

protected:

	uint uid = 0;
	std::string assetsFile;
	std::string libraryFile;
	Type type = UNKNOWN;
	uint referenceCount = 0;
};

#endif
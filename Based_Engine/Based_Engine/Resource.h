#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "Globals.h"
#include <config.h>

enum class FileType;

class Resource
{
public:

	Resource(uint uid, FileType type);
	virtual ~Resource();

	FileType GetType() const;
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
	FileType type;
	uint referenceCount = 0;
};

#endif
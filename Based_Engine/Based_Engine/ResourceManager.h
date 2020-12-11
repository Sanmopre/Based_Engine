#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include "Module.h"
#include <map>

class Resource;
enum class FileType;

class Entry
{
public:

	Entry(const char* name, Entry* parent);
	virtual ~Entry();

	std::string name;
	Entry* parent = nullptr;
};

class Folder : public Entry
{
public:

	Folder(const char* name, Entry* parent);
	virtual ~Folder();

	std::string GetDirectory();

	std::vector<Entry*> entries;
};
class Archive : public Entry
{
public:

	Archive(const char* name, FileType type, Entry* parent);
	virtual ~Archive();

	FileType type;
};

class ResourceManager : public Module
{
public:

	ResourceManager(Application* app, bool active = true);
	~ResourceManager();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();


	uint Find(const char* file_in_assets) const;
	uint ImportFile(const char* new_file_in_assets);

	const Resource* RequestResource(uint uid) const;
	Resource* RequestResource(uint uid);
	void ReleaseResource(uint uid);

private:

	uint GenerateUID();
	Resource* CreateNewResource(const char* assetsFile, FileType type);
	void UpdateEntriesTree();

	std::map<uint, Resource*> resources;
	std::string currentFolder;

	Folder* assets = nullptr;

	friend class Assets;
};

#endif //__RESOURCES_H__ 
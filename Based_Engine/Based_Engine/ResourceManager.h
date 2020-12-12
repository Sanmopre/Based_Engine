#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include "Module.h"
#include <map>

class Resource;
enum class FileType;

class Entry
{
public:

	enum Type
	{
		ARCHIVE,
		FOLDER
	};

	Entry(const char* name, Entry* parent, Type type);
	virtual ~Entry();

	std::string GetDirectory();

	std::string name;
	Entry* parent = nullptr;

	Type type;
};

class Folder : public Entry
{
public:

	Folder(const char* name, Entry* parent);
	virtual ~Folder();
	 
	bool HasFolders();

	std::vector<Entry*> entries;
};
class Archive : public Entry
{
public:

	Archive(const char* name, FileType type, Entry* parent);
	virtual ~Archive();

	FileType fileType;
};

class ResourceManager : public Module
{
public:

	ResourceManager(Application* app, bool active = true);
	~ResourceManager();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();


	uint Find(const char* assetsFile) const;
	uint ImportFile(const char* newAssetsFile, bool newFile, bool redo = false);

	const Resource* RequestResource(uint uid) const;
	Resource* RequestResource(uint uid);
	void ReleaseResource(uint uid);

	const std::string GetCurrentFolder();

private:

	uint GenerateUID();
	Resource* CreateNewResource(const char* assetsFile, const char* libraryFile, FileType type);
	void UpdateEntriesTree(bool start = false);

	void IterateFolder(Folder* folder, bool start);

	std::map<uint, Resource*> resources;
	std::string currentFolder;

	Folder* assets = nullptr;

	friend class Assets;
};

#endif //__RESOURCES_H__ 
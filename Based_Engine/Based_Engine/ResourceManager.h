#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include "Module.h"

enum class FileType;

enum class ArchiveType
{
	FOLDER,
	ARCHIVE
};

class Archive
{
public:

	Archive(char* name, ArchiveType type, Archive* parent);
	Archive(const char* name, ArchiveType type, Archive* parent);
	virtual ~Archive();

	std::string name;
	ArchiveType type;

	Archive* parent;
};

class Folder : public Archive
{
public:

	Folder(char* name, Archive* parent);
	Folder(const char* name, Archive* parent);
	virtual ~Folder();

	std::vector<Archive*> archives;
};

class Resource : public Archive
{
public:

	Resource(char* name, FileType type, Archive* parent);
	Resource(const char* name, FileType type, Archive* parent);
	virtual ~Resource();

	FileType fileType;
};

class ResourceManager : public Module
{
public:

	ResourceManager(Application* app, bool active = true);
	~ResourceManager();

	bool Start();
	update_status Update();
	bool CleanUp();

	Folder* assets;

private:

	void SaveFolderFiles(Folder* folder);
	void LoadFolderFiles(Folder* folder);
};

#endif //__RESOURCES_H__ 

#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include "Module.h"
#include <map>

class Resource
{
public:

	enum class Type;
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
	Resource* CreateNewResource(const char* assetsFile, Resource::Type type);

	std::map<uint, Resource*> resources;
	std::string currentFolder;
};

#endif //__RESOURCES_H__ 
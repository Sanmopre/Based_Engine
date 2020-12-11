#include "ResourceManager.h"

#include "FileSystem.h"

#include "Algorithm/Random/LCG.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "misc/cpp/imgui_stdlib.h" 

ResourceManager::ResourceManager(Application* app, bool active) : Module(app, active)
{
}

ResourceManager::~ResourceManager()
{
}

bool ResourceManager::Start()
{
	currentFolder = "";
	return true;
}

update_status ResourceManager::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ResourceManager::CleanUp()
{
	return true;
}

uint ResourceManager::Find(const char* file) const
{
	for (uint i = 0; i < resources.size(); i++)
	{
		
	}

	return NULL;
}

uint ResourceManager::ImportFile(const char* newFile)
{
	return uint();
}

const Resource* ResourceManager::RequestResource(uint uid) const
{
	return resources.find(uid)->second;
}

Resource* ResourceManager::RequestResource(uint uid)
{
	return resources.find(uid)->second;
}

void ResourceManager::ReleaseResource(uint uid)
{
	std::map<uint, Resource*>::iterator entry = resources.find(uid);
	delete entry->second;

	resources.erase(entry);
}

uint ResourceManager::GenerateUID()
{
	return LCG().Int();
}

Resource* ResourceManager::CreateNewResource(const char* file, Resource::Type type)
{
	return nullptr;
}

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
	std::vector<std::string> files = FileSystem::GetFiles(currentFolder.c_str());

	std::string name = "Assets";
	if (currentFolder != "")
		name += "/" + currentFolder;

	if (ImGui::Begin(name.c_str(), nullptr))
	{
		if(ImGui::Button("Back"))
		{
			if (currentFolder != "")
			{
				bool found = false;
				for (std::string::iterator itr = currentFolder.end() - 1; itr != currentFolder.begin(); itr--)
					if (*itr == '/')
					{
						found = true;
						currentFolder.erase(itr, currentFolder.end());
						break;
					}
				if(!found)
					currentFolder.erase(currentFolder.begin(), currentFolder.end());
			}
		}
		for (uint i = 0; i < files.size(); i++)
		{
			if (ImGui::Button(files[i].c_str()))
			{
				if (FileSystem::IsAFolder(files[i]))
				{
					if (currentFolder != "")
						currentFolder += "/";
					currentFolder += files[i];
				}
			}
		}
	}
	ImGui::End();
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

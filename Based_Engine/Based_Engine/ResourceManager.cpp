#include "ResourceManager.h"

#include "FileSystem.h"

#include "Algorithm/Random/LCG.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "misc/cpp/imgui_stdlib.h" 

Entry::Entry(const char* name, Entry* parent) : name(name), parent(parent) {}

Entry::~Entry() {}

Folder::Folder(const char* name, Entry* parent) : Entry(name, parent) {}

Folder::~Folder()
{
	while (entries.size() != 0)
	{
		delete* entries.begin();
		entries.erase(entries.begin());
	}
}

std::string Folder::GetDirectory()
{
	if (parent)
	{
		std::vector<std::string> names;

		names.push_back(name);
		Entry* p = parent;
		while (p != nullptr)
		{
			if(p->parent)
				names.push_back(p->name);
			p = p->parent;
		}

		std::reverse(names.begin(), names.end());

		std::string directory;
		for (uint i = 0; i < names.size(); i++)
		{
			if (i != 0)
				directory += "/";
			directory = names[i];
		}

		return directory;
	}
	return "";
}

Archive::Archive(const char* name, FileType type, Entry* parent) : Entry(name, parent), type(type) {}

Archive::~Archive() {}

ResourceManager::ResourceManager(Application* app, bool active) : Module(app, active) {}

ResourceManager::~ResourceManager() {}

bool ResourceManager::Start()
{
	currentFolder = "";

	UpdateEntriesTree();

	return true;
}

update_status ResourceManager::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ResourceManager::CleanUp()
{
	if (assets)
		delete assets;

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
	return 0;
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

Resource* ResourceManager::CreateNewResource(const char* file, FileType type)
{
	return nullptr;
}

void IterateFolder(Folder* folder)
{
	std::string dir = folder->GetDirectory();
	std::vector<std::string> files = FileSystem::GetFiles(dir.c_str());

	for (uint i = 0; i < files.size(); i++)
	{
		if (FileSystem::IsAFolder(files[i]))
		{
			Folder* newFolder = new Folder(files[i].c_str(), folder);
			folder->entries.push_back(newFolder);

			IterateFolder(newFolder);
		}
		else
		{
			Archive* newArchive = new Archive(files[i].c_str(), FileSystem::GetFileType(files[i]), folder);
			folder->entries.push_back(newArchive);
		}
	}
}

void ResourceManager::UpdateEntriesTree()
{
	if (assets)
		delete assets;

	assets = new Folder(FileSystem::GetMainDirectory(), nullptr);

	IterateFolder(assets);
}

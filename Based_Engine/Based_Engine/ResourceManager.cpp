#include "ResourceManager.h"

#include "Resource.h"
#include "MeshResource.h"
#include "TextureResource.h"

#include "FileSystem.h"
#include "Simp.h"

#include "Algorithm/Random/LCG.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "misc/cpp/imgui_stdlib.h" 

Entry::Entry(const char* name, Entry* parent, Entry::Type type) : name(name), parent(parent), type(type) {}

Entry::~Entry() {}

std::string Entry::GetDirectory()
{
	if (parent)
	{
		std::vector<std::string> names;

		names.push_back(name);
		Entry* p = parent;
		while (p != nullptr)
		{
			if (p->parent)
				names.push_back(p->name);
			p = p->parent;
		}

		std::reverse(names.begin(), names.end());

		std::string directory;
		for (uint i = 0; i < names.size(); i++)
		{
			if (i != 0)
				directory += "/";
			directory += names[i];
		}

		return directory;
	}
	return "";
}

Folder::Folder(const char* name, Entry* parent) : Entry(name, parent, Entry::Type::FOLDER) {}

Folder::~Folder()
{
	while (entries.size() != 0)
	{
		delete* entries.begin();
		entries.erase(entries.begin());
	}
}

bool Folder::HasFolders()
{
	for (uint i = 0; i < entries.size(); i++)
		if (entries[i]->type == Entry::FOLDER)
			return true;

	return false;
}

Archive::Archive(const char* name, FileType type, Entry* parent) : Entry(name, parent, Entry::Type::ARCHIVE), fileType(type) {}

Archive::~Archive() {}

ResourceManager::ResourceManager(Application* app, bool active) : Module(app, active) {}

ResourceManager::~ResourceManager() {}

bool ResourceManager::Start()
{
	currentFolder = "";

	UpdateEntriesTree(true);

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

uint ResourceManager::Find(const char* assetsFile) const
{
	for (std::map<uint, Resource*>::const_iterator itr = resources.begin(); itr != resources.end(); itr++)
	{
		Resource* resource = itr->second;

		if (resource->GetAssetFile() == assetsFile)
		{
			if (!resource->IsReferenced())
				resource->LoadInMemory();

			resource->AddReference();
			
			return resource->GetUID();
		}
	}

	return NULL;
}

uint ResourceManager::ImportFile(const char* newAssetsFile, bool newFile, bool redo)
{
	std::string file = newAssetsFile;
	LOG("Loading file: %s", file.c_str());

	FileType type = FileSystem::GetFileType(file);

	if(newFile)
		file = FileSystem::CopyFileToAssets(currentFolder.c_str(), file.c_str(), type);

	std::string libraryPath;
	switch (type)
	{
	case FileType::MESH:
		libraryPath = Simp::LoadMesh(file.c_str(), redo);
		break;
	case FileType::IMAGE:
		libraryPath = "";
		break;
	}

	uint uid = CreateNewResource(file.c_str(), libraryPath.c_str(), type)->GetUID();

	return uid;
}

const Resource* ResourceManager::RequestResource(uint uid) const
{
	return resources.find(uid)->second;
}

Resource* ResourceManager::RequestResource(uint uid)
{
	return resources[uid];
}

void ResourceManager::ReleaseResource(uint uid)
{
	Resource* resource = resources[uid];

	if (!resource)
		return;

	if (!resource->IsReferenced())
		return;

	resource->SubstractReference();
	if (!resource->IsReferenced())
		resource->Unload();
}

const std::string ResourceManager::GetCurrentFolder()
{
	return currentFolder;
}

uint ResourceManager::GenerateUID()
{
	return LCG().Int();
}

Resource* ResourceManager::CreateNewResource(const char* file, const char* libraryFile, FileType type)
{
	Resource* output = nullptr;

	uint uid = 0;
	bool notRepeated = false;
	while (!notRepeated)
	{
		uid = GenerateUID();

		if (resources[uid] == nullptr)
			notRepeated = true;
	}

	switch (type)
	{
	case FileType::MESH:
		output = new MeshResource(uid, file, libraryFile);
		break;
	case FileType::IMAGE:
		output = new TextureResource(uid, file, libraryFile);
		break;
	case FileType::UNKNOWN:
		break;
	}

	resources.insert(std::make_pair(uid, output));

	return output;
}

void ResourceManager::IterateFolder(Folder* folder, bool start)
{
	std::string dir = folder->GetDirectory();
	std::vector<std::string> files = FileSystem::GetFiles(dir.c_str());

	for (uint i = 0; i < files.size(); i++)
	{
		if (files[i] == "Library")
			continue;
		if (FileSystem::IsAFolder(files[i]))
		{
			Folder* newFolder = new Folder(files[i].c_str(), folder);
			folder->entries.push_back(newFolder);

			IterateFolder(newFolder, start);
		}
		else
		{
			Archive* newArchive = new Archive(files[i].c_str(), FileSystem::GetFileType(files[i]), folder);
			folder->entries.push_back(newArchive);

			std::string path = "Assets/" + newArchive->GetDirectory();
			bool redo = false;
			if (folder->name == "Primitives")
				redo = true;
			ImportFile(path.c_str(), false, redo);
		}
	}
}

void ResourceManager::UpdateEntriesTree(bool start)
{
	if (assets)
		delete assets;

	assets = new Folder(FileSystem::GetMainDirectory(), nullptr);

	IterateFolder(assets, start);
}

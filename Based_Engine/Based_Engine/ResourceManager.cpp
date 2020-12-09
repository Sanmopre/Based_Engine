#include "ResourceManager.h"
#include "FileSystem.h"
#include "Simp.h"
#include "Binary.h"

Archive::Archive(char* name, ArchiveType type, Archive* parent)
{
    this->name = name;
    this->type = type;
    this->parent = parent;
}
Archive::Archive(const char* name, ArchiveType type, Archive* parent)
{
    this->name = name;
    this->type = type;
    this->parent = parent;
}

Archive::~Archive()
{
}

Folder::Folder(char* name, Archive* parent) : Archive(name, ArchiveType::FOLDER, parent)
{
}
Folder::Folder(const char* name, Archive* parent) : Archive(name, ArchiveType::FOLDER, parent)
{
}

Folder::~Folder()
{
    while (archives.size() != 0)
    {
        delete * archives.begin();
        archives.erase(archives.begin());
    }
}

Resource::Resource(char* name, FileType type, Archive* parent) : Archive(name, ArchiveType::ARCHIVE, parent)
{
    this->fileType = type;
}
Resource::Resource(const char* name, FileType type, Archive* parent) : Archive(name, ArchiveType::ARCHIVE, parent)
{
    this->fileType = type;
}

Resource::~Resource()
{
}

ResourceManager::ResourceManager(Application* app, bool active) : Module(app, active)
{

}

ResourceManager::~ResourceManager()
{
    if(assets)
        delete assets;
}

bool ResourceManager::Start()
{
    assets = new Folder("Assets", nullptr);

    std::vector<Resource*> resources;

    std::vector<std::string> primitives = FileSystem::GetFiles("Meshes/Primitives");
    for (int i = 0; i < primitives.size(); i++)
    {
        std::string path = "Assets/Meshes/Primitives/" + primitives[i];
        Simp::LoadMesh(path.c_str(), true);

        resources.push_back(new Resource(path.c_str(), FileType::MESH, nullptr));
    }

    std::vector<std::string> meshes = FileSystem::GetFiles("Meshes");
    for (int i = 0; i < meshes.size(); i++)
    {
        std::string path = "Assets/Meshes/" + meshes[i];
        Simp::LoadMesh(path.c_str());

        resources.push_back(new Resource(path.c_str(), FileType::MESH, nullptr));
    }

    for (uint i = 0; i < resources.size(); i++)
    {
        resources[i]->parent = assets;
        assets->archives.push_back(resources[i]);
    }

    SaveFolderFiles(assets);

    return true;
}

update_status ResourceManager::Update()
{
    return UPDATE_CONTINUE;
}

bool ResourceManager::CleanUp()
{
    if (assets)
        delete assets;
    assets = nullptr;

    return true;
}

void ResourceManager::SaveFolderFiles(Folder* folder)
{
    std::string path = "Library/Folders/" + folder->name + ".banana";
    FileSystem::Delete(path.c_str());

    File* file = FileSystem::Open(path.c_str(), APPEND);
    if (!file)
    {
        LOG("Error creating banana file %s", path.c_str());
        return;
    }

    char* data = "FOL";
    FileSystem::Write(file, data, 3u, 1u);
    FileSystem::Enter(file);

    if (folder->parent)
    {
        data = "PAR";
        FileSystem::Write(file, data, 3u, 1u);
        FileSystem::Enter(file);
        FileSystem::Write(file, folder->parent->name.c_str(), folder->parent->name.size(), 1u);
        FileSystem::Enter(file);
    }

    data = "RES";
    FileSystem::Write(file, data, 3u, 1u);
    FileSystem::Enter(file);
    for (uint i = 0; i < folder->archives.size(); i++)
    {
        if (folder->archives[i]->type == ArchiveType::FOLDER)
        {
            SaveFolderFiles((Folder*)folder->archives[i]);
            FileSystem::Write(file, folder->archives[i]->name.c_str(), folder->archives[i]->name.size(), 1u);
            FileSystem::Enter(file);
        }
        else
        {
            FileSystem::Write(file, folder->archives[i]->name.c_str(), folder->archives[i]->name.size(), 1u);
            FileSystem::Enter(file);
        }
    }

    if (!FileSystem::Close(file))
        LOG("Sadge");
}

void ResourceManager::LoadFolderFiles(Folder* folder)
{

}

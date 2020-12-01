#include "ResourceManager.h"
#include "FileSystem.h"
#include "Simp.h"

ResourceManager::ResourceManager(Application* app, bool active) : Module(app, active)
{

}

ResourceManager::~ResourceManager()
{
}

bool ResourceManager::Start()
{
    std::vector<std::string> primitives = FileSystem::GetFiles("Meshes/Primitives");
    for (int i = 0; i < primitives.size(); i++)
    {
        std::string path = "Assets/Meshes/Primitives/" + primitives[i];
        Simp::LoadMesh(path.c_str(), true);
    }

    std::vector<std::string> meshes = FileSystem::GetFiles("Meshes");
    for (int i = 0; i < meshes.size(); i++)
    {
        std::string path = "Assets/Meshes/" + meshes[i];

        Simp::LoadMesh(path.c_str());
    }

    return true;
}

update_status ResourceManager::Update()
{
    return UPDATE_CONTINUE;
}

bool ResourceManager::CleanUp()
{
    return true;
}

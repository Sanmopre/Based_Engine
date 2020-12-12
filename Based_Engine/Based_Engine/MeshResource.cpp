#include "MeshResource.h"

#include "FileSystem.h"
#include "Simp.h"

MeshResource::MeshResource(uint uid, const char* assetsFile, const char* libraryFile)
    : Resource(uid, FileType::MESH, assetsFile, libraryFile)
{
}

MeshResource::~MeshResource()
{
}

const std::vector<Mesh> MeshResource::GetMeshes() const
{
    return meshes;
}

bool MeshResource::LoadInMemory()
{
    std::vector<Mesh> meshes = Simp::LoadMeshFile(libraryFile.c_str());

    if (meshes.size() == 0)
        return false;
    return true;
}

bool MeshResource::Unload()
{
    meshes.erase(meshes.begin(), meshes.end());

    return true;
}

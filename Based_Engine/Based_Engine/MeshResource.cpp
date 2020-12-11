#include "MeshResource.h"

#include "FileSystem.h"
#include "Simp.h"

MeshResource::MeshResource(uint uid) : Resource(uid, FileType::MESH)
{
}

MeshResource::~MeshResource()
{
}

const Mesh* MeshResource::GetMesh() const
{
    return &mesh;
}

bool MeshResource::LoadInMemory()
{
    std::vector<Mesh> meshes = Simp::LoadMeshFile(libraryFile.c_str());

    if (meshes.size() != 0)
    {
        mesh = meshes[0];
        return true;
    }
    return false;
}
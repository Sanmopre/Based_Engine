#ifndef __MESHRESOURCE_H__
#define __MESHRESOURCE_H__

#include "Resource.h"

#include "Mesh.h"

class MeshResource : public Resource
{
public:

	MeshResource(uint uid, const char* assetsFile, const char* libraryFile);
	~MeshResource();

	const std::vector<Mesh> GetMeshes() const;
	bool LoadInMemory() override;
	bool Unload() override;

private:

	std::vector<Mesh> meshes;
};

#endif //__MESHRESOURCE_H__
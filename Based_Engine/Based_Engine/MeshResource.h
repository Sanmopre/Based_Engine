#ifndef __MESHRESOURCE_H__
#define __MESHRESOURCE_H__

#include "Resource.h"

#include "Mesh.h"

class MeshResource : public Resource
{
public:

	MeshResource(uint uid);
	~MeshResource();

	const Mesh* GetMesh() const;
	bool LoadInMemory() override;

private:

	Mesh mesh;
};

#endif //__MESHRESOURCE_H__

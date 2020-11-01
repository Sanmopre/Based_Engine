#include "Simp.h"

#include "cimport.h"
#include "scene.h"
#include "postprocess.h"
#include "Mesh.h"

void Simp::InitializeDebugger() 
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
}

void Simp::CleanDebugger() 
{
	aiDetachAllLogStreams();
}

std::vector<Mesh> Simp::LoadFile(const char* file_path) 
{
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);

	std::vector<Mesh> loadedMeshes;

	if (scene != nullptr && scene->HasMeshes())
	{
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			Mesh NewMesh;

			NewMesh.buffersId[Mesh::index] = 0;
			NewMesh.buffersId[Mesh::normal] = 0;
			NewMesh.buffersId[Mesh::vertex] = 0;

			aiMesh* loaded_mesh = scene->mMeshes[i];

			NewMesh.buffersLength[Mesh::vertex] = loaded_mesh->mNumVertices;
			NewMesh.vertices = new float[NewMesh.buffersLength[Mesh::vertex] * 3];
			memcpy(NewMesh.vertices, loaded_mesh->mVertices, sizeof(float) * NewMesh.buffersLength[Mesh::vertex] * 3);

			if (loaded_mesh->HasFaces())
			{
				NewMesh.buffersLength[Mesh::index] = loaded_mesh->mNumFaces * 3;
				NewMesh.indices = new uint[NewMesh.buffersLength[Mesh::index]]; 
				for (uint j = 0; j < loaded_mesh->mNumFaces; ++j)
				{
					if (loaded_mesh->mFaces[j].mNumIndices == 3)
					{
						memcpy(&NewMesh.indices[j * 3], loaded_mesh->mFaces[j].mIndices, 3 * sizeof(uint));
					}
				}
			}
			if (loaded_mesh->HasNormals())
			{
				NewMesh.buffersLength[Mesh::normal] = loaded_mesh->mNumVertices;
				NewMesh.normals = new float[NewMesh.buffersLength[Mesh::normal] * 3];
				memcpy(NewMesh.normals, loaded_mesh->mNormals, sizeof(float) * NewMesh.buffersLength[Mesh::normal] * 3);

			}
			if (scene->mMeshes[i]->HasTextureCoords(0))
			{
				NewMesh.buffersLength[Mesh::texture] = scene->mMeshes[i]->mNumVertices;
				NewMesh.texture_coord = new float[scene->mMeshes[i]->mNumVertices * 2];

				for (int j = 0; j < NewMesh.buffersLength[Mesh::texture]; j++)
				{
					NewMesh.texture_coord[j * 2] = scene->mMeshes[i]->mTextureCoords[0][j].x;
					NewMesh.texture_coord[j * 2 + 1] = scene->mMeshes[i]->mTextureCoords[0][j].y;
				}
			}

			loadedMeshes.push_back(NewMesh);
		}
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s", file_path);
	}

	return loadedMeshes;
}
#include "Module.h"
#include "cimport.h"
#include "scene.h"
#include "postprocess.h"
#include "Globals.h"
#include "Mesh.h"
#include "Application.h"
#include "Renderer3D.h"

#include "Math/float3.h"
#include "Math/float2.h"

namespace Simp {

	std::vector<Mesh*> mesh_vec;

	void InitializeDebugger() {
		struct aiLogStream stream;
		stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
		aiAttachLogStream(&stream);
	}

	void CleanDebugger() {
		aiDetachAllLogStreams();
	}

	std::vector<Mesh> LoadFile(char* file_path) {
		
		const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);

		std::vector<Mesh> loadedMeshes;

		if (scene != nullptr && scene->HasMeshes())
		{
			// Use scene->mNumMeshes to iterate on scene->mMeshes array
			for (int i = 0; i < scene->mNumMeshes; i++)
			{
				std::vector<Vertex> vertices;
				std::vector<uint> indices;

				aiMesh* mesh = scene->mMeshes[i];

				for (unsigned int i = 0; i < mesh->mNumVertices; i++)
				{
					Vertex vertex;

					float3 vector;
					vector.x = mesh->mVertices[i].x;
					vector.y = mesh->mVertices[i].y;
					vector.z = mesh->mVertices[i].z;
					vertex.Position = vector;

					if (mesh->HasNormals())
					{
						vector.x = mesh->mNormals[i].x;
						vector.y = mesh->mNormals[i].y;
						vector.z = -1 * mesh->mNormals[i].z;
						vertex.Normal = vector;
					}

					if (mesh->mTextureCoords[0]) 
					{
						float2 vec;
						vec.x = mesh->mTextureCoords[0][i].x;
						vec.y = mesh->mTextureCoords[0][i].y;
						vertex.TexCoords = vec;
					}
					else
						vertex.TexCoords = float2(0.0f, 0.0f);

					vertices.push_back(vertex);
				}

				for (uint i = 0; i < mesh->mNumFaces; i++)
				{
					aiFace face = mesh->mFaces[i];
					assert(face.mNumIndices == 3);

					for (uint j = 0; j < face.mNumIndices; j++)
						indices.push_back(face.mIndices[j]);
				}

				loadedMeshes.push_back(Mesh(vertices, indices));
			}
				aiReleaseImport(scene);
		}

		return loadedMeshes;

	}
}
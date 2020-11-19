#include "Simp.h"
#include "Binary.h"

#include "FileSystem.h"

#include "cimport.h"
#include "scene.h"
#include "postprocess.h"
#include "Mesh.h"

void Simp::InitializeDebugger() 
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	LOG("Initialize Assimp Debugger");
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

void Simp::LoadMesh(const char* file_path)
{
	std::string name = file_path;
	for (std::string::iterator c = name.end() - 1; c != name.begin(); c--)
	{
		if (*c == '.')
		{
			name.erase(c, name.end());
			break;
		}
	}
	for (std::string::iterator c = name.end() - 1; c != name.begin(); c--)
	{
		if (*c == '/')
		{
			name.erase(name.begin(), c + 1);
			break;
		}
	}
	std::string path = "Library/Meshes/" + name + ".monki";

	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		FileSystem::Delete(path.c_str());
		File* file = FileSystem::Open(path.c_str(), APPEND);
		if (file == nullptr)
		{
			aiReleaseImport(scene);
			return;
		}

		char* data = "obj";
		FileSystem::Write(file, data, 3u, 1u);
		data = Binary::GetBinaryStream<unsigned int>(scene->mNumMeshes);
		FileSystem::Write(file, data, sizeof(unsigned int), 1u);
		delete[] data;

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[i];

			data = Binary::GetBinaryStream<unsigned int>(mesh->mNumVertices);
			FileSystem::Write(file, data, sizeof(unsigned int), 1u);
			delete[] data;

			int vertex_size = mesh->mNumVertices * 3;
			float* vertices = new float[vertex_size];
			memcpy(vertices, mesh->mVertices, sizeof(float) * vertex_size);

			for (int v = 0; v < vertex_size; v++)
			{
				data = Binary::GetBinaryStream<float>(vertices[v]);
				FileSystem::Write(file, data, sizeof(float), 1u);
				delete[] data;
			}

			if (mesh->HasFaces())
			{
				data = Binary::GetBinaryStream<unsigned int>(mesh->mNumFaces);
				FileSystem::Write(file, data, sizeof(unsigned int), 1u);
				delete[] data;

				int index_size = mesh->mNumFaces * 3;
				unsigned int* indices = new unsigned int[index_size];
				for (int j = 0; j < mesh->mNumFaces; ++j)
					if (mesh->mFaces[j].mNumIndices == 3)
						memcpy(indices + j * 3, mesh->mFaces[j].mIndices, sizeof(unsigned int) * 3);

				for (int j = 0; j < index_size; j++)
				{
					data = Binary::GetBinaryStream<float>(indices[j]);
					FileSystem::Write(file, data, sizeof(unsigned int), 1u);
					delete[] data;
				}
			}

			if (mesh->HasNormals())
			{
				data = Binary::GetBinaryStream<unsigned int>(mesh->mNumVertices);
				FileSystem::Write(file, data, sizeof(unsigned int), 1u);
				delete[] data;

				int normals_size = mesh->mNumVertices * 3;
				float* normals = new float[normals_size];
				memcpy(normals, mesh->mNormals, sizeof(float) * normals_size);

				for (int n = 0; n < normals_size; n++)
				{
					data = Binary::GetBinaryStream<float>(normals[n]);
					FileSystem::Write(file, data, sizeof(float), 1u);
					delete[] data;
				}
			}
			if (scene->mMeshes[i]->HasTextureCoords(0))
			{
				int textures_size = scene->mMeshes[i]->mNumVertices;
				float* textures = new float[textures_size * 2];
			
				for (int t = 0; t < textures_size / 2; t++)
				{
					textures[t * 2] = scene->mMeshes[i]->mTextureCoords[0][t].x;
					textures[t * 2 + 1] = scene->mMeshes[i]->mTextureCoords[0][t].y;
				}
				for (int t = 0; t < textures_size / 2; t++)
				{
					data = Binary::GetBinaryStream<float>(textures[t]);
					FileSystem::Write(file, data, sizeof(float), 1u);
					delete[] data;
				}
			}
		}

		if (!FileSystem::Close(file))
			LOG("Sadge");
	}

	aiReleaseImport(scene);
}
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

void Enter(File* file)
{
	char data = 10;
	FileSystem::Write(file, &data, 1u, 1u);
}

std::string Simp::LoadMesh(const char* file_path)
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
			return nullptr;
		}

		char* data = "OBJ";
		FileSystem::Write(file, data, 3u, 1u);
		data = Binary::GetBinaryStream<unsigned int>(scene->mNumMeshes);
		FileSystem::Write(file, data, sizeof(unsigned int), 1u);
		delete[] data;
		Enter(file);

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[i];

			data = "MESH";
			FileSystem::Write(file, data, 4u, 1u);
			Enter(file);
			data = "VEC";
			FileSystem::Write(file, data, 3u, 1u);
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
				Enter(file);
				data = "IND";
				FileSystem::Write(file, data, 3u, 1u);
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
					data = Binary::GetBinaryStream<unsigned int>(indices[j]);
					FileSystem::Write(file, data, sizeof(unsigned int), 1u);
					delete[] data;
				}
			}

			if (mesh->HasNormals())
			{
				Enter(file);
				data = "NOR";
				FileSystem::Write(file, data, 3u, 1u);
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
				Enter(file);
				data = "TEX";
				FileSystem::Write(file, data, 3u, 1u);
				data = Binary::GetBinaryStream<unsigned int>(scene->mMeshes[i]->mNumVertices);
				FileSystem::Write(file, data, sizeof(unsigned int), 1u);
				delete[] data;

				int textures_size = scene->mMeshes[i]->mNumVertices;
				float* textures = new float[textures_size * 2];
			
				for (int t = 0; t < textures_size; t++)
				{
					textures[t * 2] = scene->mMeshes[i]->mTextureCoords[0][t].x;
					textures[t * 2 + 1] = scene->mMeshes[i]->mTextureCoords[0][t].y;
				}
				for (int t = 0; t < textures_size * 2; t++)
				{
					data = Binary::GetBinaryStream<float>(textures[t]);
					FileSystem::Write(file, data, sizeof(float), 1u);
					delete[] data;
				}
			}

			Enter(file);
		}
	}

	aiReleaseImport(scene);

	return path;
}

std::vector<Mesh> Simp::LoadMeshFile(const char* path)
{
	std::vector<Mesh> meshes;

	File* file = FileSystem::Open(path, READ);
	if (!file)
	{
		return meshes;
	}
	char* data = FileSystem::ReadAll(file);
	if (!FileSystem::Close(file))
		LOG("Sadge");
	if (!data)
		return meshes;

	std::string type;
	for (uint i = 0; i < 3; i++)
		type += data[i];
	if (type != "OBJ")
	{
		LOG("Invalid type: %s", type.c_str());
		delete[] data;
		return meshes;
	}

	unsigned int meshNum = Binary::GetDataFromStream<unsigned int>(&data[3]);
	unsigned int bit = 8;
	for (int m = 0; m < meshNum; m++)
	{
		std::string bufferName;
		int limit = bit + 4;
		for (uint i = bit; i < limit; i++)
		{
			bufferName += data[i];
			bit++;
		}
		bit++;
		if (bufferName != "MESH")
		{
			LOG("No mesh found: %s", path);
			delete[] data;
			return meshes;
		}
		Mesh mesh;

		mesh.buffersId[Mesh::index] = 0;
		mesh.buffersId[Mesh::normal] = 0;
		mesh.buffersId[Mesh::vertex] = 0;

		bufferName = "";
		limit = bit + 3;
		for (uint i = bit; i < limit; i++)
		{
			bufferName += data[i];
			bit++;
		}
		if (bufferName != "VEC")
		{
			LOG("No vertex found: %s", path);
			delete[] data;
			return meshes;
		}

		unsigned int vertexNum = Binary::GetDataFromStream<unsigned int>(&data[bit]);
		bit += 4;

		mesh.buffersLength[Mesh::vertex] = vertexNum;
		mesh.vertices = new float[vertexNum * 3];

		int firstBit = bit;
		limit = bit + vertexNum * 3;
		for (int i = bit; i < limit; i ++)
		{
			float vertex = Binary::GetDataFromStream<float>(&data[bit]);
			mesh.vertices[i - firstBit] = vertex;

			bit += 4;
		}
		bit++;

		bufferName = "";
		limit = bit + 3;
		for (uint i = bit; i < limit; i++)
		{
			bufferName += data[i];
			bit++;
		}
		if (bufferName != "IND")
		{
			LOG("No index found: %s", path);
			delete[] mesh.vertices;
			delete[] data;
			return meshes;
		}

		unsigned int indexNum = Binary::GetDataFromStream<unsigned int>(&data[bit]);
		bit += 4;

		mesh.buffersLength[Mesh::index] = indexNum * 3;
		mesh.indices = new uint[indexNum * 3];

		firstBit = bit;
		limit = bit + indexNum * 3;
		for (int i = bit; i < limit; i++)
		{
			uint index = Binary::GetDataFromStream<uint>(&data[bit]);
			mesh.indices[i - firstBit] = index;

			bit += 4;
		}
		bit++;

		bufferName = "";
		limit = bit + 3;
		for (uint i = bit; i < limit; i++)
		{
			bufferName += data[i];
			bit++;
		}
		if (bufferName != "NOR")
		{
			LOG("No normals found: %s", path);
			delete[] mesh.vertices;
			delete[] mesh.indices;
			delete[] data;
			return meshes;
		}

		unsigned int normalNum = Binary::GetDataFromStream<unsigned int>(&data[bit]);
		bit += 4;

		mesh.buffersLength[Mesh::normal] = normalNum;
		mesh.normals = new float[normalNum * 3];

		firstBit = bit;
		limit = bit + normalNum * 3;
		for (int i = bit; i < limit; i++)
		{
			float normal = Binary::GetDataFromStream<float>(&data[bit]);
			mesh.normals[i - firstBit] = normal;

			bit += 4;
		}
		bit++;

		bufferName = "";
		limit = bit + 3;
		for (uint i = bit; i < limit; i++)
		{
			bufferName += data[i];
			bit++;
		}
		if (bufferName != "TEX")
		{
			LOG("No texture coordinates found: %s", path);
			delete[] mesh.vertices;
			delete[] mesh.indices;
			delete[] mesh.normals;
			delete[] data;
			return meshes;
		}

		unsigned int textureNum = Binary::GetDataFromStream<unsigned int>(&data[bit]);
		bit += 4;

		mesh.buffersLength[Mesh::texture] = textureNum;
		mesh.texture_coord = new float[textureNum * 2];
		
		firstBit = bit;
		limit = bit + textureNum * 2;
		for (int i = bit; i < limit; i++)
		{
			float texture = Binary::GetDataFromStream<float>(&data[bit]);
			mesh.texture_coord[i - firstBit] = texture;
		
			bit += 4;
		}
		bit++;

		meshes.push_back(mesh);
	}

	delete[] data;

	return meshes;
}
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

void Enter(File* file)
{
	char data = 10;
	FileSystem::Write(file, &data, 1u, 1u);
}

std::string CreateFileName(const char* path)
{
	std::string name = path;
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
		if (*c == '/' || *c == '\\')
		{
			name.erase(name.begin(), c + 1);
			break;
		}
	}

	std::string output = "Library/Meshes/" + name + ".monki";
	return output;
}

void SimpToMonki(int i, aiMatrix4x4 transform, File* file, const aiScene* scene)
{
	aiMesh* mesh = scene->mMeshes[i];

	char* data = "MESH";
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

void ExpandNode(aiNode* node, File* file, const aiScene* scene)
{
	for (int i = 0; i < node->mNumMeshes; i++)
		SimpToMonki(node->mMeshes[i], node->mTransformation, file, scene);

	for (int i = 0; i < node->mNumChildren; i++)
		ExpandNode(node->mChildren[i], file, scene);
}

std::string Simp::LoadMesh(const char* file_path)
{
	std::string path = CreateFileName(file_path);

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

		ExpandNode(scene->mRootNode, file, scene);

		FileSystem::Close(file);
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
			bit -= 3;
			delete[] mesh.vertices;
			delete[] mesh.indices;
			delete[] mesh.normals;
			continue;
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
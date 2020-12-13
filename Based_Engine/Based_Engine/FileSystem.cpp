#include "FileSystem.h"

#include "Application.h"

#include "physfs.h"

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>

#define ASSETS_PATH "Assets"

File::File()
{
}

File::~File()
{
}

bool FileSystem::Init()
{
	if (!PHYSFS_init(NULL))
	{
		LOG("Error initializing PhysFS, %s", PHYSFS_getLastError());
	}
	else
	{
		LOG("PhysFS succesfully initialized");
	}

	if (!PHYSFS_mount(ASSETS_PATH, NULL, 0))
	{
		LOG("Error adding %s to the virtual file system, %s", ASSETS_PATH, PHYSFS_getLastError());
	}
	else
	{
		LOG("PhysFS succesfully added %s to the virtual file system", ASSETS_PATH);
	}

	if (!PHYSFS_setWriteDir(ASSETS_PATH))
	{
		LOG("Error setting %s to the write directory, %s", ASSETS_PATH, PHYSFS_getLastError());
	}
	else
	{
		LOG("PhysFS succesfully set %s as the write directory", ASSETS_PATH);

		CreateFolder("Library/LMeshes");
		CreateFolder("Library/LMaterials");
	}

	return true;
}

bool FileSystem::Deinit()
{
	if (!PHYSFS_deinit())
	{
		LOG("Error deinitializing PhysFS, %s", PHYSFS_getLastError());
	}
	else
	{
		LOG("PhysFS succesfully deinitialized");
	}

	return true;
}

File* FileSystem::Open(const char* path, OpenFormat format)
{
	File* file = new File();
	file->path = path;

	switch (format)
	{
	case READ:
		file->handle = PHYSFS_openRead(path);
		break;
	case WRITE:
		file->handle = PHYSFS_openWrite(path);
		break;
	case APPEND:
		file->handle = PHYSFS_openAppend(path);
		break;
	}

	if (file->handle == NULL)
	{
		LOG("Couldn't open file %s, %s", path, PHYSFS_getLastError());
		return nullptr;
	}

	return file;
}

bool FileSystem::Close(File* file)
{
	int output = PHYSFS_close(file->handle);
	if (!output)
	{
		LOG("Couldn't close file %s, %s", file->path, PHYSFS_getLastError());
		return false;
	}
	delete file;

	return true;
}

bool FileSystem::Write(File* file, const char* buffer, uint size, uint count)
{
	int output = PHYSFS_write(file->handle, buffer, size, count);

	if (output == -1)
	{
		LOG("Couldn't write in the file %s [COMPLETE FAILURE], %s", file->path, PHYSFS_getLastError());
		return false;
	}
	else if (output < count)
	{
		LOG("Couldn't write all data in the file %s (objects written: %d), %s", file->path, output, PHYSFS_getLastError());
		return false;
	}

	return true;
}

char* FileSystem::ReadAll(File* file)
{
	uint size = PHYSFS_fileLength(file->handle);

	char* output = new char[size];
	int error = PHYSFS_read(file->handle, output, size, 1);

	if (error == -1)
	{
		LOG("Couldn't read the file %s [COMPLETE FAILURE], %s", file->path, PHYSFS_getLastError());
		delete[] output;
		return nullptr;
	}
	else if (error < 1)
	{
		LOG("Couldn't read all data in the file %s (objects read: %d), %s", file->path, output, PHYSFS_getLastError());
		delete[] output;
		return false;
	}

	return output;
}

bool FileSystem::Read(File* file, void* buffer, uint size, uint count)
{
	int output = PHYSFS_read(file->handle, buffer, size, count);

	if (output == -1)
	{
		LOG("Couldn't read the file %s [COMPLETE FAILURE], %s", file->path, PHYSFS_getLastError());
		return false;
	}
	else if (output < count)
	{
		LOG("Couldn't read all data in the file %s (objects read: %d), %s", file->path, output, PHYSFS_getLastError());
		return false;
	}

	return true;
}

bool FileSystem::Exists(const char* path)
{
	if (!PHYSFS_exists(path))
	{
		LOG("File %s does't exist", path);
		return false;
	}

	return true;
}

int FileSystem::FileLength(File* file)
{
	return PHYSFS_fileLength(file->handle);
}

int FileSystem::FileLength(const char* path)
{
	return std::experimental::filesystem::file_size(path);;
}

FileType FileSystem::GetFileType(std::string path)
{
	std::string type;
	for (uint i = path.size() - 1; i != -1; i--)
	{
		if (path[i] == '.')
		{
			std::string reverse;
			for (uint c = type.size() - 1; c != -1; c--)
				reverse.push_back(type[c]);
			type = reverse;

			break;
		}

		type.push_back(path[i]);
	}
	LOG("File type: %s", type.c_str());


	if (type == "fbx" || type == "FBX" || type == "obj" || type == "OBJ")
		return FileType::MESH;
	else if (type == "png" || type == "PNG" || type == "jpg" || type == "JPG" || type == "dds" || type == "DDS" || type == "tga" || type == "TGA")
		return FileType::IMAGE;

	LOG("Unable to load file, unknown file type");
	return FileType::UNKNOWN;
}

bool FileSystem::IsAFolder(std::string fileName)
{
	bool output = true;
	for (uint i = 0; i < fileName.size(); i++)
		if (fileName[i] == '.')
		{
			output = false;
			break;
		}

	return output;
}

const char* FileSystem::GetMainDirectory()
{
	return ASSETS_PATH;
}

std::string FileSystem::CopyFileToAssets(const char* destDir, const char* path, FileType type)
{
	std::string name = path;
	for (std::string::iterator c = name.end() - 1; c != name.begin(); c--)
	{
		if (*c == '/' || *c == '\\')
		{
			name.erase(name.begin(), c + 1);
			break;
		}
	}

	std::string newPath = destDir;
	std::string newDest;
	if (newPath != "")
	{
		newDest = "../Game/Assets/" + newPath;
		newPath = "../Game/Assets/" + newPath + "/" + name;
	}
	else
	{
		newDest = "../Game/Assets";
		newPath = "../Game/Assets/" + name;
	}

	std::experimental::filesystem::copy(path, newDest.c_str());
	
	return newPath;
}

bool FileSystem::CreateFolder(char* directory)
{
	if (!PHYSFS_mkdir(directory))
	{
		LOG("Couldn't create directory %s, %s", directory, PHYSFS_getLastError());
		return false;
	}

	return true;
}

bool FileSystem::Delete(const char* path)
{
	if (!Exists(path))
		return false;

	int output = PHYSFS_delete(path);
	if (!output)
	{
		LOG("Couldn't delete file %s, %s", path, PHYSFS_getLastError());
		return false;
	}

	return true;
}

std::vector<std::string> FileSystem::GetFiles(char* directory)
{
	std::vector<std::string> output;

	char** rc = PHYSFS_enumerateFiles(directory);
	char** i;

	for (i = rc; *i != NULL; i++)
	{
		std::string file = *i;
		output.push_back(file);
	}
	PHYSFS_freeList(rc);

	return output;
}

std::vector<std::string> FileSystem::GetFiles(const char* directory)
{
	std::vector<std::string> output;

	char** rc = PHYSFS_enumerateFiles(directory);
	char** i;

	for (i = rc; *i != NULL; i++)
	{
		std::string file = *i;
		output.push_back(file);
	}
	PHYSFS_freeList(rc);

	return output;
}

void FileSystem::Enter(File* file)
{
	char data = 10;
	FileSystem::Write(file, &data, 1u, 1u);
}

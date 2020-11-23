#include "FileSystem.h"

#include "physfs.h"

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

		PHYSFS_mkdir("Library/Meshes");
		PHYSFS_mkdir("Library/Materials");
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

bool FileSystem::Exists(const char* path)
{
	if (!PHYSFS_exists(path))
	{
		LOG("File %s does't exist", path);
		return false;
	}

	return true;
}

int FileLength(File* file)
{
	return PHYSFS_fileLength(file->handle);
}

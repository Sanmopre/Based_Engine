#pragma once

#include "Globals.h"

class SDL_RWops;
struct PHYSFS_File;

enum OpenFormat
{
	READ = 0,
	WRITE = 1,
	APPEND = 2
};

enum class FileType
{
	MESH,
	IMAGE,
	UNKNOWN
};

class File
{
public:
	File();
	~File();

	const char* path;
	PHYSFS_File* handle;
};

namespace FileSystem
{
	bool Init();
	bool Deinit();

	File* Open(const char* path, OpenFormat format);
	bool Close(File* file);
	bool Write(File* file, const char* buffer, uint size, uint count);
	char* ReadAll(File* file);
	bool Read(File* file, void* buffer, uint size, uint count);

	bool Exists(const char* path);
	int FileLength(File* file);
	int FileLength(const char* path);
	FileType GetFileType(std::string path);
	bool IsAFolder(std::string fileName);
	const char* GetMainDirectory();

	std::string CopyFileToAssets(const char* destDir, const char* file, FileType type);
	bool CreateFolder(char* directory);
	bool Delete(const char* path);
	std::vector<std::string> GetFiles(char* directory);
	std::vector<std::string> GetFiles(const char* directory);

	void Enter(File* file);
}
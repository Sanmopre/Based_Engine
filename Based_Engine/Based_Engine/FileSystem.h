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
	bool Delete(const char* path);

	bool Exists(const char* path);
	int FileLength(File*);
}
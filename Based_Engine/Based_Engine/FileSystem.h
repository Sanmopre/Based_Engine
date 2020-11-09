#pragma once

#include "Module.h"

class SDL_RWops;

class Entry
{
	char* name;
};
class Folder : public Entry
{
	std::vector<Entry*> entries;
};
class File : public Entry
{
	char* path;
	Folder* parent;
};

class FileSystem : public Module
{
public:

	FileSystem(Application* app, bool enabled = true);
	~FileSystem();

	bool Start();
	bool CleanUp();

	SDL_RWops LoadFile(const char* path);

	Entry* main_folder = nullptr;
};
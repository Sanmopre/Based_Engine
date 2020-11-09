#include "FileSystem.h"

#include "physfs.h"
#include "SDL.h"

#define ASSETS_PATH "Assets"

FileSystem::FileSystem(Application* app, bool enabled) : Module(app, enabled)
{
}

FileSystem::~FileSystem()
{
}

bool FileSystem::Start()
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

	return true;
}

bool FileSystem::CleanUp()
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

SDL_RWops FileSystem::LoadFile(const char* path)
{
	SDL_RWops output;

	return output;
}
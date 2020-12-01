#pragma once

#include "Globals.h"

class Mesh;

namespace Simp 
{
	void InitializeDebugger();
	void CleanDebugger();

	std::string LoadMesh(const char* file_path, bool redo = false);
	std::vector<Mesh> LoadMeshFile(const char* file_path);
}
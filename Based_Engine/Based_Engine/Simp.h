#pragma once

#include "Globals.h"

class Mesh;

namespace Simp 
{
	void InitializeDebugger();
	void CleanDebugger();

	std::vector<Mesh> LoadFile(const char* file_path);

	std::string LoadMesh(const char* file_path);
	std::vector<Mesh> LoadMeshFile(const char* file_path);
}
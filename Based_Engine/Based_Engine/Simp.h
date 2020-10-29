#pragma once

#include "Globals.h"

class Mesh;

namespace Simp 
{
	void InitializeDebugger();
	void CleanDebugger();

	std::vector<Mesh> LoadFile(const char* file_path);
}
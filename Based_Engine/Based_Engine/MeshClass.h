
#include "Globals.h"

class Mesh {


public:
	Mesh();
	~Mesh();
	uint id_index = 0;
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0;
	uint num_vertex = 0;
	float* vertex = nullptr;
};
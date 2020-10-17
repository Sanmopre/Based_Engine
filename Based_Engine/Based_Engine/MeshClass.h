
#include "Globals.h"

struct Mesh {

	Mesh() {};

	enum Buffers
	{
		index,
		vertex,
		maxBuffers
	};

	uint buffersId[maxBuffers];
	uint buffersSize[maxBuffers];

	uint* indices = nullptr;
	float* vertices = nullptr;

	//uint buffer;
};
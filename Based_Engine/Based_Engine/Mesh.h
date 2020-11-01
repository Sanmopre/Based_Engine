#pragma once
#include "Globals.h"
#include "Color.h"

#include "Math/float3.h"
#include "Math/float2.h"


struct Vertex
{
	float3 Position;
	float3 Normal;
	float2 TexCoords;
};


class Mesh
{
public:
	Mesh();
	~Mesh();

	void GenerateBuffers();

	void Render(bool globalWireMode = false);
	void InnerRender() const;

	void DrawNormals() const;
	void UpdatePosition(float3 position, float3 last_position);
	void UpdateScale(float3 scale, float3 last_scale);

public:

	enum Buffers
	{
		index,
		normal,
		vertex,
		texture,
		maxBuffers
	};

	uint buffersId[maxBuffers];
	uint buffersLength[maxBuffers];

	uint* indices = nullptr;
	float* normals = nullptr;
	float* vertices = nullptr;
	float* texture_coord = nullptr;

	uint tex_id;
	bool drawnormals = false;

private:

	Color color;
	bool wire, noFace;
	bool generated_frame_buffers = false;

};

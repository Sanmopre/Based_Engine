#pragma once
#include "Globals.h"
#include "Color.h"

#include "Math/float3.h"
#include "Math/float2.h"
#include "Math/float4x4.h"

#include "MathGeoLib.h"

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

	//AABB
	void Generate_AABB();
	void DrawBoundingBox(float4x4 transform, bool active);

	void UpdateMeshTransform(float4x4 transform);
	void UpdatePosition(float3 position, float3 last_position);
	void UpdateScale(float3 scale, float3 last_scale);

	void SetMaterialColor(float r, float g, float b, float a = 1.0f);

public:

	enum Buffers
	{
		index = 0,
		normal = 1,
		vertex = 2,
		texture = 3,
		maxBuffers = 4
	};

	uint buffersId[maxBuffers];
	uint buffersLength[maxBuffers];

	uint* indices = nullptr;
	float* normals = nullptr;
	float* vertices = nullptr;
	float* texture_coord = nullptr;

	uint tex_id;
	bool drawnormals = false;
	bool show_bounding_box = false;
	float4x4 transform;

private:

	Color color;
	bool wire, noFace;
	bool generated_frame_buffers = false;


	AABB aabb;
};

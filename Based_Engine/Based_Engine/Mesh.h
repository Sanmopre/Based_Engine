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
	Mesh(std::vector<Vertex> vertices, std::vector<uint> indices);
	~Mesh();

	void GenerateBuffers();

	void Render(bool globalWireMode = false) const;
	void InnerRender() const;

	void DrawNormals() const;
	void UpdatePosition(float x, float y, float z);
	void UpdateScale(float x, float y, float z);

public:
	uint idVertex;
	std::vector<Vertex> vertices;

	uint idIndex; 
	std::vector<uint> indices;

	uint VAO;
	bool drawnormals = false;

	uint tex_id;
private:
	Color color;
	bool wire, noFace;

};

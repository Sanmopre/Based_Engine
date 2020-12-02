#include "Mesh.h"
#include <GL/glew.h>

#include "Math/Quat.h"

Mesh::Mesh()
{
	GenerateBuffers();
	color = Color(0,0,0 );
}

Mesh::~Mesh()
{

}

void Mesh::GenerateBuffers()
{
	glGenBuffers(1, (GLuint*)& buffersId[vertex]);
	glBindBuffer(GL_ARRAY_BUFFER, buffersId[vertex]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) *buffersLength[vertex] * 3, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*)& buffersId[index]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffersId[index]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) *buffersLength[index], indices, GL_STATIC_DRAW);

	if (normals != nullptr)
	{
		glGenBuffers(1, (GLuint*)& buffersId[normal]);
		glBindBuffer(GL_ARRAY_BUFFER, buffersId[normal]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * buffersLength[normal],normals, GL_STATIC_DRAW);
	}

	if (texture_coord != nullptr)
	{
		glGenBuffers(1, (GLuint*)&buffersId[texture]);
		glBindBuffer(GL_ARRAY_BUFFER, buffersId[texture]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * buffersLength[texture] * 2, texture_coord, GL_STATIC_DRAW);
	}
}


void Mesh::Render(bool globalWireMode) 
{
	if (!generated_frame_buffers) 
	{	
	GenerateBuffers();
	generated_frame_buffers = true;
	}

	glPushMatrix();
	//SetMaterialColor(10, 10, 10);
	DrawBoundingBox(show_bounding_box);
	if(drawnormals)
		DrawNormals();
	InnerRender();
	glPopMatrix();
}


void Mesh::InnerRender() const
{

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D, tex_id);

	glBindBuffer(GL_ARRAY_BUFFER, buffersId[texture]);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER,buffersId[vertex]);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, buffersId[normal]);
	glNormalPointer(GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffersId[index]);

	glDrawElements(GL_TRIANGLES, buffersLength[index], GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D, 0);
}


void Mesh::DrawNormals() const
{
	glBegin(GL_LINES);
	for (size_t i = 0; i <buffersLength[vertex] * 3; i += 3)
	{
		glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]); 
		glVertex3f(vertices[i] + normals[i], vertices[i + 1] + normals[i + 1], vertices[i + 2] + normals[i + 2]);
	}
	glEnd();
}

void Mesh::UpdateMeshTransform(float4x4 transform)
{
	float3 position, scale;
	Quat rotation;
	transform.Decompose(position, rotation, scale);
}

void Mesh::UpdatePosition(float3 position, float3 last_position)
{
	for (int v = 0; v < buffersLength[Mesh::vertex] * 3; v += 3)
	{
		vertices[v] -= last_position.x;
		vertices[v] += position.x;

		vertices[v + 1] -= last_position.y;
		vertices[v + 1] += position.y;

		vertices[v + 2] -= last_position.z;
		vertices[v + 2] += position.z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, buffersId[vertex]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * buffersLength[vertex] * 3, vertices, GL_STATIC_DRAW);
}

void Mesh::UpdateScale(float3 scale, float3 last_scale)
{
	for (int v = 0; v < buffersLength[Mesh::vertex] * 3; v += 3)
	{
		vertices[v] /= last_scale.x;
		vertices[v] *= scale.x;

		vertices[v + 1] /= last_scale.y;
		vertices[v + 1] *= scale.y;

		vertices[v + 2] /= last_scale.z;
		vertices[v + 2] *= scale.z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, buffersId[vertex]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * buffersLength[vertex] * 3, vertices, GL_STATIC_DRAW);
}

void Mesh::DrawBoundingBox(bool active)
{
	if (active) {
	
	}
}

void Mesh::SetMaterialColor(float r, float g, float b, float a)
{
	glColor3f(r, g, b);
}


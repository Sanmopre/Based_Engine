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
	Generate_AABB();
	generated_frame_buffers = true;
	}

	glPushMatrix();
	//SetMaterialColor(10, 10, 10);
	DrawBoundingBox(float4x4::identity,show_bounding_box);
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

void Mesh::Generate_AABB()
{
	local_aabb.SetNegativeInfinity();
	local_aabb.Enclose((float3*)vertices, buffersLength[vertex]);
}

void Mesh::RecalculateAABB_OBB(float4x4 transform)
{
	obb = local_aabb;
	obb.Transform(transform);

	global_aabb.SetNegativeInfinity();
	global_aabb.Enclose(obb);
}

void Mesh::DrawBoundingBox(float4x4 transform, bool active)
{
	if (active) {
		float3 corners[8];
		glColor4f(255, 255, 0, 255);
		local_aabb.GetCornerPoints(corners);

		glPushMatrix();
		glMultMatrixf((float*)&transform.Transposed());

		glBegin(GL_LINES);

		//Between-planes right
		glVertex3fv((GLfloat*)&corners[1]);
		glVertex3fv((GLfloat*)&corners[5]);
		glVertex3fv((GLfloat*)&corners[7]);
		glVertex3fv((GLfloat*)&corners[3]);

		//Between-planes left
		glVertex3fv((GLfloat*)&corners[4]);
		glVertex3fv((GLfloat*)&corners[0]);
		glVertex3fv((GLfloat*)&corners[2]);
		glVertex3fv((GLfloat*)&corners[6]);

		//Far plane horizontal
		glVertex3fv((GLfloat*)&corners[5]);
		glVertex3fv((GLfloat*)&corners[4]);
		glVertex3fv((GLfloat*)&corners[6]);
		glVertex3fv((GLfloat*)&corners[7]);

		//Near plane horizontal
		glVertex3fv((GLfloat*)&corners[0]);
		glVertex3fv((GLfloat*)&corners[1]);
		glVertex3fv((GLfloat*)&corners[3]);
		glVertex3fv((GLfloat*)&corners[2]);

		//Near plane vertical
		glVertex3fv((GLfloat*)&corners[1]);
		glVertex3fv((GLfloat*)&corners[3]);
		glVertex3fv((GLfloat*)&corners[0]);
		glVertex3fv((GLfloat*)&corners[2]);

		//Far plane vertical
		glVertex3fv((GLfloat*)&corners[5]);
		glVertex3fv((GLfloat*)&corners[7]);
		glVertex3fv((GLfloat*)&corners[4]);
		glVertex3fv((GLfloat*)&corners[6]);

		glEnd();

		glPopMatrix();
		glColor4f(255, 255, 255, 255);
	}
}

void Mesh::DrawOBB(bool active)
{
	if (active)
	{
		glColor3f(1, 1, 1);
		glLineWidth(1);
		float3* obb_points = nullptr;
		obb.GetCornerPoints(obb_points);

		glBegin(GL_LINES);

		glVertex3f(obb.CornerPoint(0).x, obb.CornerPoint(0).y, obb.CornerPoint(0).z);
		glVertex3f(obb.CornerPoint(1).x, obb.CornerPoint(1).y, obb.CornerPoint(1).z);

		glVertex3f(obb.CornerPoint(0).x, obb.CornerPoint(0).y, obb.CornerPoint(0).z);
		glVertex3f(obb.CornerPoint(4).x, obb.CornerPoint(4).y, obb.CornerPoint(4).z);

		glVertex3f(obb.CornerPoint(0).x, obb.CornerPoint(0).y, obb.CornerPoint(0).z);
		glVertex3f(obb.CornerPoint(2).x, obb.CornerPoint(2).y, obb.CornerPoint(2).z);

		glVertex3f(obb.CornerPoint(2).x, obb.CornerPoint(2).y, obb.CornerPoint(2).z);
		glVertex3f(obb.CornerPoint(3).x, obb.CornerPoint(3).y, obb.CornerPoint(3).z);

		glVertex3f(obb.CornerPoint(1).x, obb.CornerPoint(1).y, obb.CornerPoint(1).z);
		glVertex3f(obb.CornerPoint(5).x, obb.CornerPoint(5).y, obb.CornerPoint(5).z);

		glVertex3f(obb.CornerPoint(1).x, obb.CornerPoint(1).y, obb.CornerPoint(1).z);
		glVertex3f(obb.CornerPoint(3).x, obb.CornerPoint(3).y, obb.CornerPoint(3).z);

		glVertex3f(obb.CornerPoint(4).x, obb.CornerPoint(4).y, obb.CornerPoint(4).z);
		glVertex3f(obb.CornerPoint(5).x, obb.CornerPoint(5).y, obb.CornerPoint(5).z);

		glVertex3f(obb.CornerPoint(4).x, obb.CornerPoint(4).y, obb.CornerPoint(4).z);
		glVertex3f(obb.CornerPoint(6).x, obb.CornerPoint(6).y, obb.CornerPoint(6).z);

		glVertex3f(obb.CornerPoint(2).x, obb.CornerPoint(2).y, obb.CornerPoint(2).z);
		glVertex3f(obb.CornerPoint(6).x, obb.CornerPoint(6).y, obb.CornerPoint(6).z);

		glVertex3f(obb.CornerPoint(5).x, obb.CornerPoint(5).y, obb.CornerPoint(5).z);
		glVertex3f(obb.CornerPoint(7).x, obb.CornerPoint(7).y, obb.CornerPoint(7).z);

		glVertex3f(obb.CornerPoint(6).x, obb.CornerPoint(6).y, obb.CornerPoint(6).z);
		glVertex3f(obb.CornerPoint(7).x, obb.CornerPoint(7).y, obb.CornerPoint(7).z);

		glVertex3f(obb.CornerPoint(3).x, obb.CornerPoint(3).y, obb.CornerPoint(3).z);
		glVertex3f(obb.CornerPoint(7).x, obb.CornerPoint(7).y, obb.CornerPoint(7).z);

		glLineWidth(1);
		glEnd();
	}
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

void Mesh::SetMaterialColor(float r, float g, float b, float a)
{
	glColor3f(r, g, b);
}


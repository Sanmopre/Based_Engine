#include "Mesh.h"
#include <GL/glew.h>


Mesh::Mesh()
{
	GenerateBuffers();
	color = Color(200,200,0 );
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
	//glColor3f(color.r, color.g, color.b);
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
	/*
	glLineWidth(2.0f);

	glBegin(GL_LINES);


	for (uint i = 0, j = 0; i < vertices.size(); i++)
	{
		//VERTEX NORMALS
		glColor4f(1.0f, 0.0f, 1.0f, 1.0f);

		float3 vector = vertices[i].Position;
		float3 normals = vector + vertices[i].Normal * 2;

		glVertex3f(vector.x, vector.y, vector.z); glVertex3f(normals.x, normals.y, normals.z);

		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		j++;

		//FACE NORMALS
		if (j == 3)
		{
			float3 P0 = vertices[i - 2].Position;
			float3 P1 = vertices[i - 1].Position;
			float3 P2 = vertices[i].Position;

			float3 V0 = P0 - P1;
			float3 V1 = P2 - P1;


			float3 N = V1.Cross(V0);
			N.Normalize();


			float3 P = (P0 + P1 + P2) / 3.0;
			float3 normal = P + N * 2;
			glVertex3f(P.x, P.y, P.z); glVertex3f(normal.x, normal.y, normal.z);

			j = 0;
		}
	}

	glEnd();

	glLineWidth(1.0f);*/
}

void Mesh::UpdatePosition(float x, float y, float z)
{
	/*
	for (int v = 0; v < vertices.size(); v++)
	{
		vertices[v].Position.x += x;
		vertices[v].Position.y += y;
		vertices[v].Position.z += z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, idVertex);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
*/
}

void Mesh::UpdateScale(float x, float y, float z)
{
	/*
	for (int v = 0; v < vertices.size(); v++)
	{
		vertices[v].Position.x *= x;
		vertices[v].Position.y *= y;
		vertices[v].Position.z *= z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, idVertex);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
*/
}

#include "MeshClass.h"
#include <GL/glew.h>


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint> indices)
{
	this->vertices = vertices;
	this->indices = indices;

	GenerateBuffers();
	color = Color(200,200,0 );
}

Mesh::~Mesh()
{
	vertices.clear();
	indices.clear();
}

void Mesh::GenerateBuffers()
{
	if (!indices.empty() && !vertices.empty())
	{
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, (GLuint*)&(idVertex));
		glBindBuffer(GL_ARRAY_BUFFER, idVertex);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*)&(idIndex));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndex);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), &indices[0], GL_STATIC_DRAW);

		//vertex
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		
		//normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		
		//texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}
}


void Mesh::Render(bool globalWireMode) const
{
	glPushMatrix();
	glColor3f(color.r, color.g, color.b);
	InnerRender();
	glPopMatrix();
}


void Mesh::InnerRender() const
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}
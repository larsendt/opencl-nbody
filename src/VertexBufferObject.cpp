#include "VertexBufferObject.h"
#include <stdio.h>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

VertexBufferObject::VertexBufferObject()
{
	m_vertexCount = 0;
	m_vbo = 0;
	m_ibo = 0;
}

VertexBufferObject::VertexBufferObject(int vertex_count, Vertex* vertex_data, GLuint* index_data)
{
	setData(vertex_count, vertex_data, index_data);	
}

VertexBufferObject::~VertexBufferObject()
{
	clear();
}

void VertexBufferObject::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(0));
	
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 3));
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glDrawElements(GL_TRIANGLES, m_vertexCount, GL_UNSIGNED_INT, 0);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void VertexBufferObject::clear()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
}

void VertexBufferObject::setData(int vertex_count, Vertex* vertex_data, GLuint* index_data)
{
	m_vertexCount = vertex_count;
	
	printf("%f %f %f\n", vertex_data[0].coord[0], vertex_data[0].coord[1], vertex_data[0].coord[2]);
	printf("%d %d %d\n", index_data[0], index_data[1], index_data[2]);
	
	glGenBuffers(1, &m_vbo); 
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertexCount*sizeof(Vertex), vertex_data, GL_STATIC_DRAW);
	
	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vertexCount*sizeof(GLuint), index_data, GL_STATIC_DRAW);
}

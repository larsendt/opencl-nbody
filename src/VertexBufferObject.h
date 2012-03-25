#ifndef VERTEX_BUFFER_OBJECT_H
#define VERTEX_BUFFER_OBJECT_H

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

struct Vertex
{	
	float coord[3];
	float normal[3];
};

class VertexBufferObject
{
	public:
		VertexBufferObject();
		VertexBufferObject(int vertex_count, Vertex* vertex_data, GLuint* index_data);
		~VertexBufferObject();
		
		void setData(int vertex_count, Vertex* vertex_data, GLuint* index_data);
		void draw();
		void clear();
		
	private:
		GLuint m_vbo;
		GLuint m_ibo;
		int m_vertexCount;
};

#endif

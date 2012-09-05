#include "AttributeBuffer.h"


AttributeBuffer::AttributeBuffer(GLenum usage)
{
    m_usage = usage;
    glGenBuffers(1, &m_buffer);
}

AttributeBuffer::~AttributeBuffer()
{
    glDeleteBuffers(1, &m_buffer);
}

void AttributeBuffer::bind(void *data, size_t size)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    glBufferData(GL_ARRAY_BUFFER, size, data, m_usage);
}

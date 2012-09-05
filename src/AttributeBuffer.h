#ifndef ATTRIBUTE_BUFFER_H
#define ATTRIBUTE_BUFFER_H

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>

class AttributeBuffer {
    public:
        AttributeBuffer(GLenum usage);

        void bind(void *data, size_t size);

    private:
        GLuint m_buffer;
        GLenum m_usage;
};

#endif

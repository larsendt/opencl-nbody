#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>

class FrameBuffer
{
    public:
        FrameBuffer(int width, int height);
        void bind();
        void release();
        void resize(int width, int height);
        GLuint texture();
        
    protected:
        void deleteBuffers();
        void createBuffers();
    
    private:
        GLuint m_texture;
        GLuint m_depthBuffer;
        GLuint m_frameBuffer;
        int m_width;
        int m_height;
};

#endif

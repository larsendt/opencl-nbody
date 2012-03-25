#include "FrameBuffer.h"
#include <stdio.h>

FrameBuffer::FrameBuffer(int width, int height)
{
    m_width = width;
    m_height = height;
    createBuffers();
}

void FrameBuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBuffer::release()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::resize(int width, int height)
{
    m_width = width;
    m_height = height;
    deleteBuffers();
    createBuffers();
}

GLuint FrameBuffer::texture()
{
    return m_texture;
}

//----------------------------------
//            Protected
//----------------------------------

void FrameBuffer::createBuffers()
{
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glGenRenderbuffers(1, &m_depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenFramebuffers(1, &m_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);

	GLenum fbo_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(fbo_status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Frame buffer cannot be generated!\n");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::deleteBuffers()
{
    glDeleteTextures(1, &m_texture);
	glDeleteRenderbuffers(1, &m_depthBuffer);
	glDeleteFramebuffers(1, &m_frameBuffer);
}

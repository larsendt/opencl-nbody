#ifndef SHADER_H
#define SHADER_H

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>
#include <string>

class Shader
{
    public:
        Shader(const char* vert_path, const char* frag_path, const char* geom_path = NULL);
        void bind();
        void release();

        void vertexAttribPointer(const char* name, int size, GLenum type);

        void setUniform1f(const char* name, float value);
        void setUniform2f(const char* name, float v1, float v2);
        void setUniform3f(const char* name, float v1, float v2, float v3);
        void setUniform4f(const char* name, float v1, float v2, float v3, float v4);
        
        void setUniform1i(const char* name, int value);
        void setUniform2i(const char* name, int v1, int v2);
        void setUniform3i(const char* name, int v1, int v2, int v3);
        void setUniform4i(const char* name, int v1, int v2, int v3, int v4);
        
        void setUniformMatrix4fv(const char* name, float* value);

    protected:
        GLuint compileShader(GLenum shader_type, const char* file_path);
        GLuint linkProgram(GLuint vert_handle, GLuint frag_handle, GLuint geom_handle);
        void printShaderLog(GLuint shader_handle, const char* shader_path);
        void printProgramLog(GLuint program_handle);
        
    private:
        GLuint m_program;
};

#endif

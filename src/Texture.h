#ifndef TEXTURE_H
#define TEXTURE_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <GL/glu.h>
class Texture
{
    public:
        enum ScalingMode {
            NEAREST,
            LINEAR
        };
        
        static GLuint loadTexture(std::string img_path, ScalingMode mode);
        static GLuint loadFromMemory(int width, int height, unsigned char* data, ScalingMode mode);
        static void deleteTexture(GLuint texture_handle);
        
    private:
        Texture();
};

#endif

#ifndef TEXTURE_H
#define TEXTURE_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class Texture
{
    public:
        static GLuint loadTexture(std::string img_path);
        static GLuint loadFromMemory(int width, int height, unsigned char* data);
        static void deleteTexture(GLuint texture_handle);
        
    private:
        Texture();
};

#endif

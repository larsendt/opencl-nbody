#ifndef GLENGINE_H
#define GLENGINE_H

#define GL_GLEXT_PROTOTYPES
#include <SFML/Window.hpp>
#include <vector>
#include "OCLParticleEngine.h"
#include "Shader.h"
#include "AttributeBuffer.h"

#define GLM_SWIZZLE
#include "glm/glm.hpp"

class GLEngine
{
	public:
		GLEngine(int argc, char** argv);
		~GLEngine();
		
		void initGL(int argc, char** argv);
		void drawScene();
		void update();
		void resize(int width, int height);
		int begin();
		
	private:
        glm::vec3 windowCoords(int x, int y); 

        sf::Window *m_window;
		sf::Clock *m_clock;
		OCLParticleEngine *m_particles;
		Shader *m_particleShader;
		GLuint m_texture;
        AttributeBuffer *m_velocityBuffer;
		
		float m_updateRate;
        float m_timeSpeed;
		float m_aspectRatio;
		int m_screenWidth;
		int m_screenHeight;
		float m_rotation;
		int m_mouseLastX;
		int m_mouseLastY;
        float m_mouseRotX;
        float m_mouseRotY;
        float m_scale;
};

#endif

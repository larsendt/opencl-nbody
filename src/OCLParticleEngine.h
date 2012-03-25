#ifndef OCL_PARTICLE_ENGINE_H
#define OCL_PARTICLE_ENGINE_H

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include "OCLKernel.h"

struct Particle 
{
	float x;
	float y;
	float z;
};

class OCLParticleEngine
{
	public:
		OCLParticleEngine(int nparticles);
		~OCLParticleEngine();
		void draw();
		void update(float multiplier);
		
	private:
		GLuint m_vbo;
		GLuint m_ibo;
		int m_numVertices;
		float m_time;
		
		OCLKernel* m_kernel;
		Particle* m_particleArray;
		Particle* m_velocityArray;
		Particle* m_traceArray;
		

};

#endif

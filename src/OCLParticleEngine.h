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

        Particle *velocityData() { return m_velocityArray; }
        float *massData() { return m_massArray; }
        Particle *positionData() { return m_particleArray; }
        GLuint count() { return m_numVertices; }

	private:
        // get a normally-distributed random number
        float normalRandom();

		GLuint m_vbo;
		GLuint m_ibo;
		int m_numVertices;
		float m_time;

		OCLKernel* m_kernel;
		Particle* m_particleArray;
		Particle* m_velocityArray;
	    float* m_massArray;	
};

#endif

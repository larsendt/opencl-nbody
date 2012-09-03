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
		void setExternalForce(float x, float y, float z, float strength);

        Particle *velocityData() { return m_velocityArray; }
        GLuint count() { return m_numVertices; }

	private:
		GLuint m_vbo;
		GLuint m_ibo;
		int m_numVertices;
		float m_time;
        float m_externalForcePos[4];
        float m_externalForceStrength;

		OCLKernel* m_kernel;
		Particle* m_particleArray;
		Particle* m_velocityArray;
	    float* m_massArray;	
};

#endif

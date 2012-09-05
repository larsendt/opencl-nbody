#include "OCLParticleEngine.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

OCLParticleEngine::OCLParticleEngine(int nparticles)
{
	srand(time(NULL));
	GLushort* indexArray;

	m_numVertices = nparticles;
	m_particleArray = new Particle[m_numVertices];
	m_velocityArray = new Particle[m_numVertices];
    m_massArray = new float[m_numVertices];
	indexArray = new GLushort[m_numVertices];

	for(int i = 0; i < m_numVertices; i++)
	{
		Particle p;
		float theta = 2*3.14159*(normalRandom());
		float phi = 2*3.14159*(normalRandom());
		float radius = normalRandom();
		
		p.x = cos(theta) * sin(phi) * radius;
		p.y = cos(phi) * radius * 0.25;
		p.z = sin(theta) * sin(phi) * radius;
		
		m_particleArray[i] = p;
		indexArray[i] = i;

		const float s = 500.0;
        p.x = normalRandom() / s; 
		p.y = normalRandom() / s;
		p.z = normalRandom() / s;
		
		m_velocityArray[i] = p;

        float base_m = 100.0;
        m_massArray[i] = base_m * fabs(normalRandom());
	}

	glGenBuffers(1, &m_vbo); 
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_numVertices*sizeof(Particle), m_particleArray, GL_DYNAMIC_DRAW);
	
	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numVertices*sizeof(GLushort), indexArray, GL_STATIC_DRAW);
	
	delete[] indexArray;
	
	m_kernel = new OCLKernel("kernels/nbody.cl", "nbody_move");
	
	m_time = 0.0;
}

OCLParticleEngine::~OCLParticleEngine()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
    delete m_kernel;
    delete[] m_particleArray;
    delete[] m_massArray;
    delete[] m_velocityArray;
}

void OCLParticleEngine::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Particle), BUFFER_OFFSET(0));
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glDrawElements(GL_POINTS, m_numVertices, GL_UNSIGNED_SHORT, 0);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	
}

void OCLParticleEngine::update(float multiplier)
{
    OCLArgumentArray array;

    array.appendBufferArgument(m_particleArray, m_numVertices*sizeof(*m_particleArray));
    array.appendBufferArgument(m_velocityArray, m_numVertices*sizeof(*m_velocityArray));
    array.appendBufferArgument(m_massArray, m_numVertices*sizeof(*m_massArray));
    array.appendArgument(&m_numVertices, sizeof(m_numVertices));
    
	m_kernel->run(array, m_numVertices, 1);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, (GLintptr)0, m_numVertices*sizeof(Particle), m_particleArray);
	glBindBuffer(GL_ARRAY_BUFFER, 0);	
	
}

//-------------------------------
// Private Methods
//-------------------------------

// Box-Muller Transform
// http://en.wikipedia.org/wiki/Box_Muller_transform
//
// returns normally distributed random variables with
// a standard deviation of 1
float OCLParticleEngine::normalRandom()
{
    float u = (float)rand()/RAND_MAX;
    float v = (float)rand()/RAND_MAX;

    return sqrt(-2 * log(u)) * cos(2 * 3.1415926535 * v);
}





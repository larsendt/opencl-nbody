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

    m_externalForcePos[0] = 0;
    m_externalForcePos[1] = 0;
    m_externalForcePos[2] = 0;
    m_externalForcePos[3] = 0;
    m_externalForceStrength = 0;
	
	m_numVertices = nparticles;
	m_particleArray = new Particle[m_numVertices];
	m_velocityArray = new Particle[m_numVertices];
    m_massArray = new float[m_numVertices];
	indexArray = new GLushort[m_numVertices];

	for(int i = 0; i < m_numVertices; i++)
	{
		Particle p;
		float theta = 2*3.14159*((float)rand()/RAND_MAX);
		float phi = 2*3.14159*((float)rand()/RAND_MAX);
		float radius = (float)rand()/RAND_MAX;
		
		p.x = cos(theta) * sin(phi) * radius;
		p.y = cos(phi) * radius * 0.25;
		p.z = sin(theta) * sin(phi) * radius;
		
		m_particleArray[i] = p;
		indexArray[i] = i;

		p.x = 0; //p.x/400;
		p.y = 0; //-p.y/400; 
		p.z = 0;
		
		m_velocityArray[i] = p;

        float base_m = 1000.0;
        float dev = 10.0;
        int randmass = (base_m * dev * 2 * (float)rand()/RAND_MAX) - (base_m/2);
        m_massArray[i] = base_m + randmass;
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
}

void OCLParticleEngine::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Particle), BUFFER_OFFSET(0));
	//glEnableClientState(GL_COLOR_ARRAY);
	//glColorPointer(4, GL_FLOAT, sizeof(Particle), BUFFER_OFFSET(12));
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glDrawElements(GL_POINTS, m_numVertices, GL_UNSIGNED_SHORT, 0);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	
}

void OCLParticleEngine::update(float multiplier)
{
	OCLArgument args[6];
	OCLArgument buffers[3];
	
	OCLArgument a;
	a.data = m_particleArray;
	a.byte_size = m_numVertices * sizeof(Particle);
	a.is_buffer = true;
	a.buffer_index = 0;
	a.buffer_type = READ_WRITE;
	args[0] = a;
	buffers[0] = a;

	a.data = m_velocityArray;
	a.byte_size = m_numVertices * sizeof(Particle);
	a.is_buffer = true;
	a.buffer_index = 1;
	a.buffer_type = READ_WRITE;
	args[1] = a;
	buffers[1] = a;

    a.data = m_massArray;
    a.byte_size = m_numVertices * sizeof(float);
    a.is_buffer = true;
    a.buffer_index = 2;
    a.buffer_type = READ_WRITE;
    args[2] = a;
    buffers[2] = a;

	a.data = &m_numVertices;
	a.byte_size = sizeof(m_numVertices);
	a.is_buffer = false;
	args[3] = a;

    a.data = m_externalForcePos;
    a.byte_size = sizeof(m_externalForcePos);
    a.is_buffer = false;
    args[4] = a;

    a.data = &m_externalForceStrength;
    a.byte_size = sizeof(m_externalForceStrength);
    a.is_buffer = false;
    args[5] = a;

	m_kernel->run(6, args, 3, buffers, m_numVertices, 1);

    m_externalForceStrength = 0;

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, (GLintptr)0, m_numVertices*sizeof(Particle), m_particleArray);
	glBindBuffer(GL_ARRAY_BUFFER, 0);	
	
}

void OCLParticleEngine::setExternalForce(float x, float y, float z, float strength)
{
    m_externalForcePos[0] = x; 
    m_externalForcePos[1] = y; 
    m_externalForcePos[2] = z; 
    m_externalForcePos[3] = 0; 

    m_externalForceStrength = strength;
}



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

		p.x = p.x/400;
		p.y = -p.y/400; 
		p.z = 0;
		
		m_velocityArray[i] = p;
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
	OCLArgument args[3];
	OCLArgument buffers[2];
	
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

	a.data = &m_numVertices;
	a.byte_size = sizeof(m_numVertices);
	a.is_buffer = false;
	args[2] = a;
	
	m_kernel->run(3, args, 2, buffers, m_numVertices, 1);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, (GLintptr)0, m_numVertices*sizeof(Particle), m_particleArray);
	glBindBuffer(GL_ARRAY_BUFFER, 0);	
	
}





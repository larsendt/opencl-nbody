#include "GLEngine.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Texture.h"

#define MAX_ROTATION_SPEED 0.1
#define IDLE_TRIGGER_TIME 100.0

GLEngine::GLEngine(int argc, char** argv)
{
	(void) argc;
	(void) argv;
	
	fprintf(stdout, "Initializing GLEngine\n");
	initGL(argc, argv);
}

GLEngine::~GLEngine()
{
	delete m_window;
	delete m_clock;
    delete m_particles;
    delete m_particleShader;
    delete m_massBuffer;
    delete m_velocityBuffer;
    delete m_vertexBuffer;
}

void GLEngine::initGL(int argc, char** argv)
{
	m_window = new sf::Window(sf::VideoMode(1300, 700), "GLEngine");
	m_clock = new sf::Clock();
	m_screenWidth = m_window->GetWidth();
	m_screenHeight = m_window->GetHeight();
	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	
    int numparticles = 1024;
    if(argc > 1 && atoi(argv[1]) > 0)
        numparticles = atoi(argv[1]);

	m_particles = new OCLParticleEngine(numparticles);
	m_rotation = 0;
	m_mouseRotX = 0;
	m_mouseRotY = 0;
	m_mouseLastX = 0;
	m_mouseLastY = 0;
	m_scale = 1.0;
    m_timeSpeed = 0.2;
    m_rotationSpeed = 0.0;
    m_idleTime = 0.0;

	m_particleShader = new Shader("shaders/nbody.vert", "shaders/nbody.frag", "shaders/nbody.geom");
	m_texture = Texture::loadTexture("resources/star.bmp", Texture::LINEAR);
    m_velocityBuffer = new AttributeBuffer(GL_DYNAMIC_DRAW);
    m_massBuffer = new AttributeBuffer(GL_STATIC_DRAW);
    m_vertexBuffer = new AttributeBuffer(GL_DYNAMIC_DRAW);
	
	m_updateRate = 1.0/60.0;
	resize(m_screenWidth, m_screenHeight);
}

int GLEngine::begin()
{
	while(m_window->IsOpened())
	{
		sf::Event Event;
		while(m_window->GetEvent(Event))
		{
			if(Event.Type == sf::Event::Closed)
			{
				m_window->Close();
				return 0;
			}
			else if(Event.Type == sf::Event::MouseButtonPressed)
			{
			    m_mouseLastX = Event.MouseButton.X;
			    m_mouseLastY = Event.MouseButton.Y;
		    }
		    else if(Event.Type == sf::Event::MouseMoved && m_window->GetInput().IsMouseButtonDown(sf::Mouse::Left))
		    {
		        int dx = Event.MouseMove.Y - m_mouseLastY; 
		        int dy = Event.MouseMove.X - m_mouseLastX;
		        m_mouseRotX += dx;
		        m_mouseRotY += dy;
		        m_mouseLastX = Event.MouseMove.X;
		        m_mouseLastY = Event.MouseMove.Y;
                m_idleTime = 0.0;
                m_rotationSpeed = 0.0;
	        }
	        else if(Event.Type == sf::Event::MouseWheelMoved)
	        {
	            m_scale += (float)Event.MouseWheel.Delta / 10.0;
            }
			else if(Event.Type == sf::Event::KeyPressed)
			{
				switch(Event.Key.Code)
				{
					case sf::Key::Escape:
						m_window->Close();
						return 0;
                    case sf::Key::Up:
                        m_timeSpeed += 0.05;
                        break;
                    case sf::Key::Down:
                        m_timeSpeed -= 0.05;
                        break;
                    default:
						break;
				}
			}
			else if(Event.Type == sf::Event::Resized)
			{
				resize(Event.Size.Width, Event.Size.Height);
			}
		}
		
		update();
		drawScene();
		m_window->Display();
	}
	return 0;
}


void GLEngine::drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	
    gluLookAt(0, 0, 2, 0, 0, 0, 0, 1, 0);	
	
    glRotatef(m_mouseRotX, 1, 0, 0);
    glRotatef(m_mouseRotY, 0, 1, 0);
    glScalef(m_scale, m_scale, m_scale);
	
	glScalef(0.7, 0.7, 0.7);
	glRotatef(m_rotation, 0, 1, 0);
	glRotatef(30, 1, 0, 0);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	
	m_particleShader->bind();
	m_particleShader->setUniform1i("star", 0);

    // pass the velocity data into the shader for coloring
    Particle *velocities = m_particles->velocityData();
    Particle *vertices = m_particles->positionData();
    float *masses = m_particles->massData();
    GLuint particle_count = m_particles->count();

    m_velocityBuffer->bind(velocities, particle_count*sizeof(*velocities));
    m_particleShader->vertexAttribPointer("velocity", 3, GL_FLOAT);

    m_massBuffer->bind(masses, particle_count*sizeof(*masses));
    m_particleShader->vertexAttribPointer("mass", 1, GL_FLOAT);

    m_vertexBuffer->bind(vertices, particle_count*sizeof(*vertices));
    m_particleShader->vertexAttribPointer("vertex", 3, GL_FLOAT);
        
    glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);

	m_particles->draw();
	
	glDisable(GL_BLEND);
	
	m_particleShader->release();
}


void GLEngine::update()
{
	float time = m_clock->GetElapsedTime();
	float multiplier = m_timeSpeed;
    m_idleTime += time;

	if(time < m_updateRate)
	{
		return;
	}
	else if(time > m_updateRate)
	{
		multiplier *= time / m_updateRate;
	}

    if(m_idleTime > IDLE_TRIGGER_TIME)
    {
        m_rotationSpeed += 0.001;
        m_mouseRotY += m_rotationSpeed;
        if(m_rotationSpeed > MAX_ROTATION_SPEED)
        {
            m_rotationSpeed = MAX_ROTATION_SPEED;
        }
    }
    
	m_particles->update(multiplier);
	m_clock->Reset();
}


void GLEngine::resize(int width, int height)
{
	printf("Viewport: %d %d\n", width, height);
	m_aspectRatio = (height>0) ? (GLfloat)width/height : 1;
	m_screenWidth = width;
	m_screenHeight = height;
	
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, m_aspectRatio, 0.05, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

glm::vec3 GLEngine::windowCoords(int x, int y)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;
 
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );
 
    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
 
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
 
    return glm::vec3(posX, posY, posZ);
}

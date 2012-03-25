#include "GLEngine.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Texture.h"


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
}

void GLEngine::initGL(int argc, char** argv)
{
	m_window = new sf::Window(sf::VideoMode(800, 800), "GLEngine");
	m_clock = new sf::Clock();
	m_screenWidth = m_window->GetWidth();
	m_screenHeight = m_window->GetHeight();
	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	
	m_particles = new OCLParticleEngine(2048);
	m_rotation = 0;
	m_mouseRotX = 0;
	m_mouseRotY = 0;
	m_mouseLastX = 0;
	m_mouseLastY = 0;
	m_scale = 1.0;

	m_particleShader = new Shader("shaders/nbody.vert", "shaders/nbody.frag", "shaders/nbody.geom");
	m_texture = Texture::loadTexture("resources/star.bmp");
	
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
	
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);

	m_particles->draw();
	
	glDisable(GL_BLEND);
	
	m_particleShader->release();
}


void GLEngine::update()
{
	float time = m_clock->GetElapsedTime();
	float multiplier = 1.0;
		
	if(time < m_updateRate)
	{
		return;
	}
	else if(time > m_updateRate)
	{
		multiplier = time / m_updateRate;
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


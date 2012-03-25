#include "Light.h"
#include <GL/glu.h>

Light::Light()
{
    m_position = vec4(0.0);    
    m_diffuse = vec4(0.7);
    m_specular = vec4(0.2);
    m_ambient = vec4(0.3);
    
    m_visible = true;
	m_modelView = new float[16];
}

void Light::setPosition(vec4 pos)
{
    m_position = pos;
}

void Light::setDiffuse(vec4 color)
{
    m_diffuse = color;
}

void Light::setSpecular(vec4 color)
{
    m_specular = color;
}

void Light::setAmbient(vec4 color)
{
    m_ambient = color;
}

void Light::setVisible(bool visible)
{
    m_visible = visible;
}

void Light::enlighten()
{
    glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, m_ambient.pointer());
	glLightfv(GL_LIGHT0, GL_DIFFUSE, m_diffuse.pointer());
	glLightfv(GL_LIGHT0, GL_SPECULAR, m_specular.pointer());
	glLightfv(GL_LIGHT0, GL_POSITION, m_position.pointer());
	
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(m_position.x(), m_position.y(), m_position.z());
    glGetFloatv(GL_MODELVIEW_MATRIX, m_modelView);
    
    if(m_visible)
    {   
        GLUquadricObj* sphere = gluNewQuadric();
	    gluQuadricDrawStyle(sphere, GLU_FILL);
	    glColor3f(1.0, 1.0, 1.0);
	    gluSphere(sphere, 0.4, 50, 50);
	}
	
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
}

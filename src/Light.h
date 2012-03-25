#ifndef LIGHT_H
#define LIGHT_H

#include <GL/gl.h>
#include <GL/glu.h>
#include "Vector.h"

class Light
{
    public:
        Light();
        void enlighten();
        void setPosition(vec4 pos);
        void setVisible(bool visible);
		void setDiffuse(vec4 color);
		void setSpecular(vec4 color);
		void setAmbient(vec4 color);
        bool visible() { return m_visible; }
        float* modelViewMatrix() { return m_modelView; }
   
    private:
        vec4 m_position;
        vec4 m_diffuse;
        vec4 m_specular;
        vec4 m_ambient;
        bool m_visible;        
        float* m_modelView;
};

#endif

//
//  NBody Vertex shader
//

void main()
{
   //  Remember the color
   gl_FrontColor = gl_Color;
   //  Defer all transformations to geometry shader
   gl_Position   = gl_Vertex;
}

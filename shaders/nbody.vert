//
//  NBody Vertex shader
//

attribute vec3 velocity;

void main()
{
    float v = length(velocity) * 100.0;
    vec4 fast = vec4(1.0, 0.4, 0.4, 1.0);
    vec4 slow = vec4(0.4, 0.4, 1.0, 1.0);
    gl_FrontColor = mix(slow, fast, v);
    //  Defer all transformations to geometry shader
    gl_Position   = gl_Vertex;
}

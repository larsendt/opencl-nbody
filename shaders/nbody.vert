//
//  NBody Vertex shader
//

in vec3 velocity;

void main()
{
    float v = pow(length(velocity) * 80.0, 2.0);
    vec4 fast = vec4(0.0, 1.0, 1.0, 1.0);
    vec4 slow = vec4(0.0, 0.0, 0.2, 1.0);
    gl_FrontColor = mix(slow, fast, v);
    //  Defer all transformations to geometry shader
    gl_Position = gl_Vertex;
}

#version 130
//
//  NBody Vertex shader
//

in vec3 velocity;
in vec3 vertex;
in float mass;

void main()
{
    float v = pow(length(velocity) * 80.0, 2.0);
    vec4 fast = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 slow = vec4(0.5, 0.3, 0.0, 1.0);
    gl_FrontColor = mix(slow, fast, mass/100.0);
    gl_Position = vec4(vertex, 1.0);
}

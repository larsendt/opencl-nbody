//
//  Nbody fragment shader
//
uniform sampler2D star;

void main()
{
   gl_FragColor = texture2D(star,gl_TexCoord[0].st) * gl_Color;
}

//#version 120
varying vec3 normal;
varying vec3 light;
varying vec4 position;
uniform vec3 _WorldSpaceCameraPos;
uniform mat4 M,V,P;
uniform mat4 pvm;
uniform float shininess;
attribute vec3 norm;

void main(void)
{
    gl_Position =  pvm * gl_Vertex;
//    gl_Position = gl_Vertex * pvm;
//    gl_Position =  gl_ModelViewProjectionMatrix * gl_Vertex;
    normal = gl_Normal;

    gl_FrontColor = gl_Color;
    gl_BackColor = gl_Color;
}

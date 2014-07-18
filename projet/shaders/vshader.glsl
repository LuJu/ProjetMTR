
varying vec3 normal;
varying vec3 light;
varying vec4 position;
uniform vec3 _WorldSpaceCameraPos;
uniform mat4 M,V,P;
uniform mat4 pvm;
//uniform vec3 view_direction;
attribute vec3 norm;
uniform float intensity;

uniform bool texture_activated;

void main(void)
{
  vec3 lightDir = vec3(0.0,0.0,3.0);
  vec4 v = vec4(gl_Vertex);
  vec4 color;
  vec3 view_direction;

//  intensity = dot(lightDir,gl_Normal);
//  color = gl_Color;
  color = gl_Color * 0.5f;
//  view_direction = vec3(V[3]);
////  if (dot(vec3(view_direction.xyz), gl_Normal) < 1.0) {
//    if (dot(vec3(view_direction.xyz), gl_Normal) < mix(0.1, 0.4, max(0.0, dot(vec3(gl_Normal.xyz), lightDir)))) {
////    color = vec4(1.0) /** vec4(0.0)*/;
//    color = vec4(1.0,0.1,0.1,0.1);
//      }

  gl_TexCoord[0] = gl_MultiTexCoord0;
  gl_FrontColor = color;
  gl_BackColor = color;
  gl_Position =  pvm * v;

}

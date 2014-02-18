//#version 120
varying vec3 normal;
varying vec3 light;
varying vec4 position;
uniform vec3 _WorldSpaceCameraPos;
uniform mat4 M,V,P;
uniform mat4 pvm;
uniform float shininess;
attribute vec3 norm;
varying float intensity;


void main(void)
{
  bool test = false;
  vec3 lightDir = vec3(1.0,0.0,0.0);

  if (test){

    vec4 v = vec4(gl_Vertex);
//    v.z = sin(5.0*v.y )*0.25;
    gl_Position =  pvm * v;
    intensity = dot(lightDir,gl_Normal);

    vec4 color;

    if (intensity > 0.5)
        color = vec4(0.0,1.0,0.0,1.0);
      else
        color = vec4(1.0,0.0,0.0,1.0);
    color = color * intensity;

//      if (intensity > 0.95)
//        color = vec4(1.0,0.5,0.5,1.0);
//      else if (intensity > 0.5)
//        color = vec4(0.6,0.3,0.3,1.0);
//      else if (intensity > 0.25)
//        color = vec4(0.4,0.2,0.2,1.0);
//      else
//        color = vec4(0.2,0.1,0.1,1.0);

    gl_FrontColor = color;
    //    gl_BackColor = vec4(0,0,0,1);
    //    gl_FrontColor = gl_Color;
    gl_BackColor = gl_Color;

  } else {
  //    gl_Position = gl_Vertex * pvm;
  //    gl_Position =  gl_ModelViewProjectionMatrix * gl_Vertex;
    normal = gl_Normal;

    gl_FrontColor = gl_Color;
  //    gl_BackColor = vec4(0,0,0,1);
  //    gl_FrontColor = gl_Color;
    gl_BackColor = gl_Color;
    gl_Position =  pvm * gl_Vertex;
  }

}

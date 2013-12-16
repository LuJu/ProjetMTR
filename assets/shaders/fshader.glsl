uniform sampler2D Texture0;
varying vec3 light;
varying vec3 normal;
varying vec3 lightDirection;
varying vec4 position;
varying vec3 norm;
//uniform float factor;
uniform float shininess;
uniform vec3 _color;


vec4 CelShading ( vec4 color ){
    float intensity = dot( light , normalize(normal) );

    float factor = 1.0 ;
    if      ( intensity > 0.95 ) factor = 1.0;
    else if ( intensity > 0.90 ) factor = 0.9;
    else if ( intensity > 0.70 ) factor = 0.8;
    else if ( intensity > 0.60 ) factor = 0.7;
    else if ( intensity > 0.50 ) factor = 0.6;
    else if ( intensity > 0.30 ) factor = 0.5;
    else if ( intensity > 0.20 ) factor = 0.4;
    else factor = 0.0;
    color *= vec4 ( factor, factor, factor, 1.0 );

    return color;


}

void main (void)
{
//    vec3 normalDirection = normalize(normal);
//    vec3 lightDirection = normalize(light);
////    vec3 viewDirection = normalize(_WorldSpaceCameraPos - vec3(position));
//    vec3 camera0 = vec3(0.0,0.0,3.0);
//    vec3 viewDirection = normalize(camera0 - vec3(position));

//    vec4 color = texture2D( Texture0 , vec2( gl_TexCoord[0] ) );
//    color = CelShading(color);
//    vec3 fragmentColor = vec3(1.0-color.x,1.0-color.y,1.0-color.z);

////    if (dot(viewDirection, normalDirection) < mix(0.000000004,
////                                  0.0000,
//////                                  max(0.0, dot(normalDirection, lightDirection)))) {
////                                  dot(normalDirection, lightDirection))) {
////        fragmentColor = vec3(1.0,1.0,1.0) * vec3(0.0,0.0,0.0);
////    }

//    if (dot(viewDirection, normalDirection) < 0.0005 ) {
//        fragmentColor = vec3(1.0,1.0,1.0) * vec3(0.0,0.0,0.0);
//    }

//    color = vec4(fragmentColor,color.w);


//    gl_FragColor = vec4(0.0,0.0,1.0,0.3);
    vec4 col = vec4((gl_Color.rgba)*shininess);
    gl_FragColor = col;
}


varying vec3 normal, eyeVec;
varying vec3 lightDir[NUM_LIGHTS];
varying float dist[NUM_LIGHTS];

attribute vec3 tangent, bitangent;

void main()
{
    vec3 vert = (gl_ModelViewMatrix * gl_Vertex).xyz;
    vec3 n = normalize(gl_NormalMatrix * gl_Normal);
    eyeVec = normalize(-vert);
#ifdef BUMP_MAP
    vec3 t = normalize(gl_NormalMatrix * tangent);
    //vec3 b = cross(n,t);
    vec3 b = normalize(gl_NormalMatrix * bitangent);

    vec3 ev = eyeVec;
	//transform eye vector into tangent space
    eyeVec.x = dot(ev,t);
 	eyeVec.y = dot(ev,b);
    eyeVec.z = dot(ev,n);
#endif

    for (int i = 0; i < NUM_LIGHTS; ++i) {

        if (gl_LightSource[i].position.w == 0.0) {// if directional light
            lightDir[i] = 
                gl_LightSource[i].position.xyz; 
        }
        else { // else assume positional light
            vec3 lv = gl_LightSource[i].position.xyz - vert;
            dist[i] = length(lv);
            lightDir[i] =
                normalize(lv);
        }
#ifdef BUMP_MAP
        // transform lightdir into tangent space
        vec3 ld = lightDir[i];
        ld.x = dot(lightDir[i],t);
        ld.y = dot(lightDir[i],b);
        ld.z = dot(lightDir[i],n);
        lightDir[i] = normalize(ld);
#endif
    }
    gl_TexCoord[0] = gl_MultiTexCoord0; 
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    normal = n;
}

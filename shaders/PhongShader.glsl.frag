varying vec3 normal, eyeVec;
varying vec3 lightDir[NUM_LIGHTS];
varying float dist[NUM_LIGHTS];

uniform sampler2D ambientMap, diffuseMap, specularMap, opacityMap;
uniform sampler2D bumpMap;

//#undef BUMP_MAP
void main (void)
{
#ifdef BUMP_MAP
    vec3 n = texture2D(bumpMap, gl_TexCoord[BUMP_INDEX].st).rgb * vec3(2.0) - vec3(1.0);
#else
    vec3 n = normalize(normal);
#endif  
    
    vec4 color = 
        gl_LightModel.ambient * 
        gl_FrontMaterial.ambient;

#ifdef OPACITY_MAP  
#ifdef OPACITY_INDEX
    color.a = texture2D(opacityMap, gl_TexCoord[OPACITY_INDEX].st).r;

    // weird hack ... should be if (color.a == 0.0)
    if (color.a < .9) 
        discard;
#endif
#endif 



    for (int i = 0; i < NUM_LIGHTS; ++i) {
        float att = 1.0;
        if (gl_LightSource[i].position.w == 1.0) {// if point light
             att /=
                 gl_LightSource[i].constantAttenuation +
                 gl_LightSource[i].linearAttenuation * dist[i] +
                 gl_LightSource[i].quadraticAttenuation * dist[i] * dist[i];
        }

        color +=
#ifdef AMBIENT_MAP
            texture2D(ambientMap, gl_TexCoord[0].st) *
#endif
            gl_LightSource[i].ambient *
            gl_FrontMaterial.ambient;
        
        vec3 l = normalize(lightDir[i]);
        vec3 e = normalize(eyeVec);

        float lambertTerm = dot(n,l);
        if (lambertTerm > 0.0)
            {
                color +=
                    att *
                    gl_LightSource[i].diffuse *
                    gl_FrontMaterial.diffuse *
                    //texture2D(diffuseMap, gl_TexCoord[0].st) *
                    lambertTerm;
              
                vec3 r = reflect(-l, n);
                float specular = pow(max(dot(r, e), 0.0),
                                     gl_FrontMaterial.shininess);
                color +=
                    att *
                    gl_LightSource[i].specular *
#ifdef SPECULAR_MAP
                    texture2D(specularMap, gl_TexCoord[0].st) *
#endif
                    gl_FrontMaterial.specular *
                    specular;
            }
    }
#ifdef DIFFUSE_MAP  
    // Weight the final color with the diffuse map.
    // This resembles the gl fixed function pipeline way.
#ifdef DIFFUSE_INDEX
    color *= texture2D(diffuseMap, gl_TexCoord[DIFFUSE_INDEX].st);
#endif
#endif 

    gl_FragColor = color; 
    //gl_FragColor.rgb = n; 
}


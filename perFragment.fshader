#version 120

varying vec4 Position;
varying vec3 Normal;

struct LightInfo{
    vec4 Position;
    vec3 La; //Intensidad Luz ambiental
    vec3 Ld; //Intensidad Luz difusa
    vec3 Ls; //Intensidad Luz especular
};

uniform LightInfo Lights[2];

struct MaterialInfo{
    vec3 Ka; // Reflectividad Ambiental
    vec3 Kd; // Difusa
    vec3 Ks; // Especular
    float Shininess; // Factor de brillo
};

uniform MaterialInfo Material;

struct FogInfo {
    float maxDist;
    float minDist;
    float density;
    vec4 color;
};

uniform FogInfo Fog;

uniform int Fog_Type;

//out vec4 FragColor;

vec3 ads(int lightIndex, vec4 position, vec3 norm){

	LightInfo l = Lights[lightIndex];

	vec3 n = normalize( norm );
	vec3 s = normalize( vec3(l.Position - position) );
	vec3 v = normalize(vec3(-position));
	vec3 r = reflect( -s, n );
	return	 l.La * Material.Ka +
				l.Ld * Material.Kd * max( dot(s, n), 0.0 ) +
				l.Ls * Material.Ks * pow( max( dot(r,v), 0.0 ), Material.Shininess );
}


void main(){
	vec3 shadeColor = vec3(0.0);
    for(int i = 0; i < 2; i++){
        shadeColor += ads(i,Position,Normal);
    }

    vec3 Color;
    if(Fog_Type != 0){
    	float dist = abs(Position.z);
	    float fogFactor;

	    if(Fog_Type == 1)
	    		fogFactor = (Fog.maxDist - dist) / (Fog.maxDist - Fog.minDist);
	    if(Fog_Type == 2)
	    		fogFactor = exp(-Fog.density * dist);
	    if(Fog_Type == 3)
	    		fogFactor = exp(- pow(Fog.density * dist,2.0));


	    fogFactor = clamp(fogFactor,0.0,1.0);

    	Color = mix(vec3(Fog.color), shadeColor, fogFactor);
    }
    else
    	Color = shadeColor;

	gl_FragColor = vec4(Color, 1.0);
	
}
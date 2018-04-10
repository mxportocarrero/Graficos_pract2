#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 LightIntensity;

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

uniform mat4 ModelViewMatrix;

uniform mat3 NormalMatrix;
uniform mat4 MVP;

uniform int LightingEnabled;
/**
void main(){
	//Convertimos las normales al Camera Coords o Eye Coords
	vec3 tnorm = normalize(NormalMatrix * VertexNormal);
    vec4 eyeCoords = ModelViewMatrix * vec4(VertexPosition,1.0);
    vec3 s = normalize(vec3(Light.Position - eyeCoords));
	

	//Aplicamos la ecuación de luz difusa
	LightIntensity = Light.Ld * Material.Kd * max(dot(s,tnorm),0.0);

	gl_Position = MVP * vec4(VertexPosition,1.0);

}
**/

vec3 ads(int lightIndex, vec4 position, vec3 norm){

    LightInfo l = Lights[lightIndex];

    float dist, att = 1.0;
    if(lightIndex == 2){
        dist = distance(l.Position, position);
        att=1.0/(2.0+0.001*dist+0.0001*dist*dist);
    }

    vec3 s = normalize( vec3(l.Position - position));
    vec3 v = normalize(vec3(-position));
    vec3 r = reflect(-s,norm);

    return att * (l.La * Material.Ka +
            l.Ld * Material.Kd * max(dot(s,norm),0.0) +
            l.Ls * Material.Ks * pow(max(dot(r,v),0.0),Material.Shininess));


}

void main(){
    vec3 eyeNorm = normalize(NormalMatrix * VertexNormal);
    vec4 eyeCoords = ModelViewMatrix * vec4(VertexPosition,1.0);

    // Evaluamos la ecuación de la luz para cada fuente
    vec3 shadeColor = vec3(0.0);
    for(int i = 0; i < 2; i++)
        shadeColor += ads(i,eyeCoords,eyeNorm);

    LightIntensity = shadeColor;

    gl_Position = MVP * vec4(VertexPosition,1.0);

}

//FUNCION PARA UNA SOLA LUZ
/**
void main(){
        vec3 eyeNorm = normalize(NormalMatrix * VertexNormal);
        vec4 eyeCoords = ModelViewMatrix * vec4(VertexPosition,1.0);

        vec3 s = normalize(vec3(Light.Position - eyeCoords));
        vec3 v = normalize(-eyeCoords.xyz);
        vec3 r = reflect(-s,tnorm);

        vec3 ambient = Light.La * Material.Ka;

        float sDotN = max( dot(s,tnorm),0.0);
        
        vec3 diffuse = Light. Ld * Material.Kd * sDotN;
        vec3 spec = vec3(0.0);

        if(sDotN > 0.0)
           spec = Light.Ls * Material.Ks *
                  pow( max( dot(r,v), 0.0 ), Material.Shininess );
        LightIntensity = ambient + diffuse + spec;
    gl_Position = MVP * vec4(VertexPosition,1.0);

}
**/
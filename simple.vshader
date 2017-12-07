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

uniform LightInfo Light;

struct MaterialInfo{
    vec3 Ka; //Reflectividad Ambiental
    vec3 Kd; // Difusa
    vec3 Ks; // Especular
    float Shininess; // Factor de brillo
};

uniform MaterialInfo Material;

uniform mat4 mv;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

uniform int LightingEnabled;

void main(){
        vec3 tnorm = normalize(NormalMatrix * VertexNormal);
        vec4 eyeCoords = mv * vec4(VertexPosition,1.0);

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

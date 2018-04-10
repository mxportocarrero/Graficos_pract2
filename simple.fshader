#version 150

in vec3 LightIntensity;

out vec4 FragColor;

uniform int LightingEnabled;

void main(){

        FragColor = vec4(LightIntensity,1.0);

        //FragColor = vec4(1.0,0.85,0.0,1.0);
}

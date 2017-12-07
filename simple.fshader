#version 410

in vec3 LightIntensity;

layout(location = 0) out vec4 FragColor;

uniform int LightingEnabled;

void main(){
        FragColor = vec4(1.0 * LightIntensity.r,0.84* LightIntensity.r,0.0,1.0);
        //FragColor = vec4(1.0,0.85,0.0,1.0);
}

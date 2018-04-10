#version 120

varying vec3 LightIntensity;



uniform int LightingEnabled;

void main(){

        gl_FragColor = vec4(LightIntensity,1.0);

        //FragColor = vec4(1.0,0.85,0.0,1.0);
}

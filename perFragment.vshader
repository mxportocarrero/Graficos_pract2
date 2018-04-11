#version 120
attribute vec3 VertexPosition;
attribute vec3 VertexNormal;

varying vec4 Position;
varying vec3 Normal;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;

uniform mat4 MVP;

uniform int isLightingEnabled;

void main(){
	Normal = normalize( NormalMatrix * VertexNormal);
	Position = ModelViewMatrix * vec4(VertexPosition,1.0);
	gl_Position = MVP * vec4(VertexPosition,1.0);
}
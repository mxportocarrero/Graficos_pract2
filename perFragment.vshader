#version 150
in vec3 VertexPosition;
in vec3 VertexNormal;

out vec4 Position;
out vec3 Normal;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;

uniform mat4 MVP;

void main(){
	Normal = normalize( NormalMatrix * VertexNormal);
	Position = ModelViewMatrix * vec4(VertexPosition,1.0);
	gl_Position = MVP * vec4(VertexPosition,1.0);
}
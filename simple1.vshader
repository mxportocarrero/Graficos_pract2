#version 150 
in vec3 vertexPosition_modelspace;

void main(){

	gl_Position = vec4(vertexPosition_modelspace,1);
	gl_Position.w = 1.0;
}

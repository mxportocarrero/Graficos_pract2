#version 150

in vec3 VertexColor;
in vec3 VertexPosition;

uniform mat4 VP;

out vec3 Color;

void main(){
    Color = VertexColor;
    gl_Position = VP * vec4(VertexPosition,1);
}

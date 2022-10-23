#version 330 core
layout (location=0) in vec2 vertex_pos;
layout (location=1) in vec2 texture_pos;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 projection;
void main(){
	gl_Position = projection * model * vec4(vertex_pos,0,1);
	texCoord=texture_pos;
}
#version 330 core
in vec2 texCoord;
out vec4 color;
uniform sampler2D atexture;
void main(){
	//color=vec4(0.8594,0.8828,0.9375,1);
	color=texture(atexture,texCoord);
}
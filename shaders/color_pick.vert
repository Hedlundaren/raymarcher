#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

out vec3 newPos;
out vec2 texCoord;

void main(void)
{
	newPos = position;
	texCoord = uv;

	gl_Position = vec4(position, 1.0); 
}

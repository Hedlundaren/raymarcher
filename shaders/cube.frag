#version 430


out vec4 outColor;

in vec3 newPos;
in vec3 newNormal;
in vec2 texCoord;
uniform float time;


void main()
{

	outColor = vec4(vec3(0.5), 1.0);
} 
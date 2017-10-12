#version 430


out vec4 outColor;

in vec3 oldPos;
in vec3 newNormal;
in vec2 texCoord;
uniform float time;
uniform vec3 camPos;


void main()
{

	float dist = length(camPos - oldPos);
	outColor = vec4(oldPos, 1.0);
} 
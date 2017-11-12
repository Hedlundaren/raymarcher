#version 430


out vec4 outColor;

in vec3 oldPos;
in vec3 newNormal;
in vec2 texCoord;
uniform float time;


void main()
{


	outColor = vec4(oldPos, 1.0);

} 
#version 430 core

in vec3 newPos;
in vec2 texCoord;

out vec4 outColor;

uniform sampler2D screenTexture;

	
void main(void)
{
	vec4 v = texture(screenTexture, texCoord);	
	outColor = vec4(0, 0.6, 0, 1);
}

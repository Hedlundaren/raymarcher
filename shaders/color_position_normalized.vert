#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec3 oldPos;
out vec3 newNormal;
out vec2 texCoord;

uniform mat4 MV;
uniform mat4 P;
uniform vec2 yzRelativex;

void main() {
	
	vec3 newPos = (position - vec3(0.5));
	oldPos = position;
	// newPos.z *= 156.0/256.0;
	newNormal = normal;
	texCoord = uv;

	newPos.y *= yzRelativex.x;
	newPos.z *= yzRelativex.y;

    gl_Position = P * MV * vec4(newPos, 1.0f);
}
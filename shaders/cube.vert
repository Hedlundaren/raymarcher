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

float cubeSize = 		1.0;
float borderLeft = 	0.0;
float borderRight = 	cubeSize;
float borderDown = 	0.0;
float borderUp = 		cubeSize;
float borderNear = 	0.0;
float borderFar = 	cubeSize;

bool insideUnitCube(vec3 p){
	if(
		p.x < borderLeft ||
		p.x > borderRight ||
		p.y < borderDown ||
		p.y > borderUp ||
		p.z < borderNear ||
		p.z > borderFar
	){
		return false;
	}else{
		return true;
	}

	return false;
}

void main() {
	
	vec3 newPos = position - vec3(0.5);
	oldPos = position;
	newNormal = normal;
	texCoord = uv;

	newPos.y *= yzRelativex.x;
	newPos.z *= yzRelativex.y;

    gl_Position = P * MV * vec4(newPos, 1.0f);
}
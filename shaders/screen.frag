#version 430 core

in vec3 newPos;
in vec2 texCoord;

out vec4 outColor;

uniform sampler2D volumeTexture;
uniform sampler2D cubeTexture;
uniform sampler2D rayEnterTexture;
uniform sampler2D rayExitTexture;
uniform sampler3D test;

uniform vec2 resolution;
uniform vec3 volumeResolution;
uniform float time;
uniform vec3 camPos;

const float cubeSize = 		1.0;
const float borderLeft = 	0.0;
const float borderRight = 	cubeSize;
const float borderDown = 	0.0;
const float borderUp = 		cubeSize;
const float borderNear = 	0.0;
const float borderFar = 	cubeSize;

const float EPSILON = 0.016;
#define PI 3.14159265359

float intersectSDF(float distA, float distB) {
    return max(distA, distB);
}

float unionSDF(float distA, float distB) {
    return min(distA, distB);
}

float differenceSDF(float distA, float distB) {
    return max(distA, -distB);
}

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float sphereSDF(vec3 p) {
    return length(p) - 1.0;
}

float cubeSDF( vec3 p, vec3 b, float r )
{
  return length(max(abs(p)-b,0.0))-r;
}


float waterSDF(vec3 p) {
	float surface = 0.01*(sin(20*p.x + time/5) + 4*sin(20*p.z + 30*sin((p.x + p.z)*0.4 + 0.05*time) + time/2.3) + 25 * sin(p.x * p.z + time/3))
	+ 0.0;
    return p.y - surface;
}

float sceneSDF(vec3 p) {
    return waterSDF(p - vec3(0.5));
    // return intersectSDF(waterSDF(p), cubeSDF(p, vec3(0.5), 0.1));
    // return unionSDF(
	// 		intersectSDF(waterSDF(p), sphereSDF(p)),
	// 		cubeSDF(p - vec3(0,0.3,0), vec3(0.1, 0.2*sin(time/4) + 0.1 + 0.1, 0.1), 0.08)
	// 		);
}


vec3 estimateNormalSDF(vec3 p) {
    return normalize(vec3(
        sceneSDF(vec3(p.x + EPSILON, p.y, p.z)) - sceneSDF(vec3(p.x - EPSILON, p.y, p.z)),
        sceneSDF(vec3(p.x, p.y + EPSILON, p.z)) - sceneSDF(vec3(p.x, p.y - EPSILON, p.z)),
        sceneSDF(vec3(p.x, p.y, p.z  + EPSILON)) - sceneSDF(vec3(p.x, p.y, p.z - EPSILON))
    ));
}

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

vec4 readVolume(int x, int y, int z){
	vec4 voxel;
	vec2 volumeCoord;
	ivec2 texSize = textureSize(volumeTexture, 0);

	float squareSize = texSize.x / volumeResolution.x;
	volumeCoord.x = (x + mod(z, squareSize) * volumeResolution.x) / (texSize.x);
	volumeCoord.y = (y + floor(z / squareSize) * volumeResolution.y ) / (texSize.y);
	voxel = vec4(0, 0, 0, texture(volumeTexture, volumeCoord).x);
	return voxel;
}

vec4 readVolume(float x, float y, float z){

	// Transformations of data
	
	
	// Morph
	// x += 0.05*sin(time/4.0 + 7 * y);
	// y += 0.03*sin(time/4.0 + 10 * x);
	// z += 0.04*sin(time/4.0 + 13* x);

	// 3D texture
	// return vec4(1,1,1,texture(test, vec3(x,y,z)));
	
	// Scale
	// ============================
	// z = volumeResolution.x/volumeResolution.z * (z - 0.25);
	// y *= volumeResolution.x/volumeResolution.y;
	// float factor = 0.6;
	// x -= factor*(0.5 - x);
	// y -= factor*(0.5 - y);
	// z -= factor*(0.5 - z);
	// if(!insideUnitCube(vec3(x,y,z))) return vec4(0);
	//==============================

	// Read at correct place in volume
	x *= volumeResolution.x;
	y *= volumeResolution.y;
	z *= volumeResolution.z;

	int ix = int(floor(x));
	int iy = int(floor(y));
	int iz = int(floor(z));

	return readVolume(ix, iy, iz);
}

vec3 estimateNormal(vec3 p) {
    return normalize(vec3(
        readVolume(p.x + EPSILON, p.y, p.z).a - readVolume(p.x - EPSILON, p.y, p.z).a,
        readVolume(p.x, p.y + EPSILON, p.z).a - readVolume(p.x, p.y - EPSILON, p.z).a,
        readVolume(p.x, p.y, p.z  + EPSILON).a - readVolume(p.x, p.y, p.z - EPSILON).a
    ));
}

void transferFunction(inout vec4 data){

		if(data.a > 0.1) data.xyz = vec3(0.6,0.2,0.2);
		if(data.a > 0.3) data.xyz = vec3(0.63,0.46,.34);
		if(data.a > 0.6) data.xyz = vec3(0.6,0.5,.4);
		if(data.a > 0.8) data.xyz = vec3(0.8,0.8,0.7);
		if(data.a < 0.1) data.a = 0.0;
		else data.a *= 0.4;
}

void main(void)
{

	// ====== CPU =========
	vec3 normal = vec3(0);
	vec3 light = normalize(vec3 (1, 1, 1));

	float x = 2.0 * gl_FragCoord.x/resolution.x - 1.0; // x, y => [-1, 1]
	float y = 2.0 * gl_FragCoord.y/resolution.y - 1.0;
	
	// Pixel position
	vec3 enterPos = texture(rayEnterTexture, texCoord).xyz;
	vec3 exitPos = texture(rayExitTexture, texCoord).xyz;
	float stepSize = 0.001;
	stepSize = 0.005;

	float randomStart = 1.0 * rand(vec2(x, y)) * stepSize;
	// randomStart = 0;

	// Ray starting position
	vec3 rayDirection = normalize(exitPos - enterPos);
	vec3 ray = enterPos + rayDirection * randomStart;
	exitPos -= rayDirection * randomStart;
	float travelDistance = length(exitPos-enterPos);
	const int MAX_MARCHING_STEPS = int(travelDistance / stepSize);
	
	vec4 v = vec4(0); 
	vec4 boundingCube =  vec4(0,0,0,1);
	vec4 boundingCubeColor = vec4(0.1, 0.1, 0.1, 1);
	 
	
	for(int i = 0; i < MAX_MARCHING_STEPS; i++){

		if(v.a > 1.0) break;

		// float dist = sceneSDF(ray);

		// if(dist < EPSILON){ // Hit surface
		// 	normal = estimateNormalSDF(ray);
		// 	float diffuse = max(dot(normal, light), 0.0);
		// 	float ambient = 0.004;
		// 	vec3 R = reflect(-light, normal);
		// 	vec3 V = normalize(camPos - ray); // View direction
		// 	float specular = pow(max(dot(R, V), 0), 5);
		// 	float specular2 = pow(max(dot(R, V), 0), 100);
		// 	// v.r +=  5.7 * (ambient +  0.01 * diffuse + 0.01 * specular + 0.1 * specular2);
		// 	v.a += 0.004;
		// }

		ray += stepSize * rayDirection;
		normal = estimateNormal(ray);
		float ambient = 0.4;
		float diffuse = 0.4*max(dot(normal, light), 0.0);
		float specular = 0.4 * pow(max(dot(reflect(light, normal), normalize(camPos - ray)), 0), 50);
		vec4 data = readVolume(ray.x, ray.y, ray.z);
		transferFunction(data);
		
		v += vec4((ambient + (1.0 - ambient) * (diffuse + specular)) * data.xyz, data.a + v.a) * data.a;
		
		if(texture(cubeTexture, texCoord).a != 0){
			if(length(texture(cubeTexture, texCoord).xyz - camPos) > length(ray - camPos))
				boundingCube = vec4(max(1.0 - v.a, 0.0));
			else {
				boundingCube = vec4(1.0);
			}
		} 
	}


	outColor = v;// + boundingCube * boundingCubeColor;

	outColor = v + vec4((vec3(0.4 - 0.3 * (pow(0.5 - texCoord.x, 2.0) + pow(0.5 - texCoord.y, 2.0))) * (1.0 - v.a)), 1) + boundingCube * boundingCubeColor;
}

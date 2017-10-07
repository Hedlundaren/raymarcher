#version 430 core

in vec3 newPos;
in vec2 texCoord;

out vec4 outColor;

uniform sampler2D volumeTexture;
uniform sampler2D cubeTexture;
uniform sampler2D rayEnterTexture;
uniform sampler2D rayExitTexture;
uniform vec2 resolution;
uniform vec3 volumeResolution;
uniform float time;
uniform vec3 camPos;

const float cubeSize = 1.0;
const float borderLeft = 	0.0;
const float borderRight = 	cubeSize;
const float borderDown = 	0.0;
const float borderUp = 		cubeSize;
const float borderNear = 	0.0;
const float borderFar = 	cubeSize;

const float EPSILON = 0.01;
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
    // return intersectSDF(waterSDF(p), cubeSDF(p, vec3(0.5), 0.1));
    return unionSDF(
			intersectSDF(waterSDF(p), sphereSDF(p)),
			cubeSDF(p - vec3(0,0.3,0), vec3(0.1, 0.2*sin(time/4) + 0.1 + 0.1, 0.1), 0.08)
			);
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
	ivec2 size = textureSize(volumeTexture, 0);
	float size_x = size.x / volumeResolution.x;
	volumeCoord.x = ((x + size_x*z) + 0.5)/size.x;
	volumeCoord.y = (y + 0.5)/size.y;
	voxel = texture(volumeTexture, volumeCoord);

	return voxel;
}

vec4 readVolume(float x, float y, float z){



	// Transformations of data
	// Works as vertext shader
	//y += 0.3*sin(time/4.0 + x);
	

	if(!insideUnitCube(vec3(x,y,z))) return vec3(0);


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





void main(void)
{

	// ====== CPU =========
	float screenRatio = resolution.y / resolution.x; 
	float fov = 50.0 * PI / 180.0;
	float nearClip = 2.0/tan(fov/2.0);

	vec3 normal = vec3(0);
	vec3 light = normalize(vec3 (1, 1, 1));

	// Calculate camera orientation
	vec3 camDirection = normalize(vec3(0.5)-camPos); // Point camera towards origo
	vec3 up = vec3(0,1,0);
	vec3 right = normalize(cross(camDirection, up));
	up = normalize(cross(right, camDirection));
	float x = 2.0 * gl_FragCoord.x/resolution.x - 1.0; // x, y => [-1, 1]
	float y = 2.0 * gl_FragCoord.y/resolution.y - 1.0;
	
	// Pixel position
	vec3 enterPos = texture(rayEnterTexture, texCoord).xyz;
	vec3 exitPos = texture(rayExitTexture, texCoord).xyz;
	float travelDistance = length(exitPos-enterPos);
	float stepSize = 0.005;
	const int MAX_MARCHING_STEPS = int(travelDistance / stepSize);
	float randomStart = 1.0 * rand(vec2(x, y)) * stepSize;
	//randomStart = 0;
	vec3 pixelPos = camPos + camDirection * ( nearClip + randomStart) + x * right + y * screenRatio * up;

	// Ray starting position
	vec3 ray = enterPos;
	vec3 rayDirection = normalize(exitPos - enterPos);
	

	vec4 v = vec4(0,0,0,1); 
	v = texture(cubeTexture, texCoord);

	// For mathematical visualizations
	// for(int i = 0; i < MAX_MARCHING_STEPS; i++){

	// 	float dist = sceneSDF(ray);

	// 	if(dist < EPSILON){ // Hit surface
	// 		normal = estimateNormalSDF(ray);

	// 		float diffuse = max(dot(normal, light), 0.0);
	// 		float ambient = 0.004;
	// 		vec3 R = reflect(-light, normal);
	// 		vec3 V = normalize(camPos - ray); // View direction
	// 		float specular = pow(max(dot(R, V), 0), 5);
	// 		float specular2 = pow(max(dot(R, V), 0), 100);
	// 		v.r +=  5.7 * (ambient +  0.01 * diffuse + 0.01 * specular + 0.1 * specular2);
	// 		v.y += 5.0 * (ray.y / 100.0);
	// 	}
		
	// 	ray += dist * rayDirection;
	// }

	bool intersect = false;
	
	for(int i = 0; i < MAX_MARCHING_STEPS; i++){

		ray += stepSize * rayDirection;
		
		if(v.a > 1.0) break;

		normal = estimateNormal(ray);
		vec4 data = readVolume(ray.x, ray.y, ray.z);
		float diffuse = max(dot(normal, light), 0.0);
		v += vec4(data.xyz, 1.0) * data.a;

	}

	outColor = v;
	outColor = mix(v, texture(rayEnterTexture, texCoord), 0.0);
	
}

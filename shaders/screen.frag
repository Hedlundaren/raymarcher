#version 430 core

in vec3 newPos;
in vec2 texCoord;

out vec4 outColor;

uniform sampler2D volumeTexture;
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

const float EPSILON = 0.1;

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


vec3 estimateNormal(vec3 p) {
    return normalize(vec3(
        sceneSDF(vec3(p.x + EPSILON, p.y, p.z)) - sceneSDF(vec3(p.x - EPSILON, p.y, p.z)),
        sceneSDF(vec3(p.x, p.y + EPSILON, p.z)) - sceneSDF(vec3(p.x, p.y - EPSILON, p.z)),
        sceneSDF(vec3(p.x, p.y, p.z  + EPSILON)) - sceneSDF(vec3(p.x, p.y, p.z - EPSILON))
    ));
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

	// center volume
	x *= volumeResolution.x;
	y *= volumeResolution.y;
	z *= volumeResolution.z;

	int ix = int(floor(x));
	int iy = int(floor(y));
	int iz = int(floor(z));

	return readVolume(ix, iy, iz);
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
}

void main(void)
{

	// ====== CPU =========
	float screenRatio = resolution.y / resolution.x; 
	float fov = 50.0 * 3.1415 / 180.0;
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
	float randomStart = 0.01 * rand(vec2(x, y));
	randomStart = 0;
	vec3 pixelPos = camPos + camDirection * ( nearClip + randomStart) + x * right + y * screenRatio * up;

	// Ray starting position
	vec3 ray = pixelPos;
	vec3 rayDirection = normalize(pixelPos - camPos);
	const int MAX_MARCHING_STEPS = 100;
	const float MAX_DISTANCE = 5.0;
	float stepSize = MAX_DISTANCE / MAX_MARCHING_STEPS;

	vec3 v = vec3(0.0); 

	// for(int i = 0; i < MAX_MARCHING_STEPS; i++){

	// 	float dist = sceneSDF(ray);

	// 	if(dist < EPSILON){ // Hit surface
	// 		normal = estimateNormal(ray);

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
		
		if(length(v) > 0) return;
		if(intersect && !insideUnitCube(ray)) return;

		if(intersect){
			v += readVolume(ray.x, ray.y, ray.z).xyz;
		}else{
			intersect = insideUnitCube(ray);
		}
	}


	//vec3 mixColor = mix(v.xyz, voxel.xyz, 0.5);

	//float depth = length(ray - pixelPos);
	// if(depth - nearClip > volume.a){
	// 	mixColor += volume.xyz;
	// }

	
	
	outColor = vec4(v, 1.0);
}

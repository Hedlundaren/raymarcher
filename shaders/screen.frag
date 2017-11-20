#version 430 core

in vec3 newPos;
in vec2 texCoord;

out vec4 outColor;

uniform sampler2D volumeTexture;
uniform sampler2D cubeTexture;
uniform sampler2D rayExitTexture;
uniform sampler2D rayEnterTexture;
uniform sampler2D controlPointValues;
uniform sampler2D controlPointPositions;
// uniform sampler3D test;

uniform float isInteracting;
uniform float opacityFactor;
uniform float numberOfControlPoints;
uniform float numberOfActiveControlPoints;
uniform vec2 resolution;
uniform vec3 volumeResolution;
uniform vec3 volumeSlicing;
uniform float time;
uniform vec3 camPos;


const float interactingFactor = 5.0;
float cubeSize = 		1.0;
float borderLeft = 	0.0;
float borderRight = 	cubeSize;
float borderDown = 	0.0;
float borderUp = 		cubeSize;
float borderNear = 	0.0;
float borderFar = 	cubeSize;

const float EPSILON = 0.016;
#define PI 3.14159265359

vec4 getControlPointValue(float id, sampler2D type) {

    vec2 controlPointCoord = vec2((id + 0.5)/numberOfControlPoints, 0.5);
    return texture(type, controlPointCoord);
} 

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

vec4 readVolume(int x, int y, int z) {
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
	if(x < volumeSlicing.x) return vec4(0.0);
	if(y < volumeSlicing.y) return vec4(0.0);
	if(z < volumeSlicing.z) return vec4(0.0);
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

    // if(data.a > 0.1) data.xyz = vec3(0.6,0.2,0.2);
    // if(data.a > 0.3) data.xyz = vec3(0.63,0.46,.34);
    // if(data.a > 0.6) data.xyz = vec3(0.6,0.5,0.4);
    // if(data.a > 0.8) data.xyz = vec3(0.9,0.9,0.8);
    // else data.a *= 0.4;
    bool dataInRange = false;
    for(float id = 1.0; id < numberOfActiveControlPoints; id++){
        vec2 pos1 = getControlPointValue(id-1, controlPointPositions).xy;
        vec2 pos2 = getControlPointValue(id, controlPointPositions).xy;
        if( data.a > pos1.x && data.a < pos2.x) {
            dataInRange = true;
            float s1 = data.a - pos1.x;
            float s2 = pos2.x - data.a;
            float S = s1 + s2;

            vec4 v1 = getControlPointValue(id-1, controlPointValues);
            vec4 v2 = getControlPointValue(id, controlPointValues);
            v1.a = pos1.y;
            v2.a = pos2.y;

            data = ( v1 * s1 + v2 * s2 ) / (S);
        }
    }
    if(!dataInRange) data = vec4(0.0);

    data.a *= opacityFactor;

    if(isInteracting > 0.5) data.a = data.a * opacityFactor  +  data.a * interactingFactor * (1.0 - opacityFactor);
}

void main(void)
{

	// ====== CPU =========
	vec3 normal = vec3(0);
	vec3 light = normalize(vec3 (1, -5, 1));

	// float x = 2.0 * gl_FragCoord.x/resolution.x - 1.0; // x, y => [-1, 1]
	// float y = 2.0 * gl_FragCoord.y/resolution.y - 1.0;
	
	// Pixel position
	vec3 enterPos = texture(rayEnterTexture, texCoord).xyz;
	vec3 exitPos = texture(rayExitTexture, texCoord).xyz;
	float stepSize = 0.001;
	stepSize = 0.5/volumeResolution.y;
    if(isInteracting > 0.5){
	    stepSize *= interactingFactor;
    }

	float randomStart = 2.0 * rand(enterPos.xy) * stepSize;
	randomStart = 0;

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

		if(v.a > 1.0 || length(enterPos) < EPSILON) break;

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
		float ambient = 0.3;
		float diffuse = 0.5*max(dot(normal, light), 0.0);
		float specular = 0.2 * pow(max(dot(reflect(light, normal), normalize(camPos - ray)), 0), 50);
		vec4 data = readVolume(ray.x, ray.y, ray.z);
		transferFunction(data);
		
		v += vec4((ambient + (1.0 - ambient) * (diffuse + specular)) * data.xyz, 1.0) * data.a;
		
		if(texture(cubeTexture, texCoord).a != 0){
			if(length(texture(cubeTexture, texCoord).xyz - camPos) > length(ray - camPos))
				boundingCube = vec4(max(1.0 - v.a, 0.0));
			else {
				boundingCube = vec4(1.0);
			}
		} 
	}


		if(length(enterPos) < EPSILON && texture(cubeTexture, texCoord).a != 0){
			boundingCube = vec4(1.0);
		} 

    v.x = clamp(v.x, 0.0, 1.0);
    v.y = clamp(v.y, 0.0, 1.0);
    v.z = clamp(v.z, 0.0, 1.0);
    v.a = clamp(v.a, 0.0, 1.0);
	outColor = v + vec4((vec3(0.4 - 0.3 * (pow(0.5 - texCoord.x, 2.0) + pow(0.5 - texCoord.y, 2.0))) * (1.0 - v.a)), 1) + boundingCube * boundingCubeColor;
	// outColor += 0.6*texture(rayEnterTexture, texCoord);
	// outColor = v;
	
}

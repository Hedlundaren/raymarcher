#version 430 core

in vec3 newPos;
in vec2 texCoord;

out vec4 outColor;

uniform sampler2D screenTexture;
uniform vec2 resolution;


float sphereSDF(vec3 p) {
    return length(p) - 1.0;
}


void main(void)
{

	vec3 camPos = vec3(0,0, -3);
	vec3 camDirection = vec3(0,0,1);

	// x, y => [-1, 1]
	float x = 2.0 * gl_FragCoord.x/resolution.x - 1.0;
	float y = 2.0 * gl_FragCoord.y/resolution.y - 1.0;

	float screenRatio = resolution.x / resolution.y; 
	
	float nearClip = 1.0;

	// Pixel position
	vec3 pixelPos = camPos + nearClip * camDirection + vec3(x * screenRatio,y,0);
	vec3 ray = pixelPos;
	vec3 rayDirection = normalize(pixelPos - camPos);
	float stepSize = 0.3;

	// Ray marching
	// Start from pixel with fixed stepsize and number of steps
	float v = 0.0; // pixelval
	for(int i = 0; i < 100; i++){

		ray += stepSize * rayDirection;

		if(sphereSDF(ray) < 0.0){
			v += 0.1;
		}
	}

	outColor = vec4(v, 0, 0, 1.0);
	//outColor = vec4(earth, 1.0);
}

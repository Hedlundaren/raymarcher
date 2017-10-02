#version 430


out vec4 outColor;

in vec3 newPos;
in vec3 newNormal;
in vec2 texCoord;

uniform float time;
uniform vec3 camPos;
uniform vec3 lDir;
uniform vec3 clear_color;
uniform vec2 window_dim;
uniform sampler2D refractionTexture;
uniform sampler2D earthTexture;

void main()
{

	const vec3 light = vec3(0.58, 0.58, 0.58); // Given in VIEW coordinates! You usually specify light sources in world coordinates.
	float ambient, diffuse, specular, shade;
	
	// Ambient
	ambient = 1.0;

	// Diffuse
	diffuse = dot(normalize(newNormal), light);
	diffuse = max(0.0, diffuse); // No negative light
	
	// Specular
	vec3 r = reflect(-light, normalize(newNormal));
	vec3 v = normalize(camPos - newPos); // View direction
	specular = dot(r, v);
	if (specular > 0.0)
		specular = pow(specular, 50.0);
	specular = max(specular, 0.0);
	
	// Mix
	shade = 0.7*diffuse + 0.0*specular + 0.3*ambient;
	vec3 color = vec3(0.8, 0.6, 0.7);
	outColor = vec4(shade * color + 0.6 * clear_color, 1.0);
} 
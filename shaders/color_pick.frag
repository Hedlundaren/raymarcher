#version 430


out vec4 outColor;

in vec3 oldPos;
in vec3 newNormal;
in vec2 texCoord;

float pointSize = 7.0;
float TF_height = 0.3;


void main()
{
    vec4 finalColor = vec4(0);
    
    // Color pick
    vec2 texCoordTF = vec2(texCoord.x, 2*texCoord.y/TF_height);
    vec4 RED = vec4(1.0,0.0,0.0,1.0);
    vec4 GREEN = vec4(0.0,1.0,0.0,1.0);
    vec4 BLUE = vec4(0.0,0.0,1.0,1.0);
    vec4 WHITE = vec4(1.0);
    vec4 colors [4];
    colors[0] = RED;
    colors[1] = GREEN;
    colors[2] = BLUE;
    colors[3] = RED;

    // Create color band
    int colorId = int(floor(texCoordTF * 3.0).x);
    float wave = mod(texCoordTF, 0.333333).x * 3;
    float waveInv = mod(-texCoordTF, 0.333333).x * 3;
    finalColor = mix(waveInv * colors[colorId], wave * colors[colorId +1], wave);
    finalColor += mix(waveInv * colors[colorId], wave * colors[colorId +1], waveInv);

    // Add black and white
    colorId = int(floor(texCoordTF).y);
    wave = mod(texCoordTF, 1).y;
    vec4 lowerColor = finalColor * wave * (1-colorId);
    vec4 upperColor = mix(finalColor, WHITE, wave) * (colorId);
    finalColor = lowerColor + upperColor;
    
	outColor = finalColor;
} 
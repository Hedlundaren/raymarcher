#version 430


out vec4 outColor;

in vec3 oldPos;
in vec3 newNormal;
in vec2 texCoord;

uniform float time;
uniform vec2 resolution;

uniform sampler2D volumeRender;
uniform sampler2D controlPointValues;

float pointSize = 7.0;
float TF_height = 0.3;
float TF_opacity = 0.7;
vec4 TF_background = vec4(0.47); 


void drawPoint(inout vec4 color, vec2 point, vec4 pointColor) {

    point.y *= TF_height;
    if(length(resolution * (point - texCoord))  < pointSize) color = vec4(0.8);
    if(length(resolution * (point - texCoord))  < pointSize*0.7) color = pointColor;

}

void main()
{

    vec4 finalColor = vec4(0);
    

    vec4 screenVolumeRender = texture(volumeRender, texCoord);
    finalColor = screenVolumeRender;


    
    if(texCoord.y < TF_height) {
        
        finalColor = TF_opacity * TF_background + (1.0-TF_opacity) * screenVolumeRender;
    drawPoint(finalColor, vec2(0.4, 0.9), vec4(1.0, 0.3, 0.1, 1.0));
    }

    
	outColor = finalColor;
	// outColor = texture(controlPointValues, texCoord);
} 
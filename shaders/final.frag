#version 430


out vec4 outColor;

in vec3 oldPos;
in vec3 newNormal;
in vec2 texCoord;

uniform float time;
uniform vec2 resolution;

uniform sampler2D volumeRender;
uniform sampler2D controlPointValues;
uniform sampler2D controlPointPositions;
uniform sampler2D colorPicker;

uniform float numberOfControlPoints;
uniform float numberOfActiveControlPoints;
uniform float hoveredControlPoint;
uniform float selectedControlPoint;
uniform float guiActive;
uniform float guiColorPickActive;

const float EPSILON = 0.0001;
vec4 lineColor = vec4(0.6);
float pointSize = 7.0;
float TF_height = 0.3;
float TF_opacity = 0.7;
vec4 TF_background = vec4(0.47);


void drawPoint(inout vec4 color, vec2 point, vec4 pointColor, float type) {

    point.y *= TF_height;
    if(length(resolution * (point - texCoord))  < pointSize) color = vec4(0.8 - type * 0.1);
    if(length(resolution * (point - texCoord))  < pointSize*0.7) color = pointColor;

}

vec4 getControlPointValue(float id, sampler2D type) {

    vec2 controlPointCoord = vec2((id + 0.5)/numberOfControlPoints, 0.5);
    return texture(type, controlPointCoord);
} 

void setControlPoints(inout vec4 color){

    for(float id = 0; id < numberOfActiveControlPoints; id++) {
        float type = 0;

        if(id == hoveredControlPoint) type = 1.0;
        if(id == selectedControlPoint) type = 2.0;
        drawPoint(color, getControlPointValue(id, controlPointPositions).xy, getControlPointValue(id, controlPointValues), type);
    }
}

void drawLines(inout vec4 color){

    for(float id = 1; id < numberOfActiveControlPoints; id++) {
        vec2 currPoint = vec2(texCoord.x, texCoord.y);
        vec2 point1 = vec2(getControlPointValue(id-1.0, controlPointPositions).x, TF_height * getControlPointValue(id-1.0, controlPointPositions).y);
        vec2 point2 = vec2(getControlPointValue(id, controlPointPositions).x, TF_height * getControlPointValue(id, controlPointPositions).y);
        if(length(point1 - currPoint)+length(currPoint - point2) - length(point1 - point2) < EPSILON ) color = lineColor;
    }
}



void main()
{
    vec4 finalColor = vec4(0);
    
    vec4 screenVolumeRender = texture(volumeRender, texCoord);
    finalColor = screenVolumeRender;
    
    if(texCoord.y < TF_height && guiActive > 0.5) {
        
    
        finalColor = TF_opacity * TF_background + (1.0-TF_opacity) * screenVolumeRender;
        drawLines(finalColor);
        setControlPoints(finalColor);

        if(guiColorPickActive > 0.5) { 
            finalColor = mix(texture(colorPicker, texCoord), finalColor, 0.0);
        }
 
    }

    
	outColor = finalColor;
	// outColor = texture(controlPointPositions, texCoord);
    // outColor = getControlPointValue(0, controlPointValues);
    // vec2 currPoint = vec2(texCoord.x, texCoord.y) / TF_height;
    // outColor = vec4(currPoint.y);    
    
} 
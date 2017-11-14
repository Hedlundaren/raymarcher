#include "gui.h"

GUI::GUI(const float &w, const float &h) 
    : resolution(glm::ivec2(w,h))
{
    initControlPoints();
}


void GUI::bindControlPointValueBuffer() {
    controlPointValueBuffer.bindBuffer();
}

void GUI::bindControlPointPositionBuffer() {
    controlPointPositionBuffer.bindBuffer();
}

void GUI::drawData(const std::vector<glm::vec4> &pixels, Framebuffer &buffer)
{
    buffer.bindBuffer();
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, numberOfControlPoints, 1, GL_RED, GL_FLOAT, pixels.data());
}

void GUI::initControlPoints(){
    std::vector<glm::vec4> pixels(numberOfControlPoints);
    
    // for(auto cp : pixels) {
    //     cp = glm::vec4(0.0);
    // }

    // Test data
    pixels[0] = glm::vec4(1,0,0,1);
    pixels[2] = glm::vec4(0,1,0,1);
    pixels[4] = glm::vec4(0,0,1,1);
    pixels[9] = glm::vec4(1,1,1,1);

    drawData(pixels, controlPointValueBuffer);
    // drawData(pixels, controlPointPositionBuffer);
}

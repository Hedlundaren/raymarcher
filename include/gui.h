#pragma once

#include "framebuffer.h"
#include "glm/glm.hpp"
#include <vector>

class GUI{

public: 
    GUI(const float &w, const float &h);
    void bindControlPointValueBuffer();
    void bindControlPointPositionBuffer();


private: 
    const int numberOfControlPoints = 10;
    Framebuffer controlPointValueBuffer = Framebuffer(10, 10);
    Framebuffer controlPointPositionBuffer = Framebuffer(10, 10);
    glm::ivec2 resolution;

    void initControlPoints();
    void drawData(const std::vector<glm::vec4> &pixels, Framebuffer &buffer);

};
#pragma once

#include "framebuffer.h"
#include "glm/glm.hpp"
#include <vector>
#include <GLFW/glfw3.h>


class GUI{

public: 
    GUI(const float &w, const float &h);
    void bindControlPointValueTexture();
    void bindControlPointPositionTexture();
    float getNumberOfControlPoints();
    void update(GLFWwindow* &window);


private: 


    void initControlPoints();
    void drawData(const std::vector<glm::vec4> &pixels, Framebuffer &buffer);


    const int numberOfControlPoints = 10;
    Framebuffer controlPointValueBuffer = Framebuffer(10, 1);
    Framebuffer controlPointPositionBuffer = Framebuffer(10, 1);
    glm::ivec2 resolution;

};
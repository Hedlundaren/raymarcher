#pragma once

#include "framebuffer.h"
#include "glm/glm.hpp"
#include <vector>
#include <GLFW/glfw3.h>

class GUI
{

  public:
    GUI(const float &w, const float &h);
    void bindControlPointValueTexture();
    void bindControlPointPositionTexture();
    float getNumberOfControlPoints();
    float getSelectedControlPoint();
    float getHoveredControlPoint();
    glm::vec2 getCursorPos();
    glm::vec2 getCursorPosTF();
    void update(GLFWwindow *&window);

  private:
    void initControlPoints();
    void drawData(const std::vector<glm::vec4> &pixels, Framebuffer &buffer);

    const int numberOfControlPoints = 10;
    Framebuffer controlPointValueBuffer = Framebuffer(10, 1);
    Framebuffer controlPointPositionBuffer = Framebuffer(10, 1);
    glm::ivec2 resolution;

    std::vector<glm::vec4> controlPointValues;
    std::vector<glm::vec4> controlPointPositions;
    int selectedControlPoint = -1;
    int hoveredControlPoint = -1;

	glm::vec2 cursorPos;
	glm::vec2 cursorPosTF;

};
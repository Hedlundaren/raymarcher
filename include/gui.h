#pragma once

#include "framebuffer.h"
#include "glm/glm.hpp"
#include <vector>
#include <GLFW/glfw3.h>

class GUI
{

public:
  GUI(const float &w, const float &h);
  float isActive();
  void bindControlPointValueTexture();
  void bindControlPointPositionTexture();
  float getNumberOfControlPoints();
  float getNumberOfActiveControlPoints();
  float getSelectedControlPoint();
  float getHoveredControlPoint();
  glm::vec2 getCursorPos();
  glm::vec2 getCursorPosTF();
  void deleteControlPoint(int id);

  void update(GLFWwindow *&window);

private:
  float guiActive = 1.0f;
  void initControlPoints();
  void drawData(const std::vector<glm::vec4> &pixels, Framebuffer &buffer);

  const int numberOfControlPoints = 10;
  int numberOfActiveControlPoints = 4;
  Framebuffer controlPointValueBuffer = Framebuffer(10, 1);
  Framebuffer controlPointPositionBuffer = Framebuffer(10, 1);
  glm::ivec2 resolution;

  std::vector<glm::vec4> controlPointValues;
  std::vector<glm::vec4> controlPointPositions;
  int selectedControlPoint = -1;
  int hoveredControlPoint = -1;
  float timeAtInteraction = 0.0f;
  float timeSinceInteraction = 0.0f;
  const float maxTimeBetweenInteractions = 0.3f;

  glm::vec2 cursorPos;
  glm::vec2 cursorPosTF;
};
#pragma once

#include "framebuffer.h"
#include "glm/glm.hpp"
#include <vector>
#include <GLFW/glfw3.h>
#include "tfm.h"

class GUI
{

public:
  GUI(const float &w, const float &h);
  float isActive();
  float isColorPickActive();
  bool isDraggedFun();
  glm::vec4 readColorData(const int &x, const int &y);
  void bindControlPointValueTexture();
  void bindControlPointPositionTexture();
  float getNumberOfControlPoints();
  float getNumberOfActiveControlPoints();
  float getSelectedControlPoint();
  float getHoveredControlPoint();
  float getOpacityFactor();
  float getRenderOption();
  glm::vec3 getVolumeSlicing();
  glm::vec2 getCursorPos();
  glm::vec2 getCursorPosTF();
  void deleteControlPoint(int id);
  bool onLine();
  void addControlPoint();


  void update(GLFWwindow *&window, Framebuffer &colorPickBuffer);

private:
  float guiActive = 1.0f;
  float guiColorPickActive = 0.0f;
  const float EPSILON = 0.010;

  void initControlPoints();
  void drawData(const std::vector<glm::vec4> &pixels, Framebuffer &buffer);

  const int numberOfControlPoints = 100;
  int numberOfActiveControlPoints = 2;
  Framebuffer controlPointValueBuffer = Framebuffer(100, 1);
  Framebuffer controlPointPositionBuffer = Framebuffer(100, 1);
  glm::ivec2 resolution;

  std::vector<glm::vec4> controlPointValues;
  std::vector<glm::vec4> controlPointPositions;
  int selectedControlPoint = -1;
  int hoveredControlPoint = -1;
  float timeAtInteraction = 0.0f;
  float timeSinceInteraction = 0.0f;
  const float maxTimeBetweenInteractions = 0.3f;

  bool isDragged = false;
  GLFWcursor *cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
  float opacityFactor = 1.0f;
  float renderOption = 1.0f;

  glm::vec3 volumeSlicing = glm::vec3(0.0);

  glm::vec2 cursorPos;
  glm::vec2 cursorPosTF;
  std::string activeTF = "-1";
};
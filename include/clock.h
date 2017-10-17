#pragma once

#include <iostream>
#include <ctime>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Clock
{

  public:
    Clock(GLFWwindow *w);
    void tic();
    void toc();
    float getTime();
    double clockToMilliseconds(clock_t ticks);

    // variables

  private:
    GLFWwindow *window;
    float time = 0.0;
    clock_t deltaTime = 0;
    clock_t beginFrame = 0;
    clock_t endFrame = 0;
    unsigned int frames = 0;
    double frameRate = 30;
    double averageFrameTimeMilliseconds = 33.333;
};
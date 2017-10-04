#pragma once

#include <ctime>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Clock{

public:
    Clock(GLFWwindow *w);
    void start();
    void stop();
    float getTime();

private:
    clock_t current_ticks, delta_ticks;
    clock_t fps = 0;
    float time = 0.0;
    GLFWwindow *window;

};
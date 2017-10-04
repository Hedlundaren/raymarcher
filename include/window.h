#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window{

public:
    Window(GLFWwindow* &window, int width, int height);
    void init();
    
private:
    int width, height;
};
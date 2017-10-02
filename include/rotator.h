#pragma once
#include<iostream>
#include <GLFW/glfw3.h>
#include <cmath>

#ifndef M_PI
#define M_PI (3.141592653589793)
#endif

class KeyTranslator {

public:
	float horizontal;
	float zoom;

private:
	double lastTime;

public:
	void init(GLFWwindow *window);
	void poll(GLFWwindow *window);
};

class MouseRotator {

public:
	float phi;
	float theta;
	float transX;
	float transY;
	float zoom;

private:
	double lastX;
	double lastY;
	int lastLeft;
	int lastRight;
	int lastMiddle;
	bool rotStarted;

public:
	void init(GLFWwindow *window);
	void poll(GLFWwindow *window);
};

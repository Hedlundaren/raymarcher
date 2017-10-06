#pragma once
#include "GL/glew.h"
#include <GLFW/glfw3.h>

class BoundingCube
{
public:
	BoundingCube();
	~BoundingCube();
	void draw();

private:
	GLuint quadVAO, quadVBO;
};
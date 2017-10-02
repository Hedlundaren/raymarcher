#pragma once

#include "GL/glew.h"
#include <GLFW/glfw3.h>

class Quad
{
public:
	Quad();
	~Quad();
	void draw();

private:
	GLuint quadVAO, quadVBO;

};
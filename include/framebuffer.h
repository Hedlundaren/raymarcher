#pragma once

#include <iostream>
#include "GL/glew.h"
#include <GLFW/glfw3.h>


class Framebuffer
{
public:
	Framebuffer();
	Framebuffer(const int w, const int h);

	~Framebuffer();

	// Generates a texture that is suited for attachments to a framebuffer
	GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil);

	void bindBuffer();
	void bindTexture();
    GLuint get();
private:
	GLuint framebuffer;
	GLuint textureColorbuffer;
	void create_framebuffer();
	const int width;
	const int height;
};
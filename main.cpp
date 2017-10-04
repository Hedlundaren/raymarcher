// External includes
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>

// Raytracer
#include "window.h"
#include "clock.h"
#include "shaderprogram.h"
#include "framebuffer.h"
#include "quad.h"
#include "sphere.h"
#include "volume.h"

#define W 1920/2
#define H 1080/2

int main()
{
	std::cout << "======== Marching Time =========" << std::endl;

	// Define window
	GLFWwindow *window = nullptr;
	Window w = Window(window, W, H);
	w.init();
	Clock clock = Clock(window);	

	// Define meshes
	Quad quad = Quad();
	Sphere sphere = Sphere(25, 25, 1.0f);

	// Define screen
	GLint screenLoc; 
	Framebuffer screen = Framebuffer(W, H);

	// Define shaders
	ShaderProgram phong_shader("shaders/phong.vert", "", "", "", "shaders/phong.frag");
	ShaderProgram screen_shader("shaders/screen.vert", "", "", "", "shaders/screen.frag");
	ShaderProgram post_shader("shaders/screen.vert", "", "", "", "shaders/post.frag");

	// Controls
	MouseRotator rotator;
	rotator.init(window);

	// Volume data
	Volume volume(100, 100, 100);

	const char *title = "Loading data...";
	glfwSetWindowTitle(window, title);
	volume.bindTexture();
	volume.loadTestData();
	
	do
	{
		rotator.poll(window);
		clock.start();

		// Ray marcher
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
		screen_shader();
		glViewport(0,0,W,H);
		screen_shader.updateCommonUniforms(rotator, W, H, clock.getTime());
		screenLoc = glGetUniformLocation(screen_shader, "volumeTexture");
		glUniform1i(screenLoc, 0);
		glActiveTexture(GL_TEXTURE0);
		volume.bindTexture();
		GLint volumeResolution_Loc = glGetUniformLocation(screen_shader, "volumeResolution");
		glUniform3fv(volumeResolution_Loc, 1, &volume.getResolution()[0]);
		
		quad.draw();

		clock.stop();

		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
			 glfwWindowShouldClose(window) == 0);

	glDisableVertexAttribArray(0);

	return 0;
}
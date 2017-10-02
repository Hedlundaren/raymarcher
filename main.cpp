// External includes
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>

// Raytracer
#include "window.h"
#include "shaderprogram.h"
#include "framebuffer.h"
#include "quad.h"
#include "sphere.h"

#define W 1000
#define H 500

int main()
{

	std::cout << "======== Marching Time =========" << std::endl;

	// Define window
	GLFWwindow *window = nullptr;
	Window w = Window(window, W, H);

	float time = 0.0f;

	// Define meshes
	Quad quad = Quad();
	Sphere sphere = Sphere(15, 15, 0.5f);

	// Define screen
	GLint screenLoc; 
	Framebuffer screen = Framebuffer(W, H);

	// Define shaders
	ShaderProgram phong_shader("shaders/phong.vert", "", "", "", "shaders/phong.frag");
	ShaderProgram screen_shader("shaders/screen.vert", "", "", "", "shaders/screen.frag");

	// Controls
	MouseRotator rotator;
	rotator.init(window);

	do
	{
		w.initFrame();
		time += 0.1f;
		glm::vec3 clear_color = glm::vec3(0.4f, 0.15f, 0.26f);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, 1.0f);
		rotator.poll(window);

		

		if(glfwGetKey(window, GLFW_KEY_W)){
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}else{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		screen_shader();
		screenLoc = glGetUniformLocation(screen_shader, "screenTexture");
		glUniform1i(screenLoc, 0);
		glActiveTexture(GL_TEXTURE0);
		screen.bindTexture();

		screen_shader.updateCommonUniforms(rotator, W, H, time, clear_color);
		quad.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
			 glfwWindowShouldClose(window) == 0);

	glDisableVertexAttribArray(0);

	return 0;
}
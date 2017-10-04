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
#include "volume.h"

#define W 1920
#define H 1080

int main()
{

	std::cout << "======== Marching Time =========" << std::endl;

	// Define window
	GLFWwindow *window = nullptr;
	Window w = Window(window, W, H);

	float time = 0.0f;

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
	Volume volume(10, 10, 10);

	do
	{
		w.initFrame();
		time += 0.1f;
		rotator.poll(window);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		// Volume data
		volume.bindBuffer();
		post_shader();
		glViewport(0,0,W,H);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		post_shader.updateCommonUniforms(rotator, W, H, time, glm::vec3(0));
		quad.draw();

		
		// Change Volume Data
		//volume.writeData(10,10,10, glm::vec4(0,1,0,1));
		
		for(int i = 0; i < 10; i++){
			for(int j = 0; j < 10; j++){
				for(int k = 0; k < 10; k++){
					volume.drawData(i, j, k, glm::vec4(0,0,0,1));
				}
			}
		}

		volume.drawData(0, 0, 0, glm::vec4(0.0,0,1,1));
		volume.drawData(9, 0, 0, glm::vec4(0.3,0,1,1));
		volume.drawData(0, 0, 9, glm::vec4(0.6,0,1,1));
		volume.drawData(9, 0, 9, glm::vec4(0.9,0,1,1));

		volume.drawData(0, 9, 9, glm::vec4(0.0,1,0,1));
		volume.drawData(9, 9, 9, glm::vec4(0.3,1,0,1));
		volume.drawData(0, 9, 0, glm::vec4(0.6,1,0,1));
		volume.drawData(9, 9, 0, glm::vec4(0.9,1,0,1));

		
		glm::vec4 pixel = volume.readData(0, 0, 1);
		std::cout << "r: " << pixel.r << "g: " << pixel.g << "b: " << pixel.b << std::endl;
		
		// Ray marcher
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		screen_shader();
		glViewport(0,0,W,H);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		screen_shader.updateCommonUniforms(rotator, W, H, time, glm::vec3(0));
		screenLoc = glGetUniformLocation(screen_shader, "volumeTexture");
		glUniform1i(screenLoc, 0);
		glActiveTexture(GL_TEXTURE0);
		volume.bindTexture();
		
		// Clamping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		// Interpolation
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		quad.draw();


		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
			 glfwWindowShouldClose(window) == 0);

	glDisableVertexAttribArray(0);

	return 0;
}
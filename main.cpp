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

#define W 1920/2
#define H 1080/2

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

	// Clamping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	// Interpolation
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Polygons
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	// Initialize Window
	glViewport(0, 0, W, H);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	// Volume data
	Volume volume(20, 20, 20);
	
	do
	{
		time += 0.1f;
		rotator.poll(window);

		// Volume data
		volume.bindBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
		
		for(int i = 0; i < volume.getResolution().x; i++){
			for(int j = 0; j < volume.getResolution().y; j++){
				for(int k = 0; k < volume.getResolution().z; k++){
					volume.drawData(i, j, k, glm::vec4(0,0,0,1));
					
					float v1 = rand() % 100;
					if(v1 < 30){
						volume.drawData(i, j, k, glm::vec4(v1/100,0,0,1));
					}
				}
			}
		}
		volume.drawData(0, 0, 0, glm::vec4(0.0,0,1,1));
		volume.drawData(19, 0, 0, glm::vec4(0.3,0,1,1));
		volume.drawData(0, 0, 19, glm::vec4(0.6,0,1,1));
		volume.drawData(19, 0, 19, glm::vec4(0.9,0,1,1));
	
		volume.drawData(0, 19, 19, glm::vec4(0.0,1,0,1));
		volume.drawData(19, 19, 19, glm::vec4(0.3,1,0,1));
		volume.drawData(0, 19, 0, glm::vec4(0.6,1,0,1));
		volume.drawData(19, 19, 0, glm::vec4(0.9,1,0,1));
	
		volume.drawData(10, 10, 10, glm::vec4(1,1,1,1));
		
		
		// Ray marcher
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
		
		screen_shader();
		glViewport(0,0,W,H);
		
		screen_shader.updateCommonUniforms(rotator, W, H, time);
		screenLoc = glGetUniformLocation(screen_shader, "volumeTexture");
		glUniform1i(screenLoc, 0);
		glActiveTexture(GL_TEXTURE0);
		volume.bindTexture();
		GLint volumeResolution_Loc = glGetUniformLocation(screen_shader, "volumeResolution");
		glUniform3fv(volumeResolution_Loc, 1, &volume.getResolution()[0]);
		
		quad.draw();


		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
			 glfwWindowShouldClose(window) == 0);

	glDisableVertexAttribArray(0);

	return 0;
}
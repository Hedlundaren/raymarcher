// External includes
#include <iostream>
#include <ctime>
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
	Volume volume(100, 100, 100);
	// Volume data
	// volume.bindBuffer();
	// glClear(GL_COLOR_BUFFER_BIT); 

	
	const char *title = "Loading data...";
	glfwSetWindowTitle(window, title);
	float TOTAL_ITERATIONS = volume.getResolution().x * volume.getResolution().y;
	
	volume.bindTexture();
	glActiveTexture(GL_TEXTURE0);
	volume.bindTexture();
	for(int i = 0; i < volume.getResolution().x; i++){
		for(int j = 0; j < volume.getResolution().y; j++){
			for(int k = 0; k < volume.getResolution().z; k++){
				volume.drawData(i, j, k, glm::vec4(0,0,0,0));
				
				float v1 = rand() % 100;
				if(v1 > 0){
					volume.drawData(i, j, k, glm::vec4(v1/100,0,0,0.6));
				}

				
				glm::vec3 pos = glm::vec3(i,j,k);
				glm::vec3 middle = glm::vec3(50.0);
				if(length(middle - pos) < 30.0){
					volume.drawData(i, j, k, glm::vec4(0,1,0,1));
				}

				if(abs(middle.x - pos.x) > middle.x * 0.9 || 
					abs(middle.y - pos.y) > middle.y * 0.9 || 
					abs(middle.z - pos.z) > middle.z * 0.9){
					volume.drawData(i, j, k, glm::vec4(0,0,1,1));
				}
			}

			float percentage = 100.0 * (j + volume.getResolution().y * i) / TOTAL_ITERATIONS;
			std::cout << (percentage) <<  "% \r";
		}
	}
	std::cout << "Loading complete." << std::endl;
	
	volume.drawData(0, 0, 0, glm::vec4(0.0,0,1,1));
	volume.drawData(99, 0, 0, glm::vec4(0.3,0,1,1));
	volume.drawData(0, 0, 99, glm::vec4(0.6,0,1,1));
	volume.drawData(99, 0, 99, glm::vec4(0.9,0,1,1));
	
	volume.drawData(0, 99, 99, glm::vec4(0.0,1,0,1));
	volume.drawData(99, 99, 99, glm::vec4(0.3,1,0,1));
	volume.drawData(0, 99, 0, glm::vec4(0.6,1,0,1));
	volume.drawData(99, 99, 0, glm::vec4(0.9,1,0,1));

	volume.drawData(50, 50, 50, glm::vec4(1,1,1,1));
	
	clock_t current_ticks, delta_ticks;
	clock_t fps = 0;
	
	do
	{
		current_ticks = clock();
		time += 0.1f;
		rotator.poll(window);

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


		delta_ticks = clock() - current_ticks; //the time, in ms, that took to render the scene
		
		fps = CLOCKS_PER_SEC / delta_ticks;
		std::string test = "Marching time " + std::to_string(fps);
		const char *title = test.c_str();
		glfwSetWindowTitle(window, title);

		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
			 glfwWindowShouldClose(window) == 0);

	glDisableVertexAttribArray(0);

	return 0;
}
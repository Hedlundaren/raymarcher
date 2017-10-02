#pragma once

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#define PI 3.1415926
const glm::vec3 RED = glm::vec3(1.0, 0.0, 0.0);


class Sphere
{
public:
	Sphere();
	Sphere(const int res_p, const int res_t, float r);
	~Sphere();

	void create_sphere();
	void draw();

protected:
	void make_face(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 face_color, glm::vec3 center, float phi, float theta);

private:

	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;

	const int res_phi, res_theta;
	float radius;
	GLuint VAO, VBO, IBO, normalbuffer, UVBO;

	float phi_stepSize, theta_stepSize;

};
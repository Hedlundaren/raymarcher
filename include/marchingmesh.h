#pragma once

#pragma once 

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "codebase.h"
#include "volume.h"

class MarchingMesh
{
public:
	MarchingMesh(const Volume data, glm::ivec3 gridRes = glm::ivec3(10,10,10), float *isoVal = 0);
	void createSurface();
	void draw();

protected:
	void createTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2);
	void createQuad(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);

private:

	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	GLuint VAO, VBO, IBO, normalbuffer, UVBO;
	glm::ivec3 gridResolution;
	Volume &data;
	float *isoValue;
};
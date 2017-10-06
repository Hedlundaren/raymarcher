#pragma once

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class ColorCube
{
  public:
    ColorCube();
    ~ColorCube();

    void create_cube();
    void draw();

  private:
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;

    GLuint VAO, VBO, IBO, NBO, UVBO;
};
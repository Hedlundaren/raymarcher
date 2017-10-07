#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "framebuffer.h"


class Volume{

public:
    Volume(const int &size);
    Volume(const int &x, const int &y, const int &z);

    void bindTexture();
    void bindBuffer();
    void drawData(const int &x, const int &y, const int &z, const glm::vec4 &v);
    void loadTestData();
    void loadData(std::string fileName);
    glm::vec4 readData(const int &x, const int &y, const int &z) const;
    glm::vec3 getResolution() const;

private:
    Framebuffer data;
    glm::vec3 resolution;
    
};
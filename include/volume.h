#pragma once


#include "glm/glm.hpp"
#include "framebufferfloat.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "ddsbase.h"
#include <stdlib.h>

class Volume{

public:
    Volume(const int &size);
    Volume(const int &x, const int &y, const int &z);

    void bindTexture();
    void bindBuffer();
    void drawData(const int &x, const int &y, const int &z, const float &v);
    void drawData(const int &z, const std::vector<float> &pixels);
    void loadTestData();

    void InitTextures3D();
    void loadDataPVM(std::string filePath);
    
    glm::vec4 readData(const int &x, const int &y, const int &z) const;
    glm::vec3 getResolution() const;
    std::vector<std::string> split(std::string line, const std::string &delimiter);        

private:
    FramebufferFloat data;
    glm::vec3 resolution;
    int originalResolution;
    
};
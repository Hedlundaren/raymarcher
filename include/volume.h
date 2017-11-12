#pragma once


#include "glm/glm.hpp"
#include "framebufferfloat.h"

#include <iostream>
#include <math.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "ddsbase.h"
#include <stdlib.h>

template<class T>
T data_ptr;

class Volume{

public:
    Volume() { };

    void bindTexture();
    void bindBuffer();
    void drawData(const int &x, const int &y, const int &z, const float &v);
    void drawData(const int &z, const std::vector<float> &pixels, const int &dimx, const int &dimy);
    
    void InitTextures3D();
    void loadTestData(const unsigned X, const unsigned Y, const unsigned Z);
    void loadDataPVM(std::string filePath);
    
    float readData(glm::vec3 p) const;
    float readData(const int &x, const int &y, const int &z) const;
    glm::vec3 getResolution() const;
    std::vector<std::string> split(std::string line, const std::string &delimiter);        

private:
    FramebufferFloat data;
    glm::ivec3 resolution;
};
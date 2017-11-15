#pragma once
#include <iostream>
#include <vector>
#include<fstream>
#include<sstream>
#include "glm/glm.hpp"

class TransferFunctionManager{

public:
    TransferFunctionManager();
    void load(std::string filePath, std::vector<glm::vec4> &p, std::vector<glm::vec4> &v, int &activeCps);
    void save(std::string filePath, const std::vector<glm::vec4> &p, const std::vector<glm::vec4> &v, const int &activeCps);
private:

};

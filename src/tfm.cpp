#include "tfm.h"

TransferFunctionManager::TransferFunctionManager() {}
void TransferFunctionManager::load(std::string filePath, std::vector<glm::vec4> &p, std::vector<glm::vec4> &v, int &activeCps)
{
    p.clear();
    v.clear();
    activeCps = 0;

    std::ifstream file;
    file.open(filePath);

    if (file.is_open())
    {
        std::string line;

        while (getline(file, line))
        {
            std::stringstream ss;
            ss << line;

            std::string type;
            ss >> type;

            if (type == "p")
            {
                activeCps++;
                glm::vec4 position;
                ss >> position.x;
                ss >> position.y;
                position.z = 0.0;
                position.a = 0.0;
                p.push_back(position);
            }
            else if (type == "v")
            {
                glm::vec4 value;
                ss >> value.x;
                ss >> value.y;
                ss >> value.z;
                ss >> value.a;
                v.push_back(value);
            }
            else
                continue;
        }

        if (activeCps < 2)
        {
            // Test data
            v.clear();
            p.clear();

            v.push_back(glm::vec4(0.5, 0.5, 0.5, 1));
            v.push_back(glm::vec4(0.5, 0.5, 0.5, 1));

            // Test data
            p.push_back(glm::vec4(0.0, 0.5, 0, 0));
            p.push_back(glm::vec4(1.0, 0.5, 0, 0));

            save(filePath, p, v, activeCps);
        }
    }
}

void TransferFunctionManager::save(std::string filePath, const std::vector<glm::vec4> &p, const std::vector<glm::vec4> &v, const int &activeCps)
{
    std::ofstream out(filePath);

    for (int i = 0; i < activeCps; i++)
    {

        std::string line = "v " + std::to_string(v[i].x) + " " + std::to_string(v[i].y) + " " + std::to_string(v[i].z) + " " + std::to_string(v[i].a) + "\n";
        out << line;
        line = "p " + std::to_string(p[i].x) + " " + std::to_string(p[i].y) + " 0 0\n";
        out << line;
    }
    out.close();
}

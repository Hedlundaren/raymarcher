#include "gui.h"

GUI::GUI(const float &w, const float &h)
    : resolution(glm::ivec2(w, h))
{
    initControlPoints();
}

void GUI::bindControlPointValueTexture()
{
    controlPointValueBuffer.bindTexture();
}

void GUI::bindControlPointPositionTexture()
{
    controlPointPositionBuffer.bindTexture();
}

void GUI::drawData(const std::vector<glm::vec4> &pixels, Framebuffer &buffer)
{
    buffer.bindTexture();
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, numberOfControlPoints, 1, GL_RGBA, GL_FLOAT, pixels.data());
}

void GUI::initControlPoints()
{
    std::vector<glm::vec4> pixels(numberOfControlPoints);

    for (auto cp : pixels)
    {
        cp = glm::vec4(0.0);
    }

    // Test data
    pixels[0] = glm::vec4(1, 0, 0, 1);
    pixels[1] = glm::vec4(0, 1, 0, 1);
    pixels[2] = glm::vec4(0, 0, 1, 1);
    pixels[3] = glm::vec4(1, 1, 1, 1);

    drawData(pixels, controlPointValueBuffer);

    std::vector<glm::vec4> pixels2(numberOfControlPoints);
    
    for (auto cp : pixels2)
    {
        cp = glm::vec4(0.0);
    }

    // Test data
    pixels2[0] = glm::vec4(0.1, 0.9, 0, 1);
    pixels2[1] = glm::vec4(0.2, 0.2, 0, 1);
    pixels2[2] = glm::vec4(0.4, 0.5, 1, 1);
    pixels2[3] = glm::vec4(0.9, 0.1, 1, 1);

    drawData(pixels2, controlPointPositionBuffer);
}

float GUI::getNumberOfControlPoints()
{
    return (float)numberOfControlPoints;
}

void GUI::update(GLFWwindow *&window)
{

    int alt = glfwGetKey(window, GLFW_KEY_LEFT_ALT);
    int ctrl = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL);
    int esc = glfwGetKey(window, GLFW_KEY_ESCAPE);
    int S = glfwGetKey(window, GLFW_KEY_S);

    if (ctrl && S)
        std::cout << "Saved.\n";
}

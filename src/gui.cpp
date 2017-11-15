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

    for (int id = 0; id < numberOfControlPoints; id++)
    {
        controlPointPositions.push_back(glm::vec4(0));
        controlPointValues.push_back(glm::vec4(0));
    }

    // Test data
    controlPointValues[0] = glm::vec4(1, 0, 0, 1);
    controlPointValues[1] = glm::vec4(0, 1, 0, 1);
    controlPointValues[2] = glm::vec4(0, 0, 1, 1);
    controlPointValues[3] = glm::vec4(1, 1, 1, 1);

    drawData(controlPointValues, controlPointValueBuffer);

    // Test data
    controlPointPositions[0] = glm::vec4(0.1, 0.9, 0, 1);
    controlPointPositions[1] = glm::vec4(0.2, 0.2, 0, 1);
    controlPointPositions[2] = glm::vec4(0.4, 0.5, 1, 1);
    controlPointPositions[3] = glm::vec4(0.9, 0.1, 1, 1);

    drawData(controlPointPositions, controlPointPositionBuffer);
}

float GUI::getNumberOfControlPoints()
{
    return (float)numberOfControlPoints;
}

float GUI::getSelectedControlPoint()
{
    return (float)selectedControlPoint;
}

float GUI::getHoveredControlPoint()
{
    return (float)hoveredControlPoint;
}

glm::vec2 GUI::getCursorPos()
{
    return cursorPos;
}

void GUI::update(GLFWwindow *&window)
{

    // int alt = glfwGetKey(window, GLFW_KEY_LEFT_ALT);
    // int esc = glfwGetKey(window, GLFW_KEY_ESCAPE);
    int currentLeft = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    int ctrl = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL);
    int S = glfwGetKey(window, GLFW_KEY_S);

    if (ctrl && S)
        std::cout << "Saved.\n";

    double cursorX, cursorY;
    glfwGetCursorPos(window, &cursorX, &cursorY);
    cursorPos.x = cursorX / (float)resolution.x;
    cursorPos.y = cursorY / (float)resolution.y;
    cursorPosTF.x = cursorPos.x;
    cursorPosTF.y = (1.0 - cursorPos.y) / 0.3;

    bool hoveringControlPoint = false;
    for (int id = 0; id < numberOfControlPoints; id++)
    {
        if (length(cursorPosTF - glm::vec2(controlPointPositions[id].x, controlPointPositions[id].y)) < 0.05)
        {
            hoveredControlPoint = id;
            hoveringControlPoint = true;
            if (currentLeft)
                selectedControlPoint = id;
        }
    }
    if (!hoveringControlPoint)
    {
        hoveredControlPoint = -1;
        if (currentLeft){
            selectedControlPoint = -1;
        }
    }

    std::cout << std::flush;
    std::cout << selectedControlPoint << "\n";

    drawData(controlPointValues, controlPointValueBuffer);
    drawData(controlPointPositions, controlPointPositionBuffer);
}

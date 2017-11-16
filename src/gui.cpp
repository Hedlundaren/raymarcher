#include "gui.h"

GUI::GUI(const float &w, const float &h)
    : resolution(glm::ivec2(w, h))
{
    initControlPoints();
}

float GUI::isActive()
{
    return guiActive;
}

float GUI::isColorPickActive()
{
    return guiColorPickActive;
}

bool GUI::isDraggedFun()
{
    return isDragged;
}

glm::vec4 GUI::readColorData(const int &x, const int &y)
{
    std::vector<glm::vec4> pixels(1);
    glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, pixels.data());
    return pixels[0];
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

    for (int id = 0; id < numberOfActiveControlPoints; id++)
    {
        controlPointPositions.push_back(glm::vec4(0));
        controlPointValues.push_back(glm::vec4(0));
    }

    // Test data
    controlPointValues[0] = glm::vec4(0.5, 0.5, 0.5, 1);
    controlPointValues[1] = glm::vec4(0.5, 0.5, 0.5, 1);

    drawData(controlPointValues, controlPointValueBuffer);

    // Test data
    controlPointPositions[0] = glm::vec4(0.0, 0.5, 0, 0);
    controlPointPositions[1] = glm::vec4(1.0, 0.5, 0, 0);

    drawData(controlPointPositions, controlPointPositionBuffer);
}

float GUI::getNumberOfControlPoints()
{
    return (float)numberOfControlPoints;
}

float GUI::getNumberOfActiveControlPoints()
{
    return (float)numberOfActiveControlPoints;
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

glm::vec2 GUI::getCursorPosTF()
{
    return cursorPosTF;
}

void GUI::deleteControlPoint(int id)
{
    if (id > 0 && id < (numberOfActiveControlPoints - 1.0))
    {
        controlPointValues.erase(controlPointValues.begin() + id);
        controlPointPositions.erase(controlPointPositions.begin() + id);
        numberOfActiveControlPoints--;
    }
}

bool GUI::onLine()
{
    bool onLine = false;
    for (float id = 1; id < numberOfActiveControlPoints; id++)
    {
        glm::vec2 currPoint = cursorPosTF;
        glm::vec2 point1 = glm::vec2(controlPointPositions[id - 1].x, controlPointPositions[id - 1].y);
        glm::vec2 point2 = glm::vec2(controlPointPositions[id].x, controlPointPositions[id].y);
        if (length(point1 - currPoint) + length(currPoint - point2) - length(point1 - point2) < EPSILON)
            onLine = true;
    }

    return onLine;
}

void GUI::addControlPoint()
{

    std::vector<glm::vec4> tempPos;
    std::vector<glm::vec4> tempVal;
    tempPos.clear();
    tempVal.clear();
    // We will decide where to add controlPoint
    for (float id = 0; id < numberOfActiveControlPoints; id++)
    {

        tempPos.push_back(controlPointPositions[id]);
        tempVal.push_back(controlPointValues[id]);

        if (controlPointPositions[id].x < cursorPosTF.x && controlPointPositions[id + 1].x > cursorPosTF.x)
        {
            numberOfActiveControlPoints++;
            tempPos.push_back(glm::vec4(cursorPosTF.x, cursorPosTF.y, 0.0, 0.0));
            tempVal.push_back(glm::vec4(1.0, 0.0, 0.0, 1.0));
        }
    }

    controlPointPositions = tempPos;
    controlPointValues = tempVal;

    // std::cout << std::flush;
    // if(onLine) std::cout << "----";
    // else std::cout << ":";
}

void GUI::update(GLFWwindow *&window, Framebuffer &colorPickBuffer)
{

    timeSinceInteraction = glfwGetTime() - timeAtInteraction;

    int alt = glfwGetKey(window, GLFW_KEY_LEFT_ALT);
    int currentLeft = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    int currentRight = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
    int ctrl = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL);
    int O = glfwGetKey(window, GLFW_KEY_O);
    int S = glfwGetKey(window, GLFW_KEY_S);
    int H = glfwGetKey(window, GLFW_KEY_H);
    int C = glfwGetKey(window, GLFW_KEY_C);
    int n1 = glfwGetKey(window, GLFW_KEY_1);
    int n2 = glfwGetKey(window, GLFW_KEY_2);
    int n3 = glfwGetKey(window, GLFW_KEY_3);
    int n4 = glfwGetKey(window, GLFW_KEY_4);
    int n5 = glfwGetKey(window, GLFW_KEY_5);
    int n6 = glfwGetKey(window, GLFW_KEY_6);
    int n7 = glfwGetKey(window, GLFW_KEY_7);
    int n8 = glfwGetKey(window, GLFW_KEY_8);
    int n9 = glfwGetKey(window, GLFW_KEY_9);
    int n0 = glfwGetKey(window, GLFW_KEY_0);
    std::string fileToLoad = "-1";
    if (n1)
        fileToLoad = "1";
    if (n2)
        fileToLoad = "2";
    if (n3)
        fileToLoad = "3";
    if (n4)
        fileToLoad = "4";
    if (n5)
        fileToLoad = "5";
    if (n6)
        fileToLoad = "6";
    if (n7)
        fileToLoad = "7";
    if (n8)
        fileToLoad = "8";
    if (n9)
        fileToLoad = "9";
    if (n0)
        fileToLoad = "0";
    activeTF = fileToLoad;
    // Load
    if (alt && fileToLoad != "-1" && timeSinceInteraction > maxTimeBetweenInteractions)
    {
        TransferFunctionManager tfm;
        tfm.load("tfs/" + fileToLoad + ".tf", controlPointPositions, controlPointValues, numberOfActiveControlPoints);
        timeAtInteraction = glfwGetTime();
        std::cout << "tf" + fileToLoad + " loaded.\n";
    }

    // Save
    if (ctrl && S && activeTF != "-1" && timeSinceInteraction > maxTimeBetweenInteractions)
    {
        timeAtInteraction = glfwGetTime();
        TransferFunctionManager tfm;
        tfm.save("tfs/" + fileToLoad + ".tf", controlPointPositions, controlPointValues, numberOfActiveControlPoints);
        std::cout << "tf" + fileToLoad + " saved.\n";
    }

    if (ctrl && O && timeSinceInteraction > maxTimeBetweenInteractions)
    {
        std::cout << "Enter filename (load): ";
        timeAtInteraction = glfwGetTime();
        std::string fileName;
        std::cin >> fileName;
        TransferFunctionManager tfm;
        tfm.load("tfs/" + fileName + ".tf", controlPointPositions, controlPointValues, numberOfActiveControlPoints);
        std::cout << "\nLoad complete.\n";
        activeTF = "tfs/" + fileName + ".tf";
    }

    // Save
    if (ctrl && alt && S && timeSinceInteraction > maxTimeBetweenInteractions)
    {
        std::cout << "Enter filename (save): ";
        timeAtInteraction = glfwGetTime();
        std::string fileName;
        std::cin >> fileName;
        TransferFunctionManager tfm;
        tfm.save("tfs/" + fileName + ".tf", controlPointPositions, controlPointValues, numberOfActiveControlPoints);
        std::cout << "\nTransfer function saved.\n";
    }

    // Pick Color
    if (C && timeSinceInteraction > maxTimeBetweenInteractions)
    {
        timeAtInteraction = glfwGetTime();
        if (guiColorPickActive)
            guiColorPickActive = 0.0;
        else
            guiColorPickActive = 1.0;
    }

    // Hide GUI
    if (H && timeSinceInteraction > maxTimeBetweenInteractions)
    {
        timeAtInteraction = glfwGetTime();
        if (guiActive)
            guiActive = 0.0;
        else
            guiActive = 1.0;
    }

    if (guiActive)
    {

        double cursorX, cursorY;
        glfwGetCursorPos(window, &cursorX, &cursorY);
        cursorPos.x = cursorX / (float)resolution.x;
        cursorPos.y = cursorY / (float)resolution.y;
        cursorPosTF.x = cursorPos.x;
        cursorPosTF.y = (1.0 - cursorPos.y) / 0.3;
        cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);

        if (!guiColorPickActive)
        {

            bool hoveringControlPoint = false;
            for (int id = 0; id < numberOfControlPoints; id++)
            {
                if (length(cursorPosTF - glm::vec2(controlPointPositions[id].x, controlPointPositions[id].y)) < 0.05)
                {
                    hoveredControlPoint = id;
                    hoveringControlPoint = true;
                    if (!isDragged)
                    {
                        cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
                        if (currentLeft)
                        {
                            selectedControlPoint = id;
                            isDragged = true;
                        }
                    }
                }
            }

            // Move control point
            if (currentLeft && isDragged && selectedControlPoint > 0 && selectedControlPoint < numberOfActiveControlPoints - 1)
            {
                float offset = 0.001;
                controlPointPositions[selectedControlPoint] = glm::vec4(cursorPosTF.x, cursorPosTF.y, 0.0, 0.0);

                if (controlPointPositions[selectedControlPoint - 1].x > controlPointPositions[selectedControlPoint].x)
                {
                    controlPointPositions[selectedControlPoint] = glm::vec4(controlPointPositions[selectedControlPoint - 1].x + offset, cursorPosTF.y, 0.0, 0.0);
                }

                if (controlPointPositions[selectedControlPoint + 1].x < controlPointPositions[selectedControlPoint].x)
                {
                    controlPointPositions[selectedControlPoint] = glm::vec4(controlPointPositions[selectedControlPoint + 1].x - offset, cursorPosTF.y, 0.0, 0.0);
                }

                if (controlPointPositions[selectedControlPoint].x < 0.0)
                {
                    controlPointPositions[selectedControlPoint] = glm::vec4(offset, cursorPosTF.y, 0.0, 0.0);
                }
                if (controlPointPositions[selectedControlPoint].x > resolution.x)
                {
                    controlPointPositions[selectedControlPoint] = glm::vec4(resolution.x - offset, cursorPosTF.y, 0.0, 0.0);;
                }
                if (controlPointPositions[selectedControlPoint].y < 0.0)
                {
                    controlPointPositions[selectedControlPoint] = glm::vec4(cursorPosTF.x, offset, 0.0, 0.0);;
                }
                if (controlPointPositions[selectedControlPoint].y > resolution.y)
                {
                    controlPointPositions[selectedControlPoint] = glm::vec4(cursorPosTF.x, resolution.y - offset, 0.0, 0.0);;
                }

            }
            else
            {
                isDragged = false;
            }

            if (!hoveringControlPoint)
            {
                hoveredControlPoint = -1;
                if (onLine())
                {
                    cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
                }
                if (!isDragged && currentLeft && timeSinceInteraction > maxTimeBetweenInteractions)
                {
                    selectedControlPoint = -1;
                    if (onLine())
                    {
                        addControlPoint();
                    }
                    timeAtInteraction = glfwGetTime();
                }
            }
            else
            {
                if (currentRight && timeSinceInteraction > maxTimeBetweenInteractions)
                {
                    deleteControlPoint(hoveredControlPoint);
                    timeAtInteraction = glfwGetTime();
                }
            }

            // std::cout << std::flush;
            // std::cout << selectedControlPoint << "\n";

            glfwSetCursor(window, cursor);
        }
        else
        {
            if (currentLeft && selectedControlPoint > -1)
            {
                colorPickBuffer.bindTexture();
                glm::vec4 color = readColorData(cursorX, resolution.y - cursorY);
                controlPointValues[selectedControlPoint] = color;
                // std::cout << std::flush;
                // std::cout << "cursor: " << cursorX << ", " << cursorY << "\n";
                // std::cout << "color: " << color.x<< ", " << color.y << ", " << color.z << "\n";
            }
        }
    }

    drawData(controlPointValues, controlPointValueBuffer);
    drawData(controlPointPositions, controlPointPositionBuffer);
}

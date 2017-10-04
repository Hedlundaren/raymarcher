#include "clock.h"

Clock::Clock(GLFWwindow *w) : window(w){

}

void Clock::start(){
    current_ticks = clock();
    time += 0.1;
}

void Clock::stop(){
    current_ticks = clock();

    // delta_ticks = clock() - current_ticks; //the time, in ms, that took to render the scene
    // fps = CLOCKS_PER_SEC / delta_ticks;
    std::string test = "Marching time ";// + std::to_string(fps);
    const char *title = test.c_str();
    glfwSetWindowTitle(window, title);
}

float Clock::getTime(){
    return time;
}


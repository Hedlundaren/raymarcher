#include "clock.h"

Clock::Clock(GLFWwindow *w) : window(w)
{
}

double Clock::clockToMilliseconds(clock_t ticks)
{
    return (ticks / (double)CLOCKS_PER_SEC) * 1000.0;
}

void Clock::tic()
{
    time += 0.1;
    beginFrame = clock();
}

void Clock::toc()
{
    endFrame = clock();
    deltaTime += endFrame - beginFrame;
    frames++;

    if (deltaTime > 1000.0)
    {  

        deltaTime = 0;
        frames = 0;
    }
    
}

float Clock::getTime()
{
    return time;
}

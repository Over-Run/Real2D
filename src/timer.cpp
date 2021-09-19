#include "real2d/timer.h"

#include "GLFW/glfw3.h"

using namespace Real2D;

Timer::Timer(double tps_) : tps(tps_) {
    lastTime = glfwGetTime() * NS_PER_SECOND;
}
void Timer::advanceTime() {
    double now = glfwGetTime() * NS_PER_SECOND;;
    double passedNs = now - lastTime;
    lastTime = now;
    if (passedNs < 0) {
        passedNs = 0;
    }
    else if (passedNs > MAX_NS_PER_UPDATE) {
        passedNs = MAX_NS_PER_UPDATE;
    }
    lastFps = MAX_NS_PER_UPDATE / passedNs;
    passedTime += passedNs * tps / NS_PER_SECOND;
    ticks = (int)passedTime;
    if (ticks > MAX_TICKS_PER_UPDATE) {
        ticks = MAX_TICKS_PER_UPDATE;
    }
    passedTime -= ticks;
    delta = passedTime;
}

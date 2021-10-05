#include "real2d/timer.h"

#include "GLFW/glfw3.h"

using Real2D::Timer;

Timer::Timer(double tps_) : tps(tps_) {
    lastTime = (__int64)(glfwGetTime() * NS_PER_SECOND);
}
void Timer::advanceTime() {
    __int64 now = (__int64)(glfwGetTime() * NS_PER_SECOND);
    __int64 passedNs = now - lastTime;
    lastTime = now;
    if (passedNs < 0) {
        passedNs = 0;
    }
    else if (passedNs > MAX_NS_PER_UPDATE) {
        passedNs = MAX_NS_PER_UPDATE;
    }
    fps = (double)(NS_PER_SECOND / passedNs);
    passedTime += passedNs * tps / NS_PER_SECOND;
    ticks = (int)passedTime;
    if (ticks > MAX_TICKS_PER_UPDATE) {
        ticks = MAX_TICKS_PER_UPDATE;
    }
    passedTime -= ticks;
    delta = passedTime;
}

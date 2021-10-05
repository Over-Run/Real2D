#pragma once

constexpr auto NS_PER_SECOND = 1000000000LL;
constexpr auto MAX_NS_PER_UPDATE = 1000000000LL;
constexpr auto MAX_TICKS_PER_UPDATE = 100;

namespace Real2D {
    class Timer {
    private:
        const double tps;
        __int64 lastTime;
    public:
        int ticks = 0;
        double delta = 0;
        double fps = 0;
        double passedTime = 0;

        Timer(double tps_);
        void advanceTime();
    };
}

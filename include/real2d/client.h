#pragma once

constexpr auto GAME_VER = "0.3.0";

namespace Real2D {
    class Client {
    public:
        void start();
        void run();
        void render();
        void tick(double delta);
        ~Client();
    };
}

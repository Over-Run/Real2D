#pragma once

namespace Real2D {
    class Client {
    public:
        void start();
        void run();
        void render(double delta);
        void tick();
        ~Client();
    };
}

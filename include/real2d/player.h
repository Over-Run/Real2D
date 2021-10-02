#pragma once

constexpr const char* TEX_PLAYER = "res/player.png";
constexpr float TEX_PLAYER_W = 64.0f;
constexpr float TEX_PLAYER_H = 64.0f;
constexpr int PLAYER_ROT_R = 0;
constexpr int PLAYER_ROT_F = 1;
constexpr int PLAYER_ROT_L = 2;
constexpr int PLAYER_ROT_B = 3;

namespace Real2D {
    struct Player {
        const float height;
        const float basestep;
        float x;
        float y;
        float z;
        int rotate;

        Player();
        void moveRelative(float xom, float yom, float zom);
        void tick(double delta);
        void render();
    };
}

extern Real2D::Player player;

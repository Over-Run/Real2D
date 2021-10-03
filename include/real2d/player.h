#pragma once
#include "aabb.h"

constexpr const char* TEX_PLAYER = "res/player.png";
constexpr float TEX_PLAYER_W = 48.0f;
constexpr float TEX_PLAYER_H = 64.0f;

extern float headXRot;
extern float headYRot;

namespace Real2D {
    struct Player {
        const float height;
        const float speed;
        Real2D::AABBox* bb;
        float x;
        float y;
        float z;

        Player();
        void moveRelative(float xom, float yom, float zom);
        void tick(double delta);
        void render();
    };
}

extern Real2D::Player player;

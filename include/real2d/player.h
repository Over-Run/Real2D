#pragma once
#include "aabb.h"

extern float headXRot;
extern float headYRot;

namespace Real2D {
    class World;
    struct Player {
        const float height;
        World* world;
        Real2D::AABBox bb;
        float x, y, z;
        float xo, yo;
        bool onGround;

        Player(World* _world);
        void move(float xa, float ya, float speed);
        void tick();
        void render(double delta);
    };
}

extern Real2D::Player* player;

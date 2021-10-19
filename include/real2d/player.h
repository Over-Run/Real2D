#pragma once
#include "aabb.h"

extern float headXRot;
extern float headYRot;

namespace Real2D {
    class World;
    class Player {
    public:
        const float bb_width;
        const float bb_height;
        World* world;
        float x, y, z;
        float prev_x, prev_y;
        AABBox bb;
        bool onGround;

        Player(World* _world);
        void move(float xa, float ya, float speed);
        void tick();
        void render(double delta);
    };
}

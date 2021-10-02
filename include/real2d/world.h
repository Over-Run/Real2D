#pragma once
#include "glad/gl.h"
#include "real2d/block.h"

#define WORLD_BLOCK(x,y,z) (x + y * WORLD_W + z * WORLD_W * WORLD_H)

constexpr int WORLD_W = 32;
constexpr int WORLD_H = 16;
constexpr int WORLD_D = 2;
constexpr int WORLD_SIZE = WORLD_W * WORLD_H * WORLD_D;

extern block_t choosingBlock;

extern int selectx;
extern int selecty;
extern int selectz;
extern GLfloat selectbx;
extern GLfloat selectbx1;
extern GLfloat selectby;
extern GLfloat selectby1;
extern block_t* selectblock;

namespace Real2D {
    class World {
    public:
        block_t* world;

        World();
        ~World();
        void create();
        void render();
        void select();
        void renderSelect();
    };
}

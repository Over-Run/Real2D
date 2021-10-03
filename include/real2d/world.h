#pragma once
#include "glad/gl.h"
#include "real2d/block.h"

constexpr int WORLD_W = 48;
constexpr int WORLD_H = 32;
constexpr int WORLD_D = 2;
constexpr int WORLD_SIZE = WORLD_W * WORLD_H * WORLD_D;

extern block_t choosingBlock;

extern int selectz;

namespace Real2D {
    class World {
    public:
        block_t* world;
        bool is_dirty;

        World();
        ~World();
        void create();
        void render();
        void select();
        void renderSelect();
        void markDirty();
        bool isDirty();
        block_t& getBlock(int x, int y, int z);
        void setBlock(int x, int y, int z, block_t block);
    };
}

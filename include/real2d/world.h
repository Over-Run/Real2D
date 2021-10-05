#pragma once
#include "glad/gl.h"
#include "block.h"
#include <vector>

extern Real2D::block_t choosingBlock;

extern int selectz;

namespace Real2D {
    class World {
    public:
        block_t* world;
        bool is_dirty;

        World();
        ~World();
        void create();
        void render(double delta);
        void select();
        void renderSelect(double delta);
        void tick();
        std::vector<AABBox> getCubes(AABBox box);
        void markDirty();
        bool isDirty();
        block_t& getBlock(int x, int y, int z);
        void setBlock(int x, int y, int z, block_t block);
    };
}

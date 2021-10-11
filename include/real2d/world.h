#pragma once
#include "block.h"
#include "glad/gl.h"
#include <vector>

extern Real2D::block_t choosingBlock;
extern int selectz;

namespace Real2D {
    class Player;
    class WorldListener {};
    class World {
    public:
        int version;
        block_t* world;
        Player* player;
        bool is_dirty;
        std::vector<WorldListener*> listeners;

        World();
        ~World();
        void create();
        void render(double delta);
        void select();
        void renderSelect(double delta);
        void tick();
        void addListener(WorldListener* listener);
        std::vector<AABBox> getCubes(AABBox box);
        void markDirty();
        bool isDirty();
        block_t& getBlock(int x, int y, int z);
        void setBlock(int x, int y, int z, block_t block);
        void save();
        bool load();
    };
}

#pragma once
#include "block.h"
#include "hit.h"
#include "glad/gl.h"
#include "real2d_def.h"
#include <vector>

extern Real2D::Block* choosingBlock;
extern int selectz;
extern Real2D::HitResult* hit_result;

namespace Real2D {
    class Player;
    class WorldListener {
    public:
        virtual void blockChanged(int x, int y, int z) = 0;
        virtual void lightColChanged(int x, int z) = 0;
    };
    class World {
    public:
        int version;
        Player* player;
        std::vector<WorldListener*>* listeners;
        int lights[WORLD_SIZE];
        Block world[WORLD_SIZE];

        World();
        ~World();
        void create();
        void tick();
        void addListener(WorldListener* listener);
        void calcLights(int x, int z);
        int getLight(int x, int y, int z);
        std::vector<AABBox> getCubes(AABBox box);
        Block& getBlock(int x, int y, int z);
        void setBlock(int x, int y, int z, Block* block);
        void save();
        bool load();
    };
}

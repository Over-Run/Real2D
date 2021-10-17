#include "real2d/player.h"
#include "real2d/world.h"
#include "real2d/window_c.h"
#include "real2d/real2d_def_c.h"
#include "real2d/reg.h"
#include "real2d/files.h"
#include <map>

#define WORLD_BLOCK_I(x,y,z) (x + y * WORLD_W + z * WORLD_W * WORLD_H)

using std::ifstream;
using std::ofstream;
using std::ios;
using std::endl;
using std::vector;
using std::map;
using std::string;
using Real2D::Block;
using Real2D::Blocks;
using Real2D::Player;
using Real2D::World;
using Real2D::AABBox;
using Real2D::HitResult;
using Real2D::Registries;

extern window_t window;

Block* choosingBlock = BLOCK(GRASS_BLOCK);

HitResult* hit_result = nullptr;
int selectz = 1;

bool isMouseDown(int button) {
    return glfwGetMouseButton(window, button);
}

World::World() :
    version(WORLD_VER),
    player(new Player(this)),
    listeners(new vector<WorldListener*>) {
    for (int i = 0; i < WORLD_SIZE; ++i) {
        world[i] = *AIR_BLOCK;
    }
    for (int i = 0; i < WORLD_SIZE; ++i) {
        lights[i] = 15;
    }
}
World::~World() {
    save();
    delete player;
    player = nullptr;
    delete listeners;
    listeners = nullptr;
}
void World::create() {
    if (!load()) {
        for (int z = 0; z < WORLD_D; ++z) {
            for (int x = 0; x < WORLD_W; ++x) {
                for (int y = 0; y < WORLD_H; ++y) {
                    Block* b = AIR_BLOCK;
                    if (y < 3) {
                        b = BLOCK(STONE);
                    }
                    else if (y == 3) {
                        b = BLOCK(GRASS_BLOCK);
                    }
                    setBlock(x, y, z, b);
                }
            }
        }
        save();
    }
    for (int z = 0; z < WORLD_D; ++z) {
        for (int x = 0; x < WORLD_W; ++x) {
            calcLights(x, z);
        }
    }
}

void World::tick() {
    if (hit_result != nullptr) {
        Block* b = hit_result->block;
        int x = hit_result->x;
        int y = hit_result->y;
        int z = hit_result->z;
        if (b == AIR_BLOCK) {
            if (isMouseDown(MBR)
                && choosingBlock != AIR_BLOCK) {
                setBlock(x, y, z, choosingBlock);
            }
        }
        else {
            if (isMouseDown(MBL)) {
                setBlock(x, y, z, AIR_BLOCK);
            }
            else if (isMouseDown(MBM)) {
                choosingBlock = b;
            }
        }
    }
    player->tick();
}

void World::addListener(WorldListener* listener) {
    listeners->push_back(listener);
}

void World::calcLights(int x, int z) {
    int light = 15;
    for (int y = WORLD_H; y >= 0; y--) {
        lights[WORLD_BLOCK_I(x, y, z)] = light;
        if (light > 0 && getBlock(x, y, z).isOpaque()) {
            --light;
        }
    }
}

int World::getLight(int x, int y, int z) {
    return lights[WORLD_BLOCK_I(x, y, z)];
}

vector<AABBox> World::getCubes(AABBox box) {
    vector<AABBox> cubes;
    int x0 = (int)box.start_x;
    int x1 = (int)(box.end_x + 1);
    int y0 = (int)box.start_y;
    int y1 = (int)(box.end_y + 1);
    if (x0 < 0) {
        x0 = 0;
    }
    if (y0 < 0) {
        y0 = 0;
    }
    if (x1 > WORLD_W) {
        x1 = WORLD_W;
    }
    if (y1 > WORLD_H) {
        y1 = WORLD_H;
    }
    for (int x = x0; x < x1; ++x) {
        for (int y = y0; y < y1; ++y) {
            Block& block = getBlock(x, y, 1);
            AABBox* cube = block.getCollision();
            if (cube != nullptr) {
                AABBox b;
                cube->move((float)x, (float)y, 1, &b);
                cubes.push_back(b);
            }
        }
    }
    return cubes;
}

Block& World::getBlock(int x, int y, int z) {
    if (x >= 0 && x < WORLD_W
        && y >= 0 && y < WORLD_H
        && z >= 0 && z < WORLD_D) {
        return world[WORLD_BLOCK_I(x, y, z)];
    }
    return *AIR_BLOCK;
}

void World::setBlock(int x, int y, int z, Block* block) {
    if (x >= 0 && x < WORLD_W
        && y >= 0 && y < WORLD_H
        && z >= 0 && z < WORLD_D) {
        if (world[WORLD_BLOCK_I(x, y, z)] == *block) {
            return;
        }
        world[WORLD_BLOCK_I(x, y, z)] = *block;
        calcLights(x, z);
        for (auto l : *listeners) {
            l->blockChanged(x, y, z);
        }
    }
}

void World::save() {
    createDir("saves/");
    ofstream level;
    level.open("saves/level.dat", ios::out | ios::trunc | ios::binary);
    if (!level.is_open()) {
        throw "Couldn't open level.dat";
    }
    stream_write(level, &version);

    stream_write(level, &WORLD_SIZE);

    int regsz = Registries::BLOCK->size();
    stream_write(level, &regsz);
    for (auto& e : Registries::BLOCK->rawIds()) {
        int& sec = e.second;
        string s = Registries::BLOCK->getSID(sec);
        int sz = (int)s.size() + 1;
        stream_write(level, &sec);
        stream_write(level, &sz);
        stream_write(level, s);
    }

    for (int i = 0; i < WORLD_SIZE; ++i) {
        int id = Registries::BLOCK->get(&world[i]);
        stream_write(level, &id);
    }

    stream_write(level, &player->x);
    stream_write(level, &player->y);
    stream_write(level, &player->z);
    level.close();
}
bool World::load() {
    createDir("saves/");
    ifstream level;
    level.open("saves/level.dat", ios::in | ios::binary);
    if (!level.is_open()) {
        return false;
    }
    stream_read(level, &version);
    int wrdsz;
    stream_read(level, &wrdsz);

    if (wrdsz > WORLD_SIZE) {
        wrdsz = WORLD_SIZE;
    }

    if (1 == version) {
        map<int, string> id_map;
        int regsz;
        stream_read(level, &regsz);
        for (int i = 0; i < regsz; ++i) {
            int rawId;
            int psz;
            string id;
            stream_read(level, &rawId);
            stream_read(level, &psz);
            stream_read(level, psz, &id);
            id_map[rawId] = id;
        }

        for (int i = 0; i < wrdsz; ++i) {
            int id;
            stream_read(level, &id);
            world[i] = *Registries::BLOCK->get(id_map[id]);
        }

        stream_read(level, &player->x);
        stream_read(level, &player->y);
        stream_read(level, &player->z);
        return true;
    }
    return false;
}

#include "real2d/texmgr_c.h"
#include "real2d/player.h"
#include "real2d/world.h"
#include "real2d/window_c.h"
#include "real2d/aabb.h"
#include "real2d/hit.h"
#include "real2d/real2d_def_c.h"
#include "real2d/reg.h"
#include "real2d/files.h"
#include <fstream>

#define WORLD_BLOCK_I(x,y,z) (x + y * WORLD_W + z * WORLD_W * WORLD_H)

using std::ifstream;
using std::ofstream;
using std::ios;
using std::endl;
using Real2D::Window;
using Real2D::block_t;
using Real2D::Blocks;
using Real2D::Player;
using Real2D::World;
using Real2D::AABBox;
using Real2D::HitResult;
using Real2D::Registries;

extern window_t window;

block_t choosingBlock = BLOCK(GRASS_BLOCK);

HitResult* hit_result = nullptr;
int selectz = 1;

GLuint list;
extern GLuint blocks;

bool isMouseDown(int button) {
    return glfwGetMouseButton(window, button);
}

World::World() :
    version(WORLD_VER),
    world(new block_t[WORLD_SIZE]),
    player(new Player(this)),
    is_dirty(true) {}
World::~World() {
    save();
    if (world != nullptr) {
        delete[] world;
    }
    if (player != nullptr) {
        delete player;
    }
    if (glDeleteLists) {
        glDeleteLists(list, 1);
    }
}
void World::create() {
    if (!load()) {
        for (int z = 0; z < WORLD_D; ++z) {
            for (int x = 0; x < WORLD_W; ++x) {
                for (int y = 0; y < WORLD_H; ++y) {
                    block_t b;
                    if (y < 3) {
                        b = BLOCK(STONE);
                    }
                    else if (y == 3) {
                        b = BLOCK(GRASS_BLOCK);
                    }
                    else {
                        b = AIR_BLOCK;
                    }
                    setBlock(x, y, z, b);
                }
            }
        }
    }
    list = glGenLists(1);
}
void World::render(double delta) {
    if (isDirty()) {
        glNewList(list, GL_COMPILE);
        glBegin(GL_QUADS);
        for (int z = 0; z < WORLD_D; ++z) {
            for (int x = 0; x < WORLD_W; ++x) {
                for (int y = 0; y < WORLD_H; ++y) {
                    block_t block = getBlock(x, y, z);
                    if (block != AIR_BLOCK) {
                        renderBlock(x, y, z, block);
                    }
                }
            }
        }
        glEnd();
        glEndList();
        is_dirty = false;
    }
}

void renderHit(double delta) {
    float x = (float)hit_result->x;
    float y = (float)hit_result->y;
    float z = (float)hit_result->z;
    AABBox bb;
    hit_result->block->getOutline()->move(x, y, z, &bb);
    GLfloat fz = UNML(bb.start_z);
    GLfloat fx = UNML(bb.start_x);
    GLfloat fx1 = UNML(bb.end_x);
    GLfloat fy = UNML(bb.start_y);
    GLfloat fy1 = UNML(bb.end_y);
    glBegin(GL_LINE_LOOP);
    glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
    glVertex3f(fx + 1, fy1, fz);
    glVertex3f(fx + 1, fy, fz);
    glVertex3f(fx1, fy, fz);
    glVertex3f(fx1, fy1 - 1, fz);
    glEnd();
}

void World::select() {
    int mx = Window::mouseX;
    int my = Window::height - Window::mouseY;
    bool selected = false;
    const GLfloat xo = X_OFFSET;
    const GLfloat yo = Y_OFFSET;
    for (int x = 0; x < WORLD_W; ++x) {
        for (int y = 0; y < WORLD_H; ++y) {
            block_t& block = getBlock(x, y, selectz);
            GLfloat bx = (GLfloat)UNML(x);
            GLfloat bx1 = bx + WORLD_RENDER_NML;
            GLfloat by = (GLfloat)UNML(y);
            GLfloat by1 = by + WORLD_RENDER_NML;
            GLfloat obx = bx + xo;
            GLfloat obx1 = bx1 + xo;
            GLfloat oby = by + yo;
            GLfloat oby1 = by1 + yo;
            if (mx >= obx
                && mx < obx1
                && my >= oby
                && my < oby1) {
                selected = true;
                hit_result = new HitResult(x, y, selectz, block);
                goto unloop;
            }
        }
    }
unloop:
    // Check outside world
    if (!selected) {
        delete hit_result;
        hit_result = nullptr;
    }
}
void World::renderSelect(double delta) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();
    glTranslatef(X_OFFSET, Y_OFFSET, 0);
    render(delta);
    texmgr.bindTexture(blocks);
    glCallList(list);
    texmgr.bindTexture(0);
    select();
    if (hit_result != nullptr) {
        renderHit(delta);
    }
    glPopMatrix();

    glDisable(GL_BLEND);

    player->render(delta);
}

void World::tick() {
    if (hit_result != nullptr) {
        block_t b = hit_result->block;
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

std::vector<AABBox> World::getCubes(AABBox box) {
    std::vector<AABBox> cubes;
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
            block_t block = getBlock(x, y, 1);
            AABBox* cube = block->getCollision();
            if (cube != nullptr) {
                AABBox b;
                cube->move((float)x, (float)y, 1, &b);
                cubes.push_back(b);
            }
        }
    }
    return cubes;
}

void World::markDirty() {
    is_dirty = true;
}

bool World::isDirty() {
    return is_dirty;
}

block_t& World::getBlock(int x, int y, int z) {
    if (x >= 0 && x < WORLD_W
        && y >= 0 && y < WORLD_H
        && z >= 0 && z < WORLD_D) {
        return world[WORLD_BLOCK_I(x, y, z)];
    }
    return AIR_BLOCK;
}

void World::setBlock(int x, int y, int z, block_t block) {
    if (x >= 0 && x < WORLD_W
        && y >= 0 && y < WORLD_H
        && z >= 0 && z < WORLD_D) {
        world[WORLD_BLOCK_I(x, y, z)] = block;
        markDirty();
    }
}

void stream_write(ofstream& os, int* i) {
    os.write((char*)i, sizeof(int));
}
void stream_write(ofstream& os, float* i) {
    os.write((char*)i, sizeof(float));
}
void stream_read(ifstream& is, int* i) {
    is.read((char*)i, sizeof(int));
}
void stream_read(ifstream& is, float* i) {
    is.read((char*)i, sizeof(float));
}
void World::save() {
    createDir("saves/");
    ofstream level;
    level.open("saves/level.dat", ios::out | ios::trunc | ios::binary);
    if (!level.is_open()) {
        throw "Couldn't open level.dat";
    }
    stream_write(level, &version);
    for (size_t i = 0; i < WORLD_SIZE; ++i) {
        int id = world[i]->getId();
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
    if (version >= 1 && version <= 1) {
        for (size_t i = 0; i < WORLD_SIZE; ++i) {
            int id;
            stream_read(level, &id);
            world[i] = Registries::BLOCK->get(id);
        }
        stream_read(level, &player->x);
        stream_read(level, &player->y);
        stream_read(level, &player->z);
        return true;
    }
    return false;
}

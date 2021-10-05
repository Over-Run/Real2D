#include "real2d/texmgr.h"
#include "real2d/player.h"
#include "real2d/world.h"
#include "real2d/window.h"
#include "real2d/aabb.h"
#include "real2d/hit.h"
#include <malloc.h>

#define WORLD_BLOCK_I(x,y,z) (x + y * WORLD_W + z * WORLD_W * WORLD_H)

using Real2D::Window;
using Real2D::block_t;
using Real2D::Blocks;
using Real2D::Player;
using Real2D::World;
using Real2D::AABBox;
using Real2D::HitResult;

extern window_t window;

block_t choosingBlock = BLOCK(GRASS_BLOCK);

HitResult* hit_result = nullptr;
int selectz = 1;

GLuint list;
extern GLuint blocks;

bool isMouseDown(int button) {
    return glfwGetMouseButton(window, button);
}

World::World() : world((block_t*)malloc(sizeof(block_t)* WORLD_SIZE)), is_dirty(true) {}
World::~World() {
    if (world != nullptr) {
        free(world);
    }
    if (glDeleteLists) {
        glDeleteLists(list, 1);
    }
}
void World::create() {
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
    AABBox bb = AABBox(*hit_result->block->getOutline());
    bb.move(x, y, z);
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
}

std::vector<AABBox> World::getCubes(AABBox box) {
    std::vector<AABBox> cubes;
    int x0 = (int)box.start_x;
    int x1 = (int)(box.end_x + 1);
    int y0 = (int)box.start_y;
    int y1 = (int)(box.end_y + 1);
    int z0 = (int)box.start_z;
    int z1 = (int)(box.end_z + 1);
    if (x0 < 0) {
        x0 = 0;
    }
    if (y0 < 0) {
        y0 = 0;
    }
    if (z0 < 0) {
        z0 = 0;
    }
    if (x1 > WORLD_W) {
        x1 = WORLD_W;
    }

    if (y1 > WORLD_H) {
        y1 = WORLD_H;
    }
    if (z1 > WORLD_D) {
        z1 = WORLD_D;
    }
    for (int x = x0; x < x1; ++x) {
        for (int y = y0; y < y1; ++y) {
            for (int z = z0; z < z1; ++z) {
                block_t block = getBlock(x, y, z);
                AABBox* cube = block->getCollision();
                if (cube != nullptr) {
                    AABBox b = AABBox(*cube);
                    b.move((float)x, (float)y, (float)z);
                    cubes.push_back(b);
                }
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

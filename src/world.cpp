#include "real2d/texmgr.h"
#include "real2d/player.h"
#include "real2d/world.h"
#include "real2d/window.h"
#include "real2d/aabb.h"
#include "real2d/hit.h"
#include <malloc.h>

#define WORLD_BLOCK_I(x,y,z) (x + y * WORLD_W + z * WORLD_W * WORLD_H)

using Real2D::Window;
using Real2D::Blocks;
using Real2D::Player;
using Real2D::World;
using Real2D::AABBox;
using Real2D::HitResult;

extern Player player;
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
    if (world) {
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
void World::render() {
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

void outline(int x, int y, int z) {
    AABBox bb = choosingBlock->getOutline().move(XLATE(x), XLATE(y), XLATE(z));
    GLfloat fz = (GLfloat)bb.end_z;
    GLfloat fx = (GLfloat)bb.start_x;
    GLfloat fx1 = (GLfloat)bb.end_x;
    GLfloat fy = (GLfloat)bb.start_y;
    GLfloat fy1 = (GLfloat)bb.end_y;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_LINE_LOOP);
    glColor4f(0.0f, 0.0f, 0.0f, 0.9f);
    glVertex3f(fx + 1, fy1, fz);
    glVertex3f(fx + 1, fy, fz);
    glVertex3f(fx1, fy, fz);
    glVertex3f(fx1, fy1 - 1, fz);
    glEnd();
    glDisable(GL_BLEND);
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
            GLfloat bx = (GLfloat)XLATE(x);
            GLfloat bx1 = bx + BLOCK_RENDER_SIZE;
            GLfloat by = (GLfloat)XLATE(y);
            GLfloat by1 = by + BLOCK_RENDER_SIZE;
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
                outline(x, y, selectz);
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
    else {
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
void World::renderSelect() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glPushMatrix();
    glTranslatef(X_OFFSET, Y_OFFSET, 0);
    render();
    texmgr.bindTexture(blocks);
    glCallList(list);
    texmgr.bindTexture(0);
    glDisable(GL_DEPTH_TEST);
    select();
    glPopMatrix();

    glEnable(GL_DEPTH_TEST);
    player.render();
    glDisable(GL_DEPTH_TEST);

    glDisable(GL_BLEND);
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

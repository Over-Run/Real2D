#include "real2d/texmgr.h"
#include "real2d/player.h"
#include "real2d/world.h"
#include <malloc.h>

using Real2D::Blocks;
using Real2D::Player;
using Real2D::World;

extern int mouseX;
extern int mouseY;
extern int width;
extern int height;
extern Player player;

block_t choosingBlock = BLOCK(GRASS_BLOCK);

int selectx = 0;
int selecty = 0;
int selectz = 0;
GLfloat selectbx = 0;
GLfloat selectbx1 = 0;
GLfloat selectby = 0;
GLfloat selectby1 = 0;
block_t* selectblock = nullptr;
extern GLuint blocks;

World::World(): world((block_t*)malloc(sizeof(block_t) * WORLD_SIZE)) {}
World::~World() {
    if (world) {
        free(world);
    }
}
void World::create() {
    for (int z = 0; z < WORLD_D; ++z) {
        for (int x = 0; x < WORLD_W; ++x) {
            for (int y = 0; y < WORLD_H; ++y) {
                int p = WORLD_BLOCK(x, y, z);
                if (y < 3) {
                    world[p] = BLOCK(STONE);
                }
                else if (y == 3) {
                    world[p] = BLOCK(GRASS_BLOCK);
                }
                else {
                    world[p] = AIR_BLOCK;
                }
            }
        }
    }
}
void World::render() {
    glBindTexture(GL_TEXTURE_2D, blocks);
    glBegin(GL_QUADS);
    for (int z = 0; z < WORLD_D; ++z) {
        for (int x = 0; x < WORLD_W; ++x) {
            for (int y = 0; y < WORLD_H; ++y) {
                int p = WORLD_BLOCK(x, y, z);
                block_t block = world[p];
                if (block != AIR_BLOCK) {
                    renderBlock(x, y, z, block, 0);
                }
            }
        }
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glBegin(GL_QUADS);
    for (int x = 0; x < WORLD_W; ++x) {
        for (int y = 0; y < WORLD_H; ++y) {
            int p = WORLD_BLOCK(x, y, 0);
            block_t block = world[p];
            if (block != AIR_BLOCK) {
                renderBlock(x, y, 0, block, 1);
            }
        }
    }
    glEnd();
}

void readyPutBlock(int x, int y, int z) {
    glBindTexture(GL_TEXTURE_2D, blocks);
    glBegin(GL_QUADS);
    renderBlock(x, y, z, choosingBlock, 2);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glBegin(GL_QUADS);
    if (selectz == 0) {
        renderBlock(x, y, z, choosingBlock, 1);
    }
    glEnd();
}

void readyDestroyBlock(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
    glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
    glRectf(x1, y1, x2, y2);
}

void World::select() {
    int mx = mouseX;
    int my = height - mouseY;
    bool selected = false;
    const GLfloat xo = X_OFFSET;
    const GLfloat yo = Y_OFFSET;
    for (int x = 0; x < WORLD_W; ++x) {
        for (int y = 0; y < WORLD_H; ++y) {
            block_t& block = world[WORLD_BLOCK(x, y, selectz)];
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
                selectx = x;
                selectx = y;
                selectbx = obx;
                selectbx1 = obx1;
                selectby = oby;
                selectby1 = oby1;
                selectblock = &block;
                if (block == AIR_BLOCK) {
                    if (choosingBlock != AIR_BLOCK) {
                        readyPutBlock(x, y, selectz);
                    }
                    else {
                        readyDestroyBlock(bx, by, bx1, by1);
                    }
                }
                else {
                    readyDestroyBlock(bx, by, bx1, by1);
                }
                goto unloop;
            }
        }
    }
unloop:
    // Check outside world
    if (!selected) {
        selectblock = nullptr;
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
    glDisable(GL_DEPTH_TEST);
    select();
    glPopMatrix();

    glEnable(GL_DEPTH_TEST);
    player.render();
    glDisable(GL_DEPTH_TEST);

    glDisable(GL_BLEND);
}

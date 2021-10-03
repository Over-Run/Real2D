#include "real2d/texmgr.h"
#include "real2d/player.h"
#include "real2d/world.h"
#include "real2d/window.h"
#include <malloc.h>

using Real2D::Window;
using Real2D::Blocks;
using Real2D::Player;
using Real2D::World;

extern Player player;
extern window_t window;

block_t choosingBlock = BLOCK(GRASS_BLOCK);

int selectx = 0;
int selecty = 0;
int selectz = 1;
GLfloat selectbx = 0;
GLfloat selectbx1 = 0;
GLfloat selectby = 0;
GLfloat selectby1 = 0;
block_t* selectblock = nullptr;
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
    list = glGenLists(1);
}
void World::render() {
    if (isDirty()) {
        glNewList(list, GL_COMPILE);
        glBegin(GL_QUADS);
        for (int z = 0; z < WORLD_D; ++z) {
            for (int x = 0; x < WORLD_W; ++x) {
                for (int y = 0; y < WORLD_H; ++y) {
                    int p = WORLD_BLOCK(x, y, z);
                    block_t block = world[p];
                    if (block != AIR_BLOCK) {
                        renderBlock(x, y, z, block, false);
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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_LINE_LOOP);
    renderBlock(x, y, z, choosingBlock, true);
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
                outline(x, y, selectz);
                goto unloop;
            }
        }
    }
unloop:
    // Check outside world
    if (!selected) {
        selectblock = nullptr;
    }
    else {
        block_t b = *selectblock;
        if (b == AIR_BLOCK) {
            if (isMouseDown(MBR)
                && choosingBlock != AIR_BLOCK) {
                *selectblock = choosingBlock;
                markDirty();
            }
        }
        else {
            if (isMouseDown(MBL)) {
                *selectblock = AIR_BLOCK;
                markDirty();
            }
            else if (isMouseDown(MBM)) {
                choosingBlock = &*b;
                markDirty();
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

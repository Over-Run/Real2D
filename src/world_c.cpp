#include "real2d/world_c.h"
#include "real2d/real2d_def_c.h"
#include "real2d/window_c.h"
#include "real2d/player.h"
#include "real2d/texmgr_c.h"
#include "real2d/block_c.h"

using Real2D::Window;
using Real2D::WorldRenderer;
using Real2D::BlockRenderer;
using Real2D::HitResult;
using Real2D::AABBox;

GLuint list;

extern GLuint blocks;

WorldRenderer::WorldRenderer(World* _world) :
    is_dirty(true),
    world(_world) {
    list = glGenLists(1);
}
WorldRenderer::~WorldRenderer() {
    if (glDeleteLists) {
        glDeleteLists(list, 1);
    }
}
void WorldRenderer::blockChanged(int x, int y, int z) {
    markDirty();
}
void WorldRenderer::lightColChanged(int x, int z) {}
void WorldRenderer::markDirty() {
    is_dirty = true;
}
bool WorldRenderer::isDirty() {
    return is_dirty;
}
void WorldRenderer::render(double delta) {
    if (isDirty()) {
        glNewList(list, GL_COMPILE);
        glBegin(GL_QUADS);
        for (int z = 0; z < WORLD_D; ++z) {
            for (int x = 0; x < WORLD_W; ++x) {
                for (int y = 0; y < WORLD_H; ++y) {
                    block_t block = world->getBlock(x, y, z);
                    if (block != AIR_BLOCK) {
                        float light = (1.0f / 30.0f) * world->getLight(x, y, z);
                        if (z != 0) {
                            light += 0.5f;
                        }
                        BlockRenderer::renderBlock(x, y, z, block, light);
                    }
                }
            }
        }
        glEnd();
        glEndList();
        is_dirty = false;
    }
}
void WorldRenderer::pick() {
    int mx = Window::mouseX;
    int my = Window::height - Window::mouseY;
    bool selected = false;
    const GLfloat xo = X_OFFSET(world->player);
    const GLfloat yo = Y_OFFSET(world->player);
    for (int x = 0; x < WORLD_W; ++x) {
        for (int y = 0; y < WORLD_H; ++y) {
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
                block_t block = world->getBlock(x, y, selectz);
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
void WorldRenderer::renderPick(double delta) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();
    glTranslatef(X_OFFSET(world->player), Y_OFFSET(world->player), 0);
    render(delta);
    TexMgr::bindTexture(blocks);
    glCallList(list);
    TexMgr::bindTexture(0);
    pick();
    if (hit_result != nullptr) {
        renderHit(delta);
    }
    glPopMatrix();

    glDisable(GL_BLEND);

    world->player->render(delta);
}

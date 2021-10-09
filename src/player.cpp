#include "real2d/player.h"
#include "real2d/texmgr.h"
#include "real2d/window.h"
#include "real2d/block.h"
#include "real2d/world.h"
#include "real2d/real2d_def_c.h"
#include <cmath>

using Real2D::Player;
using Real2D::Window;
using Real2D::AABBox;
using Real2D::block_t;
using std::vector;

float headXRot = 0.0f;
float yRot = 0.0f;

extern block_t choosingBlock;
extern GLuint blocks;

inline bool isKeyDown(int key) {
    return Window::isKeyDown(key);
}

Player::Player(World* _world) :
    bb_width(0.6f),
    bb_height(1.8f),
    world(_world),
    x(16.0f), y(9.0f), z(1.0f),
    prev_x(x), prev_y(y),
    bb(AABBox(x - 0.3f, y, z, x + 0.3f, y + bb_height, z + 1)),
    onGround(false)
{}
void Player::tick() {
    prev_x = x;
    prev_y = y;
    float xa = 0;
    float ya = 0;
    if (isKeyDown(GLFW_KEY_SPACE) || isKeyDown(GLFW_KEY_W)) {
        ++ya;
    }
    if (isKeyDown(GLFW_KEY_LEFT_SHIFT) || isKeyDown(GLFW_KEY_S)) {
        --ya;
    }
    if (isKeyDown(GLFW_KEY_A)) {
        --xa;
    }
    if (isKeyDown(GLFW_KEY_D)) {
        ++xa;
    }
    move(xa, ya, onGround ? 0.02f : 0.03f);
    headXRot = Window::mouseY - Window::height * 0.5f;
    yRot = Window::mouseX - Window::width * 0.5f;
    if (headXRot < -45) {
        headXRot = -45;
    }
    if (headXRot > 90) {
        headXRot = 90;
    }
    if (yRot < -90) {
        yRot = -90;
    }
    if (yRot > 90) {
        yRot = 90;
    }
}
void Player::move(float xa, float ya, float speed) {
    float xd = xa * speed;
    float yd = ya * speed;
    float ydOrg = yd;
    vector<AABBox> cubes = world->getCubes(bb.expand(xd, yd, 0));
    AABBox copybb;
    copybb.set(bb);
    if (xd != 0) {
        for (auto c : cubes) {
            bb.move(xd, 0, 0, &copybb);
            if (c.isIntersect(copybb)) {
                xd = 0;
                break;
            }
        }
    }
    if (yd != 0) {
        for (auto c : cubes) {
            bb.move(0, yd, 0, &copybb);
            if (c.isIntersect(copybb)) {
                yd = 0;
                break;
            }
        }
    }
    float ox = x;
    float oy = y;

    onGround = ydOrg != yd && ydOrg < 0.0f;
    x += xd;
    y += yd;

    float fxd = xd;
    float fyd = yd;

    // round (the earth is a ball)
    if (x < 0) {
        x = WORLD_W;
        fxd = x - ox;
    }
    if (x > WORLD_W) {
        x = 0;
        fxd = x - ox;
    }
    if (y < -5) {
        y = -5;
        fyd = 0;
    }

    bb.move(fxd, fyd, 0);
}
void Player::render(double delta) {
    glPushMatrix();
    glTranslatef(Window::width * 0.5f, Window::height * 0.5f, 20.125f);
    GLuint id = texmgr.loadTexture(TEX_PLAYER);

    const GLfloat u0 = 0;
    const GLfloat u4 = 4 / TEX_PLAYER_W;
    const GLfloat u8 = 8 / TEX_PLAYER_W;
    const GLfloat u12 = 12 / TEX_PLAYER_W;
    const GLfloat u16 = 16 / TEX_PLAYER_W;
    const GLfloat u20 = 20 / TEX_PLAYER_W;
    const GLfloat u24 = 24 / TEX_PLAYER_W;
    const GLfloat u28 = 28 / TEX_PLAYER_W;
    const GLfloat u32 = 32 / TEX_PLAYER_W;
    const GLfloat u36 = 36 / TEX_PLAYER_W;
    const GLfloat u40 = 40 / TEX_PLAYER_W;
    const GLfloat v0 = 0;
    const GLfloat v8 = 8 / TEX_PLAYER_H;
    const GLfloat v16 = 16 / TEX_PLAYER_H;
    const GLfloat v28 = 28 / TEX_PLAYER_H;
    const GLfloat v40 = 40 / TEX_PLAYER_H;
    const GLfloat v52 = 52 / TEX_PLAYER_H;

    glColor3f(1.0f, 1.0f, 1.0f);
    texmgr.bindTexture(id);
    // Head
    glPushMatrix();
    glTranslatef(0, 48, -8);
    glRotatef(headXRot, 1, 0, 0);
    glRotatef(yRot, 0, 1, 0);
    glScalef(WORLD_RENDER_NML, WORLD_RENDER_NML, WORLD_RENDER_NML);
    glBegin(GL_QUADS);
    // front
    glTexCoord2f(u8, v8); glVertex3f(-0.25f, 0.5f, 0.25f);
    glTexCoord2f(u8, v16); glVertex3f(-0.25f, 0, 0.25f);
    glTexCoord2f(u16, v16); glVertex3f(0.25f, 0, 0.25f);
    glTexCoord2f(u16, v8); glVertex3f(0.25f, 0.5f, 0.25f);
    // left
    glTexCoord2f(u0, v8); glVertex3f(-0.25f, 0.5f, -0.25f);
    glTexCoord2f(u0, v16); glVertex3f(-0.25f, 0, -0.25f);
    glTexCoord2f(u8, v16); glVertex3f(-0.25f, 0, 0.25f);
    glTexCoord2f(u8, v8); glVertex3f(-0.25f, 0.5f, 0.25f);
    // right
    glTexCoord2f(u16, v8); glVertex3f(0.25f, 0.5f, 0.25f);
    glTexCoord2f(u16, v16); glVertex3f(0.25f, 0, 0.25f);
    glTexCoord2f(u24, v16); glVertex3f(0.25f, 0, -0.25f);
    glTexCoord2f(u24, v8); glVertex3f(0.25f, 0.5f, -0.25f);
    // top
    glTexCoord2f(u8, v0); glVertex3f(-0.25f, 0.5f, 0.25f);
    glTexCoord2f(u8, v8); glVertex3f(-0.25f, 0.5f, -0.25f);
    glTexCoord2f(u16, v8); glVertex3f(0.25f, 0.5f, -0.25f);
    glTexCoord2f(u16, v0); glVertex3f(0.25f, 0.5f, 0.25f);
    // bottom
    glTexCoord2f(u16, v0); glVertex3f(-0.25f, 0, 0.25f);
    glTexCoord2f(u16, v8); glVertex3f(-0.25f, 0, -0.25f);
    glTexCoord2f(u24, v8); glVertex3f(0.25f, 0, -0.25f);
    glTexCoord2f(u24, v0); glVertex3f(0.25f, 0, 0.25f);
    glEnd();
    glPopMatrix();

    // Body
    glPushMatrix();
    glTranslatef(0, 48, 0);
    glRotatef(yRot, 0, 1, 0);
    glScalef(WORLD_RENDER_NML, WORLD_RENDER_NML, WORLD_RENDER_NML);
    glBegin(GL_QUADS);
    // front
    glTexCoord2f(u16, v16); glVertex3f(-0.25f, 0, 0.125f);
    glTexCoord2f(u16, v28); glVertex3f(-0.25f, -0.75f, 0.125f);
    glTexCoord2f(u24, v28); glVertex3f(0.25f, -0.75f, 0.125f);
    glTexCoord2f(u24, v16); glVertex3f(0.25f, 0, 0.125f);
    // left
    glTexCoord2f(u12, v16); glVertex3f(-0.25f, 0, -0.125f);
    glTexCoord2f(u12, v28); glVertex3f(-0.25f, -0.75f, -0.125f);
    glTexCoord2f(u16, v28); glVertex3f(-0.25f, -0.75f, 0.125f);
    glTexCoord2f(u16, v16); glVertex3f(-0.25f, 0, 0.125f);
    // right
    glTexCoord2f(u24, v16); glVertex3f(0.25f, 0, 0.125f);
    glTexCoord2f(u24, v28); glVertex3f(0.25f, -0.75f, 0.125f);
    glTexCoord2f(u28, v28); glVertex3f(0.25f, -0.75f, -0.125f);
    glTexCoord2f(u28, v16); glVertex3f(0.25f, 0, -0.125f);
    glEnd();
    glPopMatrix();

    // right arm
    glPushMatrix();
    glTranslatef(0, 48, 0);
    glRotatef(yRot, 0, 1, 0);
    glScalef(WORLD_RENDER_NML, WORLD_RENDER_NML, WORLD_RENDER_NML);
    glBegin(GL_QUADS);
    // front
    glTexCoord2f(u32, v16); glVertex3f(-0.5f, 0, 0.125f);
    glTexCoord2f(u32, v28); glVertex3f(-0.5f, -0.75f, 0.125f);
    glTexCoord2f(u36, v28); glVertex3f(-0.25f, -0.75f, 0.125f);
    glTexCoord2f(u36, v16); glVertex3f(-0.25f, 0, 0.125f);
    // left
    glTexCoord2f(u28, v16); glVertex3f(-0.5f, 0, -0.125f);
    glTexCoord2f(u28, v28); glVertex3f(-0.5f, -0.75f, -0.125f);
    glTexCoord2f(u32, v28); glVertex3f(-0.5f, -0.75f, 0.125f);
    glTexCoord2f(u32, v16); glVertex3f(-0.5f, 0, 0.125f);
    // right
    glTexCoord2f(u36, v16); glVertex3f(-0.25f, 0, 0.125f);
    glTexCoord2f(u36, v28); glVertex3f(-0.25f, -0.75f, 0.125f);
    glTexCoord2f(u40, v28); glVertex3f(-0.25f, -0.75f, -0.125f);
    glTexCoord2f(u40, v16); glVertex3f(-0.25f, 0, -0.125f);
    glEnd();
    glPopMatrix();

    // left arm
    glPushMatrix();
    glTranslatef(0, 48, 0);
    glRotatef(yRot, 0, 1, 0);
    glScalef(WORLD_RENDER_NML, WORLD_RENDER_NML, WORLD_RENDER_NML);
    glBegin(GL_QUADS);
    // front
    glTexCoord2f(u28, v40); glVertex3f(0.25f, 0, 0.125f);
    glTexCoord2f(u28, v52); glVertex3f(0.25f, -0.75f, 0.125f);
    glTexCoord2f(u32, v52); glVertex3f(0.5f, -0.75f, 0.125f);
    glTexCoord2f(u32, v40); glVertex3f(0.5f, 0, 0.125f);
    // left
    glTexCoord2f(u24, v40); glVertex3f(0.25f, 0, -0.125f);
    glTexCoord2f(u24, v52); glVertex3f(0.25f, -0.75f, -0.125f);
    glTexCoord2f(u28, v52); glVertex3f(0.25f, -0.75f, 0.125f);
    glTexCoord2f(u28, v40); glVertex3f(0.25f, 0, 0.125f);
    // right
    glTexCoord2f(u32, v40); glVertex3f(0.5f, 0, 0.125f);
    glTexCoord2f(u32, v52); glVertex3f(0.5f, -0.75f, 0.125f);
    glTexCoord2f(u36, v52); glVertex3f(0.5f, -0.75f, -0.125f);
    glTexCoord2f(u36, v40); glVertex3f(0.5f, 0, -0.125f);
    glEnd();
    glPopMatrix();

    // right leg
    glPushMatrix();
    glRotatef(yRot, 0, 1, 0);
    glScalef(WORLD_RENDER_NML, WORLD_RENDER_NML, WORLD_RENDER_NML);
    glBegin(GL_QUADS);
    // front
    glTexCoord2f(u4, v16); glVertex3f(-0.25f, 0.75f, 0.125f);
    glTexCoord2f(u4, v28); glVertex3f(-0.25f, 0, 0.125f);
    glTexCoord2f(u8, v28); glVertex3f(0, 0, 0.125f);
    glTexCoord2f(u8, v16); glVertex3f(0, 0.75f, 0.125f);
    // left
    glTexCoord2f(u0, v16); glVertex3f(-0.25f, 0.75f, -0.125f);
    glTexCoord2f(u0, v28); glVertex3f(-0.25f, 0, -0.125f);
    glTexCoord2f(u4, v28); glVertex3f(-0.25f, 0, 0.125f);
    glTexCoord2f(u4, v16); glVertex3f(-0.25f, 0.75f, 0.125f);
    // right
    glTexCoord2f(u8, v16); glVertex3f(0, 0.75f, 0.125f);
    glTexCoord2f(u8, v28); glVertex3f(0, 0, 0.125f);
    glTexCoord2f(u16, v28); glVertex3f(0, 0, -0.125f);
    glTexCoord2f(u16, v16); glVertex3f(0, 0.75f, -0.125f);
    glEnd();
    glPopMatrix();

    // left leg
    glPushMatrix();
    glRotatef(yRot, 0, 1, 0);
    glScalef(WORLD_RENDER_NML, WORLD_RENDER_NML, WORLD_RENDER_NML);
    glBegin(GL_QUADS);
    // front
    glTexCoord2f(u16, v40); glVertex3f(0, 0.75f, 0.125f);
    glTexCoord2f(u16, v52); glVertex3f(0, 0, 0.125f);
    glTexCoord2f(u20, v52); glVertex3f(0.25f, 0, 0.125f);
    glTexCoord2f(u20, v40); glVertex3f(0.25f, 0.75f, 0.125f);
    // left
    glTexCoord2f(u12, v40); glVertex3f(0, 0.75f, -0.125f);
    glTexCoord2f(u12, v52); glVertex3f(0, 0, -0.125f);
    glTexCoord2f(u16, v52); glVertex3f(0, 0, 0.125f);
    glTexCoord2f(u16, v40); glVertex3f(0, 0.75f, 0.125f);
    // right
    glTexCoord2f(u20, v40); glVertex3f(0.25f, 0.75f, 0.125f);
    glTexCoord2f(u20, v52); glVertex3f(0.25f, 0, 0.125f);
    glTexCoord2f(u24, v52); glVertex3f(0.25f, 0, -0.125f);
    glTexCoord2f(u24, v40); glVertex3f(0.25f, 0.75f, -0.125f);
    glEnd();
    glPopMatrix();

    texmgr.bindTexture(0);

    // handled block
    const int bid = choosingBlock->getId();
    const GLfloat bu0 = BLOCK_TEX_U0(bid);
    const GLfloat bv0 = BLOCK_TEX_V0(bid);
    const GLfloat bu1 = BLOCK_TEX_U1(bid);
    const GLfloat bv1 = BLOCK_TEX_V1(bid);
    const GLfloat bts = (GLfloat)BLOCK_TEX_SIZE;
    const GLfloat hbts = (GLfloat)BLOCK_TEX_SIZE * 0.5f;
    texmgr.bindTexture(blocks);
    glPushMatrix();
    glTranslatef(0, 48, 0);
    glRotatef(yRot, 0, 1, 0);
    glBegin(GL_QUADS);
    glTexCoord2f(bu0, bv0); glVertex3f(-12 - hbts, -24, 5);
    glTexCoord2f(bu0, bv1); glVertex3f(-12 - hbts, (-bts) - 24, 5);
    glTexCoord2f(bu1, bv1); glVertex3f(-12 + hbts, (-bts) - 24, 5);
    glTexCoord2f(bu1, bv0); glVertex3f(-12 + hbts, -24, 5);
    glEnd();
    glPopMatrix();
    texmgr.bindTexture(0);

    glPopMatrix();
}

#include "real2d/player.h"
#include "real2d/texmgr.h"
#include "real2d/window.h"
#include "real2d/block.h"

using Real2D::Player;
using Real2D::Window;
using Real2D::AABBox;

Player player;

float headXRot = 0.0f;
float yRot = 0.0f;

extern GLuint blocks;
extern block_t choosingBlock;

inline bool isKeyDown(int key) {
    return Window::isKeyDown(key);
}

Player::Player() :
    x(16.0f), y(5.0f), z(1.0f), height(2.0f), speed(1.0f / 60.0f), bb(new AABBox(0, 0, 0, 24, 64, 16))
{}
void Player::tick(double delta) {
    int xo = 0, yo = 0, zo = 0;
    if (isKeyDown(GLFW_KEY_W)) {
        --zo;
    }
    if (isKeyDown(GLFW_KEY_S)) {
        ++zo;
    }
    if (isKeyDown(GLFW_KEY_SPACE)) {
        ++yo;
    }
    if (isKeyDown(GLFW_KEY_LEFT_SHIFT)) {
        --yo;
    }
    if (isKeyDown(GLFW_KEY_A)) {
        --xo;
    }
    if (isKeyDown(GLFW_KEY_D)) {
        ++xo;
    }
    moveRelative((float)(xo * delta),
        (float)(yo * delta),
        (float)(zo * delta));
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
void Player::moveRelative(float xom, float yom, float zom) {
    x += speed * xom;
    y += speed * yom;
    z += speed * zom;

    // round (the earth is a ball)
    if (z < -1) {
        z = 1;
    }
    if (z > 1) {
        z = -1;
    }
}
void Player::render() {
    glPushMatrix();
    glTranslatef(Window::width * 0.5f, Window::height * 0.5f, 0);
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
    glTranslatef(0, -16, -8);
    glRotatef(headXRot, 1, 0, 0);
    glRotatef(yRot, 0, 1, 0);
    glBegin(GL_QUADS);
    // front
    glTexCoord2f(u8, v8); glVertex3f(-8, 16, 8);
    glTexCoord2f(u8, v16); glVertex3f(-8, 0, 8);
    glTexCoord2f(u16, v16); glVertex3f(8, 0, 8);
    glTexCoord2f(u16, v8); glVertex3f(8, 16, 8);
    // left
    glTexCoord2f(u0, v8); glVertex3f(-8, 16, -8);
    glTexCoord2f(u0, v16); glVertex3f(-8, 0, -8);
    glTexCoord2f(u8, v16); glVertex3f(-8, 0, 8);
    glTexCoord2f(u8, v8); glVertex3f(-8, 16, 8);
    // right
    glTexCoord2f(u16, v8); glVertex3f(8, 16, 8);
    glTexCoord2f(u16, v16); glVertex3f(8, 0, 8);
    glTexCoord2f(u24, v16); glVertex3f(8, 0, -8);
    glTexCoord2f(u24, v8); glVertex3f(8, 16, -8);
    // top
    glTexCoord2f(u8, v0); glVertex3f(-8, 16, 8);
    glTexCoord2f(u8, v8); glVertex3f(-8, 16, -8);
    glTexCoord2f(u16, v8); glVertex3f(8, 16, -8);
    glTexCoord2f(u16, v0); glVertex3f(8, 16, 8);
    // bottom
    glTexCoord2f(u16, v0); glVertex3f(-8, 0, 8);
    glTexCoord2f(u16, v8); glVertex3f(-8, 0, -8);
    glTexCoord2f(u24, v8); glVertex3f(8, 0, -8);
    glTexCoord2f(u24, v0); glVertex3f(8, 0, 8);
    glEnd();
    glPopMatrix();

    // Body
    glPushMatrix();
    glTranslatef(0, -16, 0);
    glRotatef(yRot, 0, 1, 0);
    glBegin(GL_QUADS);
    // front
    glTexCoord2f(u16, v16); glVertex3f(-8, 0, 4);
    glTexCoord2f(u16, v28); glVertex3f(-8, -24, 4);
    glTexCoord2f(u24, v28); glVertex3f(8, -24, 4);
    glTexCoord2f(u24, v16); glVertex3f(8, 0, 4);
    // left
    glTexCoord2f(u12, v16); glVertex3f(-8, 0, -4);
    glTexCoord2f(u12, v28); glVertex3f(-8, -24, -4);
    glTexCoord2f(u16, v28); glVertex3f(-8, -24, 4);
    glTexCoord2f(u16, v16); glVertex3f(-8, 0, 4);
    // right
    glTexCoord2f(u24, v16); glVertex3f(8, 0, 4);
    glTexCoord2f(u24, v28); glVertex3f(8, -24, 4);
    glTexCoord2f(u28, v28); glVertex3f(8, -24, -4);
    glTexCoord2f(u28, v16); glVertex3f(8, 0, -4);
    glEnd();
    glPopMatrix();

    // right arm
    glPushMatrix();
    glTranslatef(0, -16, 0);
    glRotatef(yRot, 0, 1, 0);
    glBegin(GL_QUADS);
    // front
    glTexCoord2f(u32, v16); glVertex3f(-16, 0, 4);
    glTexCoord2f(u32, v28); glVertex3f(-16, -24, 4);
    glTexCoord2f(u36, v28); glVertex3f(-8, -24, 4);
    glTexCoord2f(u36, v16); glVertex3f(-8, 0, 4);
    // left
    glTexCoord2f(u28, v16); glVertex3f(-16, 0, -4);
    glTexCoord2f(u28, v28); glVertex3f(-16, -24, -4);
    glTexCoord2f(u32, v28); glVertex3f(-16, -24, 4);
    glTexCoord2f(u32, v16); glVertex3f(-16, 0, 4);
    // right
    glTexCoord2f(u36, v16); glVertex3f(-8, 0, 4);
    glTexCoord2f(u36, v28); glVertex3f(-8, -24, 4);
    glTexCoord2f(u40, v28); glVertex3f(-8, -24, -4);
    glTexCoord2f(u40, v16); glVertex3f(-8, 0, -4);
    glEnd();
    glPopMatrix();

    // left arm
    glPushMatrix();
    glTranslatef(0, -16, 0);
    glRotatef(yRot, 0, 1, 0);
    glBegin(GL_QUADS);
    // front
    glTexCoord2f(u28, v40); glVertex3f(8, 0, 4);
    glTexCoord2f(u28, v52); glVertex3f(8, -24, 4);
    glTexCoord2f(u32, v52); glVertex3f(16, -24, 4);
    glTexCoord2f(u32, v40); glVertex3f(16, 0, 4);
    // left
    glTexCoord2f(u24, v40); glVertex3f(8, 0, -4);
    glTexCoord2f(u24, v52); glVertex3f(8, -24, -4);
    glTexCoord2f(u28, v52); glVertex3f(8, -24, 4);
    glTexCoord2f(u28, v40); glVertex3f(8, 0, 4);
    // right
    glTexCoord2f(u32, v40); glVertex3f(16, 0, 4);
    glTexCoord2f(u32, v52); glVertex3f(16, -24, 4);
    glTexCoord2f(u36, v52); glVertex3f(16, -24, -4);
    glTexCoord2f(u36, v40); glVertex3f(16, 0, -4);
    glEnd();
    glPopMatrix();

    // right leg
    glPushMatrix();
    glTranslatef(0, -16, 0);
    glRotatef(yRot, 0, 1, 0);
    glBegin(GL_QUADS);
    // front
    glTexCoord2f(u4, v16); glVertex3f(-8, -24, 4);
    glTexCoord2f(u4, v28); glVertex3f(-8, -48, 4);
    glTexCoord2f(u8, v28); glVertex3f(0, -48, 4);
    glTexCoord2f(u8, v16); glVertex3f(0, -24, 4);
    // left
    glTexCoord2f(u0, v16); glVertex3f(-8, -24, -4);
    glTexCoord2f(u0, v28); glVertex3f(-8, -48, -4);
    glTexCoord2f(u4, v28); glVertex3f(-8, -48, 4);
    glTexCoord2f(u4, v16); glVertex3f(-8, -24, 4);
    // right
    glTexCoord2f(u8, v16); glVertex3f(0, -24, 4);
    glTexCoord2f(u8, v28); glVertex3f(0, -48, 4);
    glTexCoord2f(u16, v28); glVertex3f(0, -48, -4);
    glTexCoord2f(u16, v16); glVertex3f(0, -24, -4);
    glEnd();
    glPopMatrix();

    // left leg
    glPushMatrix();
    glTranslatef(0, -16, 0);
    glRotatef(yRot, 0, 1, 0);
    glBegin(GL_QUADS);
    // front
    glTexCoord2f(u16, v40); glVertex3f(0, -24, 4);
    glTexCoord2f(u16, v52); glVertex3f(0, -48, 4);
    glTexCoord2f(u20, v52); glVertex3f(8, -48, 4);
    glTexCoord2f(u20, v40); glVertex3f(8, -24, 4);
    // left
    glTexCoord2f(u12, v40); glVertex3f(0, -24, -4);
    glTexCoord2f(u12, v52); glVertex3f(0, -48, -4);
    glTexCoord2f(u16, v52); glVertex3f(0, -48, 4);
    glTexCoord2f(u16, v40); glVertex3f(0, -24, 4);
    // right
    glTexCoord2f(u20, v40); glVertex3f(8, -24, 4);
    glTexCoord2f(u20, v52); glVertex3f(8, -48, 4);
    glTexCoord2f(u24, v52); glVertex3f(8, -48, -4);
    glTexCoord2f(u24, v40); glVertex3f(8, -24, -4);
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
    glTranslatef(0, -16, 0);
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

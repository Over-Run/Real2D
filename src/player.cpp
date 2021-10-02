#include "real2d/player.h"
#include "real2d/texmgr.h"
#include "GLFW/glfw3.h"

using Real2D::Player;

Player player;

extern bool isKeyDown(int key);
extern int width;
extern int height;

Player::Player() :
    x(16), y(5), z(1), height(2), basestep(0.05f), rotate(1)
{}
void Player::tick(double delta) {
    int xo = 0, yo = 0, zo = 0;
    if (isKeyDown(GLFW_KEY_W)) {
        --zo;
        rotate = PLAYER_ROT_B;
    }
    if (isKeyDown(GLFW_KEY_S)) {
        ++zo;
        rotate = PLAYER_ROT_F;
    }
    if (isKeyDown(GLFW_KEY_SPACE)) {
        ++yo;
    }
    if (isKeyDown(GLFW_KEY_LEFT_SHIFT)) {
        --yo;
    }
    if (isKeyDown(GLFW_KEY_A)) {
        --xo;
        rotate = PLAYER_ROT_L;
    }
    if (isKeyDown(GLFW_KEY_D)) {
        ++xo;
        rotate = PLAYER_ROT_R;
    }
    moveRelative((float)(xo * delta),
        (float)(yo * delta),
        (float)(zo * delta));
}
void Player::moveRelative(float xom, float yom, float zom) {
    x += basestep * xom;
    y += basestep * yom;
    z += basestep * zom;

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
    glTranslatef((GLfloat)(width >> 1), (GLfloat)(::height >> 1), 0);
    GLuint id = texmgr.loadTexture(TEX_PLAYER);
    GLfloat u0;
    GLfloat u1;
    GLfloat v0;
    GLfloat v1;
    const GLfloat v2 = 8 / TEX_PLAYER_H;
    const GLfloat v3 = 20 / TEX_PLAYER_H;
    const GLfloat v4 = 32 / TEX_PLAYER_H;
    const GLfloat v5 = 44 / TEX_PLAYER_H;
    glBindTexture(GL_TEXTURE_2D, id);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);

    GLfloat hf = 8 / TEX_PLAYER_W;
    GLfloat vx = height - 16;
    GLfloat vy = height;

    // Head
    u0 = rotate * hf;
    u1 = (rotate + 1) * hf;
    v0 = 0;
    v1 = v2;
    glTexCoord2f(u0, v1); glVertex3f(-8, vx, z);
    glTexCoord2f(u1, v1); glVertex3f(8, vx, z);
    glTexCoord2f(u1, v0); glVertex3f(8, vy, z);
    glTexCoord2f(u0, v0); glVertex3f(-8, vy, z);
    // end head
    // Body
    v0 = v2;
    v1 = v3;
    vx = height - 40;
    vy = height - 16;
    hf = (GLfloat)(rotate / 2 * 12);
    if (rotate == PLAYER_ROT_F
        || rotate == PLAYER_ROT_B) {
        u0 = (20 + hf) / TEX_PLAYER_W;
        u1 = (28 + hf) / TEX_PLAYER_W;
        glTexCoord2f(u0, v1); glVertex3f(-8, vx, z);
        glTexCoord2f(u1, v1); glVertex3f(8, vx, z);
        glTexCoord2f(u1, v0); glVertex3f(8, vy, z);
        glTexCoord2f(u0, v0); glVertex3f(-8, vy, z);
    }
    else {
        u0 = (16 + hf) / TEX_PLAYER_W;
        u1 = (20 + hf) / TEX_PLAYER_W;
        glTexCoord2f(u0, v1); glVertex3f(-4, vx, z);
        glTexCoord2f(u1, v1); glVertex3f(4, vx, z);
        glTexCoord2f(u1, v0); glVertex3f(4, vy, z);
        glTexCoord2f(u0, v0); glVertex3f(-4, vy, z);
    }
    // end body
    // Arms
    hf = (GLfloat)(rotate / 2 * 8);
    if (rotate == PLAYER_ROT_F
        || rotate == PLAYER_ROT_B) {
        // right arm
        u0 = (44 + hf) / TEX_PLAYER_W;
        u1 = (48 + hf) / TEX_PLAYER_W;
        glTexCoord2f(u0, v1); glVertex3f(-16, vx, z);
        glTexCoord2f(u1, v1); glVertex3f(-8, vx, z);
        glTexCoord2f(u1, v0); glVertex3f(-8, vy, z);
        glTexCoord2f(u0, v0); glVertex3f(-16, vy, z);
        // left arm
        v0 = v4;
        v1 = v5;
        u0 = (36 + hf) / TEX_PLAYER_W;
        u1 = (40 + hf) / TEX_PLAYER_W;
        glTexCoord2f(u0, v1); glVertex3f(16, vx, z);
        glTexCoord2f(u1, v1); glVertex3f(8, vx, z);
        glTexCoord2f(u1, v0); glVertex3f(8, vy, z);
        glTexCoord2f(u0, v0); glVertex3f(16, vy, z);
    }
    else if (rotate == PLAYER_ROT_R) {
        // right arm
        u0 = (40 + hf) / TEX_PLAYER_W;
        u1 = (44 + hf) / TEX_PLAYER_W;
        glTexCoord2f(u0, v1); glVertex3f(-4, vx, z);
        glTexCoord2f(u1, v1); glVertex3f(4, vx, z);
        glTexCoord2f(u1, v0); glVertex3f(4, vy, z);
        glTexCoord2f(u0, v0); glVertex3f(-4, vy, z);
    }
    else if (rotate == PLAYER_ROT_L) {
        // left arm
        v0 = v4;
        v1 = v5;
        u0 = (32 + hf) / TEX_PLAYER_W;
        u1 = (36 + hf) / TEX_PLAYER_W;
        glTexCoord2f(u0, v1); glVertex3f(-4, vx, z);
        glTexCoord2f(u1, v1); glVertex3f(4, vx, z);
        glTexCoord2f(u1, v0); glVertex3f(4, vy, z);
        glTexCoord2f(u0, v0); glVertex3f(-4, vy, z);
    }
    // end arms
    // Legs
    v0 = v2;
    v1 = v3;
    vx = height - 64;
    vy = height - 40;
    hf = (GLfloat)(rotate / 2 * 8);
    if (rotate == PLAYER_ROT_F
        || rotate == PLAYER_ROT_B) {
        // right leg
        u0 = (4 + hf) / TEX_PLAYER_W;
        u1 = (8 + hf) / TEX_PLAYER_W;
        glTexCoord2f(u0, v1); glVertex3f(-8, vx, z);
        glTexCoord2f(u1, v1); glVertex3f(0, vx, z);
        glTexCoord2f(u1, v0); glVertex3f(0, vy, z);
        glTexCoord2f(u0, v0); glVertex3f(-8, vy, z);
        // left leg
        v0 = v4;
        v1 = v5;
        u0 = (20 + hf) / TEX_PLAYER_W;
        u1 = (24 + hf) / TEX_PLAYER_W;
        glTexCoord2f(u0, v1); glVertex3f(8, vx, z);
        glTexCoord2f(u1, v1); glVertex3f(0, vx, z);
        glTexCoord2f(u1, v0); glVertex3f(0, vy, z);
        glTexCoord2f(u0, v0); glVertex3f(8, vy, z);
    }
    else if (rotate == PLAYER_ROT_R) {
        // right leg
        u0 = (4 + hf) / TEX_PLAYER_W;
        u1 = (8 + hf) / TEX_PLAYER_W;
        glTexCoord2f(u0, v1); glVertex3f(-4, vx, z);
        glTexCoord2f(u1, v1); glVertex3f(4, vx, z);
        glTexCoord2f(u1, v0); glVertex3f(4, vy, z);
        glTexCoord2f(u0, v0); glVertex3f(-4, vy, z);
    }
    else if (rotate == PLAYER_ROT_L) {
        // left leg
        v0 = v4;
        v1 = v5;
        u0 = (20 + hf) / TEX_PLAYER_W;
        u1 = (24 + hf) / TEX_PLAYER_W;
        glTexCoord2f(u0, v1); glVertex3f(-4, vx, z);
        glTexCoord2f(u1, v1); glVertex3f(4, vx, z);
        glTexCoord2f(u1, v0); glVertex3f(4, vy, z);
        glTexCoord2f(u0, v0); glVertex3f(-4, vy, z);
    }
    // end legs

    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    // Shadow
    glColor4f(0.0f, 0.0f, 0.0f, (1 - z) / 4);
    glBegin(GL_QUADS);

    // Body
    vx = height - 40;
    vy = height - 16;
    if (rotate == PLAYER_ROT_F
        || rotate == PLAYER_ROT_B) {
        glVertex3f(-8, vx, z);
        glVertex3f(8, vx, z);
        glVertex3f(8, vy, z);
        glVertex3f(-8, vy, z);
    }
    else {
        glVertex3f(-4, vx, z);
        glVertex3f(4, vx, z);
        glVertex3f(4, vy, z);
        glVertex3f(-4, vy, z);
    }
    // end body
    // Arms
    if (rotate == PLAYER_ROT_F
        || rotate == PLAYER_ROT_B) {
        glVertex3f(-16, vx, z);
        glVertex3f(-8, vx, z);
        glVertex3f(-8, vy, z);
        glVertex3f(-16, vy, z);
        glVertex3f(16, vx, z);
        glVertex3f(8, vx, z);
        glVertex3f(8, vy, z);
        glVertex3f(16, vy, z);
    }
    // end arms
    // Legs
    vx = height - 64;
    vy = height - 40;
    if (rotate == PLAYER_ROT_F
        || rotate == PLAYER_ROT_B) {
        // right leg
        glVertex3f(-8, vx, z);
        glVertex3f(0, vx, z);
        glVertex3f(0, vy, z);
        glVertex3f(-8, vy, z);
        // left leg
        glVertex3f(8, vx, z);
        glVertex3f(0, vx, z);
        glVertex3f(0, vy, z);
        glVertex3f(8, vy, z);
    }
    else if (rotate == PLAYER_ROT_R) {
        // right leg
        glVertex3f(-4, vx, z);
        glVertex3f(4, vx, z);
        glVertex3f(4, vy, z);
        glVertex3f(-4, vy, z);
    }
    else if (rotate == PLAYER_ROT_L) {
        // left leg
        glVertex3f(-4, vx, z);
        glVertex3f(4, vx, z);
        glVertex3f(4, vy, z);
        glVertex3f(-4, vy, z);
    }
    // end legs
    // Head
    glColor4f(0.0f, 0.0f, 0.0f, (1 - z) / 5);
    vx = height - 16;
    vy = height;
    glVertex3f(-8, vx, z);
    glVertex3f(8, vx, z);
    glVertex3f(8, vy, z);
    glVertex3f(-8, vy, z);
    // end head

    glEnd();
    // end shadow

    glPopMatrix();
}

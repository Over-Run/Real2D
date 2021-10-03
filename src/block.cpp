#include "real2d/block.h"
#include "real2d/player.h"
#include "glad/gl.h"

using Real2D::Block;
using Real2D::Blocks;
using Real2D::Player;
using Real2D::AABBox;

extern int width;
extern int height;
extern Player player;

Block::Block(const int _id) : id(_id) {}
int Block::getId() {
    return id;
}
bool Block::operator==(const Block& block_) const {
    return id == block_.id;
}
bool Block::operator!=(const Block& block_) const {
    return id != block_.id;
}
AABBox Block::getOutline() {
    return AABBox::FULL_CUBE;
}
AABBox Block::getCollision() {
    return AABBox::FULL_CUBE;
}

/*BlockStates::BlockStates(int x_, int y_, int z_, const Block& block_) :
    x(x_), y(y_), z(z_) {
    block = &block_;
}
int BlockStates::getX() {
    return x;
}
int BlockStates::getY() {
    return y;
}
int BlockStates::getZ() {
    return z;
}
const Block& BlockStates::getBlock() {
    return *block;
}
void BlockStates::setBlock(const Block& block_) {
    block = &block_;
}*/

block_t Blocks::AIR = new Block(0);
block_t Blocks::GRASS_BLOCK = new Block(1);
block_t Blocks::STONE = new Block(2);
Blocks::~Blocks() {
    delete AIR;
    delete GRASS_BLOCK;
    delete STONE;
}
void Real2D::renderBlock(int x, int y, int z, block_t block) {
    GLfloat xi = (GLfloat)XLATE(x);
    GLfloat xi1 = (GLfloat)XLATE(x + 1);
    GLfloat yi = (GLfloat)XLATE(y);
    GLfloat yi1 = (GLfloat)XLATE(y + 1);
    GLfloat zi = (GLfloat)XLATE(z);
    int id = block->getId();
    GLfloat u0 = BLOCK_TEX_U0(id);
    GLfloat u1 = BLOCK_TEX_U1(id);
    GLfloat v0 = BLOCK_TEX_V0(id);
    GLfloat v1 = BLOCK_TEX_V1(id);
    GLfloat color;
    if (z == 0) {
        color = 0.5f;
    }
    else {
        color = 1.0f;
    }
    glColor3f(color, color, color);
    glTexCoord2f(u0, v0); glVertex3f(xi, yi1, zi);
    glTexCoord2f(u0, v1); glVertex3f(xi, yi, zi);
    glTexCoord2f(u1, v1); glVertex3f(xi1, yi, zi);
    glTexCoord2f(u1, v0); glVertex3f(xi1, yi1, zi);
}

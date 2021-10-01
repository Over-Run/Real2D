#include "real2d/block.h"
#include "real2d/player.h"
#include "glad/gl.h"

#define SET_UV int id = block->getId(); \
GLfloat u0 = ((id - 1) % BLOCKS_PER_TEX) * BLOCK_TEX_UV_FACTOR; \
GLfloat u1 = (id % BLOCKS_PER_TEX) * BLOCK_TEX_UV_FACTOR; \
GLfloat v0 = ((id - 1) / BLOCKS_PER_TEX) * BLOCK_TEX_UV_FACTOR; \
GLfloat v1 = (id / BLOCKS_PER_TEX + 1) * BLOCK_TEX_UV_FACTOR

using Real2D::Block;
using Real2D::Blocks;
using Real2D::Player;

extern int width;
extern int height;
extern Player player;

Block::Block(const int id_) : id(id_) {}
const int Block::getId() const {
    return id;
}
bool Block::operator==(const Block& block_) const {
    return id == block_.id;
}
bool Block::operator!=(const Block& block_) const {
    return id != block_.id;
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

block_t const Blocks::AIR = new Block(0);
block_t const Blocks::GRASS_BLOCK = new Block(1);
block_t const Blocks::STONE = new Block(2);
Blocks::~Blocks() {
    delete AIR;
    delete GRASS_BLOCK;
    delete STONE;
}

void Real2D::renderBlock(int x, int y, int z, block_t block, int layer) {
    GLfloat fx = (GLfloat)x;
    GLfloat fy = (GLfloat)y;
    GLfloat xi = XLATE(fx);
    GLfloat xi1 = XLATE(fx + 1.0f);
    GLfloat yi = XLATE(fy);
    GLfloat yi1 = XLATE(fy + 1.0f);
    if (layer == 1) {
        glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
        glVertex2f(xi, yi1);
        glVertex2f(xi, yi);
        glVertex2f(xi1, yi);
        glVertex2f(xi1, yi1);
    }
    else {
        SET_UV;
        if (layer == 0) {
            glColor3f(1.0f, 1.0f, 1.0f);
        }
        else if (layer == 2) {
            glColor4f(1.0f, 1.0f, 1.0f, 0.7f);
        }
        GLfloat fz = (GLfloat)z;
        glTexCoord2f(u0, v0); glVertex3f(xi, yi1, fz);
        glTexCoord2f(u0, v1); glVertex3f(xi, yi, fz);
        glTexCoord2f(u1, v1); glVertex3f(xi1, yi, fz);
        glTexCoord2f(u1, v0); glVertex3f(xi1, yi1, fz);
    }
}

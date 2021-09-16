#include "real2d/block.h"

#define XLATE(axis) (GLfloat)(axis * BLOCK_RENDER_SIZE)

using namespace Real2D;

Block::Block(int id_) : id(id_) {}
int Block::getId() {
    return id;
}

BlockStates::BlockStates(int x_, int y_, int z_, Block& block_):
    x(x_), y(y_), z(z_), block(block_) {}
int BlockStates::getX() {
    return x;
}
int BlockStates::getY() {
    return y;
}
int BlockStates::getZ() {
    return z;
}
Block& BlockStates::getBlock() {
    return block;
}

const Block& Blocks::AIR = Block(0);
const Block& Blocks::GRASS_BLOCK = Block(1);
const Block& Blocks::STONE = Block(2);

void Real2D::renderBlock(BlockStates& states, GLuint texId) {
    int x = states.getX();
    int y = states.getY();
    int z = states.getZ();
    int id = states.getBlock().getId();
    float u0 = ((id - 1) % BLOCKS_PER_TEX) * BLOCK_TEX_UV_FACTOR;
    float u1 = (id % BLOCKS_PER_TEX) * BLOCK_TEX_UV_FACTOR;
    float v0 = ((id - 1) / BLOCKS_PER_TEX) * BLOCK_TEX_UV_FACTOR;
    float v1 = (id / BLOCKS_PER_TEX + 1) * BLOCK_TEX_UV_FACTOR;
    glPushMatrix();
    glTranslatef(XLATE(x), XLATE(y), XLATE(z));
    glBindTexture(GL_TEXTURE_2D, texId);
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glTexCoord2f(u0, v0); glVertex2f(0, BLOCK_RENDER_SIZE);
    glTexCoord2f(u0, v1); glVertex2f(0, 0);
    glTexCoord2f(u1, v1); glVertex2f(BLOCK_RENDER_SIZE, 0);
    glTexCoord2f(u1, v0); glVertex2f(BLOCK_RENDER_SIZE, BLOCK_RENDER_SIZE);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
}

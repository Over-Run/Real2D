#include "real2d/block.h"

#define XLATE(axis) ((axis) * BLOCK_RENDER_SIZE)

using namespace Real2D;

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

const Block* const Blocks::AIR = new Block(0);
const Block* const Blocks::GRASS_BLOCK = new Block(1);
const Block* const Blocks::STONE = new Block(2);
Blocks::~Blocks() {
    delete AIR;
    delete GRASS_BLOCK;
    delete STONE;
}

void Real2D::renderBlock(int x, int y, int z, const Block* block) {
    int id = block->getId();
    float u0 = ((id - 1) % BLOCKS_PER_TEX) * BLOCK_TEX_UV_FACTOR;
    float u1 = (id % BLOCKS_PER_TEX) * BLOCK_TEX_UV_FACTOR;
    float v0 = ((id - 1) / BLOCKS_PER_TEX) * BLOCK_TEX_UV_FACTOR;
    float v1 = (id / BLOCKS_PER_TEX + 1) * BLOCK_TEX_UV_FACTOR;
    int xi = XLATE(x);
    int xi1 = XLATE(x + 1);
    int yi = XLATE(y);
    int yi1 = XLATE(y + 1);
    int zi = XLATE(z);
    glColor3f(1, 1, 1);
    glTexCoord2f(u0, v0); glVertex3i(xi, yi1, zi);
    glTexCoord2f(u0, v1); glVertex3i(xi, yi, zi);
    glTexCoord2f(u1, v1); glVertex3i(xi1, yi, zi);
    glTexCoord2f(u1, v0); glVertex3i(xi1, yi1, zi);
}

#undef XLATE

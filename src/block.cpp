#include "real2d/player.h"
#include "real2d/real2d_def_c.h"
#include "real2d/reg.h"
#include "real2d/world.h"
#include "glad/gl.h"

using std::string;
using Real2D::Block;
using Real2D::AirBlock;
using Real2D::block_t;
using Real2D::Blocks;
using Real2D::AABBox;
using Real2D::Registry;
using Real2D::Registries;

Registry<block_t>* Registries::BLOCK = new Registry(&Blocks::AIR);

Block::Block() {}
int Block::getId() const {
    return Registries::BLOCK->get(const_cast<block_t>(this));
}
bool Block::operator==(const Block& _block) const {
    return getId() == _block.getId();
}
bool Block::operator!=(const Block& _block) const {
    return getId() != _block.getId();
}
bool Block::isOpaque() {
    return true;
}
AABBox* Block::getOutline() {
    return (AABBox*)&AABBox::FULL_CUBE;
}
AABBox* Block::getCollision() {
    return getOutline();
}

bool AirBlock::isOpaque() {
    return false;
}
AABBox* AirBlock::getCollision() {
    return nullptr;
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

template<typename T>
block_t reg(int rawId, string id, T block) {
    return Registries::BLOCK->set(rawId, id, block);
}

block_t Blocks::AIR = reg(0, "air_block", new AirBlock());
block_t Blocks::GRASS_BLOCK = reg(1, "grass_block", new Block());
block_t Blocks::STONE = reg(2, "stone", new Block());

void Real2D::renderBlock(int x, int y, int z, block_t block, World* world) {
    GLfloat xi = (GLfloat)UNML(x);
    GLfloat xi1 = (GLfloat)UNML(x + 1);
    GLfloat yi = (GLfloat)UNML(y);
    GLfloat yi1 = (GLfloat)UNML(y + 1);
    GLfloat zi = (GLfloat)UNML(z);
    int id = block->getId();
    GLfloat u0 = BLOCK_TEX_U0(id);
    GLfloat u1 = BLOCK_TEX_U1(id);
    GLfloat v0 = BLOCK_TEX_V0(id);
    GLfloat v1 = BLOCK_TEX_V1(id);
    GLfloat color;
    float f = (1.0f / 30.0f) * world->getLight(x, y, z);
    if (z == 0) {
        color = f;
    }
    else {
        color = f + 0.5f;
    }
    glColor3f(color, color, color);
    glTexCoord2f(u0, v0); glVertex3f(xi, yi1, zi);
    glTexCoord2f(u0, v1); glVertex3f(xi, yi, zi);
    glTexCoord2f(u1, v1); glVertex3f(xi1, yi, zi);
    glTexCoord2f(u1, v0); glVertex3f(xi1, yi1, zi);
}

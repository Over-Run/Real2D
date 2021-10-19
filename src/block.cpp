#include "real2d/block.h"
#include "real2d/reg.h"

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
    return const_cast<AABBox*>(&AABBox::FULL_CUBE);
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

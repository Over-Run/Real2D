#pragma once

#include "glad/gl.h"

constexpr auto BLOCK_TEX_SIZE = 16;
constexpr auto BLOCKS_TEX_SIZE = 256;
constexpr auto BLOCK_RENDER_SIZE = 32;
#define BLOCKS_PER_TEX (BLOCKS_TEX_SIZE / BLOCK_TEX_SIZE)
#define BLOCK_TEX_UV_FACTOR ((float)BLOCK_TEX_SIZE / (float)BLOCKS_TEX_SIZE)

namespace Real2D {
    class Block {
    private:
        int id;
    public:
        Block(int id_);
        Block(const Block&) = delete;
        Block operator=(const Block&) = delete;
        int getId();
    };

    class BlockStates {
    private:
        int x, y, z;
        Block& block;
    public:
        BlockStates(int x_, int y_, int z_, Block& block_);
        int getX();
        int getY();
        int getZ();
        Block& getBlock();
    };

    struct Blocks {
        static const Block& AIR;
        static const Block& GRASS_BLOCK;
        static const Block& STONE;
    };

    void renderBlock(BlockStates& states, GLuint texId);
}

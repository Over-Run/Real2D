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
        const int id;
    public:
        Block(const int id_);
        Block(const Block&) = delete;
        Block operator=(const Block&) = delete;
        bool operator==(const Block&) const;
        bool operator!=(const Block&) const;
        const int getId() const;
    };

    /*class BlockStates {
    private:
        int x, y, z;
        const Block* block;
    public:
        BlockStates(int x_, int y_, int z_, const Block& block_);
        int getX();
        int getY();
        int getZ();
        const Block& getBlock();
        void setBlock(const Block& block_);
    };*/

    struct Blocks {
        static const Block* const AIR;
        static const Block* const GRASS_BLOCK;
        static const Block* const STONE;

        ~Blocks();
    };

    void renderBlock(int x, int y, int z, const Block* block, GLuint texId);
}

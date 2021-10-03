#pragma once
#include "aabb.h"

#define XLATE(axis) ((axis) * BLOCK_RENDER_SIZE)

constexpr const char* TEX_BLOCKS = "res/block/blocks0.png";
constexpr int BLOCK_TEX_SIZE = 16;
constexpr int BLOCKS_TEX_SIZE = 256;
constexpr int BLOCK_RENDER_SIZE = 32;
#define BLOCKS_PER_TEX (BLOCKS_TEX_SIZE / BLOCK_TEX_SIZE)
#define BLOCK_TEX_UV_FACTOR ((float)BLOCK_TEX_SIZE / (float)BLOCKS_TEX_SIZE)
#define BLOCK_TEX_U0(id) ((((id) - 1) % BLOCKS_PER_TEX) * BLOCK_TEX_UV_FACTOR);
#define BLOCK_TEX_U1(id) (((id) % BLOCKS_PER_TEX) * BLOCK_TEX_UV_FACTOR);
#define BLOCK_TEX_V0(id) ((((id) - 1) / BLOCKS_PER_TEX) * BLOCK_TEX_UV_FACTOR);
#define BLOCK_TEX_V1(id) ((((id) / BLOCKS_PER_TEX) + 1) * BLOCK_TEX_UV_FACTOR);

#define X_OFFSET (GLfloat)(int)((Real2D::Window::width * 0.5f) - (int)(player.x * BLOCK_RENDER_SIZE))
#define Y_OFFSET (GLfloat)(int)((Real2D::Window::height * 0.5f) - (int)((player.y + 1) * BLOCK_RENDER_SIZE))

#define BLOCK(nm) (Real2D::Blocks::nm)
#define AIR_BLOCK BLOCK(AIR)

namespace Real2D {
    class Block {
    private:
        const int id;
    public:
        Block(const int _id);
        Block(const Block&) = delete;
        Block operator=(const Block&) = delete;
        bool operator==(const Block&) const;
        bool operator!=(const Block&) const;
        /// <summary>
        /// Get this block's raw id.
        /// </summary>
        /// <returns>The raw id.</returns>
        int getId();
        AABBox getOutline();
        AABBox getCollision();
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
}

using block_t = Real2D::Block*;

namespace Real2D {
    struct Blocks {
        static block_t AIR;
        static block_t GRASS_BLOCK;
        static block_t STONE;

        ~Blocks();
    };

    /// <summary>
    /// Render a block.
    /// </summary>
    /// <param name="x">Pos x.</param>
    /// <param name="y">Pos y.</param>
    /// <param name="z">Pos z.</param>
    /// <param name="block">The block.</param>
    extern void renderBlock(int, int, int, block_t);
}

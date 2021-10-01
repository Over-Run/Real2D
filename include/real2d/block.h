#pragma once

#define XLATE(axis) ((axis) * BLOCK_RENDER_SIZE)

constexpr const char* TEX_BLOCKS = "res/block/blocks0.png";
constexpr auto BLOCK_TEX_SIZE = 16;
constexpr auto BLOCKS_TEX_SIZE = 256;
constexpr auto BLOCK_RENDER_SIZE = 32.0f;
#define BLOCKS_PER_TEX (BLOCKS_TEX_SIZE / BLOCK_TEX_SIZE)
#define BLOCK_TEX_UV_FACTOR ((float)BLOCK_TEX_SIZE / (float)BLOCKS_TEX_SIZE)

#define X_OFFSET ((width * 0.5f) - player.x * BLOCK_RENDER_SIZE)
#define Y_OFFSET ((height * 0.5f) - (player.y + 1) * BLOCK_RENDER_SIZE)

#define BLOCK(nm) (Blocks::nm)
#define AIR_BLOCK BLOCK(AIR)

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
        /// <summary>
        /// Get this block's number id.
        /// </summary>
        /// <returns>The number id.</returns>
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
}

using block_t = const Real2D::Block*;

namespace Real2D {
    struct Blocks {
        static block_t const AIR;
        static block_t const GRASS_BLOCK;
        static block_t const STONE;

        ~Blocks();
    };

    /// <summary>
    /// Render a block.
    /// </summary>
    /// <param name="x">Pos x.</param>
    /// <param name="y">Pos y.</param>
    /// <param name="z">Pos z.</param>
    /// <param name="block">The block.</param>
    /// <param name="layer">The layer of rendering.
    /// 0 of normal, 1 of dark, 2 of select.</param>
    extern void renderBlock(int x, int y, int z, block_t block, int layer);
}

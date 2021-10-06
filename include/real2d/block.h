#pragma once
#include "aabb.h"
#include "real2d_consts.h"

namespace Real2D {
    class Block {
    protected:
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
        virtual AABBox* getOutline();
        virtual AABBox* getCollision();
    };
    class AirBlock : public Block {
    public:
        AirBlock(const int _id);

        virtual AABBox* getCollision();
    };

    using block_t = Real2D::Block*;

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

namespace Real2D {
    struct Blocks {
        static block_t AIR;
        static block_t GRASS_BLOCK;
        static block_t STONE;
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

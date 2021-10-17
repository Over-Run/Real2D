#pragma once
#include "aabb.h"

namespace Real2D {
    class Block {
    public:
        Block();
        Block(const Block&) = delete;
        Block operator=(const Block&);
        bool operator==(const Block&) const;
        bool operator!=(const Block&) const;
        /// <summary>
        /// Get this block's raw id.
        /// </summary>
        /// <returns>The raw id.</returns>
        int getId() const;
        virtual bool isOpaque();
        virtual AABBox* getOutline();
        virtual AABBox* getCollision();
    };
    class AirBlock : public Block {
    public:
        bool isOpaque() override;
        AABBox* getCollision() override;
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
        static Block* AIR;
        static Block* GRASS_BLOCK;
        static Block* STONE;
    };
    class World;

    /// <summary>
    /// Render a block.
    /// </summary>
    /// <param name="x">Pos x.</param>
    /// <param name="y">Pos y.</param>
    /// <param name="z">Pos z.</param>
    /// <param name="block">The block.</param>
    /// <param name="world">The world.</param>
    extern void renderBlock(int, int, int, Block*, World*);
}

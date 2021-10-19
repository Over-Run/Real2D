#pragma once

namespace Real2D {
    class World;
    class Block;

    class BlockRenderer {
    public:
        /// <summary>
        /// Render a block.
        /// </summary>
        /// <param name="x">Pos x.</param>
        /// <param name="y">Pos y.</param>
        /// <param name="z">Pos z.</param>
        /// <param name="block">The block.</param>
        /// <param name="light">The light.</param>
        static void renderBlock(int, int, int, Block*, float);
    };
}

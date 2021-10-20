#pragma once

namespace Real2D {
    class Block;
    struct HitResult {
        int x, y, z;
        Block* block;

        HitResult(int _x, int _y, int _z, Block* _block);
    };
}

#pragma once
#include "block.h"

namespace Real2D {
    struct HitResult {
        int x, y, z;
        Block* block;

        HitResult(int _x, int _y, int _z, Block* _block);
    };
}

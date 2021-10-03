#pragma once
#include "block.h"

namespace Real2D {
    struct HitResult {
        int x, y, z;
        block_t block;

        HitResult(int _x, int _y, int _z, block_t _block);
    };
}

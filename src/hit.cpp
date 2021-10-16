#include "real2d/hit.h"

using Real2D::HitResult;

HitResult::HitResult(int _x, int _y, int _z, block_t _block) :
    x(_x), y(_y), z(_z), block(_block) {}

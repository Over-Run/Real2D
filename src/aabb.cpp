#include "real2d/aabb.h"
#include "real2d/block.h"

using Real2D::AABBox;

const AABBox& AABBox::FULL_CUBE = AABBox(0, 0, 0, BLOCK_RENDER_SIZE, BLOCK_RENDER_SIZE, BLOCK_RENDER_SIZE);
AABBox::AABBox(int _start_x,
    int _start_y,
    int _start_z,
    int _end_x,
    int _end_y,
    int _end_z) : start_x(_start_x),
    start_y(_start_y),
    start_z(_start_z),
    end_x(_end_x),
    end_y(_end_y),
    end_z(_end_z) {}
AABBox AABBox::move(int xoffset,
    int yoffset,
    int zoffset) {
    return AABBox(start_x + xoffset,
        start_y + yoffset,
        start_z + zoffset,
        end_x + xoffset,
        end_y + yoffset,
        end_z + zoffset);
}
bool AABBox::isIntersect(AABBox& b) {
    int sx = start_x;
    int sy = start_y;
    int ex = end_x;
    int ey = end_y;
    int bsx = b.start_x;
    int bsy = b.start_y;
    int bex = b.end_x;
    int bey = b.end_y;
    return (sx >= bsx && ey <= bey)
        || (sx >= bsx && sy >= bsy)
        || (ex <= bex && sy >= bsy)
        || (ex <= bex && ey <= bey);
}
bool AABBox::isIntersect(int x, int y, int z) {
    int sx = start_x;
    int sy = start_y;
    int sz = start_z;
    int ex = end_x;
    int ey = end_y;
    int ez = end_z;
    return x >= start_x && x <= end_x
        && y >= start_y && y <= end_y
        && z >= start_z && z <= end_z;
}

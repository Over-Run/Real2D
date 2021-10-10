#include "real2d/aabb.h"

using Real2D::AABBox;

const AABBox& AABBox::FULL_CUBE = AABBox(0, 0, 0, 1, 1, 1);
AABBox::AABBox(float _start_x,
    float _start_y,
    float _start_z,
    float _end_x,
    float _end_y,
    float _end_z) : start_x(_start_x),
    start_y(_start_y),
    start_z(_start_z),
    end_x(_end_x),
    end_y(_end_y),
    end_z(_end_z) {}
AABBox::AABBox(const AABBox& o) : start_x(o.start_x),
start_y(o.start_y),
start_z(o.start_z),
end_x(o.end_x),
end_y(o.end_y),
end_z(o.end_z) {}
void AABBox::set(const AABBox& o) {
    start_x = o.start_x;
    start_y = o.start_y;
    start_z = o.start_z;
    end_x = o.end_x;
    end_y = o.end_y;
    end_z = o.end_z;
}
void AABBox::set(float _start_x,
    float _start_y,
    float _start_z,
    float _end_x,
    float _end_y,
    float _end_z) {
    start_x = _start_x;
    start_y = _start_y;
    start_z = _start_z;
    end_x = _end_x;
    end_y = _end_y;
    end_z = _end_z;
}
bool AABBox::move(float xoffset,
    float yoffset,
    float zoffset,
    AABBox* dst) {
    if (dst != nullptr) {
        dst->start_x = start_x + xoffset;
        dst->start_y = start_y + yoffset;
        dst->start_z = start_z + zoffset;
        dst->end_x = end_x + xoffset;
        dst->end_y = end_y + yoffset;
        dst->end_z = end_z + zoffset;
        return true;
    }
    return false;
}
bool AABBox::move(float xoffset,
    float yoffset,
    float zoffset) {
    return move(xoffset, yoffset, zoffset, this);
}
AABBox AABBox::expand(float xoffset,
    float yoffset,
    float zoffset) {
    float fsx = start_x;
    float fsy = start_y;
    float fsz = start_z;
    float fex = end_x;
    float fey = end_y;
    float fez = end_z;
#define _EXP(a) if (a##offset < 0) { fs##a += a##offset; } \
    if (a##offset > 0) { fe##a += a##offset; }
    _EXP(x);
    _EXP(y);
    _EXP(z);
#undef _EXP
    return AABBox(fsx, fsy, fsz, fex, fey, fez);
}
bool AABBox::isXCollide(AABBox& b, float axs) {
    if (axs < 0) {
        return b.start_x <= end_x && b.end_x > start_x;
    }
    if (axs > 0) {
        return b.end_x >= start_x && b.start_x < end_x;
    }
    return false;
}
bool AABBox::isYCollide(AABBox& b, float axs) {
    if (axs < 0) {
        return b.start_y <= end_y && b.end_y > end_y;
    }
    if (axs > 0) {
        return b.end_y >= start_y && b.start_y < start_y;
    }
    return false;
}
bool AABBox::isZCollide(AABBox& b, float axs) {
    if (axs < 0) {
        return b.start_z <= end_z && b.end_z > end_z;
    }
    if (axs > 0) {
        return b.end_z >= start_z && start_z < start_z;
    }
    return false;
}
bool AABBox::isIntersect(AABBox& b) {
    float sx = start_x;
    float sy = start_y;
    float sz = start_z;
    float ex = end_x;
    float ey = end_y;
    float ez = end_z;
    float bsx = b.start_x;
    float bsy = b.start_y;
    float bsz = b.start_z;
    float bex = b.end_x;
    float bey = b.end_y;
    float bez = b.end_z;
    return bsx < ex && bex > sx
        && bsy < ey && bey > sy
        && bsz < ez && bez > sz;
}
bool AABBox::isIntersect(float x, float y) {
    float sx = start_x;
    float sy = start_y;
    float ex = end_x;
    float ey = end_y;
    return x >= start_x && x <= end_x
        && y >= start_y && y <= end_y;
}

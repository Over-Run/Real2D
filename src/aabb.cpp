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
void AABBox::move(float xoffset,
    float yoffset,
    float zoffset) {
    start_x += xoffset;
    start_y += yoffset;
    start_z += zoffset;
    end_x += xoffset;
    end_y += yoffset;
    end_z += zoffset;
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
#define _EXP(a) if (##a##offset < 0) { fs##a## += ##a##offset; } \
    if (##a##offset > 0) { fe##a## += ##a##offset; }
    _EXP(x);
    _EXP(y);
    _EXP(z);
#undef _EXP
    return AABBox(fsx, fsy, fsz, fex, fey, fez);
}
#define _CLIP_COL(A, B, C, D, E, F) \
float AABBox::clip##D##Collide(AABBox b, ##E) { \
    float sx = start_x; \
    float sy = start_y; \
    float sz = start_z; \
    float ex = end_x; \
    float ey = end_y; \
    float ez = end_z; \
    float bsx = b.start_x; \
    float bsy = b.start_y; \
    float bsz = b.start_z; \
    float bex = b.end_x; \
    float bey = b.end_y; \
    float bez = b.end_z; \
    if (bs##B < e##B && be##B > s##B \
        && bs##C < e##C && be##C > s##C) { \
        float max; \
        if (A##a > 0 && be##A <= s##A) { \
            max = s##A - be##A; \
            if (max < ##A##a) { \
                A##a = max; \
            } \
        } \
        if (A##a < 0 && bs##A >= e##A) { \
            max = e##A - bs##A; \
            if (max > A##a) { \
                A##a = max; \
            } \
        } \
    } \
    ##F; \
}
_CLIP_COL(x, y, z, X, float xa, return xa)
_CLIP_COL(y, x, z, Y, float ya, return ya)
_CLIP_COL(z, x, y, Z, float za, return za)
#undef _CLIP_COL
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
#if 1
    return bsx < ex && bex > sx
        && bsy < ey && bey > sy
        && bsz < ez && bez > sz;
#else
    return (bsx >= sx && bsx <= ex && bsy >= sy && bsy <= ey && bsz >= sz && bsz <= ez)
        || (bsx >= sx && bsx <= ex && bsy >= sy && bsy <= ey && bez >= sz && bez <= ez)
        || (bex >= sx && bex <= ex && bsy >= sy && bsy <= ey && bez >= sz && bez <= ez)
        || (bex >= sx && bex <= ex && bsy >= sy && bsy <= ey && bsz >= sz && bsz <= ez)
        || (bsx >= sx && bsx <= ex && bey >= sy && bey <= ey && bsz >= sz && bsz <= ez)
        || (bsx >= sx && bsx <= ex && bey >= sy && bey <= ey && bez >= sz && bez <= ez)
        || (bex >= sx && bex <= ex && bey >= sy && bey <= ey && bez >= sz && bez <= ez)
        || (bex >= sx && bex <= ex && bey >= sy && bey <= ey && bsz >= sz && bsz <= ez);
#endif
}
bool AABBox::isIntersect(float x, float y) {
    float sx = start_x;
    float sy = start_y;
    float ex = end_x;
    float ey = end_y;
    return x >= start_x && x <= end_x
        && y >= start_y && y <= end_y;
}

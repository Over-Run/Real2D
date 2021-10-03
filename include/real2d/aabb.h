#pragma once

namespace Real2D {
    struct AABBox {
        static const AABBox& FULL_CUBE;
        int start_x;
        int start_y;
        int start_z;
        int end_x;
        int end_y;
        int end_z;

        AABBox(int _start_x,
            int _start_y,
            int _start_z,
            int _end_x,
            int _end_y,
            int _end_z);
        AABBox move(int xoffset,
            int yoffset,
            int zoffset);
        bool isIntersect(AABBox& b);
        bool isIntersect(int x, int y, int z);
    };
}

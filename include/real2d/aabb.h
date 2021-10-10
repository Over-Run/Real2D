#pragma once

namespace Real2D {
    struct AABBox {
        static const AABBox& FULL_CUBE;
        float start_x;
        float start_y;
        float start_z;
        float end_x;
        float end_y;
        float end_z;

        AABBox(float _start_x = 0,
            float _start_y = 0,
            float _start_z = 0,
            float _end_x = 0,
            float _end_y = 0,
            float _end_z = 0);
        AABBox(const AABBox& other);
        void set(const AABBox& other);
        void set(float _start_x,
            float _start_y,
            float _start_z,
            float _end_x,
            float _end_y,
            float _end_z);
        bool move(float xoffset,
            float yoffset,
            float zoffset,
            AABBox* dst);
        bool move(float xoffset,
            float yoffset,
            float zoffset);
        AABBox expand(float xoffset,
            float yoffset,
            float zoffset);
        bool isXCollide(AABBox& b, float axs);
        bool isYCollide(AABBox& b, float axs);
        bool isZCollide(AABBox& b, float axs);
        /// <summary>
        /// Check b is intersect to self.
        /// </summary>
        /// <param name="b">The other box.</param>
        /// <returns>Is b intersect to self</returns>
        bool isIntersect(AABBox& b);
        bool isIntersect(float x, float y);
    };
}

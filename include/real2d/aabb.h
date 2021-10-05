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

        AABBox(float _start_x,
            float _start_y,
            float _start_z,
            float _end_x,
            float _end_y,
            float _end_z);
        AABBox(const AABBox& other);
        void move(float xoffset,
            float yoffset,
            float zoffset);
        AABBox expand(float xoffset,
            float yoffset,
            float zoffset);
        float clipXCollide(AABBox b, float xa);
        float clipYCollide(AABBox b, float ya);
        float clipZCollide(AABBox b, float za);
        /// <summary>
        /// Check b is intersect to self.
        /// </summary>
        /// <param name="b">The other box.</param>
        /// <returns>Is b intersect to self</returns>
        bool isIntersect(AABBox& b);
        bool isIntersect(float x, float y);
    };
}

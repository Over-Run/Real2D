#include "real2d/rmath.h"

double deg2rad(double deg) {
    return deg / 180.0 * PI;
}
float deg2radf(float deg) {
    return (float)deg2rad(deg);
}

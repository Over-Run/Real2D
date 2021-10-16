#pragma once

// world
constexpr int WORLD_W = 48;
constexpr int WORLD_H = 32;
constexpr int WORLD_D = 2;
constexpr int WORLD_SIZE = WORLD_W * WORLD_H * WORLD_D;
constexpr int WORLD_VER = 1;

#define BLOCK(nm) (Real2D::Blocks::nm)
#define AIR_BLOCK BLOCK(AIR)

#if defined(_MSC_VER) && _MSC_VER >= 1400
#define MSVC8
#endif

#pragma once

// world
constexpr int WORLD_W = 48;
constexpr int WORLD_H = 32;
constexpr int WORLD_D = 2;
constexpr int WORLD_SIZE = WORLD_W * WORLD_H * WORLD_D;

#define BLOCK(nm) (Real2D::Blocks::nm)
#define AIR_BLOCK BLOCK(AIR)

#pragma once
#include "real2d_def.h"

// world
constexpr auto WORLD_RENDER_NML = 32.0f;
#define UNML(x) ((x) * WORLD_RENDER_NML)

// player
constexpr const char* TEX_PLAYER = "res/player.png";
constexpr auto TEX_PLAYER_W = 48.0f;
constexpr auto TEX_PLAYER_H = 64.0f;

// client
constexpr auto GAME_VER = "0.3.0";

// block
constexpr const char* TEX_BLOCKS = "res/block/blocks0.png";
constexpr int BLOCK_TEX_SIZE = 16;
constexpr int BLOCKS_TEX_SIZE = 256;
constexpr int BLOCKS_PER_TEX = BLOCKS_TEX_SIZE / BLOCK_TEX_SIZE;
constexpr auto BLOCK_TEX_UV_FACTOR = (float)BLOCK_TEX_SIZE / (float)BLOCKS_TEX_SIZE;
#define BLOCK_TEX_U0(id) ((((id) - 1) % BLOCKS_PER_TEX) * BLOCK_TEX_UV_FACTOR)
#define BLOCK_TEX_U1(id) (((id) % BLOCKS_PER_TEX) * BLOCK_TEX_UV_FACTOR)
#define BLOCK_TEX_V0(id) ((((id) - 1) / BLOCKS_PER_TEX) * BLOCK_TEX_UV_FACTOR)
#define BLOCK_TEX_V1(id) ((((id) / BLOCKS_PER_TEX) + 1) * BLOCK_TEX_UV_FACTOR)
#define X_OFFSET (GLfloat)(int)((Real2D::Window::width * 0.5f) - (int)UNML(player->prev_x))
#define Y_OFFSET (GLfloat)(int)((Real2D::Window::height * 0.5f) - (int)UNML(player->prev_y))

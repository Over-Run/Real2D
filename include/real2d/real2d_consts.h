#pragma once

// world
constexpr int WORLD_W = 48;
constexpr int WORLD_H = 32;
constexpr int WORLD_D = 2;
constexpr int WORLD_SIZE = WORLD_W * WORLD_H * WORLD_D;
constexpr auto WORLD_RENDER_NML = 32.0f;

// player
constexpr const char* TEX_PLAYER = "res/player.png";
constexpr float TEX_PLAYER_W = 48.0f;
constexpr float TEX_PLAYER_H = 64.0f;

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
#define X_OFFSET (GLfloat)(int)((Real2D::Window::width * 0.5f) - (int)(player->xo * WORLD_RENDER_NML))
#define Y_OFFSET (GLfloat)(int)((Real2D::Window::height * 0.5f) - (int)((player->yo + 1) * WORLD_RENDER_NML))
#define BLOCK(nm) (Real2D::Blocks::nm)
#define AIR_BLOCK BLOCK(AIR)

#define UNML(x) ((x) * WORLD_RENDER_NML)

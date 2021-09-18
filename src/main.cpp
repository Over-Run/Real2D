#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "glad/gl.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "real2d/stb.h"
#include "real2d/block.h"
#include "real2d/timer.h"

#define BLOCK(nm) (Blocks::nm)
#define WORLD_BLOCK(x,y,z) (x + y * WORLD_W + z * WORLD_W * WORLD_H)

using namespace Real2D;
using std::cout;
using std::cerr;
using std::endl;

constexpr int DEF_W = 854;
constexpr int DEF_H = 480;

// 2 pixels
// (step / tps)
constexpr float PLAYER_STEP = 0.03f;

constexpr float playerH = 1.62f;

constexpr int WORLD_W = 32;
constexpr int WORLD_H = 16;
constexpr int WORLD_D = 2;
constexpr int WORLD_SIZE = WORLD_W * WORLD_H * WORLD_D;

using Window = GLFWwindow*;
Window window;

int width = DEF_W;
int height = DEF_H;

float playerX = 16;
float playerY = 5;
float playerZ = 1;

Timer timer(60);

GLuint blocks;
const Block** world = (const Block**)malloc(sizeof(const Block*) * WORLD_SIZE);

bool isKeyDown(int key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

void errcb(int error_, const char* description_) {
    cerr << "GL error " << error_ << ": " << description_ << endl;
}

void keycb(Window window_, int key, int, int action, int) {
    if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window_, GLFW_TRUE);
        }
    }
}

void fbcb(Window window_, int width_, int height_) {
    width = width_;
    height = height_;
    glViewport(0, 0, width_, height_);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width_, 0, height_, -100, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void close() {
    if (world) {
        free(world);
    }
    if (blocks > 0) {
        glDeleteTextures(1, &blocks);
    }
    glfwSetKeyCallback(window, nullptr);
    glfwSetFramebufferSizeCallback(window, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
    glfwSetErrorCallback(nullptr);
}

int loadTexture() {
    int w = 0;
    int h = 0;
    int comp = 0;
    stbi_uc* tex = stbi_load_out("res/block/blocks0.png", &w, &h, &comp, STBI_rgb_alpha);
    if (!tex) {
        return -1;
    }
    glGenTextures(1, &blocks);
    glBindTexture(GL_TEXTURE_2D, blocks);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_RGBA,
        w,
        h,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        tex
    );
    stbi_image_free(tex);
    return 0;
}

int init() {
    glfwSetErrorCallback(errcb);
    if (!glfwInit()) {
        return -1;
    }
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    window = glfwCreateWindow(DEF_W, DEF_H, "Real2D", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, keycb);
    glfwSetFramebufferSizeCallback(window, fbcb);

    const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (vidmode) {
        glfwSetWindowPos(window,
            (vidmode->width - width) >> 1,
            (vidmode->height - height) >> 1);
    }

    int w = 0;
    int h = 0;
    int comp = 0;
    stbi_uc* icon16 = stbi_load_out("res/icon16.png", &w, &h, &comp, STBI_rgb_alpha);
    if (!icon16) {
        return -1;
    }
    stbi_uc* icon32 = stbi_load_out("res/icon32.png", &w, &h, &comp, STBI_rgb_alpha);
    if (!icon32) {
        return -1;
    }
    stbi_uc* icon48 = stbi_load_out("res/icon48.png", &w, &h, &comp, STBI_rgb_alpha);
    if (!icon48) {
        return -1;
    }
    GLFWimage img[] = {
        { 16, 16, icon16 },
        { 32, 32, icon32 },
        { 48, 48, icon48 }
    };
    glfwSetWindowIcon(window, 3, img);
    stbi_image_free(icon16);
    stbi_image_free(icon32);
    stbi_image_free(icon48);

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    fbcb(window, DEF_W, DEF_H);
    glEnable(GL_TEXTURE_2D);
    for (int z = 0; z < WORLD_D; ++z) {
        for (int x = 0; x < WORLD_W; ++x) {
            for (int y = 0; y < WORLD_H; ++y) {
                int p = WORLD_BLOCK(x, y, z);
                if (y < 3) {
                    world[p] = BLOCK(STONE);
                }
                else if (y == 3) {
                    world[p] = BLOCK(GRASS_BLOCK);
                }
                else {
                    world[p] = BLOCK(AIR);
                }
            }
        }
    }

    int i = loadTexture();
    if (i) {
        close();
        return i;
    }
    timer.advanceTime();

    glfwShowWindow(window);
    return 0;
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glPushMatrix();
    glTranslatef((width >> 1) - playerX * BLOCK_RENDER_SIZE,
        (height >> 1) - (playerY + 1) * BLOCK_RENDER_SIZE,
        0);
    glBindTexture(GL_TEXTURE_2D, blocks);
    glBegin(GL_QUADS);
    for (int z = 0; z < WORLD_D; ++z) {
        for (int x = 0; x < WORLD_W; ++x) {
            for (int y = 0; y < WORLD_H; ++y) {
                int p = WORLD_BLOCK(x, y, z);
                const Block* block = world[p];
                if (block != Blocks::AIR) {
                    renderBlock(x, y, z, block);
                }
            }
        }
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
    glDisable(GL_DEPTH_TEST);
    glfwSwapBuffers(window);
}

void tick(double delta) {
    float step = PLAYER_STEP * (float)delta;
    if (isKeyDown(GLFW_KEY_W)) {
        playerZ -= step;
        if (playerZ < -1) {
            playerZ = -1;
        }
    }
    if (isKeyDown(GLFW_KEY_S)) {
        playerZ += step;
        if (playerZ > 1) {
            playerZ = 1;
        }
    }
    if (isKeyDown(GLFW_KEY_SPACE)) {
        playerY += step;
    }
    if (isKeyDown(GLFW_KEY_LEFT_SHIFT)) {
        playerY -= step;
    }
    if (isKeyDown(GLFW_KEY_A)) {
        playerX -= step;
    }
    if (isKeyDown(GLFW_KEY_D)) {
        playerX += step;
    }
}

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nShowCmd
) {
    int i = init();
    if (i) {
        return i;
    }
    glClearColor(0.4f, 0.6f, 0.9f, 1.0f);
    double lastTime = glfwGetTime();
    int frames = 0;
    while (!glfwWindowShouldClose(window)) {
        timer.advanceTime();
        double delta = timer.delta;
        for (int i = 0; i < timer.ticks; ++i) {
            tick(delta);
        }
        render();
        glfwPollEvents();
        ++frames;
        while (glfwGetTime() >= lastTime + 1) {
            timer.fps = frames;
            lastTime += 1000L;
            frames = 0;
        }
    }
    close();
    return 0;
}

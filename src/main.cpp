#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "glad/gl.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "real2d/stb.h"
#include "real2d/block.h"

#define BLOCK(nm) (Block&)Blocks::nm
#define WORLD_BLOCK(x,y,z) x + y * worldW + z * worldW * worldH

using namespace Real2D;
using std::cout;
using std::cerr;
using std::endl;

constexpr int DEF_W = 854;
constexpr int DEF_H = 480;

constexpr float PLAYER_STEP = 2;

constexpr float playerH = 1.62f;

constexpr int worldW = 16;
constexpr int worldH = 16;

using Window = GLFWwindow*;
Window window;

int width = DEF_W;
int height = DEF_H;

float playerX = 0;
float playerY = 0;

int fps = 0;

GLuint blocks;

BlockStates world[worldW * worldH * 2];

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
    glOrtho(0, width_, 0, height_, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void close() {
    if (blocks) {
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
    for (int z = 0; z < 2; ++z) {
        for (int x = 0; x < 16; ++x) {
            for (int y = 0; y < 16; ++y) {
                if (y < 3) {
                    world[WORLD_BLOCK(x, y, z)] = BlockStates(x, y, z, BLOCK(STONE));
                }
            }
        }
    }

    int i = loadTexture();
    if (i) {
        close();
        return i;
    }

    glfwShowWindow(window);
    return 0;
}

void render(double delta) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glTranslatef((width >> 1) - playerX, (height >> 1) - playerY - playerH, 0);
    for (BlockStates states : world) {
        renderBlock(states, blocks);
    }
    glPopMatrix();
    glfwSwapBuffers(window);
}

void tick(double delta) {
    cerr << delta << endl;
    float step = PLAYER_STEP;
    if (isKeyDown(GLFW_KEY_W) || isKeyDown(GLFW_KEY_SPACE)) {
        playerY += step;
    }
    if (isKeyDown(GLFW_KEY_S) || isKeyDown(GLFW_KEY_LEFT_SHIFT)) {
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
    double last = glfwGetTime();
    int frames = 0;
    while (!glfwWindowShouldClose(window)) {
        double now = glfwGetTime();
        double delta = now - last;
        last = now;
        tick(delta);
        render(delta);
        ++frames;
        glfwPollEvents();
    }
    close();
    return 0;
}

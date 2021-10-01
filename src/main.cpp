#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <iostream>

#include "glad/gl.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "real2d/stb.h"
#include "real2d/block.h"
#include "real2d/timer.h"
#include "real2d/player.h"
#include "real2d/texmgr.h"
#include "real2d/world.h"

constexpr int MBL = GLFW_MOUSE_BUTTON_LEFT;
constexpr int MBM = GLFW_MOUSE_BUTTON_MIDDLE;
constexpr int MBR = GLFW_MOUSE_BUTTON_RIGHT;

using Real2D::Block;
using Real2D::Blocks;
using Real2D::Timer;
using Real2D::World;
using std::cout;
using std::cerr;
using std::endl;

constexpr int DEF_W = 854;
constexpr int DEF_H = 480;

using Window = GLFWwindow*;
Window window;

int width = DEF_W;
int height = DEF_H;

int mouseX = 0;
int mouseY = 0;

Timer timer(60);
World world;

GLuint blocks;

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
        if (key == GLFW_KEY_1) {
            choosingBlock = BLOCK(GRASS_BLOCK);
        }
        if (key == GLFW_KEY_2) {
            choosingBlock = BLOCK(STONE);
        }
        if (key == GLFW_KEY_Z) {
            selectz = !selectz;
        }
    }
}

void mbcb(Window window_, int button, int action, int) {
    if (action == GLFW_RELEASE) {
        if (selectblock != nullptr) {
            block_t b = *selectblock;
            if (b == AIR_BLOCK) {
                if (button == MBR
                    && choosingBlock != AIR_BLOCK) {
                    *selectblock = choosingBlock;
                }
            }
            else {
                if (button == MBL) {
                    *selectblock = AIR_BLOCK;
                }
                else if (button == MBM) {
                    choosingBlock = b;
                }
                else if (button == MBR
                    && choosingBlock != AIR_BLOCK) {
                    *selectblock = choosingBlock;
                }
            }
        }
    }
}

void sccb(Window window_, double xoffset, double yoffset) {
    if (yoffset) {
        if (choosingBlock == BLOCK(GRASS_BLOCK)) {
            choosingBlock = BLOCK(STONE);
        }
        else if (choosingBlock == BLOCK(STONE)) {
            choosingBlock = BLOCK(GRASS_BLOCK);
        }
    }
}

void cpcb(Window window_, double x, double y) {
    mouseX = (int)x;
    mouseY = (int)y;
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

void loadTexture() {
    blocks = texmgr.loadTexture(TEX_BLOCKS);
}

namespace Real2D {
    class Real2D {
    public:
        void start();
        void run();
        void render();
        void tick(double delta);
        ~Real2D();
    };
}
void Real2D::Real2D::start() {
    glfwSetErrorCallback(errcb);
    if (!glfwInit()) {
        throw "Unable to initialize GLFW";
    }
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    window = glfwCreateWindow(DEF_W, DEF_H, "Real2D", nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        throw "Unable to create GLFW window";
    }

    glfwSetKeyCallback(window, keycb);
    glfwSetFramebufferSizeCallback(window, fbcb);
    glfwSetCursorPosCallback(window, cpcb);
    glfwSetMouseButtonCallback(window, mbcb);
    glfwSetScrollCallback(window, sccb);

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
    stbi_uc* icon32 = stbi_load_out("res/icon32.png", &w, &h, &comp, STBI_rgb_alpha);
    stbi_uc* icon48 = stbi_load_out("res/icon48.png", &w, &h, &comp, STBI_rgb_alpha);
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
    glClearColor(0.4f, 0.6f, 0.9f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    world.create();

    loadTexture();
    timer.advanceTime();

    glfwShowWindow(window);

    run();
}
void Real2D::Real2D::run() {
    double lastTime = glfwGetTime() * 1000;
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
        while (glfwGetTime() * 1000 >= lastTime + 1000) {
            timer.fps = frames;
            lastTime += 1000;
            frames = 0;
        }
    }
}
void Real2D::Real2D::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    world.renderSelect();
    glfwSwapBuffers(window);
}
void Real2D::Real2D::tick(double delta) {
    player.tick(delta);
}
Real2D::Real2D::~Real2D() {
    glfwSetKeyCallback(window, nullptr);
    glfwSetFramebufferSizeCallback(window, nullptr);
    glfwSetCursorPosCallback(window, nullptr);
    glfwSetMouseButtonCallback(window, nullptr);
    glfwSetScrollCallback(window, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
    glfwSetErrorCallback(nullptr);
}

int main() {
    Real2D::Real2D real2d;
    real2d.start();
    return 0;
}

#ifdef _WIN32
int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nShowCmd
) {
    return main();
}
#endif

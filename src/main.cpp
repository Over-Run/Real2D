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

#define WORLD_BLOCK(x,y,z) (x + y * WORLD_W + z * WORLD_W * WORLD_H)

constexpr int MBL = GLFW_MOUSE_BUTTON_LEFT;
constexpr int MBM = GLFW_MOUSE_BUTTON_MIDDLE;
constexpr int MBR = GLFW_MOUSE_BUTTON_RIGHT;

using Real2D::Block;
using Real2D::Blocks;
using Real2D::Timer;
using std::cout;
using std::cerr;
using std::endl;

constexpr int DEF_W = 854;
constexpr int DEF_H = 480;

constexpr int WORLD_W = 32;
constexpr int WORLD_H = 16;
constexpr int WORLD_D = 2;
constexpr int WORLD_SIZE = WORLD_W * WORLD_H * WORLD_D;

using Window = GLFWwindow*;
Window window;

int width = DEF_W;
int height = DEF_H;

int mouseX = 0;
int mouseY = 0;

Timer timer(60);

GLuint blocks;
block_t* world = (block_t*)malloc(sizeof(block_t) * WORLD_SIZE);

block_t choosingBlock = BLOCK(GRASS_BLOCK);

int selectx = 0;
int selecty = 0;
float selectbx = 0;
float selectbx1 = 0;
float selectby = 0;
float selectby1 = 0;
block_t* selectblock0 = nullptr;
block_t* selectblock1 = nullptr;

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
    }
}

void mbcb(Window window_, int button, int action, int) {
    if (selectblock0 != nullptr
        && selectblock1 != nullptr) {
        if (*selectblock1 == AIR_BLOCK) {
            if (*selectblock0 == AIR_BLOCK) {
                if (choosingBlock != AIR_BLOCK) {
                    if (action == GLFW_RELEASE
                        && button == MBR) {
                        *selectblock0 = choosingBlock;
                    }
                }
                if (action == GLFW_RELEASE
                    && button == MBM) {
                    choosingBlock = AIR_BLOCK;
                }
            }
            else {
                if (choosingBlock != AIR_BLOCK) {
                    if (action == GLFW_RELEASE
                        && button == MBR) {
                        *selectblock1 = choosingBlock;
                    }
                }
                if (action == GLFW_RELEASE
                    && button == MBL) {
                    *selectblock0 = AIR_BLOCK;
                }
                else if (action == GLFW_RELEASE
                    && button == MBM) {
                    choosingBlock = *selectblock0;
                }
            }
        }
        else {
            if (action == GLFW_RELEASE
                && button == MBL) {
                *selectblock1 = AIR_BLOCK;
            }
            else if (action == GLFW_RELEASE
                && button == MBM) {
                choosingBlock = *selectblock1;
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
    blocks = texmgr.loadTexture("res/block/blocks0.png");
}

void renderWorld() {
    glBindTexture(GL_TEXTURE_2D, blocks);
    glBegin(GL_QUADS);
    for (int z = 0; z < WORLD_D; ++z) {
        for (int x = 0; x < WORLD_W; ++x) {
            for (int y = 0; y < WORLD_H; ++y) {
                int p = WORLD_BLOCK(x, y, z);
                block_t block = world[p];
                if (block != AIR_BLOCK) {
                    renderBlock(x, y, z, block, 0);
                }
            }
        }
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glBegin(GL_QUADS);
    for (int x = 0; x < WORLD_W; ++x) {
        for (int y = 0; y < WORLD_H; ++y) {
            int p = WORLD_BLOCK(x, y, 0);
            block_t block = world[p];
            if (block != AIR_BLOCK) {
                renderBlock(x, y, 0, block, 1);
            }
        }
    }
    glEnd();
}

void readyPutBlock(int x, int y, int z) {
    glBindTexture(GL_TEXTURE_2D, blocks);
    glBegin(GL_QUADS);
    renderBlock(x, y, z, choosingBlock, 2);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void readyDestroyBlock(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
    glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
    glRectf(x1, y1, x2, y2);
}

void selectWorld() {
    int mx = mouseX;
    int my = height - mouseY;
    bool selected = false;
    for (int x = 0; x < WORLD_W; ++x) {
        for (int y = 0; y < WORLD_H; ++y) {
            block_t& block0 = world[WORLD_BLOCK(x, y, 0)];
            block_t& block1 = world[WORLD_BLOCK(x, y, 1)];
            float bx = XLATE(x) + X_OFFSET;
            float bx1 = bx + BLOCK_RENDER_SIZE;
            float by = XLATE(y) + Y_OFFSET;
            float by1 = by + BLOCK_RENDER_SIZE;
            if (mx >= bx
                && mx < bx1
                && my >= by
                && my < by1) {
                selected = true;
                selectx = x;
                selectx = y;
                selectbx = bx;
                selectbx1 = bx1;
                selectby = by;
                selectby1 = by1;
                selectblock0 = &block0;
                selectblock1 = &block1;
                if (block1 == AIR_BLOCK) {
                    if (block0 == AIR_BLOCK) {
                        if (choosingBlock != AIR_BLOCK) {
                            readyPutBlock(x, y, 0);
                        }
                    }
                    else {
                        if (choosingBlock != AIR_BLOCK) {
                            readyPutBlock(x, y, 1);
                        }
                        else {
                            readyDestroyBlock(bx, by, bx1, by1);
                        }
                    }
                }
                else {
                    readyDestroyBlock(bx, by, bx1, by1);
                }
                goto unloop;
            }
        }
    }
unloop:
    // Check outside world
    if (!selected) {
        selectblock0 = nullptr;
        selectblock1 = nullptr;
    }
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
                    world[p] = AIR_BLOCK;
                }
            }
        }
    }

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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    renderWorld();
    glDisable(GL_DEPTH_TEST);
    selectWorld();
    glEnable(GL_DEPTH_TEST);
    player.render();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glfwSwapBuffers(window);
}
void Real2D::Real2D::tick(double delta) {
    player.tick(delta);
}
Real2D::Real2D::~Real2D() {
    if (world) {
        free(world);
    }
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

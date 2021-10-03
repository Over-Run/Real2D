#include "real2d/client.h"
#include "real2d/window.h"
#include "GLFW/glfw3.h"
#include "glad/gl.h"
#include "real2d/stb.h"
#include "real2d/timer.h"
#include "real2d/world.h"
#include "real2d/player.h"
#include "real2d/texmgr.h"

using Real2D::Timer;
using Real2D::World;

window_t window;

Timer timer(60);
World world;

GLuint blocks;

void loadTexture() {
    blocks = texmgr.loadTexture(TEX_BLOCKS);
}

void Real2D::Real2D::start() {
    glfwSetErrorCallback(Window::errcb);
    if (!glfwInit()) {
        throw "Unable to initialize GLFW";
    }
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    char c[20] = "Real2D ";
#if defined(_MSC_VER) && _MSC_VER >= 1400
    strcat_s(c, 20, GAME_VER);
#else
    strcat(c, GAME_VER);
#endif
    window = glfwCreateWindow(DEF_W, DEF_H, c, nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        throw "Unable to create GLFW window";
    }

    glfwSetKeyCallback(window, Window::keycb);
    glfwSetFramebufferSizeCallback(window, Window::fbcb);
    glfwSetCursorPosCallback(window, Window::cpcb);
    glfwSetScrollCallback(window, Window::sccb);

    const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (vidmode) {
        glfwSetWindowPos(window,
            (vidmode->width - Window::width) >> 1,
            (vidmode->height - Window::height) >> 1);
    }

    GLFWimage img[3];
    int w = 0;
    int h = 0;
    int comp = 0;
    auto icon16 = stbi_load_out("res/icon16.png", &w, &h, &comp, STBI_rgb_alpha);
    img[0] = { w, h, icon16 };
    auto icon32 = stbi_load_out("res/icon32.png", &w, &h, &comp, STBI_rgb_alpha);
    img[1] = { w, h, icon32 };
    auto icon48 = stbi_load_out("res/icon48.png", &w, &h, &comp, STBI_rgb_alpha);
    img[2] = { w, h, icon48 };
    glfwSetWindowIcon(window, 3, img);
    stbi_image_free(icon16);
    stbi_image_free(icon32);
    stbi_image_free(icon48);

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    Window::fbcb(window, DEF_W, DEF_H);
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
    glfwSetScrollCallback(window, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
    glfwSetErrorCallback(nullptr);
}

#include "real2d/client.h"
#include "real2d/window_c.h"
#include "GLFW/glfw3.h"
#include "glad/gl.h"
#include "real2d/stb_c.h"
#include "real2d/timer.h"
#include "real2d/world.h"
#include "real2d/texmgr_c.h"
#include "real2d/real2d_def_c.h"
#include <cstdarg>
#include <cstdlib>

using std::va_list;
using Real2D::Timer;
using Real2D::World;
using Real2D::Client;

window_t window;

Timer timer(60);
World* world;

GLuint blocks;

void loadTexture() {
    blocks = texmgr.loadTexture(TEX_BLOCKS);
}

char* appendTitle(int count, ...) {
    static char c[80];
    memset(c, 0, 80);
    va_list valist;
    va_start(valist, count);
#ifdef MSVC8
    strcat_s(c, 80, "Real2D ");
    strcat_s(c, 80, GAME_VER);
#else
    strcat(c, "Real2D ");
    strcat(c, GAME_VER);
#endif
    for (int i = 0; i < count; ++i) {
#ifdef MSVC8
        strcat_s(c, 80,
#else
        strcat(c,
#endif
            va_arg(valist, const char*));
    }
    va_end(valist);
    return c;
}

void Client::start() {
    glfwSetErrorCallback(Window::errcb);
    if (!glfwInit()) {
        throw "Unable to initialize GLFW";
    }
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    auto c = appendTitle(0);
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
    world = new World();
    world->create();

    loadTexture();
    timer.advanceTime();

    glfwShowWindow(window);

    run();
}
void Client::run() {
    int64_t lastTime = (int64_t)(glfwGetTime() * 1000);
    int frames = 0;
    while (!glfwWindowShouldClose(window)) {
        timer.advanceTime();
        for (int i = 0; i < timer.ticks; ++i) {
            tick();
        }
        render(timer.delta);
        glfwPollEvents();
        ++frames;
        while (glfwGetTime() * 1000 >= lastTime + 1000LL) {
            char s[32];
#ifdef MSVC8
            sprintf_s(s, 32, " FPS: %d", frames);
#else
            sprintf(s, " FPS: %d", frames);
#endif
            auto c = appendTitle(1, s);
            glfwSetWindowTitle(window, c);
            lastTime += 1000;
            frames = 0;
        }
    }
}
void Client::render(double delta) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    world->renderSelect(delta);
    glDisable(GL_DEPTH_TEST);
    glfwSwapBuffers(window);
}
void Client::tick() {
    world->tick();
}
Client::~Client() {
    if (world != nullptr) {
        delete world;
    }
    glfwSetKeyCallback(window, nullptr);
    glfwSetFramebufferSizeCallback(window, nullptr);
    glfwSetCursorPosCallback(window, nullptr);
    glfwSetScrollCallback(window, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
    glfwSetErrorCallback(nullptr);
}

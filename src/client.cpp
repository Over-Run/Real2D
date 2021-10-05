#include "real2d/client.h"
#include "real2d/window.h"
#include "GLFW/glfw3.h"
#include "glad/gl.h"
#include "real2d/stb.h"
#include "real2d/timer.h"
#include "real2d/world.h"
#include "real2d/player.h"
#include "real2d/texmgr.h"
#include <cstdarg>
#include <cstdlib>

using std::va_list;
using Real2D::Timer;
using Real2D::World;
using Real2D::Player;
using Real2D::Client;

window_t window;

Timer timer(60);
World* world;
Player* player;

GLuint blocks;

void loadTexture() {
    blocks = texmgr.loadTexture(TEX_BLOCKS);
}

const char* appendTitle(int count, ...) {
    char* c = new char[80]{ "Real2D " };
    va_list valist;
    va_start(valist, count);
#if defined(_MSC_VER) && _MSC_VER >= 1400
    strcat_s(c, 80, GAME_VER);
#else
    strcat(c, GAME_VER);
#endif
    for (int i = 0; i < count; ++i) {
#if defined(_MSC_VER) && _MSC_VER >= 1400
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
    delete[] c;
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
    player = new Player(world);

    loadTexture();
    timer.advanceTime();

    glfwShowWindow(window);

    run();
}
void Client::run() {
    __int64 lastTime = (__int64)(glfwGetTime() * 1000);
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
#if defined(_MSC_VER) && _MSC_VER >= 1400
            sprintf_s(s, 32, " FPS: %d", frames);
#else
            sprintf(s, " FPS: %d", frames);
#endif
            auto c = appendTitle(1, s);
            glfwSetWindowTitle(window, c);
            delete[] c;
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
    player->render(delta);
    glDisable(GL_DEPTH_TEST);
    glfwSwapBuffers(window);
}
void Client::tick() {
    world->tick();
    player->tick();
}
Client::~Client() {
    if (world != nullptr) {
        delete world;
    }
    if (player != nullptr) {
        delete player;
    }
    glfwSetKeyCallback(window, nullptr);
    glfwSetFramebufferSizeCallback(window, nullptr);
    glfwSetCursorPosCallback(window, nullptr);
    glfwSetScrollCallback(window, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
    glfwSetErrorCallback(nullptr);
}

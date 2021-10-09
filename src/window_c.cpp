#include "real2d/window_c.h"
#include "real2d/world.h"
#include "real2d/real2d_def.h"
#include <iostream>

using std::cerr;
using std::endl;
using Real2D::Window;

extern window_t window;

int Window::width = DEF_W;
int Window::height = DEF_H;
int Window::mouseX = 0;
int Window::mouseY = 0;

bool Window::isKeyDown(int key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

void Window::errcb(int error_, const char* description_) {
    cerr << "GL error " << error_ << ": " << description_ << endl;
}

void Window::keycb(window_t window_, int key, int, int action, int) {
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

void Window::sccb(window_t window_, double xoffset, double yoffset) {
    if (yoffset) {
        if (choosingBlock == BLOCK(GRASS_BLOCK)) {
            choosingBlock = BLOCK(STONE);
        }
        else if (choosingBlock == BLOCK(STONE)) {
            choosingBlock = BLOCK(GRASS_BLOCK);
        }
    }
}

void Window::cpcb(window_t window_, double x, double y) {
    mouseX = (int)x;
    mouseY = (int)y;
}

void Window::fbcb(window_t window_, int width_, int height_) {
    width = width_;
    height = height_;
    glViewport(0, 0, width_, height_);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width_, 0, height_, -100, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

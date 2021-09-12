#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "glad/gl.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

using Window = GLFWwindow*;
Window window;

int width = 854, height = 480;

using std::cerr;
using std::endl;

void errcb(int error_, const char* description_) {
    cerr << "GL error " << error_ << ": " << description_ << endl;
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

int init() {
    glfwSetErrorCallback(errcb);
    if (!glfwInit())
    {
        return -1;
    }
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    window = glfwCreateWindow(width, height, "Real2D", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, fbcb);
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);
    glfwShowWindow(window);
    return 0;
}

int render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return 0;
}

void close() {
    glfwSetFramebufferSizeCallback(window, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
    glfwSetErrorCallback(nullptr);
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
    while (!glfwWindowShouldClose(window))
    {
        i = render();
        if (i) {
            close();
            return i;
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    close();
    return 0;
}

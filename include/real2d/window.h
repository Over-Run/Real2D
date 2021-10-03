#pragma once
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

constexpr int MBL = GLFW_MOUSE_BUTTON_LEFT;
constexpr int MBM = GLFW_MOUSE_BUTTON_MIDDLE;
constexpr int MBR = GLFW_MOUSE_BUTTON_RIGHT;

constexpr int DEF_W = 854;
constexpr int DEF_H = 480;

struct GLFWwindow;
using window_t = GLFWwindow*;

namespace Real2D {
    class Window {
    public:
        static int width;
        static int height;
        static int mouseX;
        static int mouseY;

        static bool isKeyDown(int);
        static void errcb(int, const char*);
        static void keycb(window_t, int, int, int, int);
        static void sccb(window_t, double, double);
        static void cpcb(window_t, double, double);
        static void fbcb(window_t, int, int);
    };
}

#pragma once
#include <map>
#include <string>
#include <cstring>
#include "glad/gl.h"

namespace Real2D {
    class TexMgr {
    public:
        static std::map<std::string, GLuint> idmap;
        static GLuint loadTexture(std::string img);
        static void bindTexture(int id);
        static void bindTexture(std::string img);
    };
}

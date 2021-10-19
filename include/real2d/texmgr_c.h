#pragma once
#include <map>
#include <string>
#include <cstring>
#include "glad/gl.h"

namespace Real2D {
    class TexMgr {
    public:
        static std::map<std::string, GLuint> id_map;
        static GLuint loadTexture(std::string img);
        static void bindTexture(GLuint id);
        static void bindTexture(std::string img);
    };
}

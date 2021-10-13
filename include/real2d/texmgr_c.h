#pragma once
#include <map>
#include <string>
#include <cstring>
#include "glad/gl.h"

namespace Real2D {
    struct TexMgr {
        std::map<std::string, GLuint> idmap;
        GLuint lastId;
        GLuint loadTexture(std::string img);
        void bindTexture(GLuint id);
        void bindTexture(std::string img);
    };
}

extern Real2D::TexMgr texmgr;

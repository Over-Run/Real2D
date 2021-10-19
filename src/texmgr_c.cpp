#include "real2d/texmgr_c.h"
#include "real2d/stb_c.h"
#include "real2d/real2d_def.h"

using Real2D::TexMgr;
using std::map;
using std::pair;
using std::string;

std::map<std::string, GLuint> TexMgr::id_map;
GLuint TexMgr::loadTexture(string img) {
    if (id_map.count(img) > 0) {
        return id_map[img];
    }
    int w = 0;
    int h = 0;
    int comp = 0;
    GLuint id = 0;
    const size_t l = img.size() + 1;
    char* c = new char[l];
#ifdef MSVC8
    strcpy_s(c, l, img.c_str());
#else
    strcpy(c, img.c_str());
#endif
    stbi_uc* tex = stbi_load_out(c, &w, &h, &comp, STBI_rgb_alpha);
    glGenTextures(1, &id);
    bindTexture(id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_RGBA,
        w,
        h,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        tex
    );
    stbi_image_free(tex);
    id_map[img] = id;
    delete[] c;
    return id;
}
void TexMgr::bindTexture(GLuint id) {
    glBindTexture(GL_TEXTURE_2D, id);
}
void TexMgr::bindTexture(string img) {
    bindTexture(loadTexture(img));
}

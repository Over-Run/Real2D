#include "real2d/texmgr.h"
#include "real2d/stb.h"

using Real2D::TexMgr;
using std::map;
using std::pair;
using std::string;

TexMgr texmgr;

GLuint TexMgr::loadTexture(string img) {
    if (idmap.count(img) > 0) {
        return idmap[img];
    }
    int w = 0;
    int h = 0;
    int comp = 0;
    GLuint id = 0;
    const size_t l = img.size() + 1;
    char* c = new char[l];
#if defined(_MSC_VER) && _MSC_VER >= 1400
    strcpy_s(c, l, img.c_str());
#else
    strcpy(c, img.c_str());
#endif
    stbi_uc* tex = stbi_load_out(c, &w, &h, &comp, STBI_rgb_alpha);
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
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
    idmap[img] = id;
    delete[] c;
    return id;
}

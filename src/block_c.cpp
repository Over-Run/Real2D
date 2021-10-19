#include "real2d/block_c.h"
#include "real2d/player.h"
#include "real2d/real2d_def_c.h"
#include "real2d/reg.h"
#include "real2d/world.h"
#include "glad/gl.h"

using Real2D::BlockRenderer;

void BlockRenderer::renderBlock(int x, int y, int z, block_t block, float light) {
    GLfloat xi = (GLfloat)UNML(x);
    GLfloat xi1 = (GLfloat)UNML(x + 1);
    GLfloat yi = (GLfloat)UNML(y);
    GLfloat yi1 = (GLfloat)UNML(y + 1);
    GLfloat zi = (GLfloat)UNML(z);
    int id = block->getId();
    GLfloat u0 = BLOCK_TEX_U0(id);
    GLfloat u1 = BLOCK_TEX_U1(id);
    GLfloat v0 = BLOCK_TEX_V0(id);
    GLfloat v1 = BLOCK_TEX_V1(id);
    glColor3f(light, light, light);
    glTexCoord2f(u0, v0); glVertex3f(xi, yi1, zi);
    glTexCoord2f(u0, v1); glVertex3f(xi, yi, zi);
    glTexCoord2f(u1, v1); glVertex3f(xi1, yi, zi);
    glTexCoord2f(u1, v0); glVertex3f(xi1, yi1, zi);
}

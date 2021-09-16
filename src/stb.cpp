#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "real2d/stb.h"

using std::cerr;
using std::endl;

stbi_uc* stbi_load_out(const char* filename, int* x, int* y, int* comp, int req_comp) {
    stbi_uc* data = stbi_load(filename, x, y, comp, req_comp);
    if (!data) {
        cerr << "Error loading image " << filename << ": " << stbi_failure_reason() << endl;
        return nullptr;
    }
    return data;
}

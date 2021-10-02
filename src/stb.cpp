#include <iostream>
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#include "real2d/stb.h"

using std::cerr;
using std::endl;
using std::ostringstream;

stbi_uc* stbi_load_out(const char* filename, int* x, int* y, int* comp, int req_comp) {
    stbi_uc* data = stbi_load(filename, x, y, comp, req_comp);
    if (!data) {
        ostringstream oss;
        oss << "Error loading image " << filename << ": " << stbi_failure_reason();
        std::string s = oss.str();
        cerr << s << endl;
        throw s;
    }
    return data;
}

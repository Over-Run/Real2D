#pragma once
#include "stb/stb_image.h"

extern stbi_uc* stbi_load_out(const char* filename, int* x, int* y, int* comp, int req_comp);

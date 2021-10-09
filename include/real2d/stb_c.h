#pragma once
#include "stb/stb_image.h"

extern stbi_uc* stbi_load_out(const char* filename, _Out_ int* x, _Out_ int* y, _Out_ int* comp, int req_comp);

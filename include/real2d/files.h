#pragma once
#include <fstream>

extern bool createDir(const char* dir_name);
extern void stream_write(std::ofstream& os, int* i);
extern void stream_write(std::ofstream& os, float* i);
extern void stream_read(std::ifstream& is, int* i);
extern void stream_read(std::ifstream& is, float* i);

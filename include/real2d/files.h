#pragma once
#include <fstream>

using byte = int8_t;

extern bool createDir(const char* dir_name);
extern void stream_write(std::ostream& os, byte* i);
extern void stream_write(std::ostream& os, int* i);
extern void stream_write(std::ostream& os, float* i);
extern void stream_write(std::ostream& os, size_t* i);
extern void stream_read(std::istream& is, byte* i);
extern void stream_read(std::istream& is, int* i);
extern void stream_read(std::istream& is, float* i);
extern void stream_read(std::istream& is, size_t* i);

#include "real2d/files.h"
#include "real2d/real2d_def.h"
#include <filesystem>

using std::string;
using std::istream;
using std::ostream;
using std::filesystem::create_directories;

bool createDir(const char* dir_name) {
    return create_directories(dir_name);
}

void stream_write(ostream& os, byte* i) {
    os.write((char*)i, sizeof(byte));
}
void stream_write(ostream& os, const int* i) {
    os.write((char*)i, sizeof(int));
}
void stream_write(ostream& os, float* i) {
    os.write((char*)i, sizeof(float));
}
void stream_write(ostream& os, size_t* i) {
    os.write((char*)i, sizeof(size_t));
}
void stream_write(ostream& os, const char* i) {
    os.write(i, strlen(i) + 1);
}
void stream_write(ostream& os, string i) {
    size_t psz = i.size() + 1;
    char* p = new char[psz];
#ifdef MSVC8
    strcpy_s(p, psz, i.c_str());
#else
    strcpy(p, i.c_str());
#endif
    stream_write(os, p);
    delete[] p;
}

void stream_read(istream& is, byte* i) {
    is.read((char*)i, sizeof(byte));
}
void stream_read(istream& is, int* i) {
    is.read((char*)i, sizeof(int));
}
void stream_read(istream& is, float* i) {
    is.read((char*)i, sizeof(float));
}
void stream_read(istream& is, size_t* i) {
    is.read((char*)i, sizeof(size_t));
}
void stream_read(istream& is, int sz, char* i) {
    is.read(i, sz);
}
void stream_read(istream& is, int sz, string* i) {
    char* p = new char[sz];
    is.read(p, sz);
    *i = string(p);
    delete[] p;
}

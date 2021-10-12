#include "real2d/files.h"
#include <filesystem>

using std::istream;
using std::ostream;
using std::filesystem::create_directories;

bool createDir(const char* dir_name) {
    return create_directories(dir_name);
}
void stream_write(ostream& os, int* i) {
    os.write((char*)i, sizeof(int));
}
void stream_write(ostream& os, float* i) {
    os.write((char*)i, sizeof(float));
}
void stream_write(ostream& os, byte* i) {
    os.write((char*)i, sizeof(byte));
}
void stream_write(ostream& os, size_t* i) {
    os.write((char*)i, sizeof(size_t));
}
void stream_read(istream& is, int* i) {
    is.read((char*)i, sizeof(int));
}
void stream_read(istream& is, float* i) {
    is.read((char*)i, sizeof(float));
}
void stream_read(istream& is, byte* i) {
    is.read((char*)i, sizeof(byte));
}
void stream_read(istream& is, size_t* i) {
    is.read((char*)i, sizeof(size_t));
}

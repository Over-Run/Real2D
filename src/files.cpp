#include "real2d/files.h"
#include <filesystem>

using std::ifstream;
using std::ofstream;
using std::filesystem::create_directories;

bool createDir(const char* dir_name) {
    return create_directories(dir_name);
}
void stream_write(ofstream& os, int* i) {
    os.write((char*)i, sizeof(int));
}
void stream_write(ofstream& os, float* i) {
    os.write((char*)i, sizeof(float));
}
void stream_read(ifstream& is, int* i) {
    is.read((char*)i, sizeof(int));
}
void stream_read(ifstream& is, float* i) {
    is.read((char*)i, sizeof(float));
}

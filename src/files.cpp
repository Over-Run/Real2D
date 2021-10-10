#include "real2d/files.h"
#include <filesystem>

using std::filesystem::create_directories;

bool createDir(const char* dir_name) {
    return create_directories(dir_name);
}

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "real2d/client.h"

int main() {
    Real2D::Real2D real2d;
    real2d.start();
    return 0;
}

#ifdef _WIN32
int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nShowCmd
) {
    return main();
}
#endif

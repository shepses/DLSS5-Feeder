#include <new>
#include <stdexcept>
#include <cstdlib>
#include <exception>

extern "C" {
    // 1. Floating-point usage marker
    int _fltused = 1;

    // 2. Security cookie stubs for MSVC /GS stack protection
    __int64 __security_cookie = 0x2B992DDFA232LL;

    void __cdecl __security_check_cookie(__int64 _StackCookie) {}
    void __cdecl __GSHandlerCheck(void) {}
    void __cdecl __GSHandlerCheck_EH(void) {}

    // 3. Thread-safe static initialization hooks (MSVC C++ runtime)
    int _Init_thread_header(int* volatile pOnce) { return 0; }
    void _Init_thread_footer(int* volatile pOnce) {}
    int _Init_thread_epoch(int* volatile pOnce) { return 1; }
}

// Force Clang to emit the exact MSVC mangled symbols for the STL exception helpers
extern "C" {
    __declspec(dllexport) void __cdecl __asm_Xbad_alloc() {
        std::terminate();
    }
    __declspec(dllexport) void __cdecl __asm_Xlength_error(const char*) {
        std::terminate();
    }
}

// Map the MSVC mangled names directly to our implementations
namespace std {
    void __cdecl _Xbad_alloc() __asm__("?_Xbad_alloc@std@@YAXXZ");
    void __cdecl _Xlength_error(const char*) __asm__("?_Xlength_error@std@@YAXPEBD@Z");
}

// Function bodies
void __cdecl std::_Xbad_alloc() {
    __asm_Xbad_alloc();
}

void __cdecl std::_Xlength_error(const char* msg) {
    __asm_Xlength_error(msg);
}
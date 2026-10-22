#pragma once

#if defined(_MSC_VER)
    // MSVC (Windows)
    #define DEBUG_BREAK() __debugbreak()

#elif defined(__GNUC__) || defined(__clang__)
    #if defined(_WIN32)
        // MinGW / MinGW-w64 on Windows: no SIGTRAP-based debugger break,
        // so use the compiler trap builtin.
        #define DEBUG_BREAK() __builtin_trap()
    #else
        // GCC/Clang on Linux (and other POSIX systems)
        #include <csignal>
        #define DEBUG_BREAK() raise(SIGTRAP)
    #endif

#else
    // Unknown compiler: fall back to abort()
    #include <stdlib.h>
    #define DEBUG_BREAK() abort()
#endif

#define ASSERT(x)                    \
    do {                             \
        if (!(x)) DEBUG_BREAK();     \
    } while (0)

#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))


void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

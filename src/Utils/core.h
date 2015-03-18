#pragma once

#include "platform_detect.h"


#if LFS_HAS_STDINT
    #include <stdint.h>
#else
    #include "private/stdint.h"
#endif

#if LFS_COMPILER_MSVC
    #include "private/core_msvc.h"
#elif LFS_COMPILER_GCC
    #include "private/core_gcc.h"
#else
    #error Unsupported platform!
#endif

// Windows defines
#ifdef min
    #undef min
#endif

#ifdef max
    #undef max
#endif

#include <iostream>
#include <chrono>
#include <functional>
#include <future>
#include <vector>
#include "Broadcast.h"
using time_base = std::chrono::microseconds;


#define _PASTE(x, y) x ## y
#define PASTE(x, y) _PASTE(x, y)

#define signals public

#define signal(f, ...) \
    void f(__VA_ARGS__) { \
    };


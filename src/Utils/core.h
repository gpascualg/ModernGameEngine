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

#define signals public
#define SIGNAL {}

#include <GLFW/glfw3.h>
#ifdef _MSC_VER
#undef APIENTRY
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>
#endif

#pragma once

#include "platform_detect.hpp"

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


// Basic system-wide includes
#include <iostream>
#include <chrono>
#include <functional>
#include <future>
#include <vector>
#include "Broadcast.hpp"
using time_base = std::chrono::microseconds;


// Signal related defines
static volatile uint64_t signal_no_optimize = 0;
#define signals         public
#define SIGNAL(x)       void x { signal_no_optimize = __LINE__; }


// Include GLFW and GLEW
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#ifdef _MSC_VER
#undef APIENTRY
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>
#endif


// Math library
#include <glm/glm.hpp>
using point4 = glm::vec4;
using color4 = glm::vec4;


// Windows specifics
#ifdef LFS_COMPILER_MSVC
    // Undef Windows imposed defines
    #ifdef min
        #undef min
    #endif

    #ifdef max
        #undef max
    #endif

    // Assert alignments
    static_assert(sizeof(glm::vec3) == sizeof(GLfloat)* 3, "Compiler is not aligning glm::vec3 correctly.");
    static_assert(sizeof(glm::vec4) == sizeof(GLfloat)* 4, "Compiler is not aligning glm::vec4 correctly.");    

    // Disable alignment warning
    #pragma warning(disable: 4316)
#endif

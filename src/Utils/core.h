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

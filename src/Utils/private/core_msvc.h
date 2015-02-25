#pragma once

#if LFS_TARGET_XBOX_360    // Xbox 360
    #include <xtl.h>
#else                       // Windows
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
    #undef WIN32_LEAN_AND_MEAN
    #include <intrin.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif


//-------------------------------------
//  Alignment
//-------------------------------------
#define LFS_DECL_ALIGNED(declaration, amt) __declspec(align(amt)) declaration


//-------------------------------------
//  Inlining
//-------------------------------------
#ifdef __cplusplus
#define LFS_INLINE inline
#define LFS_C_INLINE inline
#else
#define LFS_C_INLINE __inline
#endif

#define LFS_NO_INLINE __declspec(noinline)


//-------------------------------------
//  C++11 support
//-------------------------------------
#define LFS_NOEXCEPT
#define LFS_CONSTEXPR


//-------------------------------------
//  Thread local
//-------------------------------------
#define LFS_THREAD_LOCAL __declspec(thread)


#ifdef __cplusplus
} // extern "C"
#endif

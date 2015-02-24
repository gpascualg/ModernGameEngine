#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


//-------------------------------------
//  Alignment
//-------------------------------------
// Note: May not work on local variables.
// http://gcc.gnu.org/bugzilla/show_bug.cgi?id=24691
#define LFS_DECL_ALIGNED(declaration, amt) declaration __attribute__((aligned(amt)))


//-------------------------------------
//  Inlining
//-------------------------------------
#define LFS_INLINE inline
#define LFS_C_INLINE static LFS_INLINE
#define LFS_NO_INLINE __attribute__((noinline))


//-------------------------------------
//  Thread local
//-------------------------------------
#define LFS_THREAD_LOCAL __thread


#ifdef __cplusplus
} // extern "C"
#endif

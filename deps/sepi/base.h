#ifndef SEPI_BASE_H
#define SEPI_BASE_H

/* ===================================================== */
/*                         TYPES                         */
/* ===================================================== */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef int8_t I8;
typedef uint8_t U8;
typedef int16_t I16;
typedef uint16_t U16;
typedef int32_t I32;
typedef uint32_t U32;
typedef int64_t I64;
typedef uint64_t U64;
typedef size_t Sz;

typedef float F32;
typedef double F64;

typedef I8 Bool;
#define TRUE 1
#define FALSE 0

typedef void Nothing;
typedef void* RawPtr;
typedef intptr_t IPtr;
typedef uintptr_t UPtr;
typedef ptrdiff_t DPtr;

typedef char* Str;
typedef const char* CStr;
typedef unsigned char* Buf;
typedef const unsigned char* CBuf;

/* ===================================================== */
/*                       KEYWORDS                        */
/* ===================================================== */

#define internal static

/* ===================================================== */
/*                       PLATFOTM                        */
/* ===================================================== */

/* COMPILER */
#if defined(__clang__)
#define CC_CLANG 1
#elif defined(__GNUC__) || defined(__GNUG__)
#define CC_GCC 1
#elif defined(_MSC_VER)
#define CC_MSVC 1
#elif defined(__TINYC__)
#define CC_TCC 1
#else
#error unsuppored c compiler!
#endif

/* OPERATING SYSTEM */
#if defined(_WIN32)
#define OS_WINDOWS 1
#elif defined(__gnu_linux__) || defined(__linux__)
#define OS_LINUX 1
#define _GNU_SOURCE
#elif defined(__APPLE__) && defined(__MACH__)
#define OS_MAC 1
#else
#error unsupported operating system!
#endif

/* CPU ARCHITECTURE */
#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64)
#define CPU_X64 1
#elif defined(i386) || defined(__i386) || defined(__i386__)
#define CPU_X86 1
#elif defined(__aarch64__)
#define CPU_ARM64 1
#elif defined(__arm__)
#define CPU_ARM32 1
#else
#error unsupported cpu architecture!
#endif

/* ===================================================== */
/*                         DEBUG                         */
/* ===================================================== */

#if defined(DEBUG) || defined(_DEBUG)
#define DEBUG_MODE
#else
#undef DEBUG_MODE
#endif /* DEBUG_MODE */

/* ===================================================== */
/*                        MEMORY                         */
/* ===================================================== */

#if CC_MSVC
#if defined(__SANITIZE_ADDRESS__)
#define ASAN_ENABLED 1
#define NO_ASAN __declspec(no_sanitize_address)
#else
#define NO_ASAN
#endif

#elif CC_CLANG
#if defined(__has_feature)
#if __has_feature(address_sanitizer) || defined(__SANITIZE_ADDRESS__)
#define ASAN_ENABLED 1
#endif
#endif
#define NO_ASAN __attribute__((no_sanitize("address")))

#elif CC_GCC
#if defined(__SANITIZE_ADDRESS__)
#define ASAN_ENABLED 1
#endif
#define NO_ASAN __attribute__((no_sanitize_address))

#else
#define NO_ASAN
#endif

#ifdef ASAN_ENABLED
void __asan_poison_memory_region(void const volatile* addr, size_t size);
void __asan_unpoison_memory_region(void const volatile* addr, size_t size);
# define AsanPoisonMemoryRegion(addr, size)   __asan_poison_memory_region((addr), (size))
# define AsanUnpoisonMemoryRegion(addr, size) __asan_unpoison_memory_region((addr), (size))
#else
# define AsanPoisonMemoryRegion(addr, size)   ((void)(addr), (void)(size))
# define AsanUnpoisonMemoryRegion(addr, size) ((void)(addr), (void)(size))
#endif /* DEBUG_MODE */

#define SetMem0(s,z) memset((s),0,(z))

/* ===================================================== */
/*                         UNITS                         */
/* ===================================================== */

#define KB(n)  (((U64)(n)) << 10)
#define MB(n)  (((U64)(n)) << 20)
#define GB(n)  (((U64)(n)) << 30)
#define TB(n)  (((U64)(n)) << 40)
#define Thousand(n)   ((n)*1000)
#define Million(n)    ((n)*1000000)
#define Billion(n)    ((n)*1000000000)

/* ===================================================== */
/*                         UTILS                         */
/* ===================================================== */

/* GENERAL */
#define noop ((void)0)
#define Ignore(_V) ((void)(_V))

/* MATH MACROS */
#define IsPow2(X) ((X) != 0 && ((X) & ((X) -1 )) ==0 )
#define IsPow2OrZero(X) ((((X) - 1) & (X)) == 0)
#define Max(A, B) ((A) > (B) ? (A) : (B))
#define Min(A, B) ((A) < (B) ? (A) : (B))

#if CC_MSVC
#define AlignOf(T) __alignof(T)
#elif CC_CLANG
#define AlignOf(T) __alignof(T)
#elif CC_GCC
#define AlignOf(T) __alignof__(T)
#elif CC_TCC
/* DO NOTHING, TCC SUPPORTS AlignOf OUT OF THE BOX*/
#endif


#define NATIVE_ALIGNMENT \
  Max(AlignOf(int),      \
      Max(AlignOf(long), \
          Max(AlignOf(long long), Max(AlignOf(double), AlignOf(void*)))))

#define AlignUp(V, B) (((V) + (B) - 1) & (~((B) - 1)))
#define AlignDown(V, B) ((V) & (~((B) - 1)))
#define AlignUpPad(X, B)  ((0 - (X)) & ((B) - 1))

#define ToString_(X) #X
#define ToString(X) ToString_(X)

#define Glue_(A,B) A##B
#define Glue(A,B) Glue_(A,B)

/* ===================================================== */
/*                      ASSERTIONS                       */
/* ===================================================== */

#ifdef DEBUG_MODE

#define MakeSure(expr, msg, ...) \
  ((expr) ? (expr) : (log_fatal(msg, ##__VA_ARGS__), abort(), (expr)))

#define NotNull(val) MakeSure((val), "<<NULL>>")
#define NotZero(val) MakeSure((val), "<<ZERO>>")
#define IsValid(val) MakeSure((val), "<<INVALID>>")
#define MustDie(msg, ...) MakeSure(false, msg, ##__VA_ARGS__)

#else /* not debug mode */

#define MakeSure(expr, msg, ...) noop
#define NotNull(val) noop
#define NotZero(val) noop
#define IsValid(val) noop
#define MustDie(msg, ...) noop

#endif /* DEBUG_MODE */

#define Abort(msg)							\
  do {									\
    fprintf(stderr, "aborting:\n  message: %s\n", (msg));		\
    abort();								\
  } while(0)

#define ErrorOut(expr, err) \
  do {                      \
    if (!(expr)) {          \
      return (err);         \
    }                       \
  } while (0)

#define Succeed(expr)           \
  do {                          \
    I64 _ms_tmp_ = (I64)(expr);	\
    if (_ms_tmp_ != 0) {        \
      return _ms_tmp_;          \
    }                           \
  } while (0)

#define StaticAssert(COND, ID) internal U8 Glue(ID, __LINE__)[(COND)?1:-1]

/* ===================================================== */
/*                     DEBUG LOGGER                      */
/* ===================================================== */

#ifdef DEBUG_MODE
#include <stdio.h>
#define Dbg(msg, ...) do { printf(msg, ##__VA_ARGS__); printf("\n"); } while(0);
#endif /* DEBUG_MODE */

/* ===================================================== */
/*                          END                          */
/* ===================================================== */

#endif  // SEPI_BASE_H

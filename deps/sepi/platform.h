#ifndef SEPI_PLATFORM_H
#define SEPI_PLATFORM_H

/* ===================================================== */
/*                     DEPENDENCIES                      */
/* ===================================================== */

#include "base.h"

/* ===================================================== */
/*                          API                          */
/* ===================================================== */

U32 platform_get_cpu_cores();
Sz platform_get_page_size();
Sz platform_get_large_page_size();
RawPtr platform_reserve_large_pages(Sz size);
U32 platform_commit_large_pages(RawPtr ptr, Sz size);
Nothing platform_release(RawPtr ptr, Sz size);

/* ===================================================== */
/*                    IMPLEMENTATION                     */
/* ===================================================== */

#ifdef SEPI_PLATFORM_IMPLEMENTATION

#if defined(OS_LINUX) || defined(OS_MAC)

#include <sys/sysinfo.h> /* get_nprocs */
#include <unistd.h> /* getpagesize */
#include <sys/mman.h> /* mmap */

U32
platform_get_cpu_cores() {
  return (U32) get_nprocs();
}

Sz
platform_get_page_size() {
  return (Sz) sysconf(_SC_PAGESIZE);
}

Sz
platform_get_large_page_size() {
  return MB(2);
}

RawPtr
platform_reserve_large_pages(Sz size) {
  U32 flags = MAP_PRIVATE|MAP_ANONYMOUS|MAP_HUGETLB;
  void* result = mmap(0, size, PROT_NONE, flags,-1, 0);
  if(result == MAP_FAILED) {
    flags = MAP_PRIVATE|MAP_ANONYMOUS;
    result = mmap(0, size, PROT_NONE, flags,-1, 0);
    if(result == MAP_FAILED) {
      result = 0;
    }
  }
  return result;
}

U32
platform_commit_large_pages(RawPtr ptr, Sz size) {
  mprotect(ptr, size, PROT_READ|PROT_WRITE);
  return 1;
}

Nothing
platform_release(RawPtr ptr, Sz size) {
  munmap(ptr, size);
}

#else /* OS_WINDOWS */

#include <sysinfoapi.h>
#include <memoryapi.h>

U32
platform_get_cpu_cores() {
  SYSTEM_INFO si = {0};
  GetSystemInfo(&si);
  return (U32) si->dwNumberOfProcessors;
}

Sz
platform_get_page_size() {
  SYSTEM_INFO si = {0};
  GetSystemInfo(&si);
  return (Sz) si->dwPageSize;
}

Sz
platform_get_large_page_size() {
  return GetLargePageMinimum();
}

RawPtr
platform_reserve_large_pages(Sz size) {
  RawPtr result = VirtualAlloc(0, size, MEM_RESERVE|MEM_COMMIT|MEM_LARGE_PAGES, PAGE_READWRITE);
  return result;
}

U32
platform_commit_large_pages(RawPtr ptr, Sz size) {
  return 1;
}

Nothing
platform_release(RawPtr ptr, Sz size) {
  Ignore(size);
  VirtualFree(ptr, 0, MEM_RELEASE);
}

#endif

/* ===================================================== */
/*                          END                          */
/* ===================================================== */

#endif /* SEPI_PLATFORM_IMPLEMENTATION */
#endif /* SEPI_PLATFORM_H */

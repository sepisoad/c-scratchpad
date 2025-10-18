#include <stdio.h>
#include <sys/mman.h>

#define AlignPow2(x,b)     (((x) + (b) - 1)&(~((b) - 1)))

//=============

void* os_reserve(unsigned long size) {
  void* result = mmap(0, size, PROT_NONE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
  if(result == MAP_FAILED) {
    result = 0;
  }
  return result;
}

signed int os_commit(void* ptr, unsigned long size) {
  mprotect(ptr, size, PROT_READ|PROT_WRITE);
  return 1;
}

void os_decommit(void* ptr, unsigned long size) {
  madvise(ptr, size, MADV_DONTNEED);
  mprotect(ptr, size, PROT_NONE);
}

void os_release(void* ptr, unsigned long size) {
  munmap(ptr, size);
}

//=============

int main(void) {
  // size_t sz = AlignPow2(65, 32);
  // void* ptr = os_reserve(sz);
  // if (ptr == 0) {
  //   printf("FUCK\n");
  //   return 1;
  // }
  // os_commit(ptr, sz);
  // int* pnum = (int*)ptr;
  // *pnum = 1987;
  // printf("%d\n", *pnum);
  // os_release(ptr, sz);

  unsigned int pos = 88;
  size_t sz = AlignPow2(pos, 4);
  size_t sz_ex = sz + 4;
  printf("%d\n", sz_ex);
}

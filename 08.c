#include <stdio.h>
#include <time.h>

#define SEPI_ENDIAN_IMPLEMENTATION
#define SEPI_PLATFORM_IMPLEMENTATION
#define SEPI_STRING_IMPLEMENTATION
#define SEPI_ARENA_IMPLEMENTATION
#define SEPI_ARRAY_IMPLEMENTATION
#define SEPI_LIST_IMPLEMENTATION
#define SEPI_STACK_IMPLEMENTATION

#include <sepi/list.h>
#include <sepi/array.h>
#include <sepi/stack.h>

int main(Nothing) {
  I32 res = 0;


  Arena* a = arena_create();
  Array* arr = array_create(a, sizeof(U32), AlignOf(U32));

  U32 max = 100000000;
  struct timespec T1, T2;
  double sec1, sec2;

  //=======================

  List* l = list_create(a);

  clock_gettime(CLOCK_MONOTONIC, &T1);
  for (U32 num = 0; num < max; num++) {
    list_push_tail(l, &num);
  }
  clock_gettime(CLOCK_MONOTONIC, &T2);
  sec1 = (T2.tv_sec - T1.tv_sec) + (T2.tv_nsec - T1.tv_nsec) * 1e-9;
  printf("list insert: %f\n", sec1);

  clock_gettime(CLOCK_MONOTONIC, &T1);
  for (U32 index = 0; index < max; index++) {
    list_pop_tail(l);
  }
  clock_gettime(CLOCK_MONOTONIC, &T2);
  sec2 = (T2.tv_sec - T1.tv_sec) + (T2.tv_nsec - T1.tv_nsec) * 1e-9;
  printf("array get: %f\n", sec2);
  // printf("array total: %f\n", sec1 + sec2);

  list_destroy(l);

  //=======================

  Stack* s = stack_create(a);

  clock_gettime(CLOCK_MONOTONIC, &T1);
  for (U32 num = 0; num < max; num++) {
    stack_push(s, &num);
  }
  clock_gettime(CLOCK_MONOTONIC, &T2);
  sec1 = (T2.tv_sec - T1.tv_sec) + (T2.tv_nsec - T1.tv_nsec) * 1e-9;
  printf("stack insert: %f\n", sec1);

  clock_gettime(CLOCK_MONOTONIC, &T1);
  for (U32 index = 0; index < max; index++) {
    stack_pop(s);
  }
  clock_gettime(CLOCK_MONOTONIC, &T2);
  sec2 = (T2.tv_sec - T1.tv_sec) + (T2.tv_nsec - T1.tv_nsec) * 1e-9;
  printf("stack get: %f\n", sec2);
  // printf("stack total: %f\n", sec1 + sec2);


  stack_destroy(s);


  //=======================

  clock_gettime(CLOCK_MONOTONIC, &T1);
  for (U32 num = 0; num < max; num++) {
    array_push(arr, &num);
  }
  clock_gettime(CLOCK_MONOTONIC, &T2);
  sec1 = (T2.tv_sec - T1.tv_sec) + (T2.tv_nsec - T1.tv_nsec) * 1e-9;
  printf("array insert: %f\n", sec1);

  clock_gettime(CLOCK_MONOTONIC, &T1);
  for (U32 index = 0; index < max; index++) {
    array_get(arr, index);
  }
  clock_gettime(CLOCK_MONOTONIC, &T2);
  sec2 = (T2.tv_sec - T1.tv_sec) + (T2.tv_nsec - T1.tv_nsec) * 1e-9;
  printf("array get: %f\n", sec2);
  // printf("array total: %f\n", sec1 + sec2);

  //=======================


  arena_destroy(a);

  return res;
}

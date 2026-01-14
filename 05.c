#include <stdio.h>

#define SEPI_ENDIAN_IMPLEMENTATION
#define SEPI_PLATFORM_IMPLEMENTATION
#define SEPI_STRING_IMPLEMENTATION
#define SEPI_ARENA_IMPLEMENTATION
#define SEPI_STACK_IMPLEMENTATION
#define SEPI_HASHMAP_IMPLEMENTATION
#define SEPI_IO_IMPLEMENTATION

#include <sepi/stack.h>


I32 test_stack_1() {
  Arena *a = arena_create();
  Stack* s = stack_create(a);

  int n1 = 10;
  int n2 = 20;
  int n3 = 30;
  int n4 = 40;
  int n5 = 50;
  int n6 = 60;

  stack_push(s, &n1);
  stack_push(s, &n2);
  stack_push(s, &n3);
  stack_push(s, &n4);
  stack_push(s, &n5);
  stack_push(s, &n6);
  printf("length: %ld\n", s->length);

  for(; s->length;) {
    int* pn = stack_pop(s);

    printf("value: %d\n", *pn);
  }

  stack_destroy(s);
  printf("length: %ld\n", s->length);

  arena_destroy(a);
  return 0;
}

I32 test_stack_2() {
  Arena *a = arena_create();
  Stack* s = stack_create(a);

  int n1 = 10;
  int n2 = 20;
  int n3 = 30;
  int n4 = 40;
  int n5 = 50;
  int n6 = 60;

  stack_pop(s);
  stack_push(s, &n1);
  stack_push(s, &n2);
  stack_pop(s);

  stack_push(s, &n3);
  stack_pop(s);

  stack_push(s, &n4);
  stack_push(s, &n5);
  stack_push(s, &n6);
  stack_pop(s);

  printf("length: %ld\n", s->length);

  for(; s->length;) {
    int* pn = stack_pop(s);

    printf("value: %d\n", *pn);
  }

  stack_destroy(s);
  printf("length: %ld\n", s->length);

  arena_destroy(a);
  return 0;
}

int main(Nothing) {
  I32 res = 0;

  // res = test_stack_1();
  // if (res) {
  //   return res;
  // }

  res = test_stack_2();
  if (res) {
    return res;
  }
}

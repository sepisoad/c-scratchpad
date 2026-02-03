#include <stdio.h>

#define SEPI_PLATFORM_IMPLEMENTATION
#define SEPI_ARENA_IMPLEMENTATION
#define SEPI_LIST_IMPLEMENTATION

#include <sepi/base.h>
#include <sepi/list.h>

void baz(void) {
  printf("%s\n", __func__);
}

void bar(void) {
  baz();
  printf("%s\n", __func__);
}

void foo(void) {
  bar();
  printf("%s\n", __func__);
}


int main(void) {
  foo();
  printf("%s\n", __func__);
  // list_create(0);g
  return 0;
}

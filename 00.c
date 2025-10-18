#include <stdio.h>
#include "deps/sepi/base.h"

typedef U8 Gender;
enum {
  Male = 0,
  Female = 1,
};

typedef struct Params Params;
struct Params {
  U8 age;
  Bool alive;
  Gender gender;
};

static void print_foo_(Params* p) {
  printf("======= info =======\n");
  printf("  age: %d\n", p->age);
  printf("  alive: %s\n", p->alive ? "Yes" : "No");
  printf("  gender: %s\n", p->gender ? "Female" : "Male");
}

#define print_foo(...) print_foo_(&(Params){.age = 120, .alive = FALSE, .gender = Female, __VA_ARGS__})

int main(int argc, char** argv) {
  Ignore(argc);
  Ignore(argv);
  // StaticAssert(1 == 0, dummy); /* YAAAY */
  StaticAssert(TRUE == 1, is_true);
  print_foo(.age = 38, .alive = TRUE, .gender = Male);
  print_foo();
  print_foo(.age = 18);
  return 0;
}

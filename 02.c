#define SEPI_DBGALLOC_IMPLEMENTATION
#define SEPI_PLATFORM_IMPLEMENTATION
#define SEPI_ARENA_IMPLEMENTATION

#include "deps/sepi/arena.h"

int main(int argc, char** argv) {
  Ignore(argc);
  Ignore(argv);

  Arena* a = arena_alloc(.requested_reserve_size = 4,
                         .requested_commit_size = 4);
  {
    U32* num1 = arena_push(a, sizeof(U32), sizeof(U32), FALSE);
    *num1 = 1987;
    U32* num2 = arena_push(a, sizeof(U32), sizeof(U32), FALSE);
    *num2 = 1366;

    printf("%d\n", *num1);
    printf("%d\n", *num2);

  }
  arena_release(a);

  return 0;
}

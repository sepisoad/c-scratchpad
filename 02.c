#define SEPI_DBGALLOC_IMPLEMENTATION
#define SEPI_PLATFORM_IMPLEMENTATION
#define SEPI_ARENA_IMPLEMENTATION

#include "deps/sepi/arena.h"

static Arena* _g_arena = NULL;

void func_a() {
  ArenaScratch s = arena_scratch_begin(_g_arena);
  Arena* a = s.arena;

  U8* c1 = arena_push(a, sizeof(U8), sizeof(U8), FALSE);
  U8* c2 = arena_push(a, sizeof(U8), sizeof(U8), FALSE);
  U8* c3 = arena_push(a, sizeof(U8), sizeof(U8), FALSE);
  U8* c4 = arena_push(a, sizeof(U8), sizeof(U8), FALSE);

  *c1 = 's';
  *c2 = 'e';
  *c3 = 'p';
  *c4 = 'i';

  printf("%c%c%c%c\n", *c1, *c2, *c3, *c4);
  arena_scratch_end(s);
}

int main(int argc, char** argv) {
  Ignore(argc);
  Ignore(argv);

  _g_arena = arena_alloc(.requested_reserve_size = 4,
                         .requested_commit_size = 4);
  Arena* a = _g_arena;
  {
    U32* num1 = arena_push(a, sizeof(U32), sizeof(U32), FALSE);
    *num1 = 1987;
    U32* num2 = arena_push(a, sizeof(U32), sizeof(U32), FALSE);
    *num2 = 1366;

    func_a();
    printf("%d\n", *num1);
    printf("%d\n", *num2);
  }

  arena_pop(a, sizeof(U32) * 200);

  for (U32 i = 0; i < 1000000; i++) {
    arena_push(a, sizeof(U32), sizeof(U32), FALSE);
  }

  printf("yay!\n");

  arena_release(a);

  return 0;
}

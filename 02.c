#define SEPI_DBGALLOC_IMPLEMENTATION
#define SEPI_PLATFORM_IMPLEMENTATION
#define SEPI_ARENA_IMPLEMENTATION

#include "deps/sepi/arena.h"

int main(int argc, char** argv) {
  Ignore(argc);
  Ignore(argv);

  Arena* a = arena_alloc();
  arena_release(a);
  Dbg("done");
}
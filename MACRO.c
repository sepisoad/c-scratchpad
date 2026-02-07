#define SEPI_PLATFORM_IMPLEMENTATION
#define SEPI_ARENA_IMPLEMENTATION
#define SEPI_STRING_IMPLEMENTATION

#include <sepi/string.h>
#include <sepi/array_ex.h>
#include <sepi/list_ex.h>
#include <sepi/stack_ex.h>
#include <sepi/tree_ex.h>
#include <sepi/map_ex.h>

typedef struct {
  Str8 name;
  U32 age;
  struct {
    Str8 title;
    struct {
      Str8 status;
    } hidden;
  } meta;
} Info;

// DefineStack(Info, Info, info);
// DefineStack(Info *, InfoPtr, info_ptr);
// DefineArray(Info, Info, info);
// DefineArray(Info *, InfoPtr, info_ptr);
// DefineList(Info, Info, info);
// DefineList(Info *, InfoPtr, info_ptr);
// DefineTree(Info, Info, info);
// DefineTree(Info *, InfoPtr, info_ptr);
// DefineMap(Info, Info, info);
DefineMap(Info *, InfoPtr, info_ptr);

int main() {
  return 0;
}

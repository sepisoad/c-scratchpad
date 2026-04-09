#define SEPI_ARENA_IMPLEMENTATION
#define SEPI_STRING_IMPLEMENTATION

#include <sepi/arena.h>
#include <sepi/base.h>
#include <sepi/string.h>

#define MAX_ITEMS_COUNT 10


typedef struct ArrayHead ArrayHead;
struct ArrayHead {
  U16 parent;
  U16 first_child;
  U16 next_sibling;
};


typedef struct IONode IONode;
struct IONode {
  Str8 name;
  ArrayHead array;
};

void test_array() {
  Arena* arena = arena_create();
  IONode* ions = arena_push(arena, sizeof(IONode) * MAX_ITEMS_COUNT, alignof(IONode), TRUE);

  ions[0].name = S("./");
  ions[0].array.parent = 0;
  ions[0].array.first_child = 1;
  ions[0].array.next_sibling = 0;

  ions[1].name = S("./root/");
  ions[1].array.parent = 0;
  ions[1].array.first_child = 2;
  ions[1].array.next_sibling = 0;

  ions[2].name = S("./root/dir1/");
  ions[2].array.parent = 1;
  ions[2].array.first_child = 5;
  ions[2].array.next_sibling = 3;

  ions[3].name = S("./root/dir2/");
  ions[3].array.parent = 1;
  ions[3].array.first_child = 8;
  ions[3].array.next_sibling = 4;

  ions[4].name = S("./root/file1");
  ions[4].array.parent = 1;
  ions[4].array.first_child = 0;
  ions[4].array.next_sibling = 0;

  ions[5].name = S("./root/dir1/dir3/");
  ions[5].array.parent = 2;
  ions[5].array.first_child = 7;
  ions[5].array.next_sibling = 6;

  ions[6].name = S("./root/dir1/file2");
  ions[6].array.parent = 2;
  ions[6].array.first_child = 0;
  ions[6].array.next_sibling = 0;

  ions[7].name = S("./root/dir1/dir3/file3");
  ions[7].array.parent = 5;
  ions[7].array.first_child = 0;
  ions[7].array.next_sibling = 0;

  ions[8].name = S("./root/dir2/file4");
  ions[8].array.parent = 3;
  ions[8].array.first_child = 0;
  ions[8].array.next_sibling = 9;

  ions[9].name = S("./root/dir2/file5");
  ions[9].array.parent = 3;
  ions[9].array.first_child = 0;
  ions[9].array.next_sibling = 0;

  U16 stack_length = 0;
  U16* stack = arena_push(arena, sizeof(U16) * MAX_ITEMS_COUNT, alignof(U16), TRUE);

  stack[stack_length++] = 0;

  while (stack_length > 0) {
    U16 index = stack[--stack_length];
    U16 iter = index;

    do {
      if (ions[iter].array.first_child) {
        stack[stack_length++] = ions[iter].array.first_child;
      }
      dbg("[%2d] name: %s", iter, CS(ions[iter].name));

      iter = ions[iter].array.next_sibling;
    } while(iter != 0);
  }
}

int main() {
  test_array();
}


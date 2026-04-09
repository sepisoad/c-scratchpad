#define SEPI_ARENA_IMPLEMENTATION
#define SEPI_STRING_IMPLEMENTATION

#include <sepi/arena.h>
#include <sepi/base.h>
#include <sepi/string.h>

#define MAX_ITEMS_COUNT 15

#define list_data(ptr, type, member)                                           \
  ((type *)((char *)(ptr) - (uintptr_t)(&((type *)0)->member)))

typedef struct ListHead ListHead;
struct ListHead {
  ListHead *parent;
  ListHead *first_child;
  ListHead *next;
};

typedef struct ArrayHead ArrayHead;
struct ArrayHead {
  U16 parent;
  U16 first_child;
  U16 next;
};

typedef struct Stack Stack;
struct Stack {
  top;
  previous;
  bottom;
};

typedef struct IONode IONode;
struct IONode {
  Str8 name;
  ListHead list;
  ArrayHead array;
};

void test_list() {
  IONode ion1 = {.name = S("root")};
  IONode ion2 = {.name = S("root/dir1")};
  IONode ion3 = {.name = S("root/dir2")};
  IONode ion4 = {.name = S("root/file1")};
  IONode ion5 = {.name = S("root/dir1/dir3")};
  IONode ion6 = {.name = S("root/dir1/file2")};
  IONode ion7 = {.name = S("root/dir1/dir3/file3")};
  IONode ion8 = {.name = S("root/dir2/file4")};
  IONode ion9 = {.name = S("root/dir2/file5")};

  ion1.list.parent = &ion1.list;
  ion1.list.first_child = &ion2.list;
  ion1.list.next = &ion1.list;

  ion2.list.parent = &ion1.list;
  ion2.list.first_child = &ion5.list;
  ion2.list.next = &ion3.list;

  ion3.list.parent = &ion1.list;
  ion3.list.first_child = &ion8.list;
  ion3.list.next = &ion4.list;

  ion4.list.parent = &ion1.list;
  ion4.list.first_child = &ion4.list;
  ion4.list.next = &ion4.list;

  ion5.list.parent = &ion2.list;
  ion5.list.first_child = &ion7.list;
  ion5.list.next = &ion6.list;

  ion6.list.parent = &ion2.list;
  ion6.list.first_child = &ion6.list;
  ion6.list.next = &ion6.list;

  ion7.list.parent = &ion5.list;
  ion7.list.first_child = &ion7.list;
  ion7.list.next = &ion7.list;

  ion8.list.parent = &ion3.list;
  ion8.list.first_child = &ion8.list;
  ion8.list.next = &ion9.list;

  ion9.list.parent = &ion3.list;
  ion9.list.first_child = &ion9.list;
  ion9.list.next = &ion9.list;


  ListHead *iter = &ion2.list;
  ListHead *prev = iter;

  do {
    IONode* e = list_data(iter, IONode, list);
    dbg("name: %s ", CS(e->name));
    prev = iter;
    iter = iter->next;
  } while(iter->next != prev);

  dbg("----");

  iter = &ion1.list;
  prev = iter;

  do {
    IONode* e = list_data(iter, IONode, list);
    dbg("name: %s ", CS(e->name));
    prev = iter;
    iter = iter->first_child;
  } while(iter->first_child != prev);
}

void test_array() {
  Arena* arena = arena_create();
  IONode* ions = arena_push(arena, sizeof(IONode) * 9, alignof(IONode), TRUE);

  ions[0].name = S("root");
  ions[0].array.parent = 0;
  ions[0].array.first_child = 1;
  ions[0].array.next = 0;

  ions[1].name = S("root/dir1");
  ions[1].array.parent = 0;
  ions[1].array.first_child = 4;
  ions[1].array.next = 2;

  ions[2].name = S("root/dir2");
  ions[2].array.parent = 0;
  ions[2].array.first_child = 7;
  ions[2].array.next = 3;

  ions[3].name = S("root/file1");
  ions[3].array.parent = 0;
  ions[3].array.first_child = 3;
  ions[3].array.next = 3;

  ions[4].name = S("root/dir1/dir3");
  ions[4].array.parent = 1;
  ions[4].array.first_child = 6;
  ions[4].array.next = 5;

  ions[5].name = S("root/dir1/file2");
  ions[5].array.parent = 1;
  ions[5].array.first_child = 5;
  ions[5].array.next = 5;

  ions[6].name = S("root/dir1/dir3/file3");
  ions[6].array.parent = 4;
  ions[6].array.first_child = 6;
  ions[6].array.next = 6;

  ions[7].name = S("root/dir2/file4");
  ions[7].array.parent = 2;
  ions[7].array.first_child = 7;
  ions[7].array.next = 8;

  ions[8].name = S("root/dir2/file5");
  ions[8].array.parent = 2;
  ions[8].array.first_child = 8;
  ions[8].array.next = 8;

  U16 iter = 1;
  U16 prev = iter;

  do {
    dbg("name: %s ", CS(ions[iter].name));
    prev = iter;
    iter = ions[iter].array.next;
  } while(iter != prev);

  dbg("----");

  iter = 0;
  prev = iter;

  do {
    dbg("name: %s ", CS(ions[iter].name));
    prev = iter;
    iter = ions[iter].array.first_child;
  } while(iter != prev);
}

int main() {
  test_list();
  dbg("====");
  test_array();
}


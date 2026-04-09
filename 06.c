#define SEPI_ARENA_IMPLEMENTATION
#define SEPI_STRING_IMPLEMENTATION

#include <sepi/arena.h>
#include <sepi/base.h>
#include <sepi/string.h>

#define MAX_ITEMS_COUNT 15

typedef struct IOItem IOItem;
struct IOItem {
  struct {
    U32 parent;
    U32 first; // The start of the sibling ring
    U32 next;  // Next sibling (circular)
    U32 first_child;
  } item;

  Bool is_directory;
  Str8 name;
};

typedef struct IOItems IOItems;
struct IOItems {
  IOItem *list;
};

int main(Nothing) {
  Arena *arena = arena_create();
  IOItems items = {0};
  items.list = arena_push(arena, sizeof(IOItem) * (MAX_ITEMS_COUNT + 1),
                          alignof(IOItem), TRUE);

  // --- Level 0 ---
  // dir1 (Index 1)
  items.list[1] =
      (IOItem){.is_directory = TRUE,
               .name = S("dir1"),
               .item = {.parent = 0, .first = 1, .next = 1, .first_child = 2}};

  // --- Level 1 (Children of dir1) ---
  // dir2 (2), file1 (3), dir3 (4)
  items.list[2] =
      (IOItem){.is_directory = TRUE,
               .name = S("dir2"),
               .item = {.parent = 1, .first = 2, .next = 3, .first_child = 5}};
  items.list[3] =
      (IOItem){.is_directory = FALSE,
               .name = S("file1"),
               .item = {.parent = 1, .first = 2, .next = 4, .first_child = 0}};
  items.list[4] =
      (IOItem){// Loops to 2
               .is_directory = TRUE,
               .name = S("dir3"),
               .item = {.parent = 1, .first = 2, .next = 2, .first_child = 8}};

  // --- Level 2 (Children of dir2) ---
  // file2 (5), dir4 (6), file3 (7)
  items.list[5] =
      (IOItem){.is_directory = FALSE,
               .name = S("file2"),
               .item = {.parent = 2, .first = 5, .next = 6, .first_child = 0}};
  items.list[6] =
      (IOItem){.is_directory = TRUE,
               .name = S("dir4"),
               .item = {.parent = 2, .first = 5, .next = 7, .first_child = 10}};
  items.list[7] =
      (IOItem){// Loops to 5
               .is_directory = FALSE,
               .name = S("file3"),
               .item = {.parent = 2, .first = 5, .next = 5, .first_child = 0}};

  // --- Level 2 (Children of dir3) ---
  // file4 (8), file5 (9)
  items.list[8] =
      (IOItem){.is_directory = FALSE,
               .name = S("file4"),
               .item = {.parent = 4, .first = 8, .next = 9, .first_child = 0}};
  items.list[9] =
      (IOItem){// Loops to 8
               .is_directory = FALSE,
               .name = S("file5"),
               .item = {.parent = 4, .first = 8, .next = 8, .first_child = 0}};

  // --- Level 3 (Children of dir4) ---
  // file6 (10)
  items.list[10] = (IOItem){.is_directory = FALSE,
                            .name = S("file6"),
                            .item = {.parent = 6,
                                     .first = 10,
                                     .next = 10,
                                     .first_child = 0}}; // Loops to 10

  /* --------------------------------------------------------- */
  /* RECURSIVE-STYLE TRAVERSAL USING THE CIRCULAR LOGIC        */
  /* --------------------------------------------------------- */

  // Let's print the siblings of dir2 (which includes file1 and dir3)
  U32 start_node = 2;
  U32 curr = start_node;

  dbg("--- Siblings of dir2 ---");
  do {
    IOItem *it = &items.list[curr];
    dbg("Name: %s", CS(it->name));

    // If it has children, let's peek at the first one
    if (it->item.first_child != 0) {
      dbg("  -> First Child: %s", CS(items.list[it->item.first_child].name));
    }

    curr = it->item.next;
  } while (curr != start_node);

  return 0;
}

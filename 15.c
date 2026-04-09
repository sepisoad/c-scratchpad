#define SEPI_ARENA_IMPLEMENTATION

#include <sepi/arena.h>
#include <sepi/base.h>

typedef struct ListNode ListNode;
struct ListNode {
  ListNode *next;
  I8 data[];
};

#define List(type)                                                             \
  union {                                                                      \
    ListNode *head;                                                            \
    type *payload;                                                             \
  }

Nothing _list_prepend(Arena *arena, ListNode **head, RawPtr data,
                      Sz data_size) {
  ListNode *node =
      arena_push(arena, sizeof(ListNode) + data_size, alignof(ListNode), FALSE);
  memcpy(node->data, data, data_size);
  node->next = *head;
  *head = node;
}

#define list_prepend(arena, list, item)                                        \
  _list_prepend(arena, &((list)->head), (1 ? (item) : (list)->payload),        \
                sizeof(*(list)->payload))

int main() {
  typeof(100) num = 100;
  dbg("%d", num);

  Arena *arena = arena_create();
  List(U32) nums = {0};

  U32 num1 = 10;
  U32 num2 = 20;
  U32 num3 = 30;
  U32 num4 = 40;

  list_prepend(arena, &nums, &num1);
  list_prepend(arena, &nums, &num2);
  list_prepend(arena, &nums, &num3);
  list_prepend(arena, &nums, &num4);

  ListNode *curr = nums.head;
  while (curr) {
    U32 *n = (U32 *)curr->data;
    dbg("num: %d", *n);
    curr = curr->next;
  }
  return 0;
}

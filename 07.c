#define SEPI_ARENA_IMPLEMENTATION
#define SEPI_STRING_IMPLEMENTATION

#include <sepi/arena.h>
#include <sepi/base.h>
#include <sepi/string.h>

#define MAX_ITEMS_COUNT 15

#define list_data(ptr, type, member) \
    ((type *)((char *)(ptr) - (uintptr_t)(&((type *)0)->member)))

typedef struct ListHead ListHead;
struct ListHead {
  ListHead* next;
};

typedef struct ArrayHead ArrayHead;
struct ArrayHead {
  U16 next;
};

typedef struct Employee Employee;
struct Employee {
  U8 age;
  Str8 name;
  ListHead list;
  ArrayHead array;
};

void test_list() {
  ListHead list = {.next = &list};

  Employee e1 = {.age = 20, .name = S("james") };
  Employee e2 = {.age = 25, .name = S("alex") };
  Employee e3 = {.age = 21, .name = S("martin") };
  Employee e4 = {.age = 30, .name = S("bob") };
  Employee e5 = {.age = 28, .name = S("fred") };

  e1.list.next = &list;
  e2.list.next = &e1.list;
  e3.list.next = &e2.list;
  e4.list.next = &e3.list;
  e5.list.next = &e4.list;

  list.next = &e5.list;

  ListHead* iter = &list;

  do {
    iter = iter->next;
    Employee* e = list_data(iter, Employee, list);
    dbg("name: %s | age: %d", CS(e->name), e->age);

  } while(iter->next != &list);


}

void test_array() {
  Arena* arena = arena_create();
  Employee* pe = arena_push(arena, sizeof(Employee) * 5, alignof(Employee), TRUE);

  pe[0].age = 20;
  pe[0].name = S("james");
  pe[0].array.next = 1;

  pe[1].age = 25;
  pe[1].name = S("alex");
  pe[1].array.next = 2;

  pe[2].age = 21;
  pe[2].name = S("martin");
  pe[2].array.next = 3;

  pe[3].age = 30;
  pe[3].name = S("bob");
  pe[3].array.next = 4;

  pe[4].age = 28;
  pe[4].name = S("fred");
  pe[4].array.next = 0;

  U16 iter = 0;
  do {
    dbg("name: %s | age: %d", CS(pe[iter].name), pe[iter].age);
    iter = pe[iter].array.next;
  } while(iter != 0);

}

int main() {
  test_list();
  test_array();
}

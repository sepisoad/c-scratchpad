#include <stdio.h>

#define SEPI_ENDIAN_IMPLEMENTATION
#define SEPI_PLATFORM_IMPLEMENTATION
#define SEPI_STRING_IMPLEMENTATION
#define SEPI_ARENA_IMPLEMENTATION
#define SEPI_STACK_IMPLEMENTATION
#define SEPI_LIST_IMPLEMENTATION

#define SEPI_HASHMAP_IMPLEMENTATION
#define SEPI_IO_IMPLEMENTATION

#include <sepi/list.h>

I32 test_list_1() {
  Arena *a = arena_create();

  List* l = list_create(a);

  int n1 = 10;
  int n2 = 20;
  int n3 = 30;
  int n4 = 40;
  int n5 = 50;
  int n6 = 60;

  list_push_tail(l, &n1);
  ListNode* ln1 = list_push_tail(l, &n2);
  ListNode* ln2 = list_push_head(l, &n3);
  list_push_head(l, &n4);
  list_push_before(l, ln1, &n5);
  list_push_after(l, ln2, &n6);

  printf("length: %ld\n", l->length);
  for(ListNode* iter = l->head; iter; iter = iter->next) {
    printf("%d\n", *((int* )(iter->ptr)));
  }
  list_destroy(l);
  printf("length: %ld\n", l->length);

  arena_destroy(a);
  return 0;y
}

I32 test_list_2() {
  Arena *a = arena_create();

  List* l = list_create(a);

  int n1 = 10;
  int n2 = 20;
  int n3 = 30;
  int n4 = 40;
  int n5 = 50;
  int n6 = 60;

  list_push_tail(l, &n1);
  list_push_tail(l, &n2);
  list_push_tail(l, &n3);
  list_push_tail(l, &n4);
  list_push_tail(l, &n5);
  list_push_tail(l, &n6);

  printf("length: %ld\n", l->length);
  for(ListNode* iter = l->head; iter; iter = iter->next) {
    printf("%d\n", *((int* )(iter->ptr)));
  }
  list_destroy(l);
  printf("length: %ld\n", l->length);

  arena_destroy(a);
  return 0;
}

I32 test_list_3() {
  Arena *a = arena_create();

  List* l = list_create(a);

  int n1 = 10;
  int n2 = 20;
  int n3 = 30;
  int n4 = 40;
  int n5 = 50;
  int n6 = 60;

  list_push_head(l, &n1);
  list_push_head(l, &n2);
  list_push_head(l, &n3);
  list_push_head(l, &n4);
  list_push_head(l, &n5);
  list_push_head(l, &n6);

  printf("length: %ld\n", l->length);
  for(ListNode* iter = l->head; iter; iter = iter->next) {
    printf("%d\n", *((int* )(iter->ptr)));
  }
  list_destroy(l);
  printf("length: %ld\n", l->length);

  arena_destroy(a);
  return 0;
}


I32 test_list_4() {
  Arena *a = arena_create();

  List* l = list_create(a);

  int n1 = 10;
  int n2 = 20;
  int n3 = 30;
  int n4 = 40;
  int n5 = 50;
  int n6 = 60;

  list_push_head(l, &n1);
  list_push_tail(l, &n2);
  list_push_head(l, &n3);
  list_push_tail(l, &n4);
  list_push_head(l, &n5);
  list_push_tail(l, &n6);

  printf("length: %ld\n", l->length);
  for(ListNode* iter = l->head; iter; iter = iter->next) {
    printf("%d\n", *((int* )(iter->ptr)));
  }
  list_destroy(l);
  printf("length: %ld\n", l->length);

  arena_destroy(a);
  return 0;
}

I32 test_list_5() {
  Arena *a = arena_create();

  List* l = list_create(a);

  int n1 = 10;
  int n2 = 20;
  int n3 = 30;
  int n4 = 40;
  int n5 = 50;
  int n6 = 60;

  ListNode* ln = list_push_head(l, &n1);
  list_push_before(l, ln,  &n2);
  list_push_before(l, ln,  &n3);
  list_push_before(l, ln,  &n4);
  list_push_before(l, ln,  &n5);
  list_push_before(l, ln,  &n6);

  printf("length: %ld\n", l->length);
  for(ListNode* iter = l->head; iter; iter = iter->next) {
    printf("%d\n", *((int* )(iter->ptr)));
  }
  list_destroy(l);
  printf("length: %ld\n", l->length);

  arena_destroy(a);
  return 0;
}

I32 test_list_6() {
  Arena *a = arena_create();

  List* l = list_create(a);

  int n1 = 10;
  int n2 = 20;
  int n3 = 30;
  int n4 = 40;
  int n5 = 50;
  int n6 = 60;

  ListNode* ln = list_push_head(l, &n1);
  list_push_after(l, ln,  &n2);
  list_push_after(l, ln,  &n3);
  list_push_after(l, ln,  &n4);
  list_push_after(l, ln,  &n5);
  list_push_after(l, ln,  &n6);

  printf("length: %ld\n", l->length);
  for(ListNode* iter = l->head; iter; iter = iter->next) {
    printf("%d\n", *((int* )(iter->ptr)));
  }
  list_destroy(l);
  printf("length: %ld\n", l->length);

  arena_destroy(a);
  return 0;
}

I32 test_list_7() {
  Arena *a = arena_create();

  List* l = list_create(a);

  int n1 = 10;
  int n2 = 20;
  int n3 = 30;
  int n4 = 40;
  int n5 = 50;
  int n6 = 60;

  list_pop_tail(l);

  list_push_head(l, &n1);
  list_push_tail(l, &n2);
  list_pop_tail(l);
  list_push_tail(l, &n3);
  list_push_tail(l, &n4);
  list_pop_tail(l);
  list_push_tail(l, &n5);
  list_push_tail(l, &n6);
  list_pop_tail(l);

  printf("length: %ld\n", l->length);
  for(ListNode* iter = l->head; iter; iter = iter->next) {
    printf("%d\n", *((int* )(iter->ptr)));
  }
  list_destroy(l);
  printf("length: %ld\n", l->length);

  list_pop_tail(l);

  arena_destroy(a);
  return 0;
}


I32 test_list_8() {
  Arena *a = arena_create();

  List* l = list_create(a);

  int n1 = 10;
  int n2 = 20;
  int n3 = 30;
  int n4 = 40;
  int n5 = 50;
  int n6 = 60;

  list_pop_head(l);

  list_push_head(l, &n1);
  list_push_head(l, &n2);
  list_pop_head(l);
  list_push_head(l, &n3);
  list_push_head(l, &n4);
  list_pop_head(l);
  list_push_head(l, &n5);
  list_push_head(l, &n6);
  list_pop_head(l);

  printf("length: %ld\n", l->length);
  for(ListNode* iter = l->head; iter; iter = iter->next) {
    printf("%d\n", *((int* )(iter->ptr)));
  }
  list_destroy(l);
  printf("length: %ld\n", l->length);

  list_pop_head(l);

  arena_destroy(a);
  return 0;
}

I32 test_list_9() {
  Arena *a = arena_create();

  List* l = list_create(a);

  int n1 = 10;
  int n2 = 20;
  int n3 = 30;
  int n4 = 40;
  int n5 = 50;
  int n6 = 60;

  list_pop_head(l);

  list_push_tail(l, &n1);
  list_push_tail(l, &n2);
  list_push_tail(l, &n3);
  list_push_tail(l, &n4);
  list_push_tail(l, &n5);
  list_push_tail(l, &n6);

  printf("length: %ld\n", l->length);
  for(U64 i = 0; i < l->length; i++) {
    printf("%d\n", *((int* )(list_get_at(l, i))));
  }
  list_destroy(l);
  printf("length: %ld\n", l->length);

  list_pop_head(l);

  arena_destroy(a);
  return 0;
}

// I32 test_list_9() {
//   Arena *a = arena_create();

//   List* l = list_create(a);

//   int n1 = 10;
//   int n2 = 20;
//   int n3 = 30;
//   int n4 = 40;
//   int n5 = 50;
//   int n6 = 60;

//   list_pop_head(l);

//   list_push_tail(l, &n1);
//   list_push_tail(l, &n2);
//   list_push_tail(l, &n3);
//   list_push_tail(l, &n4);
//   list_push_tail(l, &n5);
//   list_push_tail(l, &n6);

//   printf("length: %ld\n", l->length);
//   for(U64 i = 0; i < l->length; i++) {
//     printf("%d\n", *((int* )(list_get_at(l, i))));
//   }
//   list_destroy(l);
//   printf("length: %ld\n", l->length);

//   list_pop_head(l);

//   arena_destroy(a);
//   return 0;
// }

I32 test_list_10() {
  Arena *a = arena_create();

  List* l = list_create(a);

  int n1 = 10;
  int n2 = 20;
  int n3 = 30;
  int n4 = 40;
  int n5 = 50;
  int n6 = 60;

  list_pop_head(l);

  ListNode* ln1 = list_push_tail(l, &n1);
  ListNode* ln2 = list_push_tail(l, &n2);
  ListNode* ln3 = list_push_tail(l, &n3);
  ListNode* ln4 = list_push_tail(l, &n4);
  ListNode* ln5 = list_push_tail(l, &n5);
  ListNode* ln6 = list_push_tail(l, &n6);

  printf("length: %ld\n", l->length);

  list_pop(l, ln2);
  list_pop(l, ln4);
  list_pop(l, ln3);
  list_pop(l, ln6);
  list_pop(l, ln5);
  list_pop(l, ln1);

  list_destroy(l);
  printf("length: %ld\n", l->length);

  list_pop_head(l);

  arena_destroy(a);
  return 0;
}

int main(Nothing) {
  I32 res = 0;

  res = test_list_1();
  if (res) {
    return res;
  }

  printf("-=-=-=-=-=-=-=-=-=-=-=-=-\n");

  res = test_list_2();
  if (res) {
    return res;
  }

  printf("-=-=-=-=-=-=-=-=-=-=-=-=-\n");

  res = test_list_3();
  if (res) {
    return res;
  }

  printf("-=-=-=-=-=-=-=-=-=-=-=-=-\n");

  res = test_list_4();
  if (res) {
    return res;
  }

  printf("-=-=-=-=-=-=-=-=-=-=-=-=-\n");

  res = test_list_5();
  if (res) {
    return res;
  }

  printf("-=-=-=-=-=-=-=-=-=-=-=-=-\n");

  res = test_list_6();
  if (res) {
    return res;
  }

  printf("-=-=-=-=-=-=-=-=-=-=-=-=-\n");

  res = test_list_7();
  if (res) {
    return res;
  }

  printf("-=-=-=-=-=-=-=-=-=-=-=-=-\n");

  res = test_list_8();
  if (res) {
    return res;
  }

  printf("-=-=-=-=-=-=-=-=-=-=-=-=-\n");

  res = test_list_9();
  if (res) {
    return res;
  }

  printf("-=-=-=-=-=-=-=-=-=-=-=-=-\n");

  res = test_list_10();
  if (res) {
    return res;
  }
}


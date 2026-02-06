#include <stdio.h>

#define SEPI_PLATFORM_IMPLEMENTATION
#define SEPI_ARENA_IMPLEMENTATION
#define SEPI_STRING_IMPLEMENTATION

#include <sepi/array_ex.h>
#include <sepi/list_ex.h>
#include <sepi/stack_ex.h>
#include <sepi/tree_ex.h>

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

DefineStack(Info, Info, info);
DefineStack(Info *, InfoPtr, info_ptr);
DefineArray(Info, Info, info);
DefineArray(Info *, InfoPtr, info_ptr);
DefineList(Info, Info, info);
DefineList(Info *, InfoPtr, info_ptr);

/* ****************************************** */
void stack_value() {
  Arena *arena = arena_create();
  StackInfo stack = stack_info_init(arena);

  Info info1 = {
      .name = S("sepi"),
      .age = 38,
      .meta = {.title = S("mr"), .hidden = {.status = S("SHIT SEPI")}}};
  Info info2 = {
      .name = S("matilde"),
      .age = 27,
      .meta = {.title = S("mrs"), .hidden = {.status = S("SHIT MATILDE")}}};

  stack_info_push(&stack, info1);
  stack_info_push(&stack, info2);

  Info vinfo = stack_info_pop(&stack);
  Dbg("%s: %d # %s ## %s", CS(vinfo.name), vinfo.age, CS(vinfo.meta.title),
      CS(vinfo.meta.hidden.status));

  vinfo = stack_info_pop(&stack);
  Dbg("%s: %d # %s ## %s", CS(vinfo.name), vinfo.age, CS(vinfo.meta.title),
      CS(vinfo.meta.hidden.status));

  stack_info_deinit(&stack);
  arena_destroy(arena);
  Dbg("end of %s\n", __func__);
}

/* ****************************************** */
void stack_pointer() {
  Arena *arena = arena_create();
  StackInfoPtr stack = stack_info_ptr_init(arena);

  Info info1 = {
      .name = S("sepi"),
      .age = 38,
      .meta = {.title = S("mr"), .hidden = {.status = S("SHIT SEPI")}}};
  Info info2 = {
      .name = S("matilde"),
      .age = 27,
      .meta = {.title = S("mrs"), .hidden = {.status = S("SHIT MATILDE")}}};

  stack_info_ptr_push(&stack, &info1);
  stack_info_ptr_push(&stack, &info2);

  Info *pinfo = stack_info_ptr_pop(&stack);
  Dbg("%s: %d # %s ## %s", CS(pinfo->name), pinfo->age, CS(pinfo->meta.title),
      CS(pinfo->meta.hidden.status));

  pinfo = stack_info_ptr_pop(&stack);
  Dbg("%s: %d # %s ## %s", CS(pinfo->name), pinfo->age, CS(pinfo->meta.title),
      CS(pinfo->meta.hidden.status));

  stack_info_ptr_deinit(&stack);
  arena_destroy(arena);
  Dbg("end of %s\n", __func__);
}

/* ****************************************** */
void array_value() {
  Arena *arena = arena_create();
  ArrayInfo array = array_info_init(arena);

  Info info1 = {
      .name = S("sepi"),
      .age = 38,
      .meta = {.title = S("mr"), .hidden = {.status = S("SHIT SEPI")}}};
  Info info2 = {
      .name = S("matilde"),
      .age = 27,
      .meta = {.title = S("mrs"), .hidden = {.status = S("SHIT MATILDE")}}};

  array_info_push(&array, &info1);
  array_info_push(&array, &info2);

  Info *pinfo = array_info_get(&array, 0);
  Dbg("%s: %d # %s ## %s", CS(pinfo->name), pinfo->age, CS(pinfo->meta.title),
      CS(pinfo->meta.hidden.status));

  pinfo = array_info_get(&array, 1);
  Dbg("%s: %d # %s ## %s", CS(pinfo->name), pinfo->age, CS(pinfo->meta.title),
      CS(pinfo->meta.hidden.status));

  arena_destroy(arena);
  Dbg("end of %s\n", __func__);
}

/* ****************************************** */
void array_pointer() {
  Arena *arena = arena_create();
  ArrayInfoPtr array = array_info_ptr_init(arena);

  Info* pinfo1 = arena_push(arena, sizeof(Info), AlignOf(Info), TRUE);
  Info* pinfo2 = arena_push(arena, sizeof(Info), AlignOf(Info), TRUE);

  pinfo1->name = S("sepi");
  pinfo1->age = 38;
  pinfo1->meta.title = S("mr");
  pinfo1->meta.hidden.status = S("SHIT SEPI");


  pinfo2->name = S("matilde");
  pinfo2->age = 27;
  pinfo2->meta.title = S("mrs");
  pinfo2->meta.hidden.status = S("SHIT MATILDE");

  array_info_ptr_push(&array, &pinfo1);
  array_info_ptr_push(&array, &pinfo2);

  Info** ppinfo = array_info_ptr_get(&array, 0);
  Info* pinfo = *ppinfo;
  Dbg("%s: %d # %s ## %s", CS(pinfo->name), pinfo->age, CS(pinfo->meta.title),
      CS(pinfo->meta.hidden.status));

  ppinfo = array_info_ptr_get(&array, 1);
  pinfo = *ppinfo;
  Dbg("%s: %d # %s ## %s", CS(pinfo->name), pinfo->age, CS(pinfo->meta.title),
      CS(pinfo->meta.hidden.status));

  arena_destroy(arena);
  Dbg("end of %s\n", __func__);
}

/* ****************************************** */
void list_value() {
  Arena *arena = arena_create();
  ListInfo list = list_info_init(arena);

  Info info1 = {
      .name = S("sepi"),
      .age = 38,
      .meta = {.title = S("mr"), .hidden = {.status = S("SHIT SEPI")}}};
  Info info2 = {
      .name = S("matilde"),
      .age = 27,
      .meta = {.title = S("mrs"), .hidden = {.status = S("SHIT MATILDE")}}};

  list_info_push_back(&list, info1);
  list_info_push_back(&list, info2);

  Info vinfo = list_info_get(&list, 0);
  Dbg("%s: %d # %s ## %s", CS(vinfo.name), vinfo.age, CS(vinfo.meta.title),
      CS(vinfo.meta.hidden.status));

  vinfo = list_info_get(&list, 1);
  Dbg("%s: %d # %s ## %s", CS(vinfo.name), vinfo.age, CS(vinfo.meta.title),
      CS(vinfo.meta.hidden.status));


  ListInfoNode* node = list_info_pop_back(&list);
  Dbg("%s: %d # %s ## %s", CS(node->value.name), node->value.age, CS(node->value.meta.title),
      CS(node->value.meta.hidden.status));

  node = list_info_pop_back(&list);
  Dbg("%s: %d # %s ## %s", CS(node->value.name), node->value.age, CS(node->value.meta.title),
      CS(node->value.meta.hidden.status));


  list_info_deinit(&list);
  arena_destroy(arena);
  Dbg("end of %s\n", __func__);
}

/* ****************************************** */
void list_pointer() {
  Arena *arena = arena_create();
  ListInfoPtr list = list_info_ptr_init(arena);

  Info info1 = {
      .name = S("sepi"),
      .age = 38,
      .meta = {.title = S("mr"), .hidden = {.status = S("SHIT SEPI")}}};
  Info info2 = {
      .name = S("matilde"),
      .age = 27,
      .meta = {.title = S("mrs"), .hidden = {.status = S("SHIT MATILDE")}}};

  list_info_ptr_push_back(&list, &info1);
  list_info_ptr_push_back(&list, &info2);

  Info* pinfo = list_info_ptr_get(&list, 0);
  Dbg("%s: %d # %s ## %s", CS(pinfo->name), pinfo->age, CS(pinfo->meta.title),
      CS(pinfo->meta.hidden.status));

  pinfo = list_info_ptr_get(&list, 1);
  Dbg("%s: %d # %s ## %s", CS(pinfo->name), pinfo->age, CS(pinfo->meta.title),
      CS(pinfo->meta.hidden.status));


  ListInfoPtrNode* node = list_info_ptr_pop_back(&list);
  Dbg("%s: %d # %s ## %s", CS(node->value->name), node->value->age, CS(node->value->meta.title),
      CS(node->value->meta.hidden.status));

  node = list_info_ptr_pop_back(&list);
  Dbg("%s: %d # %s ## %s", CS(node->value->name), node->value->age, CS(node->value->meta.title),
      CS(node->value->meta.hidden.status));


  list_info_ptr_deinit(&list);
  arena_destroy(arena);
  Dbg("end of %s\n", __func__);
}

/* ****************************************** */
int main() {
  // stack_value();
  // stack_pointer();
  // array_value();
  array_pointer();
  // list_value();
  // list_pointer();
  return 0;
}

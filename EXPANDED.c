
typedef struct {
  Str8 name;
  U32 age;
} Info;

typedef struct StackInfoNode StackInfoNode;
struct StackInfoNode {
  Info val;
  StackInfoNode *previous;
};
typedef struct StackInfo StackInfo;
struct StackInfo {
  U64 length;
  Arena *arena;
  StackInfoNode *top;
};
StackInfo stack_info_init(Arena *arena) {
  ;
  StackInfo stack = {.arena = arena, .length = 0, .top = 0};
  return stack;
}
Nothing stack_info_push(StackInfo *stack, Info val) {
  ;
  StackInfoNode *node = arena_push(stack->arena, sizeof(StackInfoNode),
                                   __alignof(StackInfoNode), 0);
  node->val = val;
  node->previous = stack->top;
  stack->top = node;
  stack->length++;
  ;
}
Info stack_info_pop(StackInfo *stack) {
  ;
  Info val = {0};
  if (!stack->length) {
    goto cleanup;
  }
  StackInfoNode *old_top = stack->top;
  val = old_top->val;
  stack->top = old_top->previous;
  old_top->previous = 0;
  stack->length--;
cleanup:;
  return val;
}
Nothing stack_info_deinit(StackInfo *stack) {
  ;
  for (; stack->length > 0;) {
    stack_info_pop(stack);
  }
  stack->arena = 0;
  stack->length = 0;
  stack->top = 0;
  ;
}
U32 stack_info_length(StackInfo *stack) { return stack->length; };

/////////////////////////////////////////////////////////////

typedef struct StackInfoPtrNode StackInfoPtrNode;
struct StackInfoPtrNode {
  Info *val;
  StackInfoPtrNode *previous;
};
typedef struct StackInfoPtr StackInfoPtr;
struct StackInfoPtr {
  U64 length;
  Arena *arena;
  StackInfoPtrNode *top;
};
StackInfoPtr stack_info_ptr_init(Arena *arena) {
  ;
  StackInfoPtr stack = {.arena = arena, .length = 0, .top = 0};
  return stack;
}
Nothing stack_info_ptr_push(StackInfoPtr *stack, Info *val) {
  ;
  StackInfoPtrNode *node = arena_push(stack->arena, sizeof(StackInfoPtrNode),
                                      __alignof(StackInfoPtrNode), 0);
  node->val = val;
  node->previous = stack->top;
  stack->top = node;
  stack->length++;
  ;
}
Info *stack_info_ptr_pop(StackInfoPtr *stack) {
  ;
  Info *val = {0};
  if (!stack->length) {
    goto cleanup;
  }
  StackInfoPtrNode *old_top = stack->top;
  val = old_top->val;
  stack->top = old_top->previous;
  old_top->previous = 0;
  stack->length--;
cleanup:;
  return val;
}
Nothing stack_info_ptr_deinit(StackInfoPtr *stack) {
  ;
  for (; stack->length > 0;) {
    stack_info_ptr_pop(stack);
  }
  stack->arena = 0;
  stack->length = 0;
  stack->top = 0;
  ;
}
U32 stack_info_ptr_length(StackInfoPtr *stack) { return stack->length; };

void stack_value() {
  Arena *arena = arena_create_(&(ArenaParams){
      .requested_reserve_size = (((U64)(64)) << 20),
      .requested_commit_size = (((U64)(64)) << 20),
      .caller_file_name = "11.c",
      .caller_file_line = 22,
  });
  StackInfo stack = stack_info_init(arena);

  Info info1 = {.name = _Generic(("sepi"),
                    Str8: ("sepi"),
                    Str: str8((CStr)("sepi")),
                    CStr: str8(("sepi")),
                    default: str8((CStr)("sepi"))),
                .age = 38};
  Info info2 = {.name = _Generic(("matilde"),
                    Str8: ("matilde"),
                    Str: str8((CStr)("matilde")),
                    CStr: str8(("matilde")),
                    default: str8((CStr)("matilde"))),
                .age = 27};

  stack_info_push(&stack, info1);
  stack_info_push(&stack, info2);

  Info vinfo = stack_info_pop(&stack);
  ;

  vinfo = stack_info_pop(&stack);
  ;

  stack_info_deinit(&stack);
}

void stack_pointer() {
  Arena *arena = arena_create_(&(ArenaParams){
      .requested_reserve_size = (((U64)(64)) << 20),
      .requested_commit_size = (((U64)(64)) << 20),
      .caller_file_name = "11.c",
      .caller_file_line = 42,
  });
  StackInfoPtr stack = stack_info_ptr_create(arena);
# 58 "11.c"
}

int main() {
  stack_value();
  return 0;
}

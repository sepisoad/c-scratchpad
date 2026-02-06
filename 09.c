#include <stdio.h>

#define SEPI_PLATFORM_IMPLEMENTATION
#define SEPI_ENDIAN_IMPLEMENTATION
#define SEPI_ARENA_IMPLEMENTATION
#define SEPI_STRING_IMPLEMENTATION
#define SEPI_ARRAY_IMPLEMENTATION
#define SEPI_STACK_IMPLEMENTATION
#define SEPI_TREE_IMPLEMENTATION

#include <sepi/arena.h>
#include <sepi/stack.h>
#include <sepi/string.h>
#include <sepi/tree.h>

typedef struct {
  Str8 name;
  U32 age;
} Node;

void test1() {
  Arena *arena = arena_create();

  Node n0 = {.name = S("sepi"), .age = 38};
  Tree *tree = tree_create(arena, &n0, sizeof(Node), AlignOf(Node));

  Node n1 = {.name = S("manohar"), .age = 31};
  Node n2 = {.name = S("liam"), .age = 30};
  Node n3 = {.name = S("mathilde"), .age = 27};

  TreeNode *tn1 = tree_push(tree, tree->root, &n1);
  TreeNode *tn2 = tree_push(tree, tree->root, &n2);
  TreeNode *tn3 = tree_push(tree, tree->root, &n3);

  Node n4 = {.name = S("eden"), .age = 38};
  Node n5 = {.name = S("simone"), .age = 25};

  TreeNode *tn4 = tree_push(tree, tn2, &n4);
  TreeNode *tn5 = tree_push(tree, tn2, &n5);

  Node n6 = {.name = S("roman"), .age = 25};
  TreeNode *tn6 = tree_push(tree, tn5, &n6);

  Ignore(tn1);
  Ignore(tn3);
  Ignore(tn4);
  Ignore(tn6);

  Stack *stack = stack_create(arena);

  stack_push(stack, tree_root(tree));

  do {
    TreeNode *top = stack_pop(stack);
    TreeNode *parent = top->parent;
    Str8 name = ((Node *)top->data)->name;
    Str8 parent_name = parent ? ((Node *)parent->data)->name : S("*");

    printf("[%s]->%s\n", CS(parent_name), CS(name));

    for (U32 index = 0; index < tree_node_length(top); index++) {
      TreeNode *child = tree_node_get(top, index);
      if (tree_node_length(top) > 0) {
        stack_push(stack, child);
      } else {
        Str8 parent_name = ((Node *)top->data)->name;
        Str8 child_name = ((Node *)child->data)->name;
        printf("[%s]->%s\n", CS(parent_name), CS(child_name));
      }
    }
  } while (stack->length > 0);

  stack_destroy(stack);
  tree_destroy(tree);
  arena_destroy(arena);
}

void test2() {
  Arena *arena = arena_create();

  Node n0 = {.name = S("sepi"), .age = 38};
  Node n1 = {.name = S("manohar"), .age = 31};
  Node n2 = {.name = S("liam"), .age = 30};
  Node n3 = {.name = S("mathilde"), .age = 27};
  Node n4 = {.name = S("eden"), .age = 38};
  Node n5 = {.name = S("simone"), .age = 25};
  Node n6 = {.name = S("roman"), .age = 25};

  Tree *tree = tree_create(arena, &n0, sizeof(Node), AlignOf(Node));
  TreeNode *tn1 = tree_push(tree, tree->root, &n1);
  TreeNode *tn2 = tree_push(tree, tn1, &n2);
  TreeNode *tn3 = tree_push(tree, tn2, &n3);
  TreeNode *tn4 = tree_push(tree, tree->root, &n4);
  TreeNode *tn5 = tree_push(tree, tn4, &n5);
  TreeNode *tn6 = tree_push(tree, tn5, &n6);

  // * -> sepi
  // sepi ->

  Ignore(tn3);
  Ignore(tn6);
  Stack *stack = stack_create(arena);
  stack_push(stack, tree_root(tree));

  do {
    TreeNode *top = stack_pop(stack);
    TreeNode *parent = top->parent;
    Str8 name = ((Node *)top->data)->name;
    Str8 parent_name = parent ? ((Node *)parent->data)->name : S("*");

    printf("%s -> %s\n", CS(parent_name), CS(name));

    for (U32 index = 0; index < tree_node_length(top); index++) {
      TreeNode *child = tree_node_get(top, index);
      if (tree_node_length(top) > 0) {
        stack_push(stack, child);
      } else {
        Str8 parent_name = ((Node *)top->data)->name;
        Str8 child_name = ((Node *)child->data)->name;
        printf("%s -> %s\n", CS(parent_name), CS(child_name));
      }
    }
  } while (stack->length > 0);

  stack_destroy(stack);
  tree_destroy(tree);
  arena_destroy(arena);
}

int main(void) {
  test2();
  return 0;
}

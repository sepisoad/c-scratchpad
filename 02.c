#include <stdio.h>
#include <time.h>

#define SEPI_ENDIAN_IMPLEMENTATION
#define SEPI_PLATFORM_IMPLEMENTATION
#define SEPI_STRING_IMPLEMENTATION
#define SEPI_ARENA_IMPLEMENTATION
#define SEPI_ARRAY_IMPLEMENTATION
#define SEPI_LIST_IMPLEMENTATION
#define SEPI_STACK_IMPLEMENTATION
#define SEPI_IO_IMPLEMENTATION
#define SEPI_HASHMAP_IMPLEMENTATION

#include <sepi/base.h>
#include <sepi/array.h>
#include <sepi/hashmap.h>
#include <sepi/io.h>

int test_01() {
  Bool is_file = FALSE;

  IOError err = io_is_file(str8("makefile"), &is_file);
  if (IO_ERR_SUCCESS != err) {
    return 1;
  }

  if (FALSE == is_file) {
    return 1;
  }

  // ===================

  err = io_is_file(str8("."), &is_file);
  if (IO_ERR_SUCCESS != err) {
    return 1;
  }

  if (TRUE == is_file) {
    return 1;
  }

  // ===================

  return 0;
}

int test_02() {
  Bool is_dir = FALSE;

  IOError err = io_is_directory(str8("makefile"), &is_dir);
  if (IO_ERR_SUCCESS != err) {
    return 1;
  }

  if (TRUE == is_dir) {
    return 1;
  }

  // ===================

  err = io_is_directory(str8("."), &is_dir);
  if (IO_ERR_SUCCESS != err) {
    return 1;
  }

  if (FALSE == is_dir) {
    return 1;
  }

  // ===================

  return 0;
}

int test_03() {
  IOError err = io_make_directory(str8("/Users/sepi/Projects/sepi/c-scratchpad/SucMyC0ck"));
  if (IO_ERR_SUCCESS != err) {
    return 1;
  }

  err = io_make_directory(str8("./PooPoo"));
  if (IO_ERR_SUCCESS != err) {
    return 1;
  }

  err = io_make_directory(str8("ChooChoo"));
  if (IO_ERR_SUCCESS != err) {
    return 1;
  }

  return 0;
}

int test_04() {
  IOError err = 0;

  Arena* a = arena_create();

  err = io_make_nested_directory(str8_clone(a, str8("DIR1/DIR2/DIR3")));
  if (IO_ERR_SUCCESS != err) {
    return 1;
  }

  err = io_make_nested_directory(str8_clone(a, str8("./DIR1/DIR4/DIR5")));
  if (IO_ERR_SUCCESS != err) {
    return 1;
  }

  err = io_make_nested_directory(str8_clone(a, str8("/Users/sepi/Projects/sepi/c-scratchpad/DIR1/DIR6/DIR7")));
  if (IO_ERR_SUCCESS != err) {
    return 1;
  }

  return 0;
}

int test_05() {
  IOError err = 0;

  Arena* a = arena_create();
  IONode* node = arena_push(a, sizeof(IONode), AlignOf(IONode), TRUE);

  err = io_directory_children(a, str8("/tmp/you"), node);
  if (IO_ERR_SUCCESS != err) {
    return 1;
  }

  for (U32 index = 0; index < node->children->offset; index++) {
    IONode* n = array_get(node->children, index);
    printf("%s\n", n->name.cstr);
  }

  return 0;
}

int test_06() {
  IOError err = 0;

  Arena* a = arena_create();
  IONode* node = arena_push(a, sizeof(IONode), AlignOf(IONode), TRUE);

  err = io_directory_nested_children(a, str8("/tmp/you/test"), node);
  if (IO_ERR_SUCCESS != err) {
    return 1;
  }

  for (U32 index = 0; index < node->children->offset; index++) {
    IONode* n = array_get(node->children, index);
    printf("%s\n", n->name.cstr);
  }

  return 0;
}

int main(Nothing) {
  int res = 0;

  // res = test_01();
  // if (res) {
  //   goto cleanup;
  // }

  // res = test_02();
  // if (res) {
  //   goto cleanup;
  // }

  // res = test_03();
  // if (res) {
  //   goto cleanup;
  // }

  // res = test_04();
  // if (res) {
  //   goto cleanup;
  // }

  // res = test_05();
  // if (res) {
  //   goto cleanup;
  // }

  res = test_06();
  if (res) {
    goto cleanup;
  }

cleanup:
  return res;
}

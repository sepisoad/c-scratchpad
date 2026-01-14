#include <stdio.h>

#define SEPI_ENDIAN_IMPLEMENTATION
#define SEPI_PLATFORM_IMPLEMENTATION
#define SEPI_STRING_IMPLEMENTATION
#define SEPI_ARENA_IMPLEMENTATION
#define SEPI_LIST_IMPLEMENTATION
#define SEPI_HASHMAP_IMPLEMENTATION
#define SEPI_IO_IMPLEMENTATION

#include <sepi/base.h>
#include <sepi/io.h>
#include <sepi/string.h>

I32 test_io_is_directory_or_file() {
  Str8 a_file = str8("/home/sepi/Projects/sepi/c-scratchpad/makefile");
  Str8 a_dir = str8("/home/sepi/Projects/sepi/c-scratchpad/");

  Bool is_dir = FALSE;

  IOError err = io_is_directory(a_file, &is_dir);
  if (IO_ERR_SUCCESS != err) {
    printf("test_io_is_directory_or_file\n");
    return 1;
  }

  if (is_dir) {
    printf("'%s' is a dir\n", a_file.cstr);
  } else {
    printf("'%s' is NOT a dir\n", a_file.cstr);
  }

  err = io_is_directory(a_dir, &is_dir);
  if (IO_ERR_SUCCESS != err) {
    printf("test_io_is_directory_or_file\n");
    return 1;
  }

  if (is_dir) {
    printf("'%s' is a dir\n", a_dir.cstr);
  } else {
    printf("'%s' is NOT a dir\n", a_dir.cstr);
  }

  Bool is_file = FALSE;

  err = io_is_file(a_file, &is_file);
  if (IO_ERR_SUCCESS != err) {
    printf("test_io_is_directory_or_file\n");
    return 1;
  }

  if (is_file) {
    printf("'%s' is a file\n", a_file.cstr);
  } else {
    printf("'%s' is NOT a file\n", a_file.cstr);
  }

  err = io_is_file(a_dir, &is_file);
  if (IO_ERR_SUCCESS != err) {
    printf("test_io_is_directory_or_file\n");
    return 1;
  }

  if (is_file) {
    printf("'%s' is a file\n", a_dir.cstr);
  } else {
    printf("'%s' is NOT a file\n", a_dir.cstr);
  }

  return 0;
}

I32 test_io_make_directory() {
  Str8 dir_1 = str8("/tmp/sepi-1");
  IOError err = io_make_directory(dir_1);
  if (IO_ERR_SUCCESS != err) {
    printf("test_io_make_directory\n");
    return 1;
  }
  printf("made '%s' directory!\n", dir_1.cstr);

  Str8 dir_2 = str8("/tmp/sepi-1/sepi-2");
  err = io_make_directory(dir_2);
  if (IO_ERR_SUCCESS != err) {
    printf("test_io_make_directory\n");
    return 1;
  }
  printf("made '%s' directory!\n", dir_2.cstr);


  return 0;
}

I32 test_io_make_nested_directory() {
  Str8 dir_1 = str8("/tmp/sepi-a/sepi-b/sepi-c/sepi-d");
  IOError err = io_make_nested_directory(dir_1);
  if (IO_ERR_SUCCESS != err) {
    printf("test_io_make_nested_directory\n");
    return 1;
  }
  printf("made '%s' directory!\n", dir_1.cstr);

  Str8 dir_2 = str8("/tmp/sepi-a//sepi-b/sepi-c/sepi-d/sepi-e/");
  err = io_make_nested_directory(dir_2);
  if (IO_ERR_SUCCESS != err) {
    printf("test_io_make_nested_directory\n");
    return 1;
  }
  printf("made '%s' directory!\n", dir_2.cstr);


  return 0;
}

I32 test_io_directory_children() {
  Str8 dir = str8("/tmp/");
  Arena *a = arena_create();
  HashMap *hm = hashmap_init(a, 64);

  res = io_directory_children(dir, hm);
  if (IO_ERR_SUCCESS != err) {
    printf("test_io_directory_children\n");
    return 1;
  }

  return 0;
}

int main(Nothing) {
  I32 res = 0;

  // res = test_io_is_directory_or_file();
  // if (res) {
  //   return res;
  // }

  // res = test_io_make_directory();
  // if (res) {
  //   return res;
  // }

  // res = test_io_make_nested_directory();
  // if (res) {
  //   return res;
  // }

  // res = test_io_make_nested_directory();
  // if (res) {
  //   return res;
  // }

  res = test_io_directory_children();
  if (res) {
    return res;
  }
}

#include <stdio.h>

#define SEPI_ENDIAN_IMPLEMENTATION
#define SEPI_PLATFORM_IMPLEMENTATION
#define SEPI_STRING_IMPLEMENTATION
#define SEPI_ARENA_IMPLEMENTATION
#define SEPI_ARRAY_IMPLEMENTATION
#define SEPI_HASHMAP_IMPLEMENTATION
#define SEPI_IO_IMPLEMENTATION

#include <sepi/array.h>

I32 test_array_1() {
  U32 idx = 0;
  U32 seg = ARRAY_GET_IDX_SEGMENT_NORMALIZED(idx);
  U32 cap = ARRAY_SEGMENT_CAPACITY(seg);

  printf("ARRAY_GET_IDX_SEGMENT_NORMALIZED(%d) => %d\n", idx, seg);
  printf("ARRAY_SEGMENT_CAPACITY(%d) => %d\n", seg, cap);

  return 0;
}

I32 test_array_2() {
  Arena* a = arena_create();
  Array* arr = array_create(a, sizeof(U32), AlignOf(U32));

  U32 max = 10000;

  for (U32 num = 0; num < max; num++) {
    array_push(arr, &num);
  }

  for (U32 index = 0; index < max; index++) {
    U32* p1 = array_get(arr, index);
    printf("%d\n", *p1);
  }


  arena_destroy(a);
  return 0;
}

int main(Nothing) {
  I32 res = 0;

  // res = test_array_1();
  // if (res) {
  //   return res;
  // }

  //========
  res = test_array_2();
  if (res) {
    return res;
  }

  return res;
}

// LD_PRELOAD=/opt/gcc/15.2.0/lib64/libasan.so ./out

#include <stdlib.h>
#include <stdio.h>

#define SEPI_PLATFORM_IMPLEMENTATION
#define SEPI_STRING_IMPLEMENTATION
#define SEPI_ARENA_IMPLEMENTATION
#define SEPI_HASHMAP_IMPLEMENTATION

#include "deps/sepi/hashmap.h"

int
main(void) {
  Arena* a = arena_alloc(.requested_reserve_size = 256,
                         .requested_commit_size = 256);

  HashMap* hm = hashmap_init(a, 256);

  hashmap_push_str8_u32(a, hm, str8("sepi"), 38);
  hashmap_push_str8_u32(a, hm, str8("yasin"), 43);
  hashmap_push_str8_u32(a, hm, str8("amin"), 45);
  hashmap_push_str8_u32(a, hm, str8("mooa"), 67);
  hashmap_push_str8_u32(a, hm, str8("booa"), 71);

  HashMapKV* kv = hashmap_find_str8(hm, str8("amin"));
  printf("%s:  %d\n", kv->k_str.cstr, kv->v_u32);

  kv = hashmap_find_str8(hm, str8("sepi"));
  printf("%s:  %d\n", kv->k_str.cstr, kv->v_u32);

  kv = hashmap_find_str8(hm, str8("booa"));
  printf("%s:  %d\n", kv->k_str.cstr, kv->v_u32);

  kv = hashmap_find_str8(hm, str8("mooa"));
  printf("%s:  %d\n", kv->k_str.cstr, kv->v_u32);

  kv = hashmap_find_str8(hm, str8("yasin"));
  printf("%s:  %d\n", kv->k_str.cstr, kv->v_u32);


  hashmap_purge(hm);
  arena_release(a);
  return 0;
}

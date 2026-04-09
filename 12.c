#define SEPI_ENDIAN_IMPLEMENTATION
#define SEPI_ARENA_IMPLEMENTATION
#define SEPI_STRING_IMPLEMENTATION
#define SEPI_XMAP_WU16_IMPLEMENTATION
#define SEPI_XMAP_EMPTY_IMPLEMENTATION

#include <sepi/generated/xmap_wu16.h>
#include <sepi/generated/xmap_empty.h>
#include <sepi/string.h>

typedef struct WU16 WU16;
struct WU16 {
  U16 _;
};

void test_1() {
  Arena* arena = arena_create();
  MapWU16 map = map_wu16_make(arena, 64);

  WU16 sepi = {1987};
  WU16 eden = {1986};
  WU16 matilde = {1998};
  WU16 simona = {2000};

  map_wu16_push(&map, S("sepi"), &sepi);
  map_wu16_push(&map, S("eden"), &eden);
  map_wu16_push(&map, S("matilde"), &matilde);
  map_wu16_push(&map, S("simona"), &simona);

  dbg("%s: %d", "sepi", map_wu16_get(&map, S("sepi"))->_);
  dbg("%s: %d", "eden", map_wu16_get(&map, S("eden"))->_);
  dbg("%s: %d", "matilde", map_wu16_get(&map, S("matilde"))->_);
  dbg("%s: %d", "simona", map_wu16_get(&map, S("simona"))->_);

  dbg("---");

  dbg("%s: %d", "sepi", map_wu16_delete(&map, S("sepi"))->_);
  dbg("%s: %d", "eden", map_wu16_delete(&map, S("eden"))->_);
  dbg("%s: %d", "matilde", map_wu16_delete(&map, S("matilde"))->_);
  dbg("%s: %d", "simona", map_wu16_delete(&map, S("simona"))->_);

  map_wu16_get(&map, S("sepi"));
  map_wu16_get(&map, S("eden"));
  map_wu16_get(&map, S("matilde"));
  map_wu16_get(&map, S("simona"));
}

void test_2() {
  Arena* arena = arena_create();
  MapWU16 map = map_wu16_make(arena, 64);

  WU16 sepi_ = {1800};
  WU16 sepi = {1987};
  WU16 eden = {1986};
  WU16 matilde = {1998};
  WU16 simona = {2000};

  map_wu16_delete(&map, S("sepi"));
  map_wu16_push(&map, S("sepi"), &sepi);
  map_wu16_delete(&map, S("sepi"));
  map_wu16_push(&map, S("sepi"), &sepi_);
  map_wu16_push(&map, S("sepi"), &sepi);

  map_wu16_push(&map, S("eden"), &eden);
  map_wu16_push(&map, S("matilde"), &matilde);
  map_wu16_push(&map, S("simona"), &simona);

  dbg("%s: %d", "sepi", map_wu16_get(&map, S("sepi"))->_);
  dbg("%s: %d", "eden", map_wu16_get(&map, S("eden"))->_);
  dbg("%s: %d", "matilde", map_wu16_get(&map, S("matilde"))->_);
  dbg("%s: %d", "simona", map_wu16_get(&map, S("simona"))->_);

  dbg("---");

  dbg("%s: %d", "simona", map_wu16_delete(&map, S("simona"))->_);
  dbg("%s: %d", "matilde", map_wu16_delete(&map, S("matilde"))->_);
  dbg("%s: %d", "eden", map_wu16_delete(&map, S("eden"))->_);
  dbg("%s: %d", "sepi", map_wu16_delete(&map, S("sepi"))->_);

  map_wu16_get(&map, S("sepi"));
  map_wu16_get(&map, S("eden"));
  map_wu16_get(&map, S("matilde"));
  map_wu16_get(&map, S("simona"));
}

void test_3() {
  Arena* arena = arena_create();
  MapWU16 map = map_wu16_make(arena, 1000);

  WU16 sepi_ = {1800};
  WU16 sepi = {1987};
  WU16 eden = {1986};
  WU16 matilde = {1998};
  WU16 simona = {2000};

  map_wu16_delete(&map, S("sepi"));
  map_wu16_push(&map, S("sepi"), &sepi);
  map_wu16_delete(&map, S("sepi"));
  map_wu16_push(&map, S("sepi"), &sepi_);
  map_wu16_push(&map, S("sepi"), &sepi);

  map_wu16_push(&map, S("eden"), &eden);
  map_wu16_push(&map, S("matilde"), &matilde);
  map_wu16_push(&map, S("simona"), &simona);

  dbg("%s: %d", "sepi", map_wu16_get(&map, S("sepi"))->_);
  dbg("%s: %d", "eden", map_wu16_get(&map, S("eden"))->_);
  dbg("%s: %d", "matilde", map_wu16_get(&map, S("matilde"))->_);
  dbg("%s: %d", "simona", map_wu16_get(&map, S("simona"))->_);

  dbg("---");

  dbg("%s: %d", "simona", map_wu16_delete(&map, S("simona"))->_);
  dbg("%s: %d", "matilde", map_wu16_delete(&map, S("matilde"))->_);
  dbg("%s: %d", "eden", map_wu16_delete(&map, S("eden"))->_);
  dbg("%s: %d", "sepi", map_wu16_delete(&map, S("sepi"))->_);

  map_wu16_get(&map, S("sepi"));
  map_wu16_get(&map, S("eden"));
  map_wu16_get(&map, S("matilde"));
  map_wu16_get(&map, S("simona"));
}

void test_4() {
  Arena* arena = arena_create();
  MapWU16 map = map_wu16_make(arena, 64);

  WU16 sepi_ = {1800};
  WU16 sepi = {1987};
  WU16 eden = {1986};
  WU16 matilde = {1998};
  WU16 simona = {2000};

  map_wu16_delete(&map, S("sepi"));
  map_wu16_push(&map, S("sepi"), &sepi);
  map_wu16_delete(&map, S("sepi"));
  map_wu16_push(&map, S("sepi"), &sepi_);
  map_wu16_push(&map, S("sepi"), &sepi);

  map_wu16_push(&map, S("eden"), &eden);
  map_wu16_push(&map, S("matilde"), &matilde);
  map_wu16_push(&map, S("simona"), &simona);

  Str8* keys = 0;
  U64 length = 0;

  map_wu16_keys(&map, &keys, &length);

  for(U64 index = 0; index < length; index++) {
    dbg("%s", keys[index].cstr);
  }
}

void test_5() {
  Arena* arena = arena_create();
  MapEmpty map = map_empty_make(arena, 64);

  Empty _ = {};

  map_empty_push(&map, S("sepi"), &_);
  map_empty_push(&map, S("eden"), &_);
  map_empty_push(&map, S("matilde"), &_);
  map_empty_push(&map, S("simona"), &_);
  map_empty_push(&map, S("amin"), &_);
  map_empty_push(&map, S("yasin"), &_);
  map_empty_push(&map, S("mua"), &_);
  map_empty_push(&map, S("bua"), &_);
  map_empty_push(&map, S("aref"), &_);
  map_empty_push(&map, S("mona"), &_);
  map_empty_push(&map, S("maryam"), &_);

  Str8* keys = 0;
  U64 length = 0;

  map_empty_keys(&map, &keys, &length);

  for(U64 index = 0; index < length; index++) {
    dbg("%s", keys[index].cstr);
  }
}

int main() {
  // test_1();
  // dbg("===");
  // test_2();
  // dbg("===");
  // test_3();
  // dbg("===");
  // test_4();
  // dbg("===");
  test_5();

  return 0;
}

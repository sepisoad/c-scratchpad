#define SEPI_ENDIAN_IMPLEMENTATION
#define SEPI_ARENA_IMPLEMENTATION
#define SEPI_STRING_IMPLEMENTATION
// #define SEPI_MAP_WU16_IMPLEMENTATION
#define SEPI_XMAP_WU16_IMPLEMENTATION

// #include <sepi/generated/map_wu16.h>
#include <sepi/generated/xmap_wu16.h>
#include <sepi/string.h>

#define PAK_ENTRIES_COUNT 15
#define PAK_ENTRY_NAME_LEN 56

typedef struct WU16 WU16;
struct WU16 {
  U16 _;
};

const char entries[PAK_ENTRIES_COUNT][PAK_ENTRY_NAME_LEN] = {
    "a/b/c/01", "a/b/c/02", "a/b/d/03", "a/b/04", "a/b/05",
    "a/e/f/06", "a/e/f/07", "a/e/08",   "g/h/09", "g/h/10",
    "g/11",     "g/12",     "13",       "14",     "15",
};

U16 get_path_depth(CStr path, U16 length) {
  U16 depth = 1;

  for (U16 index = 0; index < length; index++) {
    if (path[index] == 0) {
      break;
    }

    if (path[index] == '/') {
      depth = depth + 1;
    }
  }

  return depth;
}

void get_path_at_depth(CStr path, U16 length, U16 depth,
                       char out[PAK_ENTRY_NAME_LEN]) {
  U16 traversed = 1;
  for (U16 index = 0; index < length; index++) {
    if (path[index] == 0 || traversed > depth) {
      break;
    }

    if (path[index] == '/') {
      traversed++;
    }

    out[index] = path[index];
  }
}

void get_name_at_depth(CStr path, U16 length, U16 depth,
                       char out[PAK_ENTRY_NAME_LEN]) {
  U16 traversed = 1;
  for (U16 idxsrc = 0, idxdst = 0; idxsrc < length; idxsrc++) {
    if (path[idxsrc] == 0 || traversed > depth) {
      break;
    }

    if (path[idxsrc] == '/') {
      traversed++;
      continue;
    }

    if (traversed == depth) {
      out[idxdst++] = path[idxsrc];
    }
  }
}

int main() {
  Arena *arena = arena_create();
  MapWU16 map = map_wu16_make(arena, 64);

  for (U16 entry_index = 0; entry_index < PAK_ENTRIES_COUNT; entry_index++) {
    U16 entry_depth =
        get_path_depth(entries[entry_index], strlen(entries[entry_index]));
    for (U16 depth_index = 1; depth_index <= entry_depth; depth_index++) {
      char item_path[PAK_ENTRY_NAME_LEN] = {0};
      char item_name[PAK_ENTRY_NAME_LEN] = {0};
      get_path_at_depth(entries[entry_index], strlen(entries[entry_index]),
                        depth_index, item_path);
      get_name_at_depth(entries[entry_index], strlen(entries[entry_index]),
                        depth_index, item_name);

      WU16 *found = map_wu16_get(&map, S(item_path));
      if (found) {
        found->_++;
      } else {
        WU16 *new = arena_push(arena, sizeof(WU16), alignof(WU16), TRUE);
        new->_ = 1;
        map_wu16_push(&map, S(item_path), new);
      }
    }
  }

  char kykey[PAK_ENTRY_NAME_LEN] = {0};
  kykey[0] = 'a';
  kykey[1] = '/';

  WU16 *count = map_wu16_get(&map, S(kykey));
  if (count) {
    dbg("%d", count->_);
  }

  return 0;
}

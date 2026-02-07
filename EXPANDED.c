
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
typedef struct MapInfoPtrNode MapInfoPtrNode;
struct MapInfoPtrNode {
  MapInfoPtrNode *next;
  Str8 key;
  Info *value;
};
typedef struct MapInfoPtrList MapInfoPtrList;
struct MapInfoPtrList {
  MapInfoPtrNode *first;
  MapInfoPtrNode *last;
};
typedef struct MapInfoPtr MapInfoPtr;
struct MapInfoPtr {
  U64 length;
  U64 capacity;
  MapInfoPtrList *list;
  MapInfoPtrList free_list;
  Arena *arena;
};
Nothing map_info_ptr_list_concat_in_place(MapInfoPtrList *to,
                                          MapInfoPtrList *from) {
  ;
  if (from->first) {
    if (to->first) {
      to->last->next = from->first;
      to->last = from->last;
    } else {
      to->first = from->first;
      to->last = from->last;
    }
    __builtin___memset_chk(((from)), 0, (sizeof(*(from))),
                           __builtin_object_size(((from)), 0));
  };
}
MapInfoPtrNode *map_info_ptr_list_pop(MapInfoPtrList *list) {
  ;
  MapInfoPtrNode *node = list->first;
  if (list->first == list->last) {
    list->first = 0;
    list->last = 0;
  } else {
    list->first = list->first->next;
  };
  return node;
}
U64 map_info_ptr_hasher(Str8 str) {
  return rapidhash_withSeed((str).cstr, (str).length, 1987);
}
MapInfoPtr map_info_ptr_make(Arena *arena, U64 capacity) {
  ;
  MapInfoPtr map = {
      .capacity = capacity,
      .list = (MapInfoPtrList *)arena_push(
          (arena), sizeof(MapInfoPtrList) * (capacity),
          (((8) > (__alignof(MapInfoPtrList)) ? (8)
                                              : (__alignof(MapInfoPtrList)))),
          (0)),
      .arena = arena,
  };
  ;
  return map;
}
Nothing map_info_ptr_clean(MapInfoPtr *map) {
  ;
  map->length = 0;
  for (U64 i = 0; i < map->capacity; ++i) {
    map_info_ptr_list_concat_in_place(&map->free_list, &map->list[i]);
  };
}
MapInfoPtrNode *map_info_ptr_push(MapInfoPtr *map, Str8 key, Info *value) {
  ;
  MapInfoPtrNode *map_node;
  U64 hash = map_info_ptr_hasher(key);
  if (map->free_list.first != 0) {
    map_node = map_info_ptr_list_pop(&map->free_list);
  } else {
    map_node = (MapInfoPtrNode *)arena_push(
        (map->arena), sizeof(MapInfoPtrNode) * (1),
        (((8) > (__alignof(MapInfoPtrNode)) ? (8)
                                            : (__alignof(MapInfoPtrNode)))),
        (0));
  }
  map_node->next = 0;
  map_node->key = key;
  map_node->value = value;
  U64 i = hash % map->capacity;
  if (map->list[i].first == 0) {
    map->list[i].first = map->list[i].last = map_node;
    map_node->next = 0;
  } else {
    map->list[i].last->next = map_node;
    map->list[i].last = map_node;
    map_node->next = 0;
  }
  map->length += 1;
  ;
  return map_node;
}
Info *map_info_ptr_find(MapInfoPtr *map, Str8 key) {
  ;
  U64 hash = map_info_ptr_hasher(key);
  U64 i = hash % map->capacity;
  MapInfoPtrList *list = map->list + i;
  for (MapInfoPtrNode *node = list->first; node != 0; node = node->next) {
    if (str8_equal(node->key, key, 0)) {
      ;
      return node->value;
    }
  };
  return (Info *){};
}
Info *map_info_ptr_pop(MapInfoPtr *map, Str8 key) {
  ;
  Info *value;
  U64 hash = map_info_ptr_hasher(key);
  U64 i = hash % map->capacity;
  MapInfoPtrList *list = map->list + i;
  MapInfoPtrNode *itr = list->first;
  MapInfoPtrNode *prv = itr;
  Bool single = list->first == list->last ? 1 : 0;
  for (; itr != 0; prv = itr, itr = itr->next) {
    if (str8_equal(itr->key, key, 0)) {
      prv->next = itr->next;
      value = itr->value;
      __builtin___memset_chk(((itr)), 0, (sizeof(*(itr))),
                             __builtin_object_size(((itr)), 0));
      if (single) {
        __builtin___memset_chk(((list)), 0, (sizeof(*(list))),
                               __builtin_object_size(((list)), 0));
      }
      map->length--;
      ;
      return value;
    }
  };
  return (Info *){};
};

int main() { return 0; }

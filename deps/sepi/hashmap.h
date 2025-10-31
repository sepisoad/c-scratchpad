#ifndef SEPI_HASHMAP_H
#define SEPI_HASHMAP_H

/* ===================================================== */
/*                     DEPENDENCIES                      */
/* ===================================================== */

#include "base.h"
#include "string.h"
#include "arena.h"
#include "../rapidhash/rapidhash.h"

/* ===================================================== */
/*                         TYPES                         */
/* ===================================================== */

typedef struct HashMapKV HashMapKV;
struct HashMapKV {
  union {
    Str8 k_str;
    RawPtr k_rawptr;
    U32 k_u32;
    U64 k_u64;
  };
  union {
    Str8 v_str;
    RawPtr v_rawptr;
    U32 v_u32;
    U64 v_u64;
  };
};

typedef struct HashMapNode HashMapNode;
struct HashMapNode {
  HashMapNode* next;
  HashMapKV kv;
};

typedef struct HashMapList HashMapList;
struct HashMapList {
  HashMapNode* first;
  HashMapNode* last;
};

typedef struct HashMap HashMap;
struct HashMap {
  U64 count;
  U64 capacity;
  HashMapList* list;
  HashMapList free_list;
};

/* ===================================================== */
/*                          API                          */
/* ===================================================== */

HashMap* hashmap_init(Arena* a, U64 cap);
Nothing hashmap_purge(HashMap *hm);
HashMapNode* hashmap_push(Arena* a, HashMap* hm, U64 hash, HashMapKV kv);
HashMapNode* hashmap_push_str8_str8(Arena *a, HashMap* hm, Str8 key,
                                    Str8 value);
HashMapNode* hashmap_push_str8_rawptr(Arena *a, HashMap* hm, Str8 key,
                                      RawPtr value);
HashMapNode* hashmap_push_str8_u32(Arena *a, HashMap* hm, Str8 key, U32 value);
HashMapNode* hashmap_push_str8_u64(Arena *a, HashMap* hm, Str8 key, U64 value);
HashMapNode* hashmap_push_u32_str8(Arena *a, HashMap* hm, U32 key, Str8 value);
HashMapNode* hashmap_push_u32_u32(Arena *a, HashMap* hm, U32 key, U32 value);
HashMapNode* hashmap_push_u32_u64(Arena *a, HashMap* hm, U32 key, U64 value);
HashMapNode* hashmap_push_u32_rawptr(Arena *a, HashMap* hm, U32 key,
                                     RawPtr value);
HashMapNode* hashmap_push_u64_str8(Arena *a, HashMap* hm, U64 key, Str8 value);
HashMapNode* hashmap_push_u64_u32(Arena *a, HashMap* hm, U64 key, U32 value);
HashMapNode* hashmap_push_u64_u64(Arena *a, HashMap* hm, U64 key, U64 value);
HashMapNode* hashmap_push_u64_rawptr(Arena *a, HashMap* hm, U64 key,
                                     RawPtr value);
HashMapKV* hashmap_find_str8(HashMap* hm, Str8 key);
HashMapKV* hashmap_find_u32(HashMap* hm, U32 key);
HashMapKV* hashmap_find_u64(HashMap* hm, U64 key);
HashMapKV* hashmap_find_rawptr(HashMap* hm, RawPtr key);

/* ===================================================== */
/*                    IMPLEMENTATION                     */
/* ===================================================== */

#ifdef SEPI_HASHMAP_IMPLEMENTATION

internal Nothing
hashmap_list_concat_in_place(HashMapList* to, HashMapList* from) {
  if (from->first) {
    if (to->first) {
      to->last->next = from->first;
      to->last = from->last;
    } else {
      to->first = from->first;
      to->last = from->last;
    }
    MemZeroStruct(from);
  }
}

internal HashMapNode*
hashmap_list_pop(HashMapList* hml) {
  HashMapNode* hmn = hml->first;

  if (hml->first == hml->last) {
    hml->first = 0;
    hml->last = 0 ;
  } else {
    hml->first = hml->first->next;
  }

  return hmn;
}

internal U64
hashmap_hasher(Str8 str) {
  // return rapidhash_withSeed(str.cstr, str.size, 1987);
  return rapidhash(str.cstr, str.size);
}

HashMap*
hashmap_init(Arena* a, U64 capacity) {
  HashMap* hm = push_array(a, HashMap, 1);
  hm->capacity = capacity;
  hm->list = push_array(a, HashMapList, capacity);
  return hm;
}

Nothing
hashmap_purge(HashMap *hm) {
  hm->count = 0;

  for (U64 i = 0; i < hm->capacity; ++i) {
    hashmap_list_concat_in_place(&hm->free_list, &hm->list[i]);
  }
}

HashMapNode*
hashmap_push(Arena* a, HashMap* hm, U64 hash, HashMapKV kv) {
  HashMapNode* hmn;

  if (hm->free_list.first != 0) {
    hmn = hashmap_list_pop(&hm->free_list);
  } else {
    hmn = push_array(a, HashMapNode, 1);
  }

  hmn->next = 0;
  hmn->kv = kv;

  U64 i = hash % hm->capacity;

  if (hm->list[i].first == 0) {
    hm->list[i].first = hm->list[i].last = hmn;
    hmn->next = 0;
  } else {
    hm->list[i].last->next = hmn;
    hm->list[i].last = hmn;
    hmn->next = 0;
  }

  hm->count += 1;

  return hmn;
}

HashMapNode*
hashmap_push_str8_str8(Arena *a, HashMap* hm, Str8 key, Str8 value) {
  U64 hash = hashmap_hasher(key);
  return hashmap_push(a, hm, hash, (HashMapKV) {
    .k_str = key, .v_str = value
  });
}

HashMapNode*
hashmap_push_str8_rawptr(Arena *a, HashMap* hm, Str8 key, RawPtr value) {
  U64 hash = hashmap_hasher(key);
  return hashmap_push(a, hm, hash, (HashMapKV) {
    .k_str = key, .v_rawptr = value
  });
}

HashMapNode*
hashmap_push_str8_u32(Arena *a, HashMap* hm, Str8 key, U32 value) {
  U64 hash = hashmap_hasher(key);
  return hashmap_push(a, hm, hash, (HashMapKV) {
    .k_str = key, .v_u32 = value
  });
}

HashMapNode*
hashmap_push_str8_u64(Arena *a, HashMap* hm, Str8 key, U64 value) {
  U64 hash = hashmap_hasher(key);
  return hashmap_push(a, hm, hash, (HashMapKV) {
    .k_str = key, .v_u64 = value
  });
}


HashMapNode*
hashmap_push_u32_str8(Arena *a, HashMap* hm, U32 key, Str8 value) {
  Str8 strkey = str8_raw(&key, sizeof(key));
  U64 hash = hashmap_hasher(strkey);
  return hashmap_push(a, hm, hash, (HashMapKV) {
    .k_u32 = key, .v_str = value
  });
}

HashMapNode*
hashmap_push_u32_u32(Arena *a, HashMap* hm, U32 key, U32 value) {
  Str8 strkey = str8_raw(&key, sizeof(key));
  U64 hash = hashmap_hasher(strkey);
  return hashmap_push(a, hm, hash, (HashMapKV) {
    .k_u32 = key, .v_u32 = value
  });
}

HashMapNode*
hashmap_push_u32_u64(Arena *a, HashMap* hm, U32 key, U64 value) {
  Str8 strkey = str8_raw(&key, sizeof(key));
  U64 hash = hashmap_hasher(strkey);
  return hashmap_push(a, hm, hash, (HashMapKV) {
    .k_u32 = key, .v_u64 = value
  });
}

HashMapNode*
hashmap_push_u32_rawptr(Arena *a, HashMap* hm, U32 key, RawPtr value) {
  Str8 strkey = str8_raw(&key, sizeof(key));
  U64 hash = hashmap_hasher(strkey);
  return hashmap_push(a, hm, hash, (HashMapKV) {
    .k_u32 = key, .v_rawptr = value
  });
}

HashMapNode*
hashmap_push_u64_str8(Arena *a, HashMap* hm, U64 key, Str8 value) {
  Str8 strkey = str8_raw(&key, sizeof(key));
  U64 hash = hashmap_hasher(strkey);
  return hashmap_push(a, hm, hash, (HashMapKV) {
    .k_u64 = key, .v_str = value
  });
}

HashMapNode*
hashmap_push_u64_u32(Arena *a, HashMap* hm, U64 key, U32 value) {
  Str8 strkey = str8_raw(&key, sizeof(key));
  U64 hash = hashmap_hasher(strkey);
  return hashmap_push(a, hm, hash, (HashMapKV) {
    .k_u64 = key, .v_u32 = value
  });
}

HashMapNode*
hashmap_push_u64_u64(Arena *a, HashMap* hm, U64 key, U64 value) {
  Str8 strkey = str8_raw(&key, sizeof(key));
  U64 hash = hashmap_hasher(strkey);
  return hashmap_push(a, hm, hash, (HashMapKV) {
    .k_u64 = key, .v_u64 = value
  });
}

HashMapNode*
hashmap_push_u64_rawptr(Arena *a, HashMap* hm, U64 key, RawPtr value) {
  Str8 strkey = str8_raw(&key, sizeof(key));
  U64 hash = hashmap_hasher(strkey);
  return hashmap_push(a, hm, hash, (HashMapKV) {
    .k_u64 = key, .v_rawptr = value
  });
}

//====

HashMapKV*
hashmap_find_str8(HashMap* hm, Str8 key) {
  U64 hash = hashmap_hasher(key);
  U64 i = hash % hm->capacity;
  HashMapList* list = hm->list + i;
  for (HashMapNode *hmn = list->first; hmn != 0; hmn = hmn->next) {
    if (str8_cmp(hmn->kv.k_str, key, 0)) {
      return &hmn->kv;
    }
  }
  return 0;
}

HashMapKV*
hashmap_find_u32(HashMap* hm, U32 key) {
  Str8 strkey = str8_raw(&key, sizeof(key));
  U64 hash = hashmap_hasher(strkey);
  U64 i = hash % hm->capacity;
  HashMapList *list = hm->list + i;
  for (HashMapNode *hmn = list->first; hmn != 0; hmn = hmn->next) {
    if (hmn->kv.k_u32 == key) {
      return &hmn->kv;
    }
  }
  return 0;
}

HashMapKV*
hashmap_find_u64(HashMap* hm, U64 key) {
  Str8 strkey = str8_raw(&key, sizeof(key));
  U64 hash = hashmap_hasher(strkey);
  U64 i = hash % hm->capacity;
  HashMapList *list = hm->list + i;
  for (HashMapNode *hmn = list->first; hmn != 0; hmn = hmn->next) {
    if (hmn->kv.k_u64 == key) {
      return &hmn->kv;
    }
  }
  return 0;
}

HashMapKV*
hashmap_find_rawptr(HashMap* hm, RawPtr key) {
  Str8 strkey = str8_raw(&key, sizeof(key));
  U64 hash = hashmap_hasher(strkey);
  U64 i = hash % hm->capacity;
  HashMapList *list = hm->list + i;
  for (HashMapNode *hmn = list->first; hmn != 0; hmn = hmn->next) {
    if (hmn->kv.k_rawptr == key) {
      return &hmn->kv;
    }
  }
  return 0;
}

/* ===================================================== */
/*                          END                          */
/* ===================================================== */

#endif /* SEPI_HASHMAP_IMPLEMENTATION */
#endif /* SEPI_HASHMAP_H */

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
/*                       CONSTANTS                       */
/* ===================================================== */

#if defined(SEPI_HASHMAP_IMPLEMENTATION)
#define MODULE
#else
#define MODULE static
#endif /* SEPI_HASHMAP_IMPLEMENTATION */

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

MODULE HashMap* hashmap_init(Arena* a, U64 cap);
MODULE Nothing hashmap_purge(HashMap *hm);
MODULE HashMapNode* hashmap_push(Arena* a, HashMap* hm, U64 hash, HashMapKV kv);
MODULE HashMapNode* hashmap_push_str8(Arena *a, HashMap* hm, Str8 key,
                                      Str8 value);
MODULE HashMapNode* hashmap_push_rawptr(Arena *a, HashMap* hm, Str8 key,
                                        RawPtr value);
MODULE HashMapNode* hashmap_push_u32(Arena *a, HashMap* hm, Str8 key,
                                     U32 value);
MODULE HashMapNode* hashmap_push_u64(Arena *a, HashMap* hm, Str8 key,
                                     U64 value);
MODULE HashMapKV* hashmap_find(HashMap* hm, Str8 key);
MODULE HashMapKV hashmap_pop(HashMap* hm, Str8 key);
MODULE Str8* hashmap_keys(Arena* a, HashMap* hm);

/* ===================================================== */
/*                    IMPLEMENTATION                     */
/* ===================================================== */

#ifdef SEPI_HASHMAP_IMPLEMENTATION

MODULE Nothing
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

MODULE HashMapNode*
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

MODULE U64
hashmap_hasher(Str8 str) {
  return rapidhash_withSeed(str.cstr, str.size, 1987);
  // return rapidhash(str.cstr, str.size);
}

MODULE HashMap*
hashmap_init(Arena* a, U64 capacity) {
  HashMap* hm = arena_push_array(a, HashMap, 1);
  hm->capacity = capacity;
  hm->list = arena_push_array(a, HashMapList, capacity);
  return hm;
}

MODULE Nothing
hashmap_purge(HashMap *hm) {
  hm->count = 0;

  for (U64 i = 0; i < hm->capacity; ++i) {
    hashmap_list_concat_in_place(&hm->free_list, &hm->list[i]);
  }
}

MODULE HashMapNode*
hashmap_push(Arena* a, HashMap* hm, U64 hash, HashMapKV kv) {
  HashMapNode* hmn;

  if (hm->free_list.first != 0) {
    hmn = hashmap_list_pop(&hm->free_list);
  } else {
    hmn = arena_push_array(a, HashMapNode, 1);
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

MODULE HashMapNode*
hashmap_push_str8(Arena *a, HashMap* hm, Str8 key, Str8 value) {
  U64 hash = hashmap_hasher(key);
  return hashmap_push(a, hm, hash, (HashMapKV) {
    .k_str = key, .v_str = value
  });
}

MODULE HashMapNode*
hashmap_push_rawptr(Arena *a, HashMap* hm, Str8 key, RawPtr value) {
  U64 hash = hashmap_hasher(key);
  return hashmap_push(a, hm, hash, (HashMapKV) {
    .k_str = key, .v_rawptr = value
  });
}

MODULE HashMapNode*
hashmap_push_u32(Arena *a, HashMap* hm, Str8 key, U32 value) {
  U64 hash = hashmap_hasher(key);
  return hashmap_push(a, hm, hash, (HashMapKV) {
    .k_str = key, .v_u32 = value
  });
}

MODULE HashMapNode*
hashmap_push_u64(Arena *a, HashMap* hm, Str8 key, U64 value) {
  U64 hash = hashmap_hasher(key);
  return hashmap_push(a, hm, hash, (HashMapKV) {
    .k_str = key, .v_u64 = value
  });
}

MODULE HashMapNode*
hashmap_push_u32_str8(Arena *a, HashMap* hm, U32 key, Str8 value) {
  Str8 strkey = str8_raw(&key, sizeof(key));
  U64 hash = hashmap_hasher(strkey);
  return hashmap_push(a, hm, hash, (HashMapKV) {
    .k_u32 = key, .v_str = value
  });
}

MODULE HashMapKV*
hashmap_find(HashMap* hm, Str8 key) {
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

MODULE HashMapKV
hashmap_pop(HashMap* hm, Str8 key) {
  HashMapKV kv = {0};
  U64 hash = hashmap_hasher(key);
  U64 i = hash % hm->capacity;
  HashMapList* list = hm->list + i;
  HashMapNode *itr = list->first;
  HashMapNode *prv = itr;
  Bool single = list->first == list->last ? TRUE : FALSE;
  for (; itr != 0; prv = itr, itr = itr->next) {
    if (str8_cmp(itr->kv.k_str, key, 0)) {
      prv->next = itr->next;
      kv = itr->kv;
      MemZeroStruct(itr);
      if(single) {
        MemZeroStruct(list);
      }
      hm->count--;
      return kv;
    }
  }
  return kv;
}

MODULE Str8*
hashmap_keys(Arena* a, HashMap* hm) {
  Str8 *keys = arena_push_array_no_zero(a, Str8, hm->count);
  for (U64 i = 0, ikey = 0; i < hm->capacity; ++i) {
    for (HashMapNode *itr = hm->list[i].first; itr != 0; itr = itr->next) {
      Assert(ikey < hm->count);
      keys[ikey++] = itr->kv.k_str;
    }
  }
  return keys;
}

/* ===================================================== */
/*                          END                          */
/* ===================================================== */

#endif /* SEPI_HASHMAP_IMPLEMENTATION */
#endif /* SEPI_HASHMAP_H */

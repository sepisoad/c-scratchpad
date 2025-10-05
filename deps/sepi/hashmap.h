#ifndef SEPI_HASHMAP_H
#define SEPI_HASHMAP_H

#include "base.h"
#include "garena.h"

typedef enum HashMapResult HashMapResult;
enum HashMapResult {
  HashMapResultSuccess = 0,
  HashMapResultNotFound,
  HashMapResultUnknown,
};

typedef struct HashMapNode HashMapNode;
struct HashMapNode {
  CStr key;
  Ptr val;
  HashMapNode* next;
};

typedef struct HashMap HashMap;
struct HashMap {
  HashMapNode** buckets;
  Sz bucket_count;
  Sz entry_count;
  GArena* arena;
};

/* ===================================================== */
/*                          API                          */
/* ===================================================== */

HashMapResult hashmap_create(HashMap* hm, GArena* ga, Sz init);
HashMapResult hashmap_destroy(HashMap* hm);
HashMapResult hashmap_add(HashMap* hm, CStr key, Ptr ival);
HashMapResult hashmap_get(HashMap* hm, CStr key, Ptr oval);
HashMapResult hashmap_del(HashMap* hm, CStr key);

/* ===================================================== */
/*                    IMPLEMENTATION                     */
/* ===================================================== */

#ifdef SEPI_HASHMAP_IMPLEMENTATION

#include <string.h>

static inline Sz
hashmap_next_power_of_2(Sz n) {
  if(n==0) return 1;
  n--;
  n |= n >> 1;
  n |= n >> 2;
  n |= n >> 4;
  n |= n >> 8;
  n |= n >> 16;
  n |= n >> 32;
  return n + 1;
}

static inline U64
hashmap_hash(CStr key) {
  U64 hash = 14695981039346656037ULL;
  for(; *key; ++key) {
    hash ^= (U64)*key;
    hash *= 1099511628211ULL;
  }

  return hash;
}

HashMapResult
hashmap_create(HashMap* hm, GArena* ga, Sz init_cap) {
  notnull(hm);
  notnull(ga);
  notzero(init_cap);

  Sz cap = hashmap_next_power_of_2(init_cap);

  memset(hm, 0, sizeof(*hm));
  hm->arena = ga;
  hm->bucket_count = cap;
  hm->entry_count = 0;

  Sz sz = sizeof(HashMapNode*) * hm->bucket_count;
  hm->buckets = (HashMapNode**)garena_alloc(ga, sz, alignof(HashMapNode*));
  notnull(hm->buckets);

  memset(hm->buckets, 0, sizeof(HashMapNode*) * hm->bucket_count);
  return HashMapResultSuccess;
}

HashMapResult
hashmap_destroy(HashMap* hm) {
  return HashMapResultSuccess;
}

HashMapResult
hashmap_add(HashMap* hm, CStr key, Ptr ival) {
  return HashMapResultSuccess;
}

HashMapResult
hashmap_get(HashMap* hm, CStr key, Ptr oval) {
  notnull(hm);
  notnull(key);
  notnull(oval);

  U64 hash = hashmap_hash(key);
  Sz idx = hash % hm->bucket_count;
  HashMapNode* node = hm->buckets[idx];
  for(; node; node = node -> next) {
    if(strcmp(node->key, key) == 0) {
      /* *oval = node->val; */ /* SEPI: FIX IT */
      return HashMapResultSuccess;
    }
  }

  return HashMapResultNotFound;
}

HashMapResult
hashmap_del(HashMap* hm, CStr key) {
  return HashMapResultSuccess;
}


/* ===================================================== */
/*                          END                          */
/* ===================================================== */

#endif /* SEPI_HASHMAP_IMPLEMENTATION */
#endif /* SEPI_HASHMAP_H */

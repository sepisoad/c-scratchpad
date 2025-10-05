#ifndef SEPI_GARENA_H
#define SEPI_GARENA_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdalign.h>

#include "base.h"
#include "walloc.h"

#ifndef GARENA_FIRST_CAP
#define GARENA_FIRST_CAP (256 * 1024) /* first block ~256 KiB */
#endif

typedef enum GArenaResult GArenaResult;
enum GArenaResult {
  GArenaResultSuccess = 0,
  GArenaResultUnknown,
};

typedef struct GArenaBlock GArenaBlock;
struct GArenaBlock {
  GArenaBlock* next;
  Sz capacity;
  Sz position;
  U8 data[];
} GArenaBlock;

typedef struct GArenaMark GArenaMark;
struct GArenaMark {
  GArenaBlock* block;
  Sz position;
} GArenaMark;

typedef struct GArena GArena;
struct GArena {
  GArenaBlock* head;
  GArenaBlock* curr;
  Sz next_capacity;
} GArena;

/* ===================================================== */
/*                          API                          */
/* ===================================================== */

GArenaResult garena_create(GArena* a, Sz init_cap);
GArenaResult garena_destroy(GArena* a);
Ptr garena_alloc(GArena* a, Sz size, Sz alignment);
Ptr garena_alloc_zero(GArena* a, Sz size, Sz alignment);
GArenaMark garena_get_mark(GArena* a);
GArenaResult garena_release(GArena* a, GArenaMark m);
GArenaResult garena_reset(GArena* a);

#define garena_push_struct(a, T) \
  (T*)garena_alloc((a), (Sz)sizeof(T), (Sz)alignof(T))

#define garena_push_array(a, T, N) \
  (T*)garena_alloc((a), (Sz)(sizeof(T) * (N)), (Sz)alignof(T))

/* ===================================================== */
/*                    IMPLEMENTATION                     */
/* ===================================================== */

#ifdef SEPI_GARENA_IMPLEMENTATION

static GArenaBlock*
garena_block_create(Sz min_cap) {
  Sz capacity = (min_cap < GARENA_FIRST_CAP) ? GARENA_FIRST_CAP : min_cap;
  Sz size = sizeof(GArenaBlock) + capacity;

  GArenaBlock* b = (GArenaBlock*)w_malloc((Sz)size);
  notnull(b);

  b->next = NULL;
  b->capacity = capacity;
  b->position = 0;
  return b;
}

GArenaResult
garena_create(GArena* a, Sz init_cap) {
  notnull(a);

  memset(a, 0, sizeof(*a));
  Sz capacity = init_cap > 0 ? init_cap : (Sz)GARENA_FIRST_CAP;
  a->head = a->curr = garena_block_create(capacity);
  a->next_capacity = capacity * 2;

  return GArenaResultSuccess;
}

GArenaResult
garena_destroy(GArena* a) {
  notnull(a);

  GArenaBlock* block = a->head;
  while(block) {
    GArenaBlock* next = block->next;
    w_free(block);
    block = next;
  }
  memset(a, 0, sizeof(*a));

  return GArenaResultSuccess;
}

Ptr
garena_alloc(GArena* a, Sz size, Sz alignment) {
  notnull(a);
  notnull(a->curr);
  notzero(size);
  notzero(alignment);
  isvalid((alignment & (alignment - 1)) == 0);

  GArenaBlock* block = a->curr;
  Sz aligned = align_up(block->position, alignment);

  if(aligned <= block->capacity && size <= (block->capacity - aligned)) {
    Ptr mem = (Ptr)(block->data + aligned);
    block->position = aligned + size;
    return mem;
  }

  Sz need = size + alignment;
  Sz capacity = a->next_capacity ? a->next_capacity : (Sz)GARENA_FIRST_CAP;
  while(capacity < need)
    capacity *= 2;

  GArenaBlock* next = garena_block_create(capacity);
  notnull(next);

  block->next = next;
  a->curr = next;
  a->next_capacity = capacity * 2;

  Sz aligned2 = align_up(next->position, alignment);
  Ptr p = (Ptr)(next->data + aligned2);
  next->position = aligned2 + size;

  return p;
}

Ptr
garena_alloc_zero(GArena* a, Sz size, Sz alignment) {
  Ptr p = garena_alloc(a, size, alignment);
  if(p)
    memset(p, 0, (Sz)size);
  return p;
}

GArenaMark
garena_get_mark(GArena* a) {
  GArenaMark m = {0};
  m.block = a->curr;
  m.position = a->curr ? a->curr->position : 0;
  return m;
}

GArenaResult
garena_release(GArena* a, GArenaMark m) {
  notnull(a);
  notnull(a->head);

  if(m.block) {
    /* free everything after the marked block */
    GArenaBlock* t = m.block->next;
    m.block->next = NULL;
    while(t) {
      GArenaBlock* n = t->next;
      w_free(t);
      t = n;
    }
    a->curr = m.block;

#ifdef DEBUG_MODE
    if(a->curr->position > m.position) {
      memset(a->curr->data + m.position, 0xDD,
             (Sz)(a->curr->position - m.position));
    }
#endif /* DEBUG_MODE */

    a->curr->position = m.position;
    /* growth target can stay; it only affects future block sizes */
  } else {
    /* rewind to "empty": keep head, free rest */
    GArenaBlock* head = a->head;
    if(head) {
#ifdef DEBUG_MODE
      if(head->position)
        memset(head->data, 0xDD, (Sz)head->position);
#endif /* DEBUG_MODE */

      GArenaBlock* t = head->next;
      head->next = NULL;
      head->position = 0;
      while(t) {
        GArenaBlock* n = t->next;
        w_free(t);
        t = n;
      }
      a->curr = head;
    }
  }

  return GArenaResultSuccess;
}

GArenaResult
garena_reset(GArena* a) {
  notnull(a);
  notnull(a->head);

#ifdef DEBUG_MODE
  /* poison used bytes in all blocks */
  for(GArenaBlock* b = a->head; b; b = b->next) {
    if(b->position)
      memset(b->data, 0xDD, (Sz)b->position);
  }
#endif /* DEBUG_MODE */

  /* keep the first block; free others */
  GArenaMark m = {a->head, 0};
  garena_release(a, m);

  return GArenaResultSuccess;
}

/* ===================================================== */
/*                          END                          */
/* ===================================================== */

#endif /* SEPI_GARENA_IMPLEMENTATION */
#endif /* SEPI_GARENA_H */

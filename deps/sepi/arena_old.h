#ifndef SEPI_ARENA_OLD_H
#define SEPI_ARENA_OLD_H

#include "base.h"
#include "walloc.h"

typedef struct ArenaOld {
  Ptr memory;
  Sz size;
  Sz offset;
  Sz estimation;
  Sz max_align;
  Bool estimating;
#ifdef DEBUG_MODE
  Sz user_requested_size;
#endif /* DEBUG_MODE */
} ArenaOld;

/* ===================================================== */
/*                          API                          */
/* ===================================================== */

#define ARENA_DEFAULT_ALIGNMENT NATIVE_ALIGNMENT
#define ARENA_MIN_ALIGNMENT 8
#define ARENA_MAX_ALIGNMENT 128

void arena_old_create(ArenaOld* a, Sz size);
void arena_old_destroy(ArenaOld* a);
Ptr arena_old_alloc(ArenaOld* a, Sz size, Sz alignment);
void arena_old_reset(ArenaOld* a);
void arena_old_estimate_begin(ArenaOld* a);
void arena_old_estimate_add(ArenaOld* a, Sz size, Sz alignment);
void arena_old_estimate_end(ArenaOld* a);

#define arena_old_push_struct(A, T) \
  (T*)arena_old_alloc((A), (Sz)sizeof(T), (Sz)ALIGNOF(T))

#define arena_old_push_array(A, T, N) \
  (T*)arena_old_alloc((A), (Sz)(sizeof(T) * (N)), (Sz)ALIGNOF(T))

#define arena_old_talloc(A, T) arena_old_alloc((A), (Sz)sizeof(T), (Sz)ALIGNOF(T))
#define arena_old_testimate_add(A, T) \
  arena_old_estimate_add((A), (Sz)sizeof(T), (Sz)ALIGNOF(T))

/* ===================================================== */
/*                    IMPLEMENTATION                     */
/* ===================================================== */

#ifdef SEPI_ARENA_OLD_IMPLEMENTATION

void arena_old_create(ArenaOld* a, Sz size) {
  notnull(a);
  notzero(size);
  makesure(a->memory == NULL, "ArenaOld is already allocated");

  Ptr memory = w_calloc(1, size);
  notnull(memory);

  a->memory = memory;
  a->size = size;
  a->offset = 0;
  a->estimating = FALSE;
  a->estimation = 0;
  a->max_align = ARENA_DEFAULT_ALIGNMENT;

#ifdef DEBUG_MODE
  a->user_requested_size = size;
#endif /* DEBUG_MODE */
}

void arena_old_destroy(ArenaOld* a) {
  notnull(a);

  if(a->memory)
    w_free(a->memory);

  a->memory = NULL;
  a->size = 0;
  a->offset = 0;
  a->estimating = FALSE;
  a->estimation = 0;
  a->max_align = 0;

#ifdef DEBUG_MODE
  a->user_requested_size = 0;
#endif /* DEBUG_MODE */
}

Ptr arena_old_alloc(ArenaOld* a, Sz size, Sz alignment) {
  notnull(a);
  notnull(a->memory);
  notzero(size);
  notzero(alignment);
  isvalid(is_pow2(alignment));

  UPtr base = (UPtr)a->memory;
  UPtr cur = base + a->offset;
  UPtr aptr = align_up(cur, alignment); /* align_up takes integers */
  isvalid(aptr >= base);

  Sz used = (Sz)(aptr - base);
  isvalid(used <= a->size);
  isvalid(size <= a->size - used);

  a->offset = used + size;
  if(a->max_align < alignment)
    a->max_align = alignment;

  return (Ptr)aptr;
}

void arena_old_reset(ArenaOld* a) {
  notnull(a);
  notnull(a->memory);

  a->offset = 0;
  a->estimating = FALSE;

#ifdef DEBUG_MODE
  memset(a->memory, 0, a->size);
  a->user_requested_size = 0;
#endif /* DEBUG_MODE */
}

void arena_old_estimate_begin(ArenaOld* a) {
  notnull(a);
  isvalid(a->memory == NULL);

  a->memory = NULL;
  a->size = 0;
  a->offset = 0;
  a->estimating = TRUE;
  a->estimation = 0;

#ifdef DEBUG_MODE
  a->user_requested_size = 0;
#endif /* DEBUG_MODE */
}

void arena_old_estimate_add(ArenaOld* a, Sz size, Sz alignment) {
  notnull(a);
  notzero(size);
  notzero(alignment);
  isvalid(is_pow2(alignment));

  a->estimation += size;
  if(a->max_align < alignment) {
    a->max_align = alignment;
  }

#ifdef DEBUG_MODE
  a->user_requested_size += size;
#endif /* DEBUG_MODE */
}

void arena_old_estimate_end(ArenaOld* a) {
  notnull(a);
  notzero(a->estimating);
  notzero(a->max_align);

  Sz size = align_up(a->estimation, a->max_align);
  Ptr memory = w_calloc(1, size);
  notnull(memory);

  a->memory = memory;
  a->size = size;
  a->offset = 0;
  a->estimating = FALSE;
  a->estimation = 0;

#ifdef DEBUG_MODE
  printf("============== arena_old.h ==============\n");
  printf(" user requested size: %zu\n", a->user_requested_size);
  printf(" real allocated size: %zu\n", a->size);
  printf("=========================================\n");
#endif /* DEBUG_MODE */
}

/* ===================================================== */
/*                          END                          */
/* ===================================================== */

#endif /* SEPI_ARENA_OLD_IMPLEMENTATION */
#endif /* SEPI_ARENA_OLD_H */

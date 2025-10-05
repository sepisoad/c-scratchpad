#ifndef SEPI_ARENA_H
#define SEPI_ARENA_H

/* ===================================================== */
/*                     DEPENDENCIES                      */
/* ===================================================== */

#include "base.h"
#include "platform.h"

/* ===================================================== */
/*                       CONSTANTS                       */
/* ===================================================== */

#define ARENA_DEFAULT_RESERVE_SIZE MB(64)
#define ARENA_DEFAULT_COMMIT_SIZE MB(64)

/* ===================================================== */
/*                         TYPES                         */
/* ===================================================== */

typedef struct ArenaParams ArenaParams;
struct ArenaParams {
  U64 reserve_size;
  U64 commit_size;
#ifdef DEBUG_MODE
  CStr caller_file_name;
  U32 caller_file_line;
#endif /* DEBUG_MODE */
};

typedef struct Arena Arena;
struct Arena {
  Arena* prev;
  Arena* current;
  Arena* free_last;
  U64 commit_size;
  U64 reserve_size;
  U64 base_position;
  U64 position;
  U64 commit;
  U64 reserve;
#ifdef DEBUG_MODE
  CStr caller_file_name;
  U32 caller_file_line;
#endif /* DEBUG_MODE */
};

typedef struct Scratch Scratch;
struct Scratch {
  Arena* arena;
  U64 position;
};

/* ===================================================== */
/*                          API                          */
/* ===================================================== */

Arena* arena_alloc_(ArenaParams* ap);
Nothing arena_release(Arena* a);
RawPtr arena_push(Arena*, U64 size, U64 align, Bool with_zero);
Nothing arena_pop(Arena*, U64 amount);
Nothing arena_pop_to(Arena* a, U64 position);
Nothing arena_clear(Arena*);
U64 arena_get_position(Arena* a);
Scratch scratch_begin(Arena* a);
Nothing scratch_end(Scratch s);

#ifdef DEBUG_MODE
#define arena_alloc(...) arena_alloc_(&(ArenaParams){.reserve_size = ARENA_DEFAULT_RESERVE_SIZE, .commit_size = ARENA_DEFAULT_COMMIT_SIZE, .caller_file_name = __FILE__, .caller_file_line = __LINE__, __VA_ARGS__})
#else
#define arena_alloc(...) arena_alloc_(&(ArenaParams){.reserve_size = ARENA_DEFAULT_RESERVE_SIZE, .commit_size = ARENA_DEFAULT_COMMIT_SIZE, __VA_ARGS__})
#endif /* DEBUG_MODE */

#define push_array_no_zero_aligned(arena, type, count, alignment) (type *)arena_push((arena), sizeof(type) * (count), (alignment), (True))
#define push_array_aligned(arena, type, count, alignment) (type *)arena_push((arena), sizeof(type) * (count), (alignment), (False))
#define push_array_no_zero(arena, type, count, alignment) push_array_no_zero_aligned(arena, type, count, Max(8, AlignOf(type)))
#define push_array(arena, type, count, alignment) push_array_aligned(arena, type, count, Max(8, AlignOf(type)))

/* ===================================================== */
/*                    IMPLEMENTATION                     */
/* ===================================================== */

#ifdef SEPI_ARENA_IMPLEMENTATION

Arena*
arena_alloc_(ArenaParams* ap) {
  /* U32 const cpu_cores = platform_get_cpu_cores(); */
  /* U64 const page_size = platform_get_page_size(); */
  U64 const large_page_size = platform_get_large_page_size();

  /* TODO: this uses large pages size by default */
  U64 reserve_size = AlignUp(ap->reserve_size, large_page_size);
  U64 commit_size = AlignUp(ap->commit_size, large_page_size);

  RawPtr base = platform_reserve_large_pages(reserve_size);
  platform_commit_large_pages(base, commit_size);

  if(!base) {
    Abort("failed to allocate memory to arena allocator");
  }

  Sz sz = sizeof(Arena);
  Arena* a = (Arena*)base;
  a->current = a;
  a->free_last = 0;
  a->commit_size = ap->commit_size;
  a->reserve_size = ap->reserve_size;
  a->base_position = 0;
  a->position = sz;
  a->commit = commit_size;
  a->reserve = reserve_size;
#ifdef DEBUG_MODE
  a->caller_file_name = ap->caller_file_name;
  a->caller_file_line = ap->caller_file_line;
#endif /* DEBUG_MODE */

  AsanPoisonMemoryRegion(base, commit_size);
  AsanUnpoisonMemoryRegion(base, sz);
  return a;
}

Nothing
arena_release(Arena* a) {
  for(Arena* it = a->current, *prev = 0; it != NULL; it = prev) {
    prev = it->prev;
    platform_release(it, it->reserve);
  }
}

// TODO: implement the following functions

RawPtr
arena_push(Arena* a, U64 size, U64 align, Bool with_zero) {
  Ignore(a);
  Ignore(size);
  Ignore(align);
  Ignore(with_zero);
  return NULL;
}

Nothing
arena_pop(Arena* a, U64 amount) {
  Ignore(a);
  Ignore(amount);
}

Nothing
arena_pop_to(Arena* a, U64 position) {
  Ignore(a);
  Ignore(position);
}

Nothing
arena_clear(Arena* a) {
  Ignore(a);
}

U64
arena_get_position(Arena* a) {
  Ignore(a);
  return 0;
}

Scratch
scratch_begin(Arena* a) {
  Ignore(a);
  return (Scratch) {
    0
  };
}

Nothing
scratch_end(Scratch s) {
  Ignore(s);
}

/* ===================================================== */
/*                          END                          */
/* ===================================================== */

#endif /* SEPI_ARENA_IMPLEMENTATION */
#endif /* SEPI_ARENA_H */

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
  U64 requested_reserve_size;
  U64 requested_commit_size;
#ifdef DEBUG_MODE
  CStr caller_file_name;
  U32 caller_file_line;
#endif /* DEBUG_MODE */
};

typedef struct Arena Arena;
struct Arena {
  Arena* previous;
  Arena* current;
  Arena* free_last;

  U64 requested_commit_size;
  U64 committed_size;

  U64 requested_reserve_size;
  U64 reserved_size;

  U64 base_position;
  U64 position;

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
#define arena_alloc(...) arena_alloc_(&(ArenaParams){.requested_reserve_size = ARENA_DEFAULT_RESERVE_SIZE, .requested_commit_size = ARENA_DEFAULT_COMMIT_SIZE, .caller_file_name = __FILE__, .caller_file_line = __LINE__, __VA_ARGS__})
#else
#define arena_alloc(...) arena_alloc_(&(ArenaParams){.requested_reserve_size = ARENA_DEFAULT_RESERVE_SIZE, .requested_commit_size = ARENA_DEFAULT_COMMIT_SIZE, __VA_ARGS__})
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
  // TODO: uncomment this
  // U64 const large_page_size = platform_get_large_page_size();
  U64 const large_page_size = 4;

  // TODO: this uses large pages size by default
  U64 requested_reserve_size = AlignUp(ap->requested_reserve_size, large_page_size);
  U64 requested_commit_size = AlignUp(ap->requested_commit_size, large_page_size);

  RawPtr base = platform_reserve_large_pages(requested_reserve_size);
  platform_commit_large_pages(base, requested_commit_size);

  if(!base) {
    Abort("failed to allocate memory to arena allocator");
  }

  Sz arena_header_size = sizeof(Arena);
  Arena* a = (Arena*)base;

  a->current = a;
  a->free_last = 0;

  a->requested_reserve_size = ap->requested_reserve_size;
  a->reserved_size = requested_reserve_size;

  a->requested_commit_size = ap->requested_commit_size;
  a->committed_size = requested_commit_size;

  a->base_position = 0;
  a->position = arena_header_size;

#ifdef DEBUG_MODE
  a->caller_file_name = ap->caller_file_name;
  a->caller_file_line = ap->caller_file_line;
#endif /* DEBUG_MODE */

  AsanPoisonMemoryRegion(base, requested_commit_size);
  AsanUnpoisonMemoryRegion(base, arena_header_size);
  return a;
}

Nothing
arena_release(Arena* a) {
  for(Arena* it = a->current, *previous = 0; it != 0; it = previous) {
    previous = it->previous;
    platform_release(it, it->reserved_size);
  }
}

// TODO: review this function
RawPtr
arena_push(Arena* a, U64 size, U64 align, Bool with_zero) {
  Arena* current_block = a->current;
  U64 position_aligned = AlignUp(current_block->position, align);
  U64 position_aligned_sized = position_aligned + size;

  if(current_block->reserved_size < position_aligned_sized) {
    Arena* new_block = 0;
    Arena* previous_block;

    for(new_block = a->free_last, previous_block = 0; new_block != 0; previous_block = new_block, new_block = new_block->previous) {
      if(new_block->reserved_size >= AlignUp(new_block->position, align) + size) {
        if(previous_block) {
          previous_block->previous = new_block->previous;
        } else {
          a->free_last = new_block->previous;
        }
        break;
      }
    }


    if(new_block == 0) {
      Sz tsize = sizeof(Arena);
      U64 requested_reserve_size = current_block->requested_reserve_size;
      U64 requested_commit_size = current_block->requested_commit_size;
      if(size + tsize > requested_reserve_size) {
        requested_reserve_size = AlignUp(size + tsize, align);
        requested_commit_size = AlignUp(size + tsize, align);
      }
      new_block = arena_alloc(.requested_reserve_size = requested_reserve_size,
                              .requested_commit_size = requested_commit_size
#ifdef DEBUG_MODE
                                ,
                              .caller_file_name = current_block->caller_file_name,
                              .caller_file_line = current_block->caller_file_line
#endif /* DEBUG_MODE */
                             );
    }

    new_block->base_position = current_block->base_position + current_block->reserved_size;
    new_block->previous = a->current;
    a->current = new_block;
    current_block = new_block;
    position_aligned = AlignUp(current_block->position, align);
    position_aligned_sized = position_aligned + size;
  }

  U64 size_to_zero = 0;
  if(with_zero) {
    size_to_zero = Min(current_block->committed_size, position_aligned_sized) - position_aligned;
  }

  if(current_block->committed_size < position_aligned_sized) {
    U64 new_commit_size = position_aligned_sized + current_block->requested_commit_size - 1;
    new_commit_size -= new_commit_size % current_block->requested_commit_size;
    U64 commit_size_clamped = Max(new_commit_size, current_block->reserved_size);
    U64 needed_commit_size = commit_size_clamped - current_block->committed_size;
    U8* committed_size_ptr = (U8*)current_block + current_block->committed_size;
    platform_commit_large_pages(committed_size_ptr, needed_commit_size);
    current_block->committed_size = commit_size_clamped;
  }

  RawPtr result = 0;
  if(current_block->committed_size >= position_aligned_sized) {
    result = (U8*)current_block + position_aligned;
    current_block->position = position_aligned_sized;
    AsanUnpoisonMemoryRegion(result, size);
    if(size_to_zero != 0) {
      SetMem0(result, size_to_zero);
    }
  }

  if(result == 0) {
    Abort("failed to allocate memory from arena allocator");
  }

  return result;
}

// TODO: implement this function
Nothing
arena_pop(Arena* a, U64 amount) {
  Ignore(a);
  Ignore(amount);
}

// TODO: implement this function
Nothing
arena_pop_to(Arena* a, U64 position) {
  Ignore(a);
  Ignore(position);
}

// TODO: implement this function
Nothing
arena_clear(Arena* a) {
  Ignore(a);
}

// TODO: implement this function
U64
arena_get_position(Arena* a) {
  Ignore(a);
  return 0;
}

// TODO: implement this function
Scratch
scratch_begin(Arena* a) {
  Ignore(a);
  return (Scratch) {
    0
  };
}

// TODO: implement this function
Nothing
scratch_end(Scratch s) {
  Ignore(s);
}

/* ===================================================== */
/*                          END                          */
/* ===================================================== */

#endif /* SEPI_ARENA_IMPLEMENTATION */
#endif /* SEPI_ARENA_H */

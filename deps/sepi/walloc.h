#ifndef SEPI_DBGALLOC_H
#define SEPI_DBGALLOC_H

#include "base.h"

#if defined(DEBUG_MODE) && defined(USE_MEM_DEBUGGER)

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ====================================================== */
/*                          API                           */
/* ====================================================== */

PTR dbg_malloc(SZ, CSTR, I32);
PTR dbg_calloc(SZ, SZ, CSTR, I32);
PTR dbg_realloc(PTR, SZ, CSTR, I32);
void dbg_free(PTR, CSTR, I32);

#define w_malloc(n) dbg_malloc((n), __FILE__, __LINE__)
#define w_calloc(n, m) dbg_calloc((n), (m), __FILE__, __LINE__)
#define w_realloc(p, n) dbg_realloc((p), (n), __FILE__, __LINE__)
#define w_free(p) dbg_free((p), __FILE__, __LINE__)

#else /* release version */

#define w_malloc(n) malloc(n)
#define w_calloc(n, m) calloc(n, m)
#define w_realloc(p, n) realloc(p, n)
#define w_free(p) free(p)

#endif /* DEBUG_MODE && USE_MEM_DEBUGGER */

/* ====================================================== */
/*                      IMPLEMENTATION                    */
/* ====================================================== */

#if defined(SEPI_DBGALLOC_IMPLEMENTATION)

#if defined(DEBUG_MODE) && defined(USE_MEM_DEBUGGER)

#include <stdlib.h>
#include <stdatomic.h>

#define DBGALLOC_BUCKETS_SIZE 4096u

typedef struct DBGNode {
  PTR ptr;
  SZ size;
  U64 id;
  I32 line;
  CSTR file;
  struct DBGNode* next;
} DBGNode;

typedef struct {
  DBGNode* buckets[DBGALLOC_BUCKETS_SIZE];
  SZ current_size;
  SZ peak_size;
  U64 alloc_call_count;
  U64 free_call_count;
  U64 realloc_call_count;
  BOOL installed;
} DBGState;

static DBGState g_state = {0};

/* === HELPER FUNCTIONS === */

static void dbg_report(void) {
  printf("===== xalloc summary =====\n");
  printf("current  : %zu bytes, peak: %zu bytes\n", g_state.current_size,
         g_state.peak_size);
  printf("allocs   : %llu\nfrees    : %llu\nreallocs : %llu\n",
         (unsigned long long)g_state.alloc_call_count,
         (unsigned long long)g_state.free_call_count,
         (unsigned long long)g_state.realloc_call_count);

  SZ leaks = 0, leaked_bytes = 0;
  for(u32 i = 0; i < DBGALLOC_BUCKETS_SIZE; ++i) {
    for(DBGNode* n = g_state.buckets[i]; n; n = n->next) {
      ++leaks;
      leaked_bytes += n->size;
    }
  }
  if(leaks == 0) {
    printf("leaks    : NONE!\n");
  } else {
    printf("leaks    : SHIT! %zu blocks, %zu bytes\n", leaks, leaked_bytes);
    for(u32 i = 0; i < DBGALLOC_BUCKETS_SIZE; ++i) {
      for(DBGNode* node = g_state.buckets[i]; node != NULL;
          node = node->next) {
        printf("  leak #%llu: ptr=%p size=%zu @ %s:%d\n", (U64)node->id,
               node->ptr, node->size, node->file, node->line);
      }
    }
  }
  printf("==========================\n");
}

static inline I64 dbg_find_ptr_slot(PTR ptr) {
  for(I32 i = 0; i < DBGALLOC_BUCKETS_SIZE; i++) {
    DBGNode* node = g_state.buckets[i];
    if(!node)
      continue;
    if(node->ptr == ptr) {
      return i;
    }
  }

  return -1;
}

static inline u32 dbg_find_free_slot() {
  for(I32 i = 0; i < DBGALLOC_BUCKETS_SIZE; i++) {
    DBGNode* node = g_state.buckets[i];
    if(!node)
      return i;

    notnull(node->ptr);
  }

  mustdie("xalloc free bucket slots are axhausted!");
}

static void dbg_at_exit(void) {
  dbg_report();
}

static void dbg_install(void) {
  if(!g_state.installed) {
    memset(&g_state, 0, sizeof(g_state));
    g_state.installed = true;
    atexit(dbg_at_exit);
  }
}

static void dbg_add_node(PTR ptr, SZ size, CSTR file, I32 line) {
  notnull(ptr);
  isvalid(dbg_find_ptr_slot(ptr) < 0);

  u32 slot = dbg_find_free_slot();
  DBGNode* node = (DBGNode*)malloc(sizeof(DBGNode));
  notnull(node);

  node->ptr = ptr;
  node->size = size;
  node->file = file;
  node->line = line;

  g_state.buckets[slot] = node;
  g_state.current_size += size;
  g_state.alloc_call_count++;

  if(g_state.current_size > g_state.peak_size)
    g_state.peak_size = g_state.current_size;
}

static void dbg_remove_node(PTR ptr, CSTR file, I32 line) {
  notnull(ptr);

  I64 slot = dbg_find_ptr_slot(ptr);
  isvalid(slot >= 0);

  DBGNode* node = g_state.buckets[slot];
  notnull(node);

  free(node->ptr);

  if(g_state.current_size >= node->size)
    g_state.current_size -= node->size;
  else
    g_state.current_size = 0;

  g_state.free_call_count++;
  g_state.buckets[slot] = NULL;
}

static void dbg_resize(PTR old_ptr,
                       PTR new_ptr,
                       SZ new_size,
                       CSTR file,
                       I32 line) {
  if(old_ptr == NULL && new_ptr) {
    dbg_add_node(new_ptr, new_size, file, line);
    return;
  }

  if(new_ptr == NULL) {
    dbg_remove_node(old_ptr, file, line);
    return;
  }

  I64 slot = dbg_find_ptr_slot(old_ptr);
  isvalid(slot >= 0);

  DBGNode* node = g_state.buckets[slot];
  notnull(node);

  if(new_size >= node->size) {
    SZ delta = new_size - node->size;

    g_state.current_size += delta;
    if(g_state.current_size > g_state.peak_size)
      g_state.peak_size = g_state.current_size;
  } else {
    SZ delta = node->size - new_size;

    if(g_state.current_size >= delta)
      g_state.current_size -= delta;
    else
      g_state.current_size = 0;
  }

  node->ptr = new_ptr;
  node->size = new_size;
  node->file = file;
  node->line = line;
}

/* === APIS === */

PTR dbg_malloc(SZ size, CSTR file, I32 line) {
  dbg_install();
  PTR ptr = malloc(size);
  dbg_add_node(ptr, size, file, line);
  return ptr;
}

PTR dbg_calloc(SZ unit_size, SZ count, CSTR file, I32 line) {
  dbg_install();
  PTR ptr = calloc(unit_size, count);
  dbg_add_node(ptr, unit_size * count, file, line);
  return ptr;
}

PTR dbg_realloc(PTR old_ptr, SZ size, CSTR file, I32 line) {
  dbg_install();
  PTR new_ptr = realloc(old_ptr, size);
  dbg_resize(old_ptr, new_ptr, size, file, line);
  return new_ptr;
}

void dbg_free(PTR ptr, CSTR file, I32 line) {
  dbg_install();
  dbg_remove_node(ptr, file, line);
}

/* ===================================================== */
/*                          END                          */
/* ===================================================== */

#endif /* SEPI_DBGALLOC_IMPLEMENTATION */
#endif /* DEBUG_MODE && USE_MEM_DEBUGGER */
#endif /* SEPI_DBGALLOC_H */

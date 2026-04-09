#define SEPI_ARENA_IMPLEMENTATION

#include <sepi/arena.h>
#include <sepi/base.h>
#include <sepi/platform.h>

#include <math.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define MAX_ARRAY_LENGTH 1000000000

typedef struct {
  U64 *array;
  U64 length;
} SumContext;

typedef struct {
  U8 thread_id;
  RawPtr data;
  pthread_t *thread;
} ThreadContext;

F64 get_elapsed_ms(struct timespec start, struct timespec end) {
  return (F64)(end.tv_sec - start.tv_sec) * 1000.0 +
         (F64)(end.tv_nsec - start.tv_nsec) / 1000000.0;
}

U64 *generate_array_of_numbers(Arena *arena) {
  U64 *arr =
      arena_push(arena, sizeof(U64) * MAX_ARRAY_LENGTH, alignof(U64), FALSE);

  for (U64 index = 0; index < MAX_ARRAY_LENGTH; index++) {
    arr[index] = index;
  }

  return arr;
}

RawPtr sum_numbers(RawPtr input) {
  ThreadContext tctx = *(ThreadContext *)input;
  SumContext sctx = *(SumContext *)tctx.data;

  U64 sum = 0;
  for (U64 index = 0; index < sctx.length; index++) {
    sum += sctx.array[index];
  }

  return (RawPtr)(uintptr_t)sum;
}

U64 sum_numbers_simple(U64* arr, U64 length) {
  U64 sum = 0;
  for (U64 index = 0; index < length; index++) {
    sum += arr[index];
  }
  return sum;
}

RawPtr parallel_entry_point(RawPtr input) {


}

int main() {
  Arena *arena = arena_create();
  U64 cpu_cores = platform_get_cpu_cores();
  U64 *big_array = generate_array_of_numbers(arena);
  pthread_t *threads =
      arena_push(arena, sizeof(pthread_t) * cpu_cores, alignof(pthread_t), TRUE);

  struct timespec start;
  struct timespec end;

  clock_gettime(CLOCK_MONOTONIC, &start);

  U64 regular_items_count = floor(MAX_ARRAY_LENGTH / cpu_cores);
  U64 last_items_count =
      MAX_ARRAY_LENGTH - (regular_items_count * (cpu_cores - 1));

  U64 index = 0;
  for (; index < cpu_cores - 1; index++) {
    SumContext *sctx =
        arena_push(arena, sizeof(SumContext), alignof(SumContext), FALSE);
    ThreadContext *tctx =
        arena_push(arena, sizeof(ThreadContext), alignof(ThreadContext), FALSE);

    U64 items =
        (index == cpu_cores - 1) ? last_items_count : regular_items_count;

    sctx->array = big_array + (index * regular_items_count);
    sctx->length = items;

    tctx->thread_id = index;
    tctx->data = sctx;
    tctx->thread = threads + index;

    // 3. Pass the pointer to the ARENA-allocated memory
    pthread_create(threads + index, 0, sum_numbers, tctx);
  }

  SumContext sctx = {.array = big_array + (index * regular_items_count),
                     .length = last_items_count};
  ThreadContext tctx = {
      .thread_id = index, .thread = threads + index, .data = (RawPtr)&sctx};
  pthread_create(threads + index, 0, sum_numbers, (RawPtr)(uintptr_t)&tctx);

  U64 total_sum = 0;
  for (U64 index = 0; index < cpu_cores; index++) {
    RawPtr opaque_value;
    pthread_join(threads[index], &opaque_value);
    U64 current_sum = (U64)(uintptr_t)opaque_value;
    total_sum += current_sum;
  }

  dbg("%llu", total_sum);

  clock_gettime(CLOCK_MONOTONIC, &end);
  double elapsed = get_elapsed_ms(start, end);
  dbg("Calculation took: %.3f ms", elapsed);

  //=================================================

  // clock_gettime(CLOCK_MONOTONIC, &start);
  // total_sum = sum_numbers_simple(big_array, MAX_ARRAY_LENGTH);

  // dbg("%llu", total_sum);

  // clock_gettime(CLOCK_MONOTONIC, &end);
  // elapsed = get_elapsed_ms(start, end);
  // dbg("Calculation took: %.3f ms", elapsed);

  return 0;
}

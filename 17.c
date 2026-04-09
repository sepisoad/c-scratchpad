#define SEPI_ARENA_IMPLEMENTATION

#include <math.h>
#include <pthread.h>
#include <unistd.h>

#include <sepi/arena.h>
#include <sepi/base.h>
#include <sepi/platform.h>

#define MAX_ARRAY_LENGTH 1000000000

typedef struct {
  U64 *array;
  U64 length;
} SumContext;

typedef struct {
  U8 thread_id;
  pthread_t *thread;
} ThreadContext;

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int count;
    int trip_count;
} sepi_barrier_t;

void barrier_init(sepi_barrier_t *b, int n) {
    pthread_mutex_init(&b->mutex, NULL);
    pthread_cond_init(&b->cond, NULL);
    b->count = 0;
    b->trip_count = n;
}

void barrier_wait(sepi_barrier_t *b) {
    pthread_mutex_lock(&b->mutex);
    b->count++;
    if (b->count >= b->trip_count) {
        b->count = 0;
        pthread_cond_broadcast(&b->cond);
    } else {
        pthread_cond_wait(&b->cond, &b->mutex);
    }
    pthread_mutex_unlock(&b->mutex);
}

thread_local sepi_barrier_t barrier = {0};

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

U64 sum_numbers(U64 *arr, U64 length) {
  U64 sum = 0;
  for (U64 index = 0; index < length; index++) {
    sum += arr[index];
  }
  return sum;
}

RawPtr parallel_entry_point(RawPtr input) {
  ThreadContext tctx = *(ThreadContext *)input;

  barrier_wait(&barrier);

  return 0;
}

int main() {
  Arena *arena = arena_create();
  U32 cpu_cores = platform_get_cpu_cores();
  U32 worker_threads_count = cpu_cores - 1;
  pthread_t *threads = arena_push(arena, sizeof(pthread_t) * worker_threads_count,
                                  alignof(pthread_t), TRUE);
  barrier_init(&barrier, worker_threads_count);

  for (U64 index = 0; index < worker_threads_count; index++) {
    ThreadContext *tctx =
        arena_push(arena, sizeof(ThreadContext), alignof(ThreadContext), FALSE);
    pthread_create(threads + index, 0, parallel_entry_point, tctx);
  }

  for (U64 index = 0; index < worker_threads_count; index++) {
    pthread_join(threads[index], 0);
  }

  dbg("DONE!");

  PlatformInfo pi = platform_get_info();

  dbg("os: %d", pi.os);
  dbg("cpu: %d", pi.cpu);
  dbg("cpu_cores: %d", pi.cpu_cores);

  return 0;
}

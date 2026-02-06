#include <stdio.h>

#define SEPI_PLATFORM_IMPLEMENTATION
#define SEPI_ARENA_IMPLEMENTATION
#define SEPI_STRING_IMPLEMENTATION

#include <sepi/arena.h>
#include <sepi/base.h>
#include <sepi/string.h>

#define ARRAY_MIN_SEGMENT 6
#define ARRAY_MAX_SEGMENT 26
#define ARRAY_64_BITS 8 * sizeof(U64)

#define ARRAY_SEGMENT_CAPACITY(SEG)                                            \
  ((1 << ARRAY_MIN_SEGMENT) << (SEG)) - (1 << ARRAY_MIN_SEGMENT)
#define ARRAY_GET_IDX_SEGMENT(VAL) ((U32)(ARRAY_64_BITS - LeadingZeroBits(VAL)))
#define ARRAY_GET_IDX_SEGMENT_NORMALIZED(IDX)                                  \
  ARRAY_GET_IDX_SEGMENT(((IDX) >> ARRAY_MIN_SEGMENT) + 1)

#define DefineArray(TYPE, FIELD)                                               \
  typedef struct Array##TYPE Array##TYPE;                                      \
  struct Array##TYPE {                                                         \
    Arena *arena;                                                              \
    U64 capacity;                                                              \
    U64 offset;                                                                \
    U64 used_segments;                                                         \
    struct {                                                  1                 \
      Sz size;                                                                 \
      Sz alignment;                                                            \
    } element;                                                                 \
    RawPtr segments[ARRAY_MAX_SEGMENT];                                        \
  };                                                                           \
                                                                               \
  Array##TYPE *array_##FIELD##_create(Arena *arena) {                          \
    START_PROFILING(1);                                                        \
                                                                               \
    Assert(arena != 0);                                                        \
                                                                               \
    Array##TYPE *array =                                                       \
        arena_push(arena, sizeof(Array##TYPE), AlignOf(Array##TYPE), TRUE);    \
                                                                               \
    array->arena = arena;                                                      \
    array->used_segments = 1;                                                  \
    array->capacity = ARRAY_SEGMENT_CAPACITY(array->used_segments);            \
    array->offset = 0;                                                         \
    array->element.size = sizeof(TYPE);                                        \
    array->element.alignment = AlignOf(TYPE);                                  \
                                                                               \
    array->segments[array->used_segments - 1] =                                \
        arena_push(arena, array->element.size * array->capacity,               \
                   array->element.alignment, TRUE);                            \
                                                                               \
    END_PROFILING();                                                           \
    return array;                                                              \
  }                                                                            \
                                                                               \
  TYPE *array_##FIELD##_get(Array##TYPE *array, U64 index) {                   \
    START_PROFILING(1);                                                        \
                                                                               \
    Assert(array != 0);                                                        \
    Assert(index < array->capacity);                                           \
                                                                               \
    TYPE *res = 0;                                                             \
    U64 seg = ARRAY_GET_IDX_SEGMENT_NORMALIZED(index);                         \
    U64 base = (seg > 1) ? ARRAY_SEGMENT_CAPACITY(seg - 1) : 0;                \
    U64 slot = index - base;                                                   \
    res = array->segments[seg - 1] + (array->element.size * slot);             \
                                                                               \
    END_PROFILING();                                                           \
    return res;                                                                \
  }                                                                            \
                                                                               \
  TYPE *array_##FIELD##_push(Array##TYPE *array, TYPE *ptr) {                  \
    START_PROFILING(1);                                                        \
                                                                               \
    Assert(array != 0);                                                        \
    Assert(ptr != 0);                                                          \
                                                                               \
    if (array->offset >= (U64)ARRAY_SEGMENT_CAPACITY(array->used_segments)) {  \
      U64 old_cap = ARRAY_SEGMENT_CAPACITY(array->used_segments);              \
      U64 new_cap = ARRAY_SEGMENT_CAPACITY(array->used_segments + 1);          \
      U64 seg_size = new_cap - old_cap;                                        \
                                                                               \
      array->segments[array->used_segments] =                                  \
          arena_push(array->arena, array->element.size * seg_size,             \
                     array->element.alignment, TRUE);                          \
                                                                               \
      array->used_segments++;                                                  \
      array->capacity = ARRAY_SEGMENT_CAPACITY(array->used_segments);          \
    }                                                                          \
                                                                               \
    RawPtr res = array_##FIELD##_get(array, array->offset);                    \
    MemCopy(res, ptr, array->element.size);                                    \
    array->offset++;                                                           \
                                                                               \
    END_PROFILING();                                                           \
    return res;                                                                \
  }

typedef struct {
  Str8 name;
  U32 age;
} Employee;

DefineArray(Employee, employee);

int main() {
  Arena *arena = arena_create();
  ArrayEmployee *array = array_employee_create(arena);

  Employee e1 = {.name = S("sepi"), .age = 38};
  Employee e2 = {.name = S("matilde"), .age = 27};
  Employee e3 = {.name = S("eden"), .age = 38};

  array_employee_push(array, &e1);
  array_employee_push(array, &e2);
  array_employee_push(array, &e3);

  Employee *pe1 = array_employee_get(array, 0);
  Employee *pe2 = array_employee_get(array, 1);
  Employee *pe3 = array_employee_get(array, 2);

  Dbg("%s: %d", pe1->name.cstr, pe1->age);
  Dbg("%s: %d", pe2->name.cstr, pe2->age);
  Dbg("%s: %d", pe3->name.cstr, pe3->age);

  // Employee e4 = {.name = S("roman"), .age = 25};
  // array_employee_push(array, &e4);
  // Employee *pe4 = array_employee_pop(array);

  // Dbg("%s: %d", pe4->name.cstr, pe4->age);

  // array_employee_destroy(array);
  return 0;
}

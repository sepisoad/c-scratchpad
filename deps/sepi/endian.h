#ifndef SEPI_ENDIAN_H
#define SEPI_ENDIAN_H

#include <stdlib.h>
#include "base.h"

/* ===================================================== */
/*                          API                          */
/* ===================================================== */

I16 nd_i16(I16 num);
I32 nd_i32(I32 num);
I64 nd_i64(I64 num);
F32 nd_f32(F32 num);

/* ===================================================== */
/*                    IMPLEMENTATION                     */
/* ===================================================== */

#ifdef SEPI_ENDIAN_IMPLEMENTATION

static inline Bool isle() {
  U16 num = 0x1;
  return (*(U8*)&num == 1);
}

I16 nd_i16(I16 num) {
  return isle() ? num : (I16)((num >> 8) | (num << 8));
}

I32 nd_i32(I32 num) {
  return isle() ? num
         : (I32)((num >> 24) | ((num >> 8) & 0x0000FF00) |
                 ((num << 8) & 0x00FF0000) | (num << 24));
}

I64 nd_i64(I64 num) {
  return isle() ? num
         : (I64)((num >> 56) | ((num >> 40) & 0x000000000000FF00LL) |
                 ((num >> 24) & 0x0000000000FF0000LL) |
                 ((num >> 8) & 0x00000000FF000000LL) |
                 ((num << 8) & 0x000000FF00000000LL) |
                 ((num << 24) & 0x0000FF0000000000LL) |
                 ((num << 40) & 0x00FF000000000000LL) | (num << 56));
}

F32 nd_f32(F32 num) {
  if(isle())
    return num;
  F32 result;
  Str src = (Str)&num;
  Str dst = (Str)&result;
  dst[0] = src[3];
  dst[1] = src[2];
  dst[2] = src[1];
  dst[3] = src[0];
  return result;
}

/* ===================================================== */
/*                          END                          */
/* ===================================================== */

#endif  // SEPI_ENDIAN_IMPLEMENTATION
#endif  // SEPI_ENDIAN_H

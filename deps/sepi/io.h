#ifndef SEPI_IO_H
#define SEPI_IO_H

/* ===================================================== */
/*                     DEPENDENCIES                      */
/* ===================================================== */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "base.h"

/* ===================================================== */
/*                          API                          */
/* ===================================================== */

SZ load_file(CSTR, BUF*);

/* ===================================================== */
/*                    IMPLEMENTATION                     */
/* ===================================================== */

#ifdef SEPI_IO_IMPLEMENTATION

SZ load_file(CSTR path, BUF* buf) {
  FILE* f = fopen(path, "rb");
  NOTNULL(f);

  fseek(f, 0, SEEK_END);
  SZ fsize = ftell(f);
  rewind(f);

  *buf = (BUF)malloc(sizeof(u8) * fsize);
  NOTNULL(*buf);

  SZ rsize = fread(*buf, 1, fsize, f);
  MAKESURE(rsize == fsize, "read size '%zu' did not match the file size '%zu'",
           rsize, fsize);

  if(f) {
    fclose(f);
  }

  return fsize;
}

/* ===================================================== */
/*                          END                          */
/* ===================================================== */

#endif  // SEPI_IO_IMPLEMENTATION
#endif  // SEPI_IO_H

#include <stdio.h>

#define DEBUG_MODE
#define USE_MEM_DEBUGGER

#define SEPI_DBGALLOC_IMPLEMENTATION
#define SEPI_GARENA_IMPLEMENTATION
#define SEPI_HASHMAP_IMPLEMENTATION
#include "deps/sepi/garena.h"
#include "deps/sepi/hashmap.h"

typedef struct CustomVal {
  I16 age;
  Bool male;
} CustomVal;

int main(int argc, char** argv) {
  GArena ga = {0};
  HashMap hm = {0};

  Sz init_cap = 256;
  succeed(garena_create(&ga, init_cap));
  succeed(hashmap_create(&hm, &ga, init_cap));
  {
    // add
    hashmap_add(&hm, (CStr)"sepi", (Ptr)(&(CustomVal) {
      .age = 38, .male = TRUE
    }));
    hashmap_add(&hm, (CStr)"amin", (Ptr)(&(CustomVal) {
      .age = 45, .male = TRUE
    }));
    hashmap_add(&hm, (CStr)"yasin", (Ptr)(&(CustomVal) {
      .age = 43, .male = TRUE
    }));
    hashmap_add(&hm, (CStr)"soghra", (Ptr)(&(CustomVal) {
      .age = 70, .male = FALSE
    }));
    hashmap_add(&hm, (CStr)"gholam", (Ptr)(&(CustomVal) {
      .age = 70, .male = TRUE
    }));

    // get
    CStr key = "sepi";
    CustomVal pcv;

    hashmap_get(&hm, key, &pcv);
    printf("key: '%s', age: '%d', male: '%s'\n",
           key,
           pcv.age,
           pcv.male ? "True" : "False");

    // del
    // TODO
  }
  hashmap_destroy(&hm);
}

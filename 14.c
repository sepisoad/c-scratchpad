#define SEPI_ENDIAN_IMPLEMENTATION
#define SEPI_ARENA_IMPLEMENTATION
#define SEPI_STRING_IMPLEMENTATION

#include <sepi/string.h>

#define _DefineFStr8_Internal(length)                                          \
  typedef struct FL##length##_Str8 {                                           \
    char cstr[(length) + 1];                                                   \
  } FL##length##_Str8;                                                         \
                                                                               \
  static inline U32 fl##length##_str8_length() { return (length); }            \
                                                                               \
  static inline FL##length##_Str8 fl##length##_str8(const char *str) {         \
    assert(str != NULL);                                                       \
    assert(strlen(str) < (length) + 1);                                        \
    struct {                                                                   \
      char c[(length) + 1];                                                    \
    } mutable_tmp = {0};                                                       \
    strncpy(mutable_tmp.c, str, (length));                                     \
    return *(FL##length##_Str8 *)&mutable_tmp;                                 \
  }                                                                            \
                                                                               \
  static inline Nothing fl##length##_str8_set(FL##length##_Str8 *f,            \
                                              const char *str) {               \
    assert(f != NULL);                                                         \
    assert(str != NULL);                                                       \
    assert(strlen(str) < (length) + 1);                                        \
    zero_memory(f->cstr, (length) + 1);                                        \
    copy_memory(f->cstr, str, strlen(str));                                    \
  }                                                                            \
                                                                               \
  static inline Nothing fl##length##_str8_reset(FL##length##_Str8 *f) {        \
    assert(f != NULL);                                                         \
    zero_memory(f->cstr, (length) + 1);                                        \
  }                                                                            \
                                                                               \
  static inline FL##length##_Str8 *fl##length##_str8_clone(                    \
      Arena *arena, FL##length##_Str8 f) {                                     \
    assert(arena != NULL);                                                     \
    FL##length##_Str8 *res = arena_push(arena, sizeof(FL##length##_Str8),      \
                                        alignof(FL##length##_Str8), TRUE);     \
    strncpy((char *)res->cstr, f.cstr, (length));                              \
    return res;                                                                \
  }                                                                            \
                                                                               \
  static inline Str8 fl##length##_str8_view(FL##length##_Str8 f) {             \
    return str8(f.cstr);                                                       \
  }

#define DefineFStr8(length) _DefineFStr8_Internal(length)

DefineFStr8(32);

int main() {
  Arena *arena = arena_create();
  FL32_Str8 str1 = fl32_str8("herro");
  FL32_Str8 *str2 = fl32_str8_clone(arena, str1);
  fl32_str8_set(str2, "mazafaka");

  dbg("%s", CS(fl32_str8_view(str1)));
  dbg("%s", CS(fl32_str8_view(*str2)));
  dbg("%d", fl32_str8_length());

  fl32_str8_reset(&str1);
  fl32_str8_reset(str2);

  dbg("%s", CS(fl32_str8_view(str1)));
  dbg("%s", CS(fl32_str8_view(*str2)));


  return 0;
}

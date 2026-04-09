#define SEPI_ENDIAN_IMPLEMENTATION
#define SEPI_ARENA_IMPLEMENTATION
#define SEPI_STRING_IMPLEMENTATION

#include <sepi/string.h>

DefineFixedStr8(32);
DefineFixedStr8(1);

int main() {
  Arena *arena = arena_create();

  Fixed32Str8 f32str1 = fixed_32_str8("foo");
  Fixed32Str8 *f32str2 = fixed_32_str8_clone(arena, "bar");
  Fixed32Str8 f32str3 = fixed_32_str8("fuck");
  Str8 str_a = fixed_32_str8_view(&f32str3);
  dbg("%s", f32str1.cstr);
  dbg("%s", f32str2->cstr);
  dbg("%s", str_a.cstr);
  dbg("%d", fixed_32_str8_length());

  Fixed1Str8 f1str1 = fixed_1_str8("f");
  Fixed1Str8 *f1str2 = fixed_1_str8_clone(arena, "b");
  Fixed1Str8 f1str3 = fixed_1_str8("f");
  Str8 str_b = fixed_1_str8_view(&f1str3);
  dbg("%s", f1str1.cstr);
  dbg("%s", f1str2->cstr);
  dbg("%s", str_b.cstr);
  dbg("%d", fixed_1_str8_length());
}

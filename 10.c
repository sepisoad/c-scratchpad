#include <stdio.h>

#define SEPI_PLATFORM_IMPLEMENTATION
#define SEPI_ENDIAN_IMPLEMENTATION
#define SEPI_ARENA_IMPLEMENTATION
#define SEPI_STRING_IMPLEMENTATION

#include <sepi/arena.h>
#include <sepi/string.h>


int main(void) {
  Str8 s1 = S("what the fuck");
  I32 s1i1 = str8_find_first(s1, ' ');
  I32 s1i2 = str8_find_last(s1, ' ');

  Dbg("%d", s1i1);
  Dbg("%d", s1i2);

  Str8 s2 = S("what the fuck");
  I32 s2i1 = str8_find_first(s2, 'k');
  I32 s2i2 = str8_find_last(s2, 'k');

  Dbg("%d", s2i1);
  Dbg("%d", s2i2);

  Str8 s3 = S("what the fuck");
  I32 s3i1 = str8_find_first(s3, '.');
  I32 s3i2 = str8_find_last(s3, '.');

  Dbg("%d", s3i1);
  Dbg("%d", s3i2);

  Str8 s4 = S("<@-@-@>");
  I32 s4i1 = str8_find_first(s4, '@');
  I32 s4i2 = str8_find_last(s4, '@');

  Dbg("%d", s4i1);
  Dbg("%d", s4i2);

  Dbg("%s", str8_cmp(S("Foo"), S("Foo"), 0) ? "YES" : "NO");
  Dbg("%s", str8_cmp(S("Foo"), S("foo"), 0) ? "YES" : "NO");
  Dbg("%s", str8_cmp(S("foo"), S("FOO"), 0) ? "YES" : "NO");
  Dbg("%s", str8_cmp(S("fOo"), S("FoO"), 0) ? "YES" : "NO");
  Dbg("%s", str8_cmp(S("Foo"), S("foo"), StringCompareFlag_CaseInsensitive) ? "YES" : "NO");
  Dbg("%s", str8_cmp(S("Foo"), S("fOO"), StringCompareFlag_CaseInsensitive) ? "YES" : "NO");
  Dbg("%s", str8_cmp(S("foo"), S("FOO"), StringCompareFlag_CaseInsensitive) ? "YES" : "NO");
  Dbg("%s", str8_cmp(S(" "), S("FOO"), StringCompareFlag_CaseInsensitive) ? "YES" : "NO");
  Dbg("%s", str8_cmp(S("Foo"), S(" "), StringCompareFlag_CaseInsensitive) ? "YES" : "NO");

  return 0;
}


#include <sepi/base.h>
#include <sepi/endian.h>
#include <sepi/io.h>

typedef struct IOItem2 IOItem2;
struct IOItem2 {
  NDBuffer buffer;
  Str8 name;
  Str8 path;
  IOItem* parent;
  IOItem* children;
  FILE* file;
  U16 children_count;
  Bool is_directory;
};

int main() {
  dbg("IOItem: %ld : %ld", sizeof(IOItem), alignof(IOItem));
  dbg("IOItem2: %ld : %ld", sizeof(IOItem2), alignof(IOItem2));

  dbg("CStr: %ld", sizeof(CStr));
  dbg("CBuf: %ld", sizeof(CBuf));
  dbg("Sz: %ld", sizeof(Sz));

  Empty e1;

  dbg("Empty: %ld", sizeof(Empty));
  dbg("e1: %ld", sizeof(e1));

  dbg("Empty: %ld", alignof(Empty));
  dbg("e1: %ld", alignof(e1));


  return 0;
}

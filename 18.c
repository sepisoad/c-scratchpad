#define SEPI_ARENA_IMPLEMENTATION
#define SEPI_STRING_IMPLEMENTATION
#define SEPI_CONTEXT_IMPLEMENTATION
#define SEPI_SYNC_IMPLEMENTATION
#define SEPI_APP_IMPLEMENTATION
#define SEPI_IO_IMPLEMENTATION

#include <sepi/app.h>
#include <sepi/base.h>
#include <sepi/io.h>

#define APP_CONTEXT_MAIN 0
#define APP_CONTEXT_UI 1
#define APP_CONTEXT_EXTRA 2

local SyncLock lock;
Str name;

RawPtr test_fn(RawPtr args) {
  Str *nameptr = (Str *)args;
  Arena *arena = context_arena();
  Str clone = str_clone(arena, *nameptr);

  sync_lock_acquire(lock);
  name = clone;
  sync_lock_release(lock);

  dbg("this is thread '%s'", ZS(name));

  return 0;
}

int main() {
  App app = app_create(APP_CONTEXT_MAIN, 0);

  Arena *arena = context_arena();

  Str name1 = str_clone(arena, S("thread 01"));
  Str name2 = str_clone(arena, S("thread 02"));

  lock = sync_lock_create();

  sync_lock_acquire(lock);
  name = S("main");
  sync_lock_release(lock);

  SyncThread *thread1 =
      sync_thread_start(test_fn, (RawPtr)&name1, APP_CONTEXT_UI);
  SyncThread *thread2 =
      sync_thread_start(test_fn, (RawPtr)&name2, APP_CONTEXT_EXTRA);

  dbg("this is thread '%s'", ZS(name));

  sync_thread_await(thread1);
  sync_thread_await(thread2);
  sync_lock_destroy(lock);

  app_destroy(app);
  return 0;
}

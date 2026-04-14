#define SEPI_ARENA_IMPLEMENTATION
#define SEPI_STRING_IMPLEMENTATION
#define SEPI_CONTEXT_IMPLEMENTATION
#define SEPI_SYNC_IMPLEMENTATION
#define SEPI_APP_IMPLEMENTATION
#define SEPI_IO_IMPLEMENTATION

#include <sepi/app.h>
#include <sepi/base.h>
#include <sepi/io.h>
#include <stdatomic.h>

#define APP_CONTEXT_MAIN 0
#define APP_CONTEXT_UI 1
#define APP_CONTEXT_EXTRA 2

SyncTokens tokens;
Str name;

RawPtr test_fn(RawPtr args) {
  with_tokens(tokens) {
    sync_thread_sleep(second * 2);
    Str *nameptr = (Str *)args;
    dbg("this is thread '%s'", ZS(*nameptr));
  }
  return 0;
}

int main() {
  App app = app_create(APP_CONTEXT_MAIN, 0);
  Arena *arena = context_arena();
  Str name1 = str_clone(arena, S("thread 01"));
  Str name2 = str_clone(arena, S("thread 02"));
  Str name3 = str_clone(arena, S("thread 03"));
  Str name4 = str_clone(arena, S("thread 04"));

  tokens = sync_tokens_create(0);

  SyncThread *thread1 =
      sync_thread_start(test_fn, (RawPtr)&name1, APP_CONTEXT_UI);
  SyncThread *thread2 =
      sync_thread_start(test_fn, (RawPtr)&name2, APP_CONTEXT_EXTRA);
  SyncThread *thread3 =
      sync_thread_start(test_fn, (RawPtr)&name3, APP_CONTEXT_UI);
  SyncThread *thread4 =
      sync_thread_start(test_fn, (RawPtr)&name4, APP_CONTEXT_EXTRA);

  sync_thread_await(thread1);
  sync_thread_await(thread2);
  sync_thread_await(thread3);
  sync_thread_await(thread4);

  sync_tokens_destroy(tokens);

  app_destroy(app);
  return 0;
}

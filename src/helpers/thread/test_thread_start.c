#include "index.h"

static void
thread_fn(thread_t *thread) {
    char *message = thread->arg;
    printf("[thread_fn] %s\n", message);
}

void
test_thread_start(void) {
    test_start();

    char *message = string_copy("hello thread");
    thread_t *thread = thread_start(thread_fn, message);

    who_printf("thread created: %p\n", (void *) thread);
    thread_join(thread);

    test_end();
}

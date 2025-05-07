#include "index.h"

static void *
thread_fn(void *arg) {
    char *message = arg;
    printf("[thread_fn] %s\n", message);
    return (void *) strlen(message);
}

void
test_thread_start(void) {
    test_start();

    char *message = string_copy("hello thread");
    thread_t *thread = thread_start(thread_fn, message);

    who_printf("thread created: %p\n", (void *) thread);
    size_t length = (size_t) thread_join(thread);
    who_printf("thread returned: %lu\n", length);

    test_end();
}

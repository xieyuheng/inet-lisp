#include "index.h"

static atomic_int x, y;
static atomic_int a, b;

static void
thread_fn_1(thread_t *thread) {
    (void) thread;

    relaxed_store(&x, 1);
    relaxed_store(&a, relaxed_load(&y));
}

static void
thread_fn_2(thread_t *thread) {
    (void) thread;

    relaxed_store(&y, 1);
    relaxed_store(&b, relaxed_load(&x));
}

void
test_thread_weak_memory_dekker(void) {
    test_start();

    // comment the follow early `return` to run this test.
    // due to weak memory model, the program exit the follow loop,
    // and report after how many loops an unintuitive behavior occurred.

    return;

    size_t count = 0;

    do {
        relaxed_store(&x, 0);
        relaxed_store(&y, 0);

        thread_t *T1 = thread_start(thread_fn_1, NULL);
        thread_t *T2 = thread_start(thread_fn_2, NULL);

        thread_join(T1);
        thread_join(T2);

        count++;
    } while (relaxed_load(&a) != 0 || relaxed_load(&b) != 0);

    who_printf("count: %lu\n", count);

    test_end();
}

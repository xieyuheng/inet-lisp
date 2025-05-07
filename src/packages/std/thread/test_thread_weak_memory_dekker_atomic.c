#include "index.h"

static atomic_int x, y;
static atomic_int a, b;

static void
thread_fn_1(void *arg) {
    (void) arg;

    atomic_store(&x, 1);
    atomic_store(&a, atomic_load(&y));
}

static void
thread_fn_2(void *arg) {
    (void) arg;

    atomic_store(&y, 1);
    atomic_store(&b, atomic_load(&x));
}

void
test_thread_weak_memory_dekker_atomic(void) {
    test_start();

    // comment the follow early `return` to run this test.
    // unlike the relaxed version, this program should loop forever.

    return;

    size_t count = 0;

    do {
        atomic_store(&x, 0);
        atomic_store(&y, 0);

        thread_t *T1 = thread_start(thread_fn_1, NULL);
        thread_t *T2 = thread_start(thread_fn_2, NULL);

        thread_join(T1);
        thread_join(T2);

        count++;
    } while (atomic_load(&a) != 0 || atomic_load(&b) != 0);

    who_printf("count: %lu\n", count);

    test_end();
}

#include "index.h"

static atomic_size_t global_count = 0;

static void
counter_add1(void *arg) {
    (void) arg;
    atomic_fetch_add(&global_count, 1);
    sleep(0); // let other threads run
}

static size_t
counter_read(void) {
    return atomic_load(&global_count);
}

void
test_counter_atomic(void) {
    test_start();
    double start_second = time_second();

    list_t *list = list_new();

    for (size_t i = 0; i < 1000; i++) {
        thread_t *T = thread_start(counter_add1, NULL);
        list_push(list, T);
    }

    while (!list_is_empty(list)) {
        thread_t *T = list_pop(list);
        thread_join(T);
    }

    list_destroy(&list);

    who_printf("final count: %lu\n", counter_read());
    who_printf("elapsed seconds: %fs\n", time_passed_second(start_second));
    test_end();
}

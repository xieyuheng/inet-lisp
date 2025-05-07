#include "index.h"

#define THREAD_COUNT 1000
#define PER_THREAD_COUNT_LIMIT 10000

static stats_counter_t *stats_counter = NULL;

static void
thread_fn(thread_t *thread) {
    for (size_t i = 0; i < PER_THREAD_COUNT_LIMIT; i++) {
        stats_counter_per_thread_add1(stats_counter, thread->id);
    }
}

void
test_stats_counter(void) {
    test_start();

    stats_counter = stats_counter_new(THREAD_COUNT);
    thread_pool_t *thread_pool = thread_pool_new();
    for (size_t i = 0; i < THREAD_COUNT; i++) {
        thread_pool_start(thread_pool, thread_fn, NULL);
    }

    {
        size_t total = stats_counter_total(stats_counter);
        who_printf("current total: %lu\n", total);
    }

    thread_pool_join_all(thread_pool);

    {
        size_t total = stats_counter_total(stats_counter);
        assert(total == THREAD_COUNT * PER_THREAD_COUNT_LIMIT);
        who_printf("final total: %lu\n", total);
    }

    thread_pool_destroy(&thread_pool);
    stats_counter_destroy(&stats_counter);

    test_end();
}

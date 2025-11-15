#include "index.h"

static size_t loop_count = 10000;
static uint64_t global_count = 0;

static void
thread_fn(thread_t *thread) {
    mutex_t *mutex = thread->arg;
    size_t count = 0;
    while (count < loop_count) {
        {
            mutex_lock(mutex);

            global_count++;
            sleep(0);

            mutex_unlock(mutex);
        }

        sleep(0);
        count++;
    }
}

void
test_mutex(void) {
    test_start();

    mutex_t *mutex = mutex_new();

    thread_t *T1 = thread_start(thread_fn, mutex);
    thread_t *T2 = thread_start(thread_fn, mutex);

    thread_join(T1);
    thread_join(T2);

    assert(global_count == loop_count * 2);

    test_end();
}

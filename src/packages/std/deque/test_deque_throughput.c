#include "index.h"

#define LENGTH 10000000

static void
uint_producer(thread_t *thread) {
    deque_t *deque = thread->arg;
    size_t count = 0;
    while (true) {
        if (count == LENGTH) return;

        deque_push_back(deque, (void *) count);
        count++;
    }
}

static void
uint_consumer(thread_t *thread) {
    deque_t *deque = thread->arg;
    size_t count = 0;
    while (true) {
        if (count == LENGTH) return;

        while (deque_is_empty(deque)) {}

        assert(((size_t) deque_pop_front(deque)) == count);
        count++;
    }
}

void
test_deque_throughput(void) {
    test_start();

    deque_t *deque = deque_new();

    double start_second = time_second();

    thread_t *producer_thread = thread_start(uint_producer, deque);
    thread_t *consumer_thread = thread_start(uint_consumer, deque);

    thread_join(producer_thread);
    thread_join(consumer_thread);

    double throughput = LENGTH / 1000 / time_passed_second(start_second);
    who_printf("throughput: %.f k/s\n", throughput);

    deque_destroy(&deque);

    test_end();
}
